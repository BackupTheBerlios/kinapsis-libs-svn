/*
  Copyright (c) 2006 by Pedro Arnedo <vaticano666@gmail.com>
  This file is part of the lib-im-msnp library.
  https://developer.berlios.de/projects/kinapsis-libs/

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/

#include "roster.h"

namespace libimmsnp {

Roster::Roster() { 
	m_contacts.setAutoDelete(true);
	m_groups.setAutoDelete(true);
}

Roster::~Roster() { }

void Roster::addContact(Contact* contact) {
	m_contacts.append(contact);
}

bool Roster::delContact(Contact* contact) {
	return m_contacts.remove(contact);
}

void Roster::addGroup (Group* group){
	m_groups.append(group);
}

//bool Roster::delGroup (Group* group){
//	// TODO : Choose to del all contacts in this group or move to default group
//}

unsigned int Roster::contactLen() {
	return m_contacts.count();
}

unsigned int Roster::groupLen() {
	return m_groups.count();
}

QPtrList<Contact>& Roster::getContacts() {
	return m_contacts;
}

QPtrList<Group>& Roster::getGroups(){
	return m_groups;
}

}
#include "roster.moc"
