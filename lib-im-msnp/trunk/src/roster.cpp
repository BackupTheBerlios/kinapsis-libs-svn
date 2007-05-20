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
}

Roster::~Roster() { 
	printf ("MSN::Roster::log ## Roster deleted\n");
}

void Roster::addContact(Contact* c) {
	m_contacts[c->getPassport()] = Contact ();
	if (c->getPassport() != "") m_contacts[c->getPassport()].setPassport (c->getPassport());
	if (c->getNickName() != "") m_contacts[c->getPassport()].setNickName (c->getNickName());
	if (c->getId() != "") m_contacts[c->getPassport()].setId (c->getId());
}
Contact*  Roster::getContact (QString passport) {
	return &m_contacts[passport];
}

Group*  Roster::getGroup (QString groupName) {
	return &m_groups[groupName];
}

bool Roster::delContact(QString c) {
	m_contacts.remove(c);
}

void Roster::addGroup (Group* g){
	m_groups[g->getName()] = Group (g->getName(), g->getId());
}

bool Roster::delGroup (QString g){
	m_groups.remove(g);
	// TODO : Choose to del all contacts in this group or move to default group
}

unsigned int Roster::contactLen() {
	return m_contacts.size();
}

unsigned int Roster::groupLen() {
	return m_groups.size();
}
QString Roster::getGroupName(QString id) {
	gIt it;
	for ( it = m_groups.begin(); it != m_groups.end(); ++it ) {
		if (it.value().getId() == id) return it.value().getName();
	}
	return "";
}

QString Roster::getGroupId (QString groupName){
	if (m_groups[groupName].getId() != "" ) return m_groups[groupName].getId();
	else return "";
}

}
