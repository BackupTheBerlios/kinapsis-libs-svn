/***************************************************************************
 *   Copyright (C) 2007 by Luis Cidoncha                                   *
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


#ifndef _ROSTERPROCESS_H_
#define _ROSTERPROCESS_H_

#include "liboscar.h"
#include "uin.h"
#include "snac_roster.h"
#include <qlist.h>
#include <qmap.h>
#include <qobject.h>

namespace liboscar {

	enum RosterOp {
		ROSTEROP_NONE,
		ROSTEROP_ADD_CONTACT,
		ROSTEROP_DEL_CONTACT,
		ROSTEROP_MOD_CONTACT,
		ROSTEROP_ADD_GROUP,
		ROSTEROP_DEL_GROUP,
		ROSTEROP_MOD_GROUP,
	};

	class Client;

class RosterProcess : public QObject {
Q_OBJECT

public:
	RosterProcess(Client* c);
	virtual ~RosterProcess();

	bool addContact(UIN uin, QString groupname="", QString nickname="",  bool reqAuth=false);
	bool delContact(UIN uin);
	bool authorize(UIN uin, QString message, bool ack);

signals:
	void notifyAuthRequest(UIN uin, QString reason);
	void rosterAcked();

public slots:
	void rosterArrivedSlot(Buffer buf);
	void handleUpdateAck(RosterModifyAck a);
	void authReq(UIN uin, QString reason);
	void addedMe(UIN uin);

private:
	void reportSuccess();
	void startEdit();
	void endEdit();

	RosterOp m_op;
	UIN m_item;

	Contact* m_ocontact;

	Client* m_parent;
};

}

#endif // _ROSTERPROCESS_H_
