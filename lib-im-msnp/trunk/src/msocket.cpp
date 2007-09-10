/*
  Copyright (c) 2006 by Pedro Arnedo <vaticano666@gmail.com>
  This file is part of the lib-im-msnp library.
  https://developer.berlios.de/projects/kinapsis-libs/

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/

#include <QtNetwork>
#include "msocket.h"
#include <QDataStream>

namespace libimmsnp {

// Constructor
msocket::msocket (bool ssl) {
	if (ssl) {
		m_ssl = true;
		m_sslSocket = new QSslSocket(this);
	}
	else {
		m_ssl = false;
		m_socket = new QTcpSocket(this);
	}
}

// connection
int msocket::connect (QString host, quint16 port) {
	m_port = port;
	m_host = host;

	if (m_ssl) {
		m_sslSocket->abort();
		m_sslSocket->connectToHostEncrypted(m_host,m_port);

		if (m_sslSocket->waitForEncrypted(30000)) 
			qDebug ("MSN:: SSL Socket::CONNECTED to %s at %d", m_host.toUtf8().data(), m_port);
		else{
			qDebug ("ERROR::MSN:: SSL Socket::Connecting : %s", m_sslSocket->errorString().toUtf8().data());
			return 0;
		}

	}
	else {
		m_socket->abort();
		m_socket->connectToHost(m_host,m_port);

		if (m_socket->waitForConnected(30000)) 
			qDebug ("MSN::Socket::CONNECTED to %s at %d", m_host.toUtf8().data(), m_port);
		else{
			qDebug ("ERROR::MSN::Socket::Connecting : %s", m_socket->errorString().toUtf8().data());
			return 0;
		}
	}
}

int msocket::send (Buffer buf){
	if (m_ssl) {
		m_sslSocket->write (buf.data());
		if (m_sslSocket->waitForBytesWritten(30000)) {
			//qDebug ("MSN::SSL Socket::%s >>> : %s",m_host.toUtf8().data(),buf.dataDebug());
			}
		else
			qDebug ("ERROR::MSN::SSL Socket::%s >>> : %s",m_host.toUtf8().data(),buf.dataDebug());
	}
	else { 
		m_socket->write (buf.data());
		if (m_socket->waitForBytesWritten(30000)) {
			qDebug ("MSN::Socket::%s >>> : %s",m_host.toUtf8().data(),buf.dataDebug());
			}
		else
			qDebug ("ERROR::MSN::Socket::%s >>> : %s",m_host.toUtf8().data(),buf.dataDebug());
	}
}

int msocket::send (QString str){
	if (m_ssl) {
		m_sslSocket->write (str.toUtf8().data());
		if (m_sslSocket->waitForBytesWritten(30000)) {
			//qDebug ("MSN::SSL Socket::%s >>> %s",m_host.replace("\r\n","\\r\\n").toUtf8().data(),str.toUtf8().data());
			}
		else
			qDebug ("ERROR::MSN::SSL Socket::%s >>>",m_host.toUtf8().data());
	}
	else {
		m_socket->write (str.toUtf8().data());
		if (m_socket->waitForBytesWritten(30000)) {
			qDebug ("MSN::Socket::%s >>> %s",m_host.replace("\r\n","\\r\\n").toUtf8().data(),str.toUtf8().data());
			}
		else
			qDebug ("ERROR::MSN::Socket::%s >>>",m_host.toUtf8().data());
	}
}

int msocket::recv (Buffer& buf){
	QByteArray tmpData;
	tmpData.clear();
	if (m_ssl) {
		if (m_sslSocket->waitForReadyRead(30000)) {
			qint64 size = m_sslSocket->bytesAvailable();
			if ((tmpData = m_sslSocket->read(size)) != "") {
				buf.append(tmpData);
			}
			else {
				qDebug ("ERROR::MSN::SSL Socket::%s <<<",m_host.toUtf8().data());
			}
			//qDebug ("MSN::SSL Socket::%s ::len:%i <<< : %s",m_host.toUtf8().data(),tmpData.size() , tmpData.replace("\r\n","\\r\\n").data());
			return size;
		}
		else return -1;
	}
	else{
		if (m_socket->waitForReadyRead(-1)) {
			qint64 size = m_socket->bytesAvailable();
			if ((tmpData = m_socket->read(size)) != "") {
				buf.append(tmpData);
			}
			else {
				qDebug ("ERROR::MSN::Socket::%s <<<",m_host.toUtf8().data());
			}
				qDebug ("MSN::Socket::%s ::len:%i <<< : %s",m_host.toUtf8().data(),tmpData.size() , tmpData.replace("\r\n","\\r\\n").data());
			return size;
		}
		else return -1;
	}
}

msocket::~msocket () {
	qDebug("MSN::Socket::CLOSED %s",m_host.toUtf8().data());

}


}
#include "msocket.moc"
