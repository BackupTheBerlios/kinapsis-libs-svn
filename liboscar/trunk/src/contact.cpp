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


#include "contact.h"

namespace liboscar {

Contact::Contact() {
	m_nick = "";
	m_group = "";
	m_auth = true;
	m_visible = true;
}

Contact::Contact(Contact *c) {
	m_uin = c->getUin();
	m_id = c->getId();
	m_nick = c->getNickname();
	m_group = c->getGroup();
	m_auth = c->getAuth();
	m_visible = c->getVisible();
}

Contact::Contact(SBLItem *i, QString groupname) {
	m_uin = i->getUin();
	m_id = i->getItemId();
	m_group = groupname;

	if (i->getTLVs().exists(TLV_TYPE_NICK))
		i->getTLVs()[TLV_TYPE_NICK]->data().readString(m_nick, 
				i->getTLVs()[TLV_TYPE_NICK]->getLength());
}


Contact::Contact(UIN uin, QString nick, QString group) {
	m_uin = uin;
	m_nick = nick;
	m_group = group;
	m_auth = true;
	m_visible = true;
}

Contact::~Contact() { }
	
/*
void Contact::setUin(UIN uin) {
	m_uin = uin;
}

void Contact::setId(Word id) {
	m_id = id;
}

void Contact::setNickname(QString nick) {
	m_nick = nick;
}

void Contact::setGroup(QString group) {
	m_group = group;
}

void Contact::setAuth(bool value) {
	m_auth = value;
}

void Contact::setVisible(bool value) {
	m_visible = value;
}
*/

UIN Contact::getUin() { 
	return m_uin;
}

Word Contact::getId(){
	return m_id;
}

QString Contact::getNickname() {
	return m_nick;
}

QString Contact::getGroup() {
	return m_group;
}

bool Contact::getAuth() {
	return m_auth;
}

bool Contact::getVisible() {
	return m_visible;
}

}
