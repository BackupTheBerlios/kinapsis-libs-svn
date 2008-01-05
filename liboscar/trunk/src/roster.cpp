/***************************************************************************
 *   Copyright (C) 2006-2007 by Luis Cidoncha                              *
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
	qDeleteAll(m_data); // delete the contacts
}

//
// CONTACTS
//

void Roster::addContact(Contact* contact) {
	m_data.append(contact);
	Contact c(contact);
	emit contactAdded(contact);
}

bool Roster::delContact(Contact* contact) {
	UIN uin = contact->getUin();
	for (int i = 0; i < m_data.size(); ++i) {
		if (m_data.at(i) == contact){
			m_data.removeAt(i);
			emit contactDeleted(uin);
			return true; //found
		}
	}
	return false; // not found
}

//
// GROUPS
//

void Roster::addGroup(QString name, int gid){
	m_groups[name] = gid;
	emit groupAdded(name);
}

bool Roster::delGroup(QString name) {
}

bool Roster::delGroup(int gid ) {

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

QList<Contact *>& Roster::getContacts() {
	return m_data;
}

GroupMap Roster::getGroupMap(){
	return m_groups;
}

Contact* Roster::findContactById(Word id){
	int i = 0;

	for (i=0;i<m_data.size();i++)
		if (m_data[i]->getId() == id) break;

	if (i == m_data.size())
		return NULL;
	else
		return m_data[i];
}

Contact* Roster::findContactByUin(UIN uin){
	int i = 0;

	for (i=0;i<m_data.size();i++)
		if (m_data[i]->getUin().getId() == uin.getId()) break;

	if (i == m_data.size())
		return NULL;
	else
		return m_data[i];
}

void Roster::parse(Buffer &b){

	UnformattedTLV tlv(TLV_TYPE_GENERIC);
	Byte by;
	Word count, len, group, id, type;
	Word i;
	DWord time;
	Contact *c = 0;

	QMap<Byte, QString> rmap;

	QString name, nick;

	b.removeFromBegin();
	b.gotoBegin();
	b >> by; // 0x00 (¿?)

	b >> count;
	i = count;
	b.removeFromBegin();

	while (i--){
		name = ""; nick = "";
		b.readString(name);
		b.removeFromBegin();
		b >> group;
		b >> id;
		if (id == 0 && group != 0){ /* We have a group  (that's not master group) */
			addGroup(name, group);
			rmap[group] = name;
		}
		b >> type;
		b >> len;
		while (len){
			// TODO: handle more TLVs and groups and ...
			b.removeFromBegin();
			tlv.parse(b);
			switch (tlv.getType()){
				case 0x0131:
					tlv.data().gotoBegin();
					tlv.data().readString(nick, tlv.len());
					break;
				case 0x0066:
					c->setAuth(false);
					break;
				default:
					break;
			}
			if (type == 0x0000){
				c = new Contact();
				c->setUin(UIN(name));
				c->setNickname(nick);
				c->setGroup(rmap[group]);
				addContact(c);
			}
			len -= (tlv.data().len() + 4);
		}
	}
	
	b >> time;
	setTimestamp(time);

	b.removeFromBegin();
}

}

#include "roster.moc"
