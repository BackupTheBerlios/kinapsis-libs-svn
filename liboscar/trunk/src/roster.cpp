/***************************************************************************
 *   Copyright (C) 2006-2008 by Luis Cidoncha                              *
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


#include "roster.h"
#include "tlv.h"

namespace liboscar {

Roster::Roster() { 
	m_timestamp = 0x00000000;
}

Roster::~Roster() { 
	qDeleteAll(m_data); // delete the items
}

void Roster::addItem(SBLItem* item) {
	m_data.append(item);
	m_idmap[item->getItemId()] = true; // id we can't use
	if (item->getType()== ITEM_BUDDY){
		Contact c(item, this->findItemById(item->getGroupId(), ITEM_GROUP)->getUin().getId());
		emit contactAdded(c);
	}
	else if (item->getType()== ITEM_GROUP)
		emit groupAdded(item->getUin().getId());
}

bool Roster::delItem(SBLItem* item) {
	UIN uin = item->getUin();
	SBLType type = item->getType();

	if (type != ITEM_GROUP) // don't remove groups id (because all of them have 0x0000)
		m_idmap.remove(item->getItemId());

	for (int i = 0; i < m_data.size(); ++i) {
		if (m_data.at(i) == item){
			m_data.removeAt(i);
			if (type == ITEM_BUDDY) 
				emit contactDeleted(uin);
			else if (type == ITEM_GROUP)
				emit groupDeleted(uin.getId());
			return true; //found
		}
	}
	return false; // not found
}

int Roster::getNextId() {
	int i = 0;

	while (m_idmap.contains(i)) i++;

	return i;
}

//
// OTHERS
//

void Roster::setTimestamp(DWord t) {
	m_timestamp = t;
}

DWord Roster::getTimestamp() {
	return m_timestamp;
}

unsigned int Roster::len() {
	return m_data.count();
}

QList<SBLItem *>& Roster::getItems() {
	return m_data;
}

SBLItem* Roster::findItemByUin(UIN uin, SBLType t) {
	int i = 0;
	for (i=0;i<m_data.size();i++)
		if ((m_data[i]->getUin().getId() == uin.getId()) && (m_data[i]->getType() == t)) break;

	if (i == m_data.size())
		return 0;
	else
		return m_data[i];
}

SBLItem* Roster::findItemByName(QString n, SBLType t) {
	return this->findItemByUin(UIN(n), t);
}

SBLItem* Roster::findItemById(int id, SBLType t) {
	int i = 0;
	if (t == ITEM_GROUP) { // all groups have id == 0, and gid
		for (i=0;i<m_data.size();i++)
			if ((m_data[i]->getGroupId() == id) && (m_data[i]->getType() == t)) break;
	}
	else {
		for (i=0;i<m_data.size();i++)
			if ((m_data[i]->getItemId() == id) && (m_data[i]->getType() == t)) break;
	}

	if (i == m_data.size())
		return 0;
	else
		return m_data[i];
}

void Roster::parse(Buffer &b){
	Byte by;
	Word count;
	DWord time;
	SBLItem *s = 0;

	b.removeFromBegin();
	b.gotoBegin();
	b >> by; // 0x00 (¿?)

	b >> count;
	b.removeFromBegin();

	while (count--){
		s = new SBLItem();
		s->parse(b);
		this->addItem(s);
	}
	
	b >> time;
	setTimestamp(time);

	b.removeFromBegin();
}

}

#include "roster.moc"
