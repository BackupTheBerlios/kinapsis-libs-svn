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

#include "connectionlistener.h"

namespace libimmsnp {

ConnectionListener::ConnectionListener() { }

ConnectionListener::~ConnectionListener() { }

void ConnectionListener::connectSlot(){
	this->onConnect();
}

void ConnectionListener::disconnectSlot(ConnectionError e){
	this->onDisconnect(e);
}


}

#include "connectionlistener.moc"
