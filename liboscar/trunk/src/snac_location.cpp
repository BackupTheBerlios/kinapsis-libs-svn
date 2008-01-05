/***************************************************************************
 *   Copyright (C) 2005-2008 by Luis Cidoncha                              *
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


#include "snac_location.h"
#include "tlv.h"
#include "errortlv.h"
#include "capabilitiestlv.h"

namespace liboscar {

	// SNAC_Location main class
SNAC_Location::SNAC_Location(Word command, bool raw) 
	: SNAC(SNAC_FAM_LOCATION, command, raw) { }

SNAC_Location::~SNAC_Location(){ }
	
	// SrvLocationErr SNAC
SrvLocationErrSNAC::SrvLocationErrSNAC() 
	: SNAC_Location(LOCATION_SRV_LOCATION_ERR, true) { }

SrvLocationErrSNAC::~SrvLocationErrSNAC() { }

bool SrvLocationErrSNAC::haveSubError() {
	return m_haveopt;
}

DisconnectReason SrvLocationErrSNAC::getSubError() {
	return m_opt;
}

SNACError SrvLocationErrSNAC::getError(){
	return m_err;
}

void SrvLocationErrSNAC::parse(Buffer& b) {
	Word err;
	ErrorTLV t;
	
	b >> err;
	switch (err) {
		case 0x0001:
			m_err = ERROR_HEADER;
			break;
		case 0x0002:
			m_err = ERROR_RATE_SERVER;
			break;
		case 0x0003:
			m_err = ERROR_RATE_CLIENT;
			break;
		case 0x0004:
			m_err = ERROR_OFFLINE;
			break;
		case 0x0005:
			m_err = ERROR_SERVICE_NA;
			break;
		case 0x0006:
			m_err = ERROR_SERVICE_UNDEF;
			break;
		case 0x0007:
			m_err = ERROR_OBSOLETE;
			break;
		case 0x0008:
			m_err = ERROR_UNSUP_SERVER;
			break;
		case 0x0009:
			m_err = ERROR_UNSUP_CLIENT;
			break;
		case 0x000a:
			m_err = ERROR_REFUSED_CLIENT;
			break;
		case 0x000b:
			m_err = ERROR_REPLY_BIG;
			break;
		case 0x000c:
			m_err = ERROR_RESPONSE_LOST;
			break;
		case 0x000d:
			m_err = ERROR_REQ_DENIED;
			break;
		case 0x000e:
			m_err = ERROR_SNAC;
			break;
		case 0x000f:
			m_err = ERROR_RIGHTS;
			break;
		case 0x0010:
			m_err = ERROR_BLOCKED;
			break;
		case 0x0011:
			m_err = ERROR_EVIL_SENDER;
			break;
		case 0x0012:
			m_err = ERROR_EVIL_RECIP;
			break;
		case 0x0013:
			m_err = ERROR_RECIP_UNAVAILABLE;
			break;
		case 0x0014:
			m_err = ERROR_NO_MATCH;
			break;
		case 0x0015:
			m_err = ERROR_LIST_OVER;
			break;
		case 0x0016:
			m_err = ERROR_REQ_AMBIGUOUS;
			break;
		case 0x0017:
			m_err = ERROR_QUEUE_FULL;
			break;
		case 0x0018:
			m_err = ERROR_IS_AOL;
			break;
	}

	b.removeFromBegin();

	if ((m_haveopt = b.len())){
		b >> err; b >> err;
		t.parse(b);
		m_opt = t.getError();
	}
}

	// SrvReplyLocationSNAC
	
SrvReplyLocationSNAC::SrvReplyLocationSNAC() 
	: SNAC_Location(LOCATION_SRV_REPLYLOCATION, false) { 
	m_maxcap = 0x0010; // Default value	
}

SrvReplyLocationSNAC::~SrvReplyLocationSNAC() { }

Word SrvReplyLocationSNAC::getMaxCap() {
	return m_maxcap;
}

void SrvReplyLocationSNAC::parse(Buffer &b) {
	UnformattedTLV tlv(TLV_TYPE_GENERIC); // Let's parse
	tlv.parse(b); // Unknown data
	tlv.parse(b);
	tlv.data() >> m_maxcap;
	tlv.parse(b); // Unknown data
	tlv.parse(b); // Unknown data
}

	// SrvUserInfoSNAC
	
SrvUserInfoSNAC::SrvUserInfoSNAC()
	: SNAC_Location(LOCATION_SRV_USERINFO, true) { }

SrvUserInfoSNAC::~SrvUserInfoSNAC() { }

InfoRequestType SrvUserInfoSNAC::getType(){
	return m_type;
}

QString SrvUserInfoSNAC::getAwayMessage(){
	if (m_type == AWAY_MESSAGE)
		return m_away;
	else
		return "";
}

QString SrvUserInfoSNAC::getProfile(){
	if (m_type == GENERAL_INFO)
		return m_profile;
	else
		return "";
}

UserInfo SrvUserInfoSNAC::getUserOnlineInfo(){
	return m_info;
}

void SrvUserInfoSNAC::parse(Buffer &b) {
	UnformattedTLV tlv(TLV_TYPE_GENERIC);

	m_info.parse(b);
	b.removeFromBegin();

	if (!b.len()){
		m_type = SHORT_USER_INFO;
		return ;
	}

	while (b.len()){
		tlv.parse(b);
		switch(tlv.getType()){
			case 0x0001:
				m_type = GENERAL_INFO;
				break;
			case 0x0002:
				tlv.data().readString(m_profile);
				break;
			case 0x0003:
				m_type = AWAY_MESSAGE;
				break;
			case 0x0004:
				tlv.data().readString(m_away, tlv.len());
				break;
			case 0x0005:
				m_type = CAPABILITIES;
				// TODO handle stuff
				break;
			default:
				qDebug("Unknown TLV in server UserInfo reply.");
		}
		b.removeFromBegin();
	}

	return ;
}

	// 
	// Client's SNACs
	//
	
	//CliReqLocationSNAC
	
CliReqLocationSNAC::CliReqLocationSNAC() 
	: SNAC_Location(LOCATION_CLI_REQLOCATION, true) { }

CliReqLocationSNAC::~CliReqLocationSNAC() { }

	//CliSetUserInfoSNAC

CliSetUserInfoSNAC::CliSetUserInfoSNAC(Capabilities cap, QString awaymessage)
	: SNAC_Location(LOCATION_CLI_SETUSERINFO, false) {

	// TODO: some AIM TLVs here (0x0001, 0x0002, 0x0003)

	UnformattedTLV *ut = new UnformattedTLV(TLV_TYPE_DESCRIPTION);
	ut->data() << awaymessage;
	if (awaymessage.length())
		ut->data() << (Byte) 0x00;

	CapabilitiesTLV *t = new CapabilitiesTLV(0);
	t->setCapabilities(cap);

	addTLV(ut);
	addTLV(t);
}

CliSetUserInfoSNAC::~CliSetUserInfoSNAC() { }

	// CliReqUserInfoSNAC
	
CliReqUserInfoSNAC::CliReqUserInfoSNAC(UIN uin, InfoRequestType type)
	: SNAC_Location(LOCATION_CLI_REQUSERINFO, true) {

	switch (type) {
		case SHORT_USER_INFO:
			m_data << (Word) 0x0002;
			break;
		case AWAY_MESSAGE:
			m_data << (Word) 0x0003;
			break;
		case CAPABILITIES:
			m_data << (Word) 0x0004;
			break;
		default:
		case GENERAL_INFO:
			m_data << (Word) 0x0001;
			break;
	}

	m_data << (Byte) uin.getId().length();
	m_data << uin.getId();
}

CliReqUserInfoSNAC::~CliReqUserInfoSNAC() { }
	

}
