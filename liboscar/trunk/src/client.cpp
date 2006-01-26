/***************************************************************************
 *   Copyright (C) 2005 by Luis Cidoncha                                   *
 *   luis.cidoncha@gmail.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "client.h"
#include "flap.h"
#include "snac_icbm.h"
#include "snac_service.h"
#include "snac_newuser.h"
#include "snac_roster.h"
#include "snac_location.h"

namespace liboscar {

Client::Client(const ProtocolType type){
	m_conn = m_logconn = 0;
	m_parser = 0;
	m_type = type;
	initvalues();
}

Client::Client(const UIN& uin, const QString& password, const ProtocolType type){
	m_uin = uin;
	m_password = password;
	m_conn = m_logconn = 0;
	m_parser = 0;
	m_type = type;
	initvalues();
}

void Client::initvalues(){
	m_bos = "";
	m_bosport = -1;
	delete m_conn;
	delete m_logconn;
	m_conn = m_logconn = 0;
	delete m_parser;
	m_parser = 0;
	m_state = CLI_NO_STATE;
	m_middledisconnect = false;
	m_exit = false;
	m_cap.setDefault();
	m_awaymsg = "I'm away now.";
}

QString Client::getPassword(){
	return m_password;
}

void Client::setPassword(const QString& password){
	m_password = password;
}

UIN Client::getUIN(){
	return m_uin;
}

void Client::setUIN(const UIN& uin){
/*	TODO: if (m_conn){
		if (m_conn->state() == CONN_CONNECTED)
			;
	} OJO */
	m_uin = uin;
}

void Client::setAwayMessage(QString message){
	m_awaymsg = message;
}

QString Client::getAwayMessage(){
	return m_awaymsg;
}

DWord Client::getLocalIP() {
	if (m_conn)
		return m_conn->getLocalIP();
	return 0;
}

Word Client::getPort() {
	if (m_conn)
		return m_conn->getPort();
	return 0;
}

FirewallConfiguration Client::getFirewall() {
	// TODO: hacer que esto rule
	return NORMAL;
}

ClientState Client::state(){
	return m_state;
}

void Client::send(Buffer& b){
	if (m_logconn)
		m_logconn->send(b);
	else if (m_conn)
		m_conn->send(b);
}

void Client::disconnect(ConnectionError err){

	if ((err == CONN_RECONNECT) || (err == CONN_NO_ERROR && m_state == CLI_AUTHING))
		m_exit = false;
	else
		m_exit = true;

	if ((err != CONN_NO_ERROR) && (err != CONN_ERR_USER_REQUEST) && m_logconn) // Got an error
		m_middledisconnect = true;

	if (m_state == CLI_AUTHING && m_logconn)
		m_logconn->disconnect();
	else if ((m_state == CLI_CONNECTING) || (m_state == CLI_CONNECTED) && m_conn){
		m_conn->disconnect();
		emit notifyDisconnect();
	}

}

void Client::sendMessage(Message message) {
	FLAP f(0x02, m_parser->getNextSeqNumber(), 0);
	CliSendMsgSNAC *s = new CliSendMsgSNAC(message);
	f.addSNAC(s);
	send(f.pack());
}

void Client::sendMessage(UIN uin, QString message) {
	Message m;
	m.setMessage(message);
	m.setUin(uin);
	m.setFormat(0x0001);
	m.setEncoding(ASCII);
	m.setType(TYPE_PLAIN);
	this->sendMessage(m);
}

void Client::setPresence(PresenceStatus status) {
	FLAP f(0x02, m_parser->getNextSeqNumber(), 0);
	SNAC *s;

	if (m_type == ICQ) // FIXME: wired values
		s = new CliSetStatusSNAC(status, 0, 0, NORMAL);
	else
		s = new CliSetUserInfoSNAC(m_cap, (status == STATUS_ONLINE ? "" : m_awaymsg));

	f.addSNAC(s);
	send(f.pack());
}

void Client::registerNewUin(QString password) {

	if (m_state == CLI_NO_STATE) ; // TODO: make a connection requesting a UIN
	else {
		FLAP f(0x02, m_parser->getNextSeqNumber(), 0);
		CliReqUINSNAC *s = new CliReqUINSNAC(password);
		f.addSNAC(s);
		send(f.pack());
	}
}

void Client::authorize(UIN uin, QString message, bool auth) {
	FLAP f(0x02, m_parser->getNextSeqNumber(), 0);
	CliAuthorizeSNAC *s = new CliAuthorizeSNAC(uin, auth, message);
	f.addSNAC(s);
	send(f.pack());
}

void Client::addContact(UIN uin, bool reqAuth) {
	// TODO:
}

void Client::rosterEditStart() {
	FLAP f(0x02, m_parser->getNextSeqNumber(), 0);
	CliAddStartSNAC *s = new CliAddStartSNAC();
	f.addSNAC(s);
	send(f.pack());
}

void Client::rosterEditEnd() {
	FLAP f(0x02, m_parser->getNextSeqNumber(), 0);
	CliAddEndSNAC *s = new CliAddEndSNAC();
	f.addSNAC(s);
	send(f.pack());
}

ConnectionError Client::connAuth() {

	ConnectionStatus s;
	ConnectionError e;

	if (!m_logconn){
		if (m_type == ICQ)
			m_logconn = new Connection(ICQ_LOGIN_SERVER, ICQ_LOGIN_PORT, m_parser);
		else if (m_type == AIM)
			m_logconn = new Connection(AIM_LOGIN_SERVER, AIM_LOGIN_PORT, m_parser);
	}

		
	m_state = CLI_AUTHING;

	s = m_logconn->connect();
	if (s != CONN_CONNECTED){
		initvalues();
		return CONN_ERR_LOGIN_CONN_FAILED;
	}
	e = m_logconn->listen();

	qDebug("Disconnecting from Authorizer");

	return e;
}

ConnectionError Client::connBOS() {

	ConnectionStatus s;
	ConnectionError e;

	if (m_conn)
		delete m_conn;

	m_conn = new Connection(m_bos, m_bosport, m_parser);

	qDebug("Connecting to BOS");
	s = m_conn->connect();
	if (s != CONN_CONNECTED){
		initvalues();
		return CONN_ERR_CONN_FAILED;
	}

	m_state = CLI_CONNECTED;

	e = m_conn->listen();

	qDebug("Disconnecting from BOS");

	return e;

}

ConnectionResult Client::connect(){
	/* FIXME: comprobar estado en que se queda despues de error */
	ConnectionError e;

	if (!m_parser){
		m_parser = new Parser(this);
		QObject::connect(m_parser, SIGNAL(receivedBOS(QString, QString)), this, SLOT(getBOSInfo(QString, QString)));
		QObject::connect(m_parser, SIGNAL(serverDisconnected(QString, DisconnectReason)), 
				this, SLOT(unexpectedDisconnect(QString, DisconnectReason)));
		QObject::connect(m_parser, SIGNAL(loginSequenceFinished()), this, SLOT(finishedConnection()));
		QObject::connect(m_parser, SIGNAL(rosterInfo(Roster)), this, SLOT(rosterArrived(Roster)));
		QObject::connect(m_parser, SIGNAL(newMessage(Message)), this, SLOT(messageArrived(Message)));
		QObject::connect(m_parser, SIGNAL(statusChanged(UIN, PresenceStatus)), this, SLOT(statusChanged(UIN, PresenceStatus)));
		QObject::connect(m_parser, SIGNAL(newUin(UIN)), this, SLOT(newUin(UIN)));
		QObject::connect(m_parser, SIGNAL(authReq(UIN, QString)), this, SLOT(authReq(UIN, QString)));
	}

	e = connAuth();

	// TODO: think about disconnect reason
	if (m_middledisconnect){
		initvalues();
		ConnectionResult res(false, CONN_ERR_LOGIN_CONN_FAILED);
		return res;
	}
	if (e != CONN_NO_ERROR){
		initvalues();
		ConnectionResult res(false, e);
		return res;
	}

	delete m_logconn;
	m_logconn = 0;

	m_state = CLI_CONNECTING;

	/* login finished; connect to BOS */
	while (!m_exit) // We can get reconnected several times
		e = connBOS();

	if (e != CONN_NO_ERROR){
		initvalues();
		ConnectionResult res(false, e);
		return res;
	}

	initvalues();

	ConnectionResult res(true, CONN_NO_ERROR);
	return res;
}

void Client::getBOSInfo(QString server, QString port){
	m_bos = server;
	m_bosport = port.toUInt();
	if (m_state == CLI_AUTHING)
		disconnect();
	else
		disconnect(CONN_RECONNECT); // Got a serverpause and a migrationreq
}

void Client::unexpectedDisconnect(QString reason, DisconnectReason error){
	qDebug("Unexpected disconnect from server");

	switch (error) {
		case MULTIPLE_LOGINS:
			qDebug("Multiple logins");
			break;
		case BAD_PASSWORD:
			qDebug("Bad password");
			break;
		case NON_EXISTANT_UIN:
			qDebug("Non existant UIN");
			break;
		case TOO_MANY_CLIENTS:
			qDebug("Too many clients for the same IP");
			break;
		case RATE_EXCEEDED:
			qDebug("Rate exceeded");
			break;
		case OLD_VERSION:
			qDebug("Old ICQ Client version");
			break;
		case RECONNECTING_TOO_FAST:
			qDebug("Reconnecting too fast");
			break;
		case CANT_REGISTER:
			qDebug("Can't register try again later");
			break;
	}
	disconnect(CONN_ERR_UNEXPECTED);
}

void Client::finishedConnection(){
	emit notifyConnect();
}

void Client::rosterArrived(Roster r){

	m_roster = r;

	QPtrList<Contact> c = r.getContacts();
	Contact *tmp;

	for (tmp = c.first(); tmp; tmp = c.next())
		emit notifyNewContact(tmp);
}

void Client::messageArrived(Message message){
	// TODO: handle more message stuff here
	if (message.getFormat() == 0x0002 && message.getRequest() == REQUEST){
		// We must reply this
		// TODO
	}
	else // App must handle this
		emit notifyMessage(message.getUin(), message.getMessage());
}

void Client::statusChanged(UIN uin, PresenceStatus status){
	emit notifyPresence(uin, status);
}

void Client::newUin(UIN uin){
	emit notifyNewUin(uin);
}

void Client::authReq(UIN uin, QString reason){
	emit notifyAuthRequest(uin, reason);
}

Roster& Client::getRoster(){
	return m_roster;
}

Capabilities& Client::getCapabilities(){
	return m_cap;
}


// LISTENERS' STUFF

void Client::addConnectionListener(ConnectionListener *cl) {
	QObject::connect(this, SIGNAL(notifyConnect()), cl, SLOT(connectSlot()));
	QObject::connect(this, SIGNAL(notifyDisconnect()), cl, SLOT(disconnectSlot()));
}

void Client::delConnectionListener(ConnectionListener *cl) {
	QObject::disconnect(this, 0, cl, 0);
}

void Client::addRosterListener(RosterListener *rl) {
	QObject::connect(this, SIGNAL(notifyNewContact(Contact *)), rl, SLOT(onNewContactSlot(Contact *)));
	QObject::connect(this, SIGNAL(notifyAuthRequest(UIN, QString)), rl, SLOT(onAuthRequestSlot(UIN, QString)));
}

void Client::delRosterListener(RosterListener *rl) {
	QObject::disconnect(this, 0, rl, 0);
}

void Client::addMessageListener(MessageListener *ml) {
	QObject::connect(this, SIGNAL(notifyMessage(UIN, QString)), ml, SLOT(incomingSlot(UIN, QString)));
}

void Client::delMessageListener(MessageListener *ml) {
	QObject::disconnect(this, 0, ml, 0);
}

void Client::addPresenceListener(PresenceListener *pl) {
	QObject::connect(this, SIGNAL(notifyPresence(UIN, PresenceStatus)), pl, SLOT(presenceChangedSlot(UIN, PresenceStatus)));
}

void Client::delPresenceListener(PresenceListener *pl) {
	QObject::disconnect(this, 0, pl, 0);
}

void Client::addUINRegistrationListener(UINRegistrationListener *ul) {
	QObject::connect(this, SIGNAL(notifyNewUin(UIN)), ul, SLOT(newUinSlot(UIN)));
}

void Client::delUINRegistrationListener(UINRegistrationListener *ul) {
	QObject::disconnect(this, 0, ul, 0);
}

Client::~Client() { 
	initvalues();
}


}

#include "client.moc"
