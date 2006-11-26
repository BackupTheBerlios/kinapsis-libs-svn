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

#include "contact.h"

namespace libimmsnp {

Contact::Contact() {
	m_msnname = "";
 	m_groupname = "";
	m_friendlyname = "";
	m_msnid = "";
	m_groupid = "";
	m_status = PresenceDIS; 
	m_persmsg = "";
	m_capabilities = "";
}

Contact::Contact(QString msnPassport, QString groupName, QString friendlyName, QString msnId, QString groupId) {
	m_msnname = msnPassport;
	m_groupname = groupName;
	m_friendlyname = friendlyName;
	m_msnid = msnId;
	m_groupid = groupId;
	m_status = PresenceDIS; 
	m_persmsg = "";
	m_capabilities = "";
}

Contact::~Contact() { }
	
void Contact::setMsnName(QString msnPassport) {
	m_msnname = msnPassport;
}

void Contact::setGroupName(QString groupName) {
	m_groupname = groupName;
}

void Contact::setFriendlyName(QString friendlyName) {
	m_friendlyname = friendlyName;
}

void Contact::setMsnId(QString msnId) {
	m_msnid = msnId;
}

void Contact::setGroupId(QString groupId) {
	m_groupid = groupId;
}

void Contact::setStatus(PresenceStatus status) {
	m_status = status;
}
	
void Contact::setPersMsg (QString persMsg){
	m_persmsg = persMsg;
}

void Contact::setCapabilities (QString msnCapabilities){
	m_capabilities = msnCapabilities;
}

// Now Group Class

Group::Group (QString groupName, QString groupId){
       m_name = groupName;
       m_id = groupId;
}

Group::~Group() {}
}
