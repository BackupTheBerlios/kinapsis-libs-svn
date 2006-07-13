/***************************************************************************
 *   Copyright (C) 2006 by Luis Cidoncha                                   *
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


#ifndef _ROSTER_H_
#define _ROSTER_H_

#include "liboscar.h"
#include "contact.h"
#include <qptrlist.h>
#include <qmap.h>

namespace liboscar {

	typedef QPtrList<Contact>::iterator ContactIterator;
	typedef QMap<QString, Byte> GroupMap;

class Roster {

public:
	Roster();
	virtual ~Roster();

	void addContact(Contact* contact);
	bool delContact(Contact* contact);

	void setTimestamp(DWord timestamp);

	DWord getTimestamp();

	void setGroupMap(GroupMap groups);

	unsigned int len();
	QPtrList<Contact>& getContacts();

	GroupMap getGroupMap();

	Contact* findContactById(Word id);
	Contact* findContactByUin(UIN uin);

private:
	QPtrList<Contact> m_data;
	GroupMap m_groups;

	DWord m_timestamp;

};

}

#endif // _ROSTER_H_
