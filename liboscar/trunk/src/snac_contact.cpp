/***************************************************************************
 *   Copyright (C) 2006 by Luis Cidoncha                                   *
 *   luis.cidoncha@gmail.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "snac_contact.h"
#include "tlv.h"

namespace liboscar {

	// SNAC_Contact main class
SNAC_Contact::SNAC_Contact(Word command, bool raw) 
	: SNAC(SNAC_FAM_CONTACT, command, raw) { }

SNAC_Contact::~SNAC_Contact(){ }
	
	// SvrContactErr SNAC
SrvContactErrSNAC::SrvContactErrSNAC() 
	: SNAC_Contact(CONTACT_SRV_CONTACT_ERR, true) { }

SrvContactErrSNAC::~SrvContactErrSNAC() { }

ContactError SrvContactErrSNAC::getError(){
	return m_err;
}

void SrvContactErrSNAC::parse(Buffer& b) {
	Word err;
	Word count;
	UIN uin;
	
	b >> err;
	switch (err) {
		case 0x000e:
			m_err = EMPTY_UIN_LIST;
			break;
		case 0x0014:
			m_err = UIN_NOT_IN_LIST;
			break;
		case 0x0015:
			m_err = CONTACT_LIST_FULL;
			break;
	}

	b.removeFromBegin();

	b >> count;
	while (count--)
		uin.parse(b);
}

	// SrvReplyBuddy SNAC
SrvReplyBuddySNAC::SrvReplyBuddySNAC()
	: SNAC_Contact(CONTACT_SRV_CONTACT_ERR, false) {
	m_maxuins = 0;
	m_maxwatchers = 0;
}

SrvReplyBuddySNAC::~SrvReplyBuddySNAC() {}

Word SrvReplyBuddySNAC::getMaxUins() {
	return m_maxuins;
}

Word SrvReplyBuddySNAC::getMaxWatchers() {
	return m_maxwatchers;
}

void SrvReplyBuddySNAC::parse(Buffer &b) {
	UnformattedTLV tlv(TLV_TYPE_GENERIC);

	tlv.parse(b); // Max UINS
	tlv.data() >> m_maxuins;
	tlv.parse(b); // Max Watchers
	tlv.data() >> m_maxwatchers;

	tlv.parse(b); // Unknown data
}

	// SrvRefused SNAC
SrvRefusedSNAC::SrvRefusedSNAC()
	: SNAC_Contact(CONTACT_SRV_REFUSED, false) {}

SrvRefusedSNAC::~SrvRefusedSNAC() {}

UIN SrvRefusedSNAC::getUin() {
	return m_uin;
}

void SrvRefusedSNAC::parse(Buffer &b) {
	m_uin.parse(b);
}

	// SrvUserOnline SNAC
SrvUserOnlineSNAC::SrvUserOnlineSNAC()
	: SNAC_Contact(CONTACT_SRV_USERONLINE, false) {}

SrvUserOnlineSNAC::~SrvUserOnlineSNAC() {}

PresenceStatus SrvUserOnlineSNAC::getStatus(){
	return m_info.getStatus();
}

UIN SrvUserOnlineSNAC::getUin(){
	return m_info.getUin();
}

void SrvUserOnlineSNAC::parse(Buffer &b) {
	// FIXME: some TLVs maybe are not getting properly parsed
	m_info.parse(b);
}

	// SrvUserOffline SNAC
SrvUserOfflineSNAC::SrvUserOfflineSNAC()
	: SNAC_Contact(CONTACT_SRV_USEROFFLINE, false) {}

SrvUserOfflineSNAC::~SrvUserOfflineSNAC() {}

UIN SrvUserOfflineSNAC::getUin() {
	return m_uin;
}

void SrvUserOfflineSNAC::parse(Buffer &b) {
	UnformattedTLV tlv(TLV_TYPE_GENERIC);
	DWord data;

	m_uin.parse(b);

	b >> data; // Unknown
	tlv.parse(b);
}

	// 
	// Client's SNACs
	//
	

	// CliReqBuddy SNAC
CliReqBuddySNAC::CliReqBuddySNAC() 
	:SNAC_Contact (CONTACT_CLI_REQBUDDY, true) { }

CliReqBuddySNAC::~CliReqBuddySNAC() { }

	// CliAddContact SNAC
CliAddContactSNAC::CliAddContactSNAC(UIN uin)
	:SNAC_Contact (CONTACT_CLI_ADDCONTACT, true) { 

	uin.appendUin(m_data);
}

CliAddContactSNAC::~CliAddContactSNAC() { }

	// CliRemContact SNAC
CliRemContactSNAC::CliRemContactSNAC(UIN uin)
	:SNAC_Contact (CONTACT_CLI_REMCONTACT, true) { 
		
	uin.appendUin(m_data);
}

CliRemContactSNAC::~CliRemContactSNAC() { }

}
