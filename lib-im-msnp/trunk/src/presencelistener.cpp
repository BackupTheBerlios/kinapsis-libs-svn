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

#include "presencelistener.h"

namespace libimmsnp {

PresenceListener::PresenceListener() { }

PresenceListener::~PresenceListener() { }

void PresenceListener::presenceChangedSlot(QString c, QString status, QString msnPersMsg, QString msnCapabilities) {
       	this->presenceChanged(c, status, msnPersMsg, msnCapabilities);
}

void PresenceListener::personalMessageSlot(QString c, QString msg) {
	this->personalMessage(c, msg);
}

//void PresenceListener::contactDisconnectedSlot(QString msnPassport) {
//	this->contactDisconnected(msnPassport);
//}

//void PresenceListener::awayMessageSlot(UIN uin, QString message) {
//	this->awayMessage(uin, message);
//}

}

#include "presencelistener.moc"
