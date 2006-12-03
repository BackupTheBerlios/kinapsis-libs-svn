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

#ifndef _CONNECTIONLISTENER_H_
#define _CONNECTIONLISTENER_H_

#include <qobject.h>
#include "libimmsnp.h"

namespace libimmsnp {

class ConnectionListener : public QObject{
Q_OBJECT
public:
	ConnectionListener();
	virtual ~ConnectionListener();

	virtual void onConnect() = 0;
	virtual void onDisconnect(ConnectionError e) = 0;

public slots:
	void connectSlot();
	void disconnectSlot(ConnectionError e);
};


}

#endif // _CONNECTIONLISTENER_H_
