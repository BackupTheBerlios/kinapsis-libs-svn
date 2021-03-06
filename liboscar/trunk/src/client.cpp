/***************************************************************************
 *   Copyright (C) 2005-2008 by Luis Cidoncha                              *
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
#include <qtextcodec.h>
#include <qmetatype.h>

namespace liboscar {

	//
	// CONSTRUCTORS & DESTRUCTORS
	//

Client::Client(const ProtocolType type) 
	: Service(type) {
	initvalues();
}

Client::Client(const UIN& uin, const QString& password, const ProtocolType type)
	: Service(type) {
	m_uin = uin;
	m_password = password;
	initvalues();
}

void Client::initvalues(){
	m_connected = false;
	m_logged = false;
	m_cap.setDefault();
	m_awaymsg = "I'm away now.";
	registerMeta();
	createSupport();
	createProcess();
}

void Client::registerMeta() {
	qRegisterMetaType<Message>("Message");
	qRegisterMetaType<UIN>("UIN");
	qRegisterMetaType<Buffer>("Buffer");
	qRegisterMetaType<Contact>("Contact");
	qRegisterMetaType<SrvFamiliesSNAC>("SrvFamiliesSNAC");
	qRegisterMetaType<SrvVersionsSNAC>("SrvVersionsSNAC");
	qRegisterMetaType<SrvRatesSNAC>("SrvRatesSNAC");
	qRegisterMetaType<SrvReplyLocationSNAC>("SrvReplyLocationSNAC");
	qRegisterMetaType<SrvReplyBuddySNAC>("SrvReplyBuddySNAC");
	qRegisterMetaType<SrvReplyICBMSNAC>("SrvReplyICBMSNAC");
	qRegisterMetaType<SrvReplyBOSSNAC>("SrvReplyBOSSNAC");
	qRegisterMetaType<SrvReplyListsSNAC>("SrvReplyListsSNAC");
	qRegisterMetaType<SrvMetaReplySNAC>("SrvMetaReplySNAC");
	qRegisterMetaType<SrvUserInfoSNAC>("SrvUserInfoSNAC");
}

void Client::createSupport() {
	m_roster = new Roster();
}

void Client::createProcess() {
	m_rp = new RosterProcess(this);
	m_l2p = new LoginSt2Process(this);
	m_ssp = new ServiceSetupProcess(this);
	m_omp = new OfflineMessagesProcess(this);
	m_pp = new PresenceProcess(this);
	m_ftp = new FileTransferProcess(this);
	m_ap = new AvatarProcess(this);

	// Connect processes between them
	QObject::connect(m_l2p, SIGNAL(stage2finished()), m_ssp, SLOT(requestServiceSetup()));

	// Connect processes to client
		//RosterProcess
	QObject::connect(m_rp, SIGNAL(rosterAcked()), this, SLOT(finishedConnection()));
		//OfflineMessagesProcess
	QObject::connect(m_omp, SIGNAL(newOfflineMessage(Message)), this, SLOT(messageArrived(Message)));
}

Client::~Client() { 
	if (m_roster) delete m_roster;
	if (m_rp) delete m_rp;
	if (m_l2p) delete m_l2p;
	if (m_ssp) delete m_ssp;
	if (m_omp) delete m_omp;
	if (m_pp) delete m_pp;
	if (m_ftp) delete m_ftp;
	if (m_ap) delete m_ap;
}

	//
	// GETTERS & SETTERS
	//
	
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
	m_uin = uin;
}

void Client::setAwayMessage(QString message){
	m_awaymsg = message;
}

QString Client::getAwayMessage(){
	return m_awaymsg;
}

FirewallConfiguration Client::getFirewall() {
	// TODO: hacer que esto rule
	return NORMAL;
}

	//
	// CONNECTION STUFF
	//
	
void Client::run(){

	m_ls = new LoginService(m_uin, m_password);	

	QObject::connect(m_ls, SIGNAL(BOSInfo(QString, QString, QByteArray)), this, SLOT(getBOSInfo(QString,QString,QByteArray)));
	QObject::connect(m_ls, SIGNAL(serviceEnded(unsigned int, ConnectionResult)), this, SLOT(loginServiceEnded(unsigned int, ConnectionResult)));

	if (m_type == ICQ)
		m_ls->connect(ICQ_LOGIN_SERVER, ICQ_LOGIN_PORT);
	else if (m_type == AIM)
		m_ls->connect(AIM_LOGIN_SERVER, AIM_LOGIN_PORT);


	this->exec();
	qDebug("[Client]: exiting");
}

void Client::connBOS(QString bos, int port) {

	if (m_conn)
		delete m_conn;

	m_conn = new Connection(bos, port, m_parser);

	QObject::connect(m_conn, SIGNAL(connConnected()), this, SLOT(BOSConnected()));
	QObject::connect(m_conn, SIGNAL(connDisconnected()), this, SLOT(BOSDisconnected()));
	QObject::connect(m_conn, SIGNAL(connError(SocketError)), this, SLOT(BOSError(SocketError)));
	m_conn->connect();
}

void Client::connect(){
	this->start();
}

void Client::create() {

	if (!m_parser){
		m_parser = new OscarParser();
		// Client signals
			//Parser
		QObject::connect(m_parser, SIGNAL(serverDisconnected(QString, DisconnectReason)), 
				this, SLOT(unexpectedDisconnect(QString, DisconnectReason)));
		QObject::connect(m_parser, SIGNAL(newMessage(Message)), this, SLOT(messageArrived(Message)));
		QObject::connect(m_parser, SIGNAL(typingEventArrived(UIN, IsTypingType)), this, SLOT(newTypingEvent(UIN, IsTypingType)));

		// RosterProcess (m_rp) signals
		QObject::connect(m_parser, SIGNAL(rosterArrived(Buffer)), m_rp, SLOT(rosterArrivedSlot(Buffer)));
		QObject::connect(m_parser, SIGNAL(rosterServerAck(RosterModifyAck)), m_rp, SLOT(handleUpdateAck(RosterModifyAck)));
		QObject::connect(m_parser, SIGNAL(authReq(UIN, QString)), m_rp, SLOT(authReq(UIN, QString)));
		QObject::connect(m_parser, SIGNAL(addedYou(UIN)), m_rp, SLOT(addedMe(UIN)));
		// LoginSt2Process (m_l2p) signals
		QObject::connect(m_parser, SIGNAL(recvHello()), m_l2p, SLOT(recvHello()));
		QObject::connect(m_parser, SIGNAL(serverFamilies(SrvFamiliesSNAC)), m_l2p, SLOT(recvFamilies(SrvFamiliesSNAC)));
		QObject::connect(m_parser, SIGNAL(serverServicesVersion(SrvVersionsSNAC)), m_l2p, SLOT(recvVersions(SrvVersionsSNAC)));
		QObject::connect(m_parser, SIGNAL(serverRateLimits(SrvRatesSNAC)), m_l2p, SLOT(recvRates(SrvRatesSNAC)));
		// ServiceSetupProcess (m_ssp) signals
		QObject::connect(m_parser, SIGNAL(locationLimits(SrvReplyLocationSNAC)), m_ssp, 
				SLOT(locationLimits(SrvReplyLocationSNAC)));
		QObject::connect(m_parser, SIGNAL(BLMLimits(SrvReplyBuddySNAC)), m_ssp, SLOT(BLMLimits(SrvReplyBuddySNAC)));
		QObject::connect(m_parser, SIGNAL(ICBMLimits(SrvReplyICBMSNAC)), m_ssp, SLOT(ICBMLimits(SrvReplyICBMSNAC)));
		QObject::connect(m_parser, SIGNAL(PRMLimits(SrvReplyBOSSNAC)), m_ssp, SLOT(PRMLimits(SrvReplyBOSSNAC)));
		QObject::connect(m_parser, SIGNAL(SSILimits(SrvReplyListsSNAC)), m_ssp, SLOT(SSILimits(SrvReplyListsSNAC)));
		// PresenceProcess (m_pp) signals
		QObject::connect(m_parser, SIGNAL(statusChanged(UIN, PresenceStatus)), m_pp, SLOT(statusChanged(UIN, PresenceStatus)));
		QObject::connect(m_parser, SIGNAL(serverUserInfo(SrvUserInfoSNAC)), m_pp, SLOT(newAwayMessage(SrvUserInfoSNAC)));
		// FileTransferProcess (m_ftp) signals
		QObject::connect(m_parser, SIGNAL(newMessage(Message)), m_ftp, SLOT(messageArrived(Message)));
		// AvatarProcess (m_ap) signals
		QObject::connect(m_parser, SIGNAL(userOnlineInfo(UserInfo)), 
				m_ap, SLOT(userOnlineInfo(UserInfo)));
		QObject::connect(m_parser, SIGNAL(ownIconAck(QByteArray)), m_ap, SLOT(ownIconAck(QByteArray)));
		QObject::connect(m_parser, SIGNAL(iconArrived(UIN, QByteArray, QByteArray)), 
				m_ap, SLOT(iconInfo(UIN, QByteArray, QByteArray)));
	}

}

	//
	// SENDING
	//

void Client::sendMessage(Message message) {
	FLAP f(0x02, Connection::getNextSeqNumber(), 0);
	CliSendMsgSNAC *s = new CliSendMsgSNAC(message);
	f.addSNAC(s);
	send(f.pack());
}

void Client::sendMessage(UIN uin, QString message) {
	Message m;
	m.setText(message);
	m.setUin(uin);
	m.setFormat(0x0001);
	QTextCodec *c = QTextCodec::codecForMib(4); //iso-8859-1
	if (c->canEncode(message))
		m.setEncoding(ASCII);
	else
		m.setEncoding(UCS2BE);
	m.setType(TYPE_PLAIN);
	this->sendMessage(m);
}

	// 
	// USER ACTIONS
	//
	
void Client::disconnect(){ // this function is ONLY for user requested disconnections
	// TODO: add disconnect to loginService !!!!
	m_conn->disconnect();
}

void Client::setPresence(PresenceStatus status) {
	FLAP f(0x02, Connection::getNextSeqNumber(), 0);
	SNAC *s;

	if (m_type == ICQ) // FIXME: wired values
		s = new CliSetStatusSNAC(status, 0, 0, NORMAL);
	else
		s = new CliSetUserInfoSNAC(m_cap, (status == STATUS_ONLINE ? "" : m_awaymsg));

	f.addSNAC(s);
	send(f.pack());
}

void Client::sendTypingNotice(UIN uin, IsTypingType type) {
	FLAP f(0x02, Connection::getNextSeqNumber(), 0);
	SrvCliTypingSNAC *s = new SrvCliTypingSNAC(uin, type);
	f.addSNAC(s);
	send(f.pack());
}

	//
	// PROCESS ACTIVATION ACTIONS (DONE BY USER)
	//
	
		//
		// ROSTERPROCESS
		//

bool Client::addContact(UIN uin) {
	return m_rp->addContact(uin, false);
}

bool Client::delContact(UIN uin) {
	return m_rp->delContact(uin);
}

bool Client::authorize(UIN uin, QString message, bool auth) {
	return m_rp->authorize(uin, message, auth);
}

bool Client::contactVisible(UIN uin, bool in) {
	return m_rp->setVisibleStatus(uin, in);
}

bool Client::contactInvisible(UIN uin, bool in) {
	return m_rp->setInvisibleStatus(uin, in);
}

		//
		// FILETRANSFERPROCESS
		//

void Client::acceptFileTransfer(QWord c, bool a) {
	m_ftp->acceptFileTransfer(c,a);
}

QWord Client::sendFile(UIN to, QString fname){
	m_ftp->sendFile(to, fname);
}

void Client::setUseFTProxy(bool p) {
	m_ftp->setUseProxy(p);
}

void Client::setFTListeningPort(int p) {
	m_ftp->setListeningPort(p);
}

		//
		// AVATARPROCESS
		//

void Client::requestIconFor(UIN uin, QByteArray md5) {
	m_ap->requestIconFor(uin, md5);
}

void Client::uploadIcon(QString fname) {
	m_ap->uploadIcon(fname);
}

	//
	// SLOTS
	//
	

void Client::BOSConnected() {
	qDebug("Connected to BOS");
	m_connected = true;
}

void Client::BOSDisconnected() {
	m_connected=false;
	emit notifyDisconnect(OscarConnectionResult(CONN_NO_ERROR, NO_ERROR));
	this->exit();
}

void Client::BOSError(SocketError e) {
	m_connected=false;
	emit notifyDisconnect(OscarConnectionResult(CONN_ERR_CONN_FAILED, NO_ERROR));
	this->exit();
}

void Client::getBOSInfo(QString server, QString port, QByteArray cookie){
	m_logged = true;
	m_l2p->setCookie(cookie);
	
	//
	// So... we got a BOS and Stage1 must be ended, continue to stage2
	//
	
	qDebug("BOS Arrived, connecting...");
	create();
	connBOS(server, port.toUInt());

}

void Client::loginServiceEnded(unsigned int id, ConnectionResult r) {
	if (!m_logged){ // Something happened to LoginService :-(
		emit notifyDisconnect(OscarConnectionResult((r.errorReason() == UNEXPECTED_DISC) ? CONN_ERR_UNEXPECTED : 
					CONN_ERR_LOGIN_CONN_FAILED, r.getUnexpectedDisconnectReason()));
		this->exit();
	}
}

void Client::unexpectedDisconnect(QString reason, DisconnectReason error){
	qDebug("Unexpected disconnect from server");

	m_reason = error;

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
	// XXX: maybe disconnect the Connection, or raise a onDisconnect()
}

void Client::finishedConnection(){
	setPresence(STATUS_ONLINE);

	// We're ready now (at last)
	FLAP* f = new FLAP(0x02, Connection::getNextSeqNumber(), 0);
	CliReadySNAC *crs = new CliReadySNAC;
	f->addSNAC(crs);
	send(f->pack());

	// Request possible offline messages
	m_omp->requestOfflineMessages();
	
	emit notifyConnect();
}

void Client::messageArrived(Message message){
	// TODO: handle more message stuff here
	// FT messages are handled in FileTransferProcess
	if (message.getFormat() == 0x0002 && message.getRequest() == REQUEST){
		if (message.getType() >= TYPE_AUTOAWAY){
			Message m;

			m.fromMessage(message);
			m.setText(getAwayMessage());
			sendMessage(m);
		}
	}
	else // App must handle this
		emit notifyMessage(message);
}

void Client::newTypingEvent(UIN uin, IsTypingType type) {
	emit notifyTypingEvent(uin, type);
}

Roster* Client::getRoster(){
	return m_roster;
}

Capabilities& Client::getCapabilities(){
	return m_cap;
}

	//
	// LISTENERS' STUFF
	//
	
void Client::addConnectionListener(ConnectionListener *cl) {
	QObject::connect(this, SIGNAL(notifyConnect()), cl, SLOT(connectSlot()));
	QObject::connect(this, SIGNAL(notifyDisconnect(OscarConnectionResult)), cl, SLOT(disconnectSlot(OscarConnectionResult))); // if we stopped...
}

void Client::delConnectionListener(ConnectionListener *cl) {
	QObject::disconnect(this, 0, cl, 0);
}

void Client::addRosterListener(RosterListener *rl) {
	QObject::connect(m_roster, SIGNAL(contactAdded(Contact)), rl, SLOT(onAddContact(Contact)));
	QObject::connect(m_roster, SIGNAL(contactUpdated(Contact)), rl, SLOT(onUpdateContact(Contact)));
	QObject::connect(m_roster, SIGNAL(contactDeleted(UIN)), rl, SLOT(onDelContact(UIN)));
	QObject::connect(m_roster, SIGNAL(groupAdded(QString)), rl, SLOT(onAddGroup(QString)));
	QObject::connect(m_roster, SIGNAL(groupUpdated(QString,QString)), rl, SLOT(onUpdateGroup(QString,QString)));
	QObject::connect(m_roster, SIGNAL(groupDeleted(QString)), rl, SLOT(onDelGroup(QString)));
	QObject::connect(m_rp, SIGNAL(notifyAuthRequest(UIN, QString)), rl, SLOT(onAuthRequest(UIN, QString)));
}

void Client::delRosterListener(RosterListener *rl) {
	QObject::disconnect(this, 0, rl, 0);
}

void Client::addMessageListener(MessageListener *ml) {
	QObject::connect(this, SIGNAL(notifyMessage(Message)), ml, SLOT(incomingMessage(Message)));
}

void Client::delMessageListener(MessageListener *ml) {
	QObject::disconnect(this, 0, ml, 0);
}

void Client::addPresenceListener(PresenceListener *pl) {
	QObject::connect(m_pp, SIGNAL(notifyPresence(UIN, PresenceStatus)), pl, SLOT(presenceChanged(UIN, PresenceStatus)));
	QObject::connect(m_pp, SIGNAL(notifyAwayMessage(UIN, QString)), pl, SLOT(awayMessage(UIN, QString)));
}

void Client::delPresenceListener(PresenceListener *pl) {
	QObject::disconnect(this, 0, pl, 0);
}

void Client::addIsTypingListener(IsTypingListener *tl) {
	QObject::connect(this, SIGNAL(notifyTypingEvent(UIN, IsTypingType)), tl, SLOT(isTypingEvent(UIN, IsTypingType)));
}

void Client::delIsTypingListener(IsTypingListener *tl) {
	QObject::disconnect(this, 0, tl, 0);
}

void Client::addFileTransferListener(FileTransferListener *fl) {
	QObject::connect(m_ftp, SIGNAL(notifyNewFTRequest(QWord, UIN, QString)), 
			fl, SLOT(newFTRequest(QWord, UIN, QString)));
	QObject::connect(m_ftp, SIGNAL(ftConnected(QWord)), fl , SLOT(ftConnected(QWord)));
	QObject::connect(m_ftp, SIGNAL(ftProgress(QWord,int,int)), fl , SLOT(ftProgress(QWord,int,int)));
	QObject::connect(m_ftp, SIGNAL(ftEnded(QWord)), fl , SLOT(ftEnded(QWord)));
}

void Client::delFileTransferListener(FileTransferListener *fl) {
	QObject::disconnect(this, 0, fl, 0);
}

void Client::addAvatarListener(AvatarListener *al) {
	QObject::connect(m_ap, SIGNAL(contactIconHash(UIN, QByteArray)), 
			al , SLOT(contactIconHash(UIN, QByteArray)));
	QObject::connect(m_ap, SIGNAL(iconInfoArrived(UIN, QByteArray, QByteArray)), 
			al , SLOT(iconInfo(UIN, QByteArray, QByteArray)));
}

void Client::delAvatarListener(AvatarListener *al) {
	QObject::disconnect(this, 0, al, 0);
}

}

#include "client.moc"
