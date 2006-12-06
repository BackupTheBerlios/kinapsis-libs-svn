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

#ifndef _PRESENCELISTENER_H_
#define _PRESENCELISTENER_H_

#include <qobject.h>
#include "libimmsnp.h"
#include "contact.h"

namespace libimmsnp {

class PresenceListener : public QObject{
Q_OBJECT
public:
	PresenceListener();
	virtual ~PresenceListener();

	virtual void presenceChanged(Contact* c) = 0;
	virtual void personalMessage(Contact* c) = 0;
//	virtual void contactDisconnected(QString msnPassport) = 0;

//	virtual void awayMessage(Contact c, QString message) = 0;

public slots:
	void presenceChangedSlot(Contact* c);
	void personalMessageSlot(Contact* c);
//	void contactDisconnectedSlot(QString msnPassport);

//	void awayMessageSlot(QString c, QString message);
};


}

#endif // _PRESENCELISTENER_H_
