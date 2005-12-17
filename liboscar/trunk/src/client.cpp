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

namespace liboscar {

Client::Client(){
	initvalues();
}

Client::Client(const UIN& uin, const QString& password){
	m_uin = uin;
	m_password = password;
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

	if (err != CONN_NO_ERROR) // Got an error
		m_middledisconnect = true;

	if (m_state == CLI_AUTHING && m_logconn)
		m_logconn->disconnect();
	else if ((m_state == CLI_CONNECTING) || (m_state == CLI_CONNECTED) && m_conn)
		m_conn->disconnect();

}

ConnectionResult Client::connect(){
	/* FIXME: comprobar estado en que se queda despues de error */
	ConnectionStatus s;
	ConnectionError e;

	if (!m_parser){
		m_parser = new Parser(this);
		QObject::connect(m_parser, SIGNAL(receivedBOS(QString, QString)), this, SLOT(getBOSInfo(QString, QString)));
		QObject::connect(m_parser, SIGNAL(serverDisconnected(QString, DisconnectReason)), 
				this, SLOT(unexpectedDisconnect(QString, DisconnectReason)));
	}

	if (!m_logconn)
		m_logconn = new Connection(ICQ_LOGIN_SERVER, ICQ_LOGIN_PORT, m_parser);

		
	m_state = CLI_AUTHING;

	s = m_logconn->connect();
	if (s != CONN_CONNECTED){
		initvalues();
		ConnectionResult res(false, CONN_ERR_LOGIN_CONN_FAILED);
		return res;
	}
	e = m_logconn->listen();

	qDebug("Desconectando del Autorizer");

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
	if (!m_conn)
		m_conn = new Connection(m_bos, m_bosport, m_parser);

	qDebug("Conectando al BOS");
	s = m_conn->connect();
	if (s != CONN_CONNECTED){
		initvalues();
		ConnectionResult res(false, CONN_ERR_CONN_FAILED);
		return res;
	}

	m_state = CLI_CONNECTED;

	e = m_conn->listen();
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
	disconnect();
}

void Client::unexpectedDisconnect(QString reason, DisconnectReason error){
	// TODO: error reports
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

Client::~Client() { 
	initvalues();
}


}

#include "client.moc"
