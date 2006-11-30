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

#include "rosterlistener.h"

namespace libimmsnp {

RosterListener::RosterListener() { }

RosterListener::~RosterListener() { }

void RosterListener::onNewContactSlot(Contact *c){
	this->onNewContact(c);
}

void RosterListener::hasBlogSlot(QString passport){
	this->hasBlog(passport);
}

}

#include "rosterlistener.moc"
