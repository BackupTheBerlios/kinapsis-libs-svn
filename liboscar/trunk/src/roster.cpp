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


#include "roster.h"

namespace liboscar {

Roster::Roster() { 
	m_data.setAutoDelete(true);
	m_timestamp = 0x00000000;
}

Roster::~Roster() { }

void Roster::addContact(Contact* contact) {
	m_data.append(contact);
}

bool Roster::delContact(Contact* contact) {
	return m_data.remove(contact);
}

void Roster::setTimestamp(DWord timestamp) {
	m_timestamp = timestamp;
}

DWord Roster::getTimestamp() {
	return m_timestamp;
}

unsigned int Roster::len() {
	return m_data.count();
}

QPtrList<Contact>& Roster::getContacts() {
	return m_data;
}

void Roster::setGroupMap(GroupMap groups){
	m_groups = groups;
}

GroupMap Roster::getGroupMap(){
	return m_groups;
}

Contact* Roster::findContactById(Word id){

	Contact* tmp;

	for (tmp = m_data.first(); tmp; tmp = m_data.next())
		if (tmp->getId() == id) break;

	return tmp;
}

Contact* Roster::findContactByUin(UIN uin){

	Contact* tmp;

	for (tmp = m_data.first(); tmp; tmp = m_data.next())
		if (tmp->getUin().getId() == uin.getId()) break;

	return tmp;
}


}
