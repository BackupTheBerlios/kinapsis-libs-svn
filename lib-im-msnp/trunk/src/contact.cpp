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
// Class Contact
// LST N=eve@passport.com F=Eavesdropper 12\r\n eve cannot see me.
// LST N=fred@passport.com F=Fred 8\r\n in reverse list but not on add or block list
//
// LST [N] -> passport = carol@passport.com
// LST [F] -> nickName = Carol
// ILN [] -> displayName = urlEncoded
// display_name : Principal's URL encoded display name
// lists = 1 = forward list | 13 = forward, block, and reverse lists| 12 | 
// groups = ... && ... && ...
// phones = home,work,...

Contact::Contact(){
	m_passport = "";		// LST [N=emily@passport.com] F=Emily C=8a99df6b-ae73-40bd-bdf7-05d0134be851 3 124153dc-a695-4f6c-93e8-8e07c9775251,...\r\n 
	m_groupName = "";		// LSG [Other%20Contacts] 124153dc-a695-4f6c-93e8-8e07c9775251\r\n 
	m_nickName = "";		// LST N=emily@passport.com [F=Emily] C=8a99df6b-ae73-40bd-bdf7-05d0134be851 3 124153dc-a695-4f6c-93e8-8e07c9775251,...\r\n 
	   					    // :Emily = the name other people see in their contactlist.
	m_id = "";			// ADC 10 AL N=emily@passport.com F=Emily [C=8a99df6b-ae73-40bd-bdf7-05d0134be851]\r\n
                                        // LST N=emily@passport.com F=Emily [C=8a99df6b-ae73-40bd-bdf7-05d0134be851] 3 124153dc-a695-4f6c-93e8-8e07c9775251,...\r\n  = PresenceDIS; 
	m_groupId = ""; 			//                                                                   
	m_status= STATUS_OFF;		// ILN 9 [BSY] emily@passport.com Emily 268435492
	m_displayName = "";		// ILN 9 BSY emily@passport.com [Emily%20The%20best] 268435492 !!urlEncoded¡¡                                 
	m_persMsg = ""; 		// UBX emily@passport.com 53\r\n[<Data><PSM></PSM><CurrentMedia></CurrentMedia></Data>]
	m_capabilities = "";		// ILN 9 BSY emily@passport.com Emily [268435492]
	m_list = "";     		// LST N=emily@passport.com F=Emily C=8a99df6b-ae73-40bd-bdf7-05d0134be851 [3] 124153dc-a695-4f6c-93e8-8e07c9775251,...\r\n 
}                       		

Contact::~Contact() { }
	
void Contact::setPassport(QString passport) {
	m_passport = passport;
}

void Contact::setGroupName(QString groupName) {
	m_groupName = groupName;
}

void Contact::setNickName(QString nickName) {
	m_nickName = nickName;
}

void Contact::setId(QString id) {
	m_id = id;
}

void Contact::setGroupId(QString groupId) {
	m_groupId = groupId;
}

void Contact::setStatus(State status) {
	m_status = status;
}
	
void Contact::setPersMsg (QString persMsg){
	m_persMsg = persMsg;
}

void Contact::setDisplayName (QString displayName){
	m_displayName = displayName;
}

void Contact::setCapabilities (QString msnCapabilities){
	m_capabilities = msnCapabilities;
}

void Contact::setList (QString list){
	// TODO: make an enumerate and convert the 'list' to that enumerate
	m_list = list;
}

// Now Group Class
// LSG Family 01eccd16-9f8a-425f-a36d-cbb73af6461e\r\n

Group::Group (){
	m_name= "";
	m_id = "";
}
Group::Group (QString groupName, QString groupId){
       m_name = groupName;
       m_id = groupId;
}

Group::~Group() {}
}
