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
	m_timestamp = 0x00000000;
}

Roster::~Roster() { 
	qDeleteAll(m_data); // delete the contacts
}

void Roster::addContact(Contact* contact) {
	m_data.append(contact);
}

bool Roster::delContact(Contact* contact) {
	for (int i = 0; i < m_data.size(); ++i) {
	     if (m_data.at(i) == contact){
		     m_data.removeAt(i);
		     return true; //found
	     }
	}
	return false; // not found
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

QList<Contact *>& Roster::getContacts() {
	return m_data;
}

void Roster::setGroupMap(GroupMap groups){
	m_groups = groups;
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


}
