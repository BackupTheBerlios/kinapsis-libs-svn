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
#include "parser.h"
#include "buffer.h"
#include <qstring.h>
#include <qobject.h>

namespace liboscar {

	class Parser;
	class Buffer;

class Connection : public QObject{
Q_OBJECT

public:
	Connection(const QString server, int port, Parser* parser);
	virtual ~Connection();

	ConnectionStatus getStatus();
	ConnectionStatus connect();
	ConnectionError listen();
	ConnectionError send(Buffer& b);
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
	bool m_exit;

	Parser* m_parser;
};

}

#endif // _CONNECTION_H_
