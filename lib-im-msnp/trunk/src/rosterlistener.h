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

#ifndef _ROSTERLISTENER_H_
#define _ROSTERLISTENER_H_

#include <qobject.h>
#include "contact.h"

namespace libimmsnp {

class RosterListener : public QObject{
Q_OBJECT
public:
	RosterListener();
	virtual ~RosterListener();

	virtual void onNewContact(Contact *c) = 0;
	virtual void onNewGroup(Group *g) = 0;
	virtual void hasBlog(QString passport) = 0;

public slots:
	void onNewContactSlot(Contact *c);
	void onNewGroupSlot(Group *g);
	void hasBlogSlot(QString passport);
};

}

#endif // _ROSTERLISTENER_H_
