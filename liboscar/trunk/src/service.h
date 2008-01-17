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


#ifndef _SERVICE_H_
#define _SERVICE_H_

#include "buffer.h"
#include "parser.h"
#include "connection.h"
#include "connectionresult.h"
#include <qthread.h>
#include <qmetatype.h>

namespace liboscar {

class Service : public QThread {
Q_OBJECT

public:
	Service();
	virtual ~Service();

	void connect(QString server, int port);
	void connect(int port);

	void send(Buffer& b);

	unsigned int getId();

signals:
	void serviceEnded(unsigned int, ConnectionResult);

public slots:
	virtual void handleConnect();
	void handleDisconnect();
	void handleConnError(SocketError);

protected:

	void run();

	virtual void create() = 0;

	Connection* m_conn;
	Parser* m_parser;

	QString m_server;
	int m_port;

	DisconnectReason m_reason;

private slots:
	void finishedSlot();

private:

	virtual void registerMeta() = 0;

	SocketError m_err;

	unsigned int m_id;

	static unsigned int m_seq;
};

}

#endif // _SERVICE_H_
