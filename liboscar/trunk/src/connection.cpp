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


#include "connection.h"

#define POSITIVE_MASK 0x7fff

namespace liboscar {

	Word Connection::m_seq = (Word) POSITIVE_MASK * (rand()/RAND_MAX); /* Keep it positive */

Connection::Connection(const QString server, int port, Parser* parser){
	m_server = server;
	m_port = port;
	m_parser = parser;
	m_socket = new QTcpSocket();

	QObject::connect(this, SIGNAL(dataReceived()), m_parser, SLOT(parse()));

	m_type = CONN_OUTGOING;
	m_connected = false;
}

Connection::Connection(int port, Parser* parser){
	m_server = "";
	m_port = port;
	m_parser = parser;
	m_srv = new QTcpServer();

	QObject::connect(this, SIGNAL(dataReceived()), m_parser, SLOT(parse()));

	m_type = CONN_INCOMING;
	m_connected = false;
}

Connection::~Connection() { 
	delete m_socket;
}

//
// USER ACTIONS
//

void Connection::connect(){
	if (m_type == CONN_INCOMING) {
		// Incoming connection
		QObject::connect(m_srv, SIGNAL(newConnection()), this, SLOT(handleNewConn()));
		m_srv->listen(QHostAddress::Any, m_port);
	}
	else {
		// Outgoing connection
		this->doSocketConnections();
		m_socket->connectToHost(m_server, m_port);
	}
}

void Connection::send(Buffer &b){
	if (m_connected){
		QByteArray a = b.toByteArray();
		m_socket->write(a);
//#ifdef DEBUG
		unsigned int i;
		fprintf (stderr, "SENDING: ");
		for (i=0; i < a.size(); i++)
			fprintf(stderr, "%02x ",(unsigned char) a[i]);
		fprintf(stderr, "\n");
//#endif
	}
}

void Connection::disconnect(){
	if(m_connected)
		m_socket->disconnectFromHost(); // this will raise the disconnect signal
}

//
// OTHERS
//

void Connection::doSocketConnections(){
	QObject::connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleError(QAbstractSocket::SocketError)));
	QObject::connect(m_socket, SIGNAL(connected()), this, SLOT(handleConnect()));
	QObject::connect(m_socket, SIGNAL(disconnected()), this, SLOT(handleDisconnect()));
	QObject::connect(m_socket, SIGNAL(readyRead()), this, SLOT(read()));
	QObject::connect(m_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(handleStateChanged(QAbstractSocket::SocketState)));
}

Word Connection::getNextSeqNumber(){
	m_seq = ++m_seq & POSITIVE_MASK;
	return m_seq;
}

void Connection::changeParser(Parser* nparser){
	QObject::disconnect(this, 0, m_parser, 0);
	m_parser = nparser;
	QObject::connect(this, SIGNAL(dataReceived()), m_parser, SLOT(parse()));
}

// 
// SLOTS
//

void Connection::handleStateChanged(QAbstractSocket::SocketState s) {
#ifdef DEBUG
	qDebug("Connection state changed: %d", (int)s);
#endif
}

void Connection::handleError(QAbstractSocket::SocketError e) {
	emit connError((SocketError)e);	
}

void Connection::handleConnect() {
	m_connected = true;
	emit connConnected();
}

void Connection::handleDisconnect() {
	m_connected = false;
	emit connDisconnected();
}

void Connection::handleNewConn() {
	// Handles an incomint connection
	// Get socket, close listening conn, connect the new socket signals and
	// report connected
	m_socket = m_srv->nextPendingConnection();
	m_srv->close();
	this->doSocketConnections();
	this->handleConnect();
}

void Connection::read() {
	QByteArray arr;

	arr = m_socket->readAll();

//#ifdef DEBUG
	unsigned int i;
	fprintf (stderr, "RECEIVING: ");
	for (i=0; i < arr.size(); i++)
		fprintf(stderr, "%02x ", (unsigned char) arr[i]);
	fprintf(stderr, "\n");
//#endif

	m_parser->add(arr);
	emit dataReceived();

}

}

#include "connection.moc"
