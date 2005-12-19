/***************************************************************************
 *   Copyright (C) 2005 by Luis Cidoncha                                   *
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


#include "snac_service.h"
#include "statustlv.h"
#include "servertlv.h"
#include "errortlv.h"
#include "directconnectiontlv.h"

namespace liboscar {

	// SNAC_Service main class
SNAC_Service::SNAC_Service(Word command, bool raw) 
	: SNAC(SNAC_FAM_SERVICE, command, raw) { }

SNAC_Service::~SNAC_Service(){ }
	
	// SvrServiceErr SNAC
SrvServiceErrSNAC::SrvServiceErrSNAC() 
	: SNAC_Service(SERVICE_SRV_SERVICE_ERR, true) { }

SrvServiceErrSNAC::~SrvServiceErrSNAC() { }

bool SrvServiceErrSNAC::haveSubError() {
	return m_haveopt;
}

DisconnectReason SrvServiceErrSNAC::getSubError() {
	return m_opt;
}

SNACError SrvServiceErrSNAC::getError(){
	return m_err;
}

void SrvServiceErrSNAC::parse(Buffer& b) {
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

	// SrvFamilies SNAC
SrvFamiliesSNAC::SrvFamiliesSNAC()
	: SNAC_Service(SERVICE_SRV_FAMILIES, true) { }

SrvFamiliesSNAC::~SrvFamiliesSNAC() { }

void SrvFamiliesSNAC::parse(Buffer &b) {

	b.wipe(); // FIXME: ignoring families
}

	// SrvRedirectSNAC
SrvRedirectSNAC::SrvRedirectSNAC()
	: SNAC_Service(SERVICE_SRV_REDIRECT, false) { }

SrvRedirectSNAC::~SrvRedirectSNAC() { }

void SrvRedirectSNAC::parse(Buffer &b) {
	b.wipe(); // TODO: redirect to service
}

	// SrvRatesSNAC
SrvRatesSNAC::SrvRatesSNAC()
	: SNAC_Service(SERVICE_SRV_RATES, true) { }

SrvRatesSNAC::~SrvRatesSNAC() { }

void SrvRatesSNAC::parse(Buffer &b){
	b.wipe(); // TODO: take the rate info
}

	// SrvRateExceededSNAC

SrvRateExceededSNAC::SrvRateExceededSNAC()
	: SNAC_Service(SERVICE_SRV_RATEEXCEEDED, true) { }

SrvRateExceededSNAC::~SrvRateExceededSNAC() { }

void SrvRateExceededSNAC::parse(Buffer &b){
	b.wipe();
}

	// SrvServerPauseSNAC

SrvServerPauseSNAC::SrvServerPauseSNAC()
	: SNAC_Service(SERVICE_SRV_SERVERPAUSE, true) { }

SrvServerPauseSNAC::~SrvServerPauseSNAC() { }

void SrvServerPauseSNAC::parse(Buffer &b){
	b.wipe();
}

	// SrvReplyInfoSNAC
	
SrvReplyInfoSNAC::SrvReplyInfoSNAC()
	: SNAC_Service(SERVICE_SRV_REPLYINFO, false) { }

SrvReplyInfoSNAC::~SrvReplyInfoSNAC() { }

void SrvReplyInfoSNAC::parse(Buffer &b){
	Word count, type, len;
	Byte by;
	StatusTLV st;
	DirectConnectionTLV dct;

	b >> by; //UIN's len
	b.advance(by); //Skip the UIN
	b.advance(2);
	b >> count;
	while (count--) {
		b >> type;
		b >> len;
		switch (type){
			case 0x0001:
			case 0x0002:
			case 0x0003:
			case 0x0005:
			case 0x000f:
			case 0x001e:
				b.advance(len); // We just don't care xD
				break;
			case 0x0006:
				st.parse(b);
				break;
			case 0x000c:
				dct.parse(b); // TODO: Relevant stuff here
				break;
			default:
				qDebug("Unknown TLV in ReplyInfo SNAC");
				b.advance(len);
				break;
		}
	}
}

	// SrvMigrationReqSNAC

SrvMigrationReqSNAC::SrvMigrationReqSNAC()
	: SNAC_Service(SERVICE_SRV_MIGRATIONREQ, false) { }

SrvMigrationReqSNAC::~SrvMigrationReqSNAC() { }

void SrvMigrationReqSNAC::parse(Buffer &b){
	ServerTLV servt;
	Word len;
	Byte by;
	unsigned int i=0;
	Buffer cookie;

	b.advance(2);
	servt.parse(b);
	b.advance(2);

	b >> len;
	cookie.gotoBegin();
	for (i=0; i < len; i++) {
		b >> by;
		cookie << by; 
	}

	// TODO:report migration and reconnect
	// handle cookie
}

	// SrvMOTDSNAC

SrvMOTDSNAC::SrvMOTDSNAC()
	: SNAC_Service(SERVICE_SRV_MOTD, false) { }

SrvMOTDSNAC::~SrvMOTDSNAC() { }

void SrvMOTDSNAC::parse(Buffer &b){
	b.wipe (); //Unknown content
}

	// SrvFamilies2SNAC

SrvFamilies2SNAC::SrvFamilies2SNAC()
	: SNAC_Service(SERVICE_SRV_FAMILIES2, true) { }

SrvFamilies2SNAC::~SrvFamilies2SNAC() { }

void SrvFamilies2SNAC::parse(Buffer &b){
	b.wipe (); // TODO: families stuff
}

	// 
	// Client's SNACs
	//
	
	// CliReady SNAC
CliReadySNAC::CliReadySNAC() 
	: SNAC_Service(SERVICE_CLI_READY, true) {

	m_data << (DWord) 0x00010003;
	m_data << (DWord) 0x0110047b;
	m_data << (DWord) 0x00130002;
	m_data << (DWord) 0x0110047b;
	m_data << (DWord) 0x00020001;
	m_data << (DWord) 0x0101047b;
	m_data << (DWord) 0x00030001;
	m_data << (DWord) 0x0101047b;
	m_data << (DWord) 0x00150001;
	m_data << (DWord) 0x0101047b;
	m_data << (DWord) 0x00040001;
	m_data << (DWord) 0x0101047b;
	m_data << (DWord) 0x00060001;
	m_data << (DWord) 0x0101047b;
	m_data << (DWord) 0x00090001;
	m_data << (DWord) 0x0101047b;
	m_data << (DWord) 0x000a0001;
	m_data << (DWord) 0x0101047b;
	m_data << (DWord) 0x000b0001;
	m_data << (DWord) 0x0101047b;

}

CliReadySNAC::~CliReadySNAC() { }

	// CliNewServiceSNAC

CliNewServiceSNAC::CliNewServiceSNAC(Word family)
	: SNAC_Service(SERVICE_CLI_NEWSERVICE, true) {

	m_data << family;
	m_family = family;
}

Word CliNewServiceSNAC::getFamily() {
	return m_family;
}

CliNewServiceSNAC::~CliNewServiceSNAC() { }

	// CliRatesRequestSNAC

CliRatesRequestSNAC::CliRatesRequestSNAC()
	: SNAC_Service(SERVICE_CLI_RATESREQUEST, true) { }

CliRatesRequestSNAC::~CliRatesRequestSNAC() { }

	// CliAckRatesSNAC

CliAckRatesSNAC::CliAckRatesSNAC()
	: SNAC_Service(SERVICE_CLI_ACKRATES, true) {

	m_data << (Word) 0x0001;
	m_data << (Word) 0x0002;
	m_data << (Word) 0x0003;
	m_data << (Word) 0x0004;
	m_data << (Word) 0x0005;

}

CliAckRatesSNAC::~CliAckRatesSNAC() { }

	// CliAckServerPauseSNAC

CliAckServerPauseSNAC::CliAckServerPauseSNAC()
	: SNAC_Service(SERVICE_CLI_ACKSERVERPAUSE, true) {

	// XXX: families stuff
	m_data << (Word) 0x0001;
	m_data << (Word) 0x0002;
	m_data << (Word) 0x0003;
	m_data << (Word) 0x0004;
	m_data << (Word) 0x0006;
	m_data << (Word) 0x0008;
	m_data << (Word) 0x0009;
	m_data << (Word) 0x000a;
	m_data << (Word) 0x000b;
	m_data << (Word) 0x000c;
	m_data << (Word) 0x0013;
	m_data << (Word) 0x0015;

}

CliAckServerPauseSNAC::~CliAckServerPauseSNAC() { }

	// CliReqInfoSNAC

CliReqInfoSNAC::CliReqInfoSNAC()
	: SNAC_Service(SERVICE_CLI_REQINFO, true) { }

CliReqInfoSNAC::~CliReqInfoSNAC() { }

	// CliSetIdleSNAC

CliSetIdleSNAC::CliSetIdleSNAC(bool allow)
	: SNAC_Service(SERVICE_CLI_SETIDLE, true) {

	m_allow = allow;

	if (allow)
		m_data << (DWord) 0x0000003c;
	else
		m_data << (DWord) 0x00000000;
}

bool CliSetIdleSNAC::getAllow() {
	return m_allow;
}

CliSetIdleSNAC::~CliSetIdleSNAC() { }

	// CliFamiliesSNAC 

CliFamiliesSNAC::CliFamiliesSNAC()
	: SNAC_Service(SERVICE_CLI_FAMILIES, true) {

		// XXX: families' stuff
	m_data << (DWord) 0x00010003;
	m_data << (DWord) 0x00130002;
	m_data << (DWord) 0x00020001;
	m_data << (DWord) 0x00030001;
	m_data << (DWord) 0x00150001;
	m_data << (DWord) 0x00040001;
	m_data << (DWord) 0x00060001;
	m_data << (DWord) 0x00090001;
	m_data << (DWord) 0x000A0001;
	m_data << (DWord) 0x000B0001;

}

CliFamiliesSNAC::~CliFamiliesSNAC() { }

	// CliSetStatusSNAC

CliSetStatusSNAC::CliSetStatusSNAC(PresenceStatus status, DWord ip, Word port, FirewallConfiguration firewall)
	: SNAC_Service(SERVICE_CLI_SETSTATUS, false) {

	StatusTLV* st;
	ErrorTLV* et;
	DirectConnectionTLV *dt;
	
	st = new StatusTLV();
	st->setStatus(status);
	this->addTLV(st);

	et = new ErrorTLV();
	et->setError(NO_ERROR);
	this->addTLV(et);

	dt = new DirectConnectionTLV();
	dt->setInfo(ip, port, firewall);
	this->addTLV(dt);

}

CliSetStatusSNAC::~CliSetStatusSNAC() { }


}
