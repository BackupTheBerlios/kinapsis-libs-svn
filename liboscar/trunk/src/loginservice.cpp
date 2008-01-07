/***************************************************************************
 *   Copyright (C) 2008 by Luis Cidoncha                                   *
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


#include "loginservice.h"
#include "oscarparser.h"

namespace liboscar {

LoginService::LoginService(UIN uin, QString pass) { 
	m_uin = uin;
	m_pass = pass;
}

LoginService::~LoginService() { }

void LoginService::registerMeta() { 
	qRegisterMetaType<DisconnectReason>("DisconnectReason");
}

void LoginService::unexpectedDisconnect(QString reason, DisconnectReason error){
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
}

void LoginService::getBOSInfo(QString bos, QString port, QByteArray cookie) {
	emit BOSInfo(bos, port, cookie);
	m_conn->disconnect();
}

void LoginService::create() {
	if (!m_parser)
		m_parser = new OscarParser();

	m_st1 = new LoginSt1Process(this, m_uin, m_pass);

	// Parser && LoginSt1Process conn
	QObject::connect(m_parser, SIGNAL(recvHello()), m_st1, SLOT(recvHello()));
	QObject::connect(m_parser, SIGNAL(receivedBOS(QString, QString, QByteArray)), m_st1, SLOT(recvBOS(QString, QString, QByteArray)));

	// Service && LoginSt1Process conn
	QObject::connect(m_st1, SIGNAL(stage1finished(QString, QString, QByteArray)), this, SLOT(getBOSInfo(QString, QString, QByteArray)), Qt::DirectConnection);

	// Parser && LoginSt1Process conn
	QObject::connect(m_parser, SIGNAL(serverDisconnected(QString, DisconnectReason)), 
				this, SLOT(unexpectedDisconnect(QString, DisconnectReason)));
}

}

#include "loginservice.moc"
