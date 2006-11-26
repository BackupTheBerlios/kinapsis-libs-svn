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

#ifndef _ROSTER_H_
#define _ROSTER_H_

#include "libimmsnp.h"
#include "contact.h"
#include <qptrlist.h>
#include <qobject.h>

namespace libimmsnp {

class Roster :public QObject{
Q_OBJECT

public:
	Roster();
	virtual ~Roster();

	void addContact(Contact* Contact);
	bool delContact(Contact* Contact);

	void addGroup(Group* group);
	bool delGroup(Group* group);
	
	unsigned int contactLen();
	unsigned int groupLen();
	
	QPtrList<Contact>& getContacts();
	QPtrList<Group>& getGroups();

private:
	QPtrList<Contact> m_contacts;
	QPtrList<Group> m_groups;
};

}

#endif // _ROSTER_H_
