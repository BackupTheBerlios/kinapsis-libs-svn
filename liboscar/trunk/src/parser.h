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


#ifndef _PARSER_H_
#define _PARSER_H_

#include "buffer.h"
#include "client.h"
#include "families.h"
#include "capabilities.h"
#include "roster.h"
#include "userinfo.h"
#include <qobject.h>
#include <qstring.h>

namespace liboscar {

	class Buffer;
	class Client;

class Parser : public QObject {
Q_OBJECT

public:
	Parser(Client *c);

	void add(Byte *data, int len);
	Word getNextSeqNumber();
	virtual ~Parser();

signals:
	void receivedBOS(QString server, QString port);
	void serverDisconnected(QString reason, DisconnectReason error);
	void loginSequenceFinished();
	void rosterInfo(Roster r);
	void newMessage(UIN uin, QString message);
	void statusChanged(UIN uin, PresenceStatus status);
	void newUin(UIN uin);
	void authReq(UIN uin, QString reason);

public slots:
	void parse();

private:
	void parseCh1(Buffer& buf);
	void parseCh2(Buffer& buf);
	void parseCh4(Buffer& buf);
	void parseCh5(Buffer& buf);

	void parseCh2Service(Buffer& buf);
	void parseCh2Location(Buffer& buf);
	void parseCh2Contact(Buffer& buf);
	void parseCh2ICBM(Buffer& buf);
	void parseCh2BOS(Buffer& buf);
	void parseCh2Interval(Buffer& buf);
	void parseCh2Roster(Buffer& buf);
	void parseCh2NewUser(Buffer& buf);

	void sendKeepAlive();

	Buffer m_buf;
	Word m_seq; /* FLAP's sequence number */
	Client *m_client;
	Families m_fam;
	Capabilities m_cap;

	Buffer m_cookie;

	bool m_inlogin;
};

}

#endif // _PARSER_H_
