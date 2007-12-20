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


#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "liboscar.h"
#include "connectionresult.h"
#include "parserbase.h"
#include "buffer.h"
#include <qstring.h>
#include <qobject.h>
#include <netdb.h>
#include <pthread.h>

namespace liboscar {

	class ParserBase;
	class Buffer;

class Connection : public QObject{
Q_OBJECT

public:
	Connection(const QString server, int port, ParserBase* parser);
	virtual ~Connection();

	ConnectionStatus getStatus();
	ConnectionStatus connect();
	ConnectionError listen();
	ConnectionError send(Buffer& b);

	DWord getLocalIP();
	Word getPort();

	void disconnect();

signals:
	void dataReceived();

private:
	ConnectionStatus clear();
	ConnectionError receive();

	ConnectionStatus m_status;

	QString m_server;
	int m_port;

	int m_socket;
	int m_socketLocal;

	bool m_exit;

	ParserBase* m_parser;

	struct sockaddr_in m_local;

	pthread_mutex_t m_rmutex;
	pthread_cond_t m_rcond;

};

}

#endif // _CONNECTION_H_
