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


#include "service.h"

namespace liboscar {

Service::Service() {
	m_conn = 0;
	m_parser = 0;
	m_port = -1;
	m_server = "";
	m_exitok = false;
	m_err = UNKNOWN_ERROR;

	QObject::connect(this, SIGNAL(finished()), this, SLOT(finishedSlot()));

	qRegisterMetaType<SocketError>("SocketError");
	qRegisterMetaType<ConnectionResult>("ConnectionResult");
}

Service::~Service() { }

void Service::connect(QString server, int port) {
	m_server = server;
	m_port = port;
	this->start();
}

void Service::send(Buffer& b) {
	if (m_conn)
		m_conn->send(b);
}

void Service::run(){
	qDebug("%d", QThread::currentThreadId());

	this->create(); // this MUST create m_parser
	this->registerMeta(); 
	
	if (!m_parser)
		this->exit(); // parser must be created in create()

	if (!m_conn)
		m_conn = new Connection(m_server, m_port, m_parser);

	QObject::connect(m_conn, SIGNAL(connError(SocketError)), this, SLOT(handleConnError(SocketError)));
	QObject::connect(m_conn, SIGNAL(connDisconnected()), this, SLOT(handleDisconnect()));
	QObject::connect(m_conn, SIGNAL(connConnected()), this, SLOT(handleConnect()));

	m_conn->connect();

	this->exec(); // event-driven
}

//
// SLOTS
//

void Service::handleConnect() {

}

void Service::handleDisconnect(){
	m_exitok = true;
	m_err = SOCK_NO_ERROR;
	this->exit();
}

void Service::handleConnError(SocketError e) {
	m_exitok = false;
	m_err = e;
	this->exit();
}

void Service::finishedSlot() {
	emit serviceEnded(ConnectionResult(m_exitok, m_err));
}

}

#include "service.moc"
