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
#include <qmap.h>

namespace libimmsnp {

class Roster {

public:
	Roster();
	virtual ~Roster();
	void addContact(Contact* contact);
	bool delContact(QString contact);

	void addGroup(Group* group);
	bool delGroup(QString group);
	
	unsigned int contactLen();
	unsigned int groupLen();
	QString getGroupName(QString id);	
	
private:
	typedef QMap<QString, Contact> ContactMap;
	typedef QMap<QString, Group> GroupMap;
	ContactMap m_contacts;
	GroupMap m_groups;
};

}

#endif // _ROSTER_H_
