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


#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "liboscar.h"
#include "connectionresult.h"
#include "parser.h"
#include "buffer.h"
#include <qstring.h>
#include <qobject.h>
#include <qtcpsocket.h>
#include <netdb.h>

namespace liboscar {

	class Buffer;

class Connection : public QObject {
Q_OBJECT

public:
	Connection(const QString server, int port, Parser* parser);
	virtual ~Connection();

	void connect();

	void send(Buffer& b);

	static Word getNextSeqNumber();

	void disconnect();

	void changeParser(Parser* nparser);

public slots:
	void handleError(QAbstractSocket::SocketError);
	void handleConnect();
	void handleDisconnect();
	void handleStateChanged(QAbstractSocket::SocketState);
	void read();

signals:
	// Auto connected to m_parser
	void dataReceived();
	// must be connected by using object
	void connError(SocketError);
	void connDisconnected();
	void connConnected();

private:
	QString m_server;
	int m_port;
	bool m_connected;

	QTcpSocket* m_socket;

	Parser* m_parser;

	static Word m_seq;

};

}

#endif // _CONNECTION_H_
