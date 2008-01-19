/***************************************************************************
 *   Copyright (C) 2006-2007 by Luis Cidoncha                              *
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


#include "snac_roster.h"
#include "tlv.h"
#include <qmap.h>

namespace liboscar {

	// SNAC_Roster main class
SNAC_Roster::SNAC_Roster(Word command, bool raw) 
	: SNAC(SNAC_FAM_ROSTER, command, raw) { }

SNAC_Roster::~SNAC_Roster(){ }
	
	// SrvReplyListsSNAC
SrvReplyListsSNAC::SrvReplyListsSNAC()
	: SNAC_Roster(ROSTER_SRV_REPLYLISTS, true) { }

SrvReplyListsSNAC::~SrvReplyListsSNAC() { }

TLVChain SrvReplyListsSNAC::getTLVs() {
	return tch;
}

void SrvReplyListsSNAC::parse(Buffer &b) {
	tch.parse(b);
}

	// SrvReplyRosterSNAC
SrvReplyRosterSNAC::SrvReplyRosterSNAC()
	: SNAC_Roster ( ROSTER_SRV_REPLYROSTER, true) { }

SrvReplyRosterSNAC::~SrvReplyRosterSNAC() { }

void SrvReplyRosterSNAC::parse(Buffer &b) {
	m_roster.parse(b);
}

	// SrvUpdateAckSNAC
SrvUpdateAckSNAC::SrvUpdateAckSNAC()
	: SNAC_Roster ( ROSTER_SRV_UPDATEACK, true) {
	m_err = ACK_OK;	
}

SrvUpdateAckSNAC::~SrvUpdateAckSNAC() { }

RosterModifyAck SrvUpdateAckSNAC::getAck(){
	return m_err;
}

void SrvUpdateAckSNAC::parse(Buffer &b) {
	Word w;

	b >> w;
	b.removeFromBegin();

	m_err = (RosterModifyAck) w;
}

	// SrvReplyRosterOkSNAC
SrvReplyRosterOkSNAC::SrvReplyRosterOkSNAC()
	: SNAC_Roster ( ROSTER_SRV_REPLYROSTEROK, true) { }

SrvReplyRosterOkSNAC::~SrvReplyRosterOkSNAC() { }

void SrvReplyRosterOkSNAC::parse(Buffer &b) {
	DWord dw;
	Word w;

	b >> dw; b >> w;
	b.removeFromBegin();

}

	// SrvFutureGrantRecSNAC
SrvFutureGrantRecSNAC::SrvFutureGrantRecSNAC()
	: SNAC_Roster ( ROSTER_SRV_FUTUREGRANTREC, true) { }

SrvFutureGrantRecSNAC::~SrvFutureGrantRecSNAC() { }

UIN SrvFutureGrantRecSNAC::getUin() {
	return m_uin;
}

QString SrvFutureGrantRecSNAC::getReason() {
	return m_reason;
}

void SrvFutureGrantRecSNAC::parse(Buffer &b) {

	Word len;
	Byte by;

	m_uin.parse(b);

	b >> len;
	while (len--){
		b >> by;
		m_reason.append(by);
	}
	b >> len;
	b.removeFromBegin();
}

	// SrvAuthReqSNAC
SrvAuthReqSNAC::SrvAuthReqSNAC()
	: SNAC_Roster ( ROSTER_SRV_AUTHREQ, true) { }

SrvAuthReqSNAC::~SrvAuthReqSNAC() { }

UIN SrvAuthReqSNAC::getUin(){
	return m_uin;
}

QString SrvAuthReqSNAC::getReason(){
	return m_reason;
}

void SrvAuthReqSNAC::parse(Buffer &b) {
	Word len;
	Byte by;

	m_uin.parse(b);

	b >> len;
	while (len--){
		b >> by;
 		m_reason.append(by);
	}
	b >> len; // Unknown
	b.removeFromBegin();
}

	// SrvAuthReplySNAC
SrvAuthReplySNAC::SrvAuthReplySNAC()
	: SNAC_Roster ( ROSTER_SRV_AUTHREPLY, true) {
	m_granted = false;	
}

SrvAuthReplySNAC::~SrvAuthReplySNAC() { }

UIN SrvAuthReplySNAC::getUin(){
	return m_uin;
}

QString SrvAuthReplySNAC::getReason(){
	return m_reason;
}

bool SrvAuthReplySNAC::getGranted(){
	return m_granted;
}

void SrvAuthReplySNAC::parse(Buffer &b) {
	Byte by;
	Word len;

	m_uin.parse(b);
	b >> by;
	m_granted = (by == 0x01);

	b >> len;
	while (len--){
		b >> by;
 		m_reason.append(by);
	}
	b >> len;
	b.removeFromBegin();

}

	// SrvAddedYouSNAC
SrvAddedYouSNAC::SrvAddedYouSNAC()
	: SNAC_Roster ( ROSTER_SRV_ADDEDYOU, true) { }

SrvAddedYouSNAC::~SrvAddedYouSNAC() { }

UIN SrvAddedYouSNAC::getUin(){
	return m_uin;
}

void SrvAddedYouSNAC::parse(Buffer &b) {
	m_uin.parse(b);
}

	// 
	// Client's SNACs
	//
	
	// CliReqListsSNAC
CliReqListsSNAC::CliReqListsSNAC()
	: SNAC_Roster ( ROSTER_CLI_REQLISTS, true) { }

CliReqListsSNAC::~CliReqListsSNAC() { }

	// CliReqRosterSNAC
CliReqRosterSNAC::CliReqRosterSNAC()
	: SNAC_Roster(ROSTER_CLI_REQROSTER, true) { }

CliReqRosterSNAC::~CliReqRosterSNAC() { }

	//CliCheckRosterSNAC
CliCheckRosterSNAC::CliCheckRosterSNAC(Roster* roster)
	: SNAC_Roster(ROSTER_CLI_CHECKROSTER, true) {
	m_data << roster->getTimestamp();
	m_data << roster->len();
}

CliCheckRosterSNAC::~CliCheckRosterSNAC() { }

	//CliRosterAckSNAC
CliRosterAckSNAC::CliRosterAckSNAC()
	: SNAC_Roster(ROSTER_CLI_ROSTERACK, true) { }

CliRosterAckSNAC::~CliRosterAckSNAC() { }

	//CliRosterAddSNAC
CliRosterAddSNAC::CliRosterAddSNAC(SBLItem* i)
	: SNAC_Roster(ROSTER_CLI_ROSTERADD, false) {

	m_data << i->pack();
}
CliRosterAddSNAC::~CliRosterAddSNAC() { }

	//CliRosterUpdateSNAC
CliRosterUpdateSNAC::CliRosterUpdateSNAC(SBLItem* i)
	: SNAC_Roster(ROSTER_CLI_ROSTERUPDATE, false) {

	m_data << i->pack(); /* the item must have the correct info */
}
CliRosterUpdateSNAC::~CliRosterUpdateSNAC() { }

	//CliRosterDeleteSNAC
CliRosterDeleteSNAC::CliRosterDeleteSNAC(SBLItem* i)
	: SNAC_Roster(ROSTER_CLI_ROSTERDELETE, true) {
	m_data << i->pack();
}
CliRosterDeleteSNAC::~CliRosterDeleteSNAC() { }

	//CliAddStartSNAC
CliAddStartSNAC::CliAddStartSNAC()
	: SNAC_Roster(ROSTER_CLI_ADDSTART, true) { }

CliAddStartSNAC::~CliAddStartSNAC() { }

	//CliAddEndSNAC
CliAddEndSNAC::CliAddEndSNAC()
	: SNAC_Roster(ROSTER_CLI_ADDEND, true) { }

CliAddEndSNAC::~CliAddEndSNAC() { }

	//CliGrantAuthSNAC
CliGrantAuthSNAC::CliGrantAuthSNAC(UIN uin)
	: SNAC_Roster(ROSTER_CLI_GRANTAUTH, true) {
	uin.appendUin(m_data);
	m_data << (DWord) 0x00000000;
}

CliGrantAuthSNAC::~CliGrantAuthSNAC() { }

	//CliDelYourselfSNAC
CliDelYourselfSNAC::CliDelYourselfSNAC(UIN uin)
	: SNAC_Roster(ROSTER_CLI_DELYOURSELF, true) {
	uin.appendUin(m_data);
}

CliDelYourselfSNAC::~CliDelYourselfSNAC() { }

	//CliReqAuthSNAC
CliReqAuthSNAC::CliReqAuthSNAC(UIN uin, QString reason)
	: SNAC_Roster(ROSTER_CLI_REQAUTH, true) {
	uin.appendUin(m_data);
	m_data << (Byte) reason.length();
	m_data << reason;
	m_data << (Word) 0x0000;
}

CliReqAuthSNAC::~CliReqAuthSNAC() { }

	//CliAuthorizeSNAC
CliAuthorizeSNAC::CliAuthorizeSNAC(UIN uin, bool granted, QString reason)
	: SNAC_Roster(ROSTER_CLI_AUTHORIZE, true) {
	uin.appendUin(m_data);
	if (granted)
		m_data << (Byte) 0x01;
	else
		m_data << (Byte) 0x00;

	m_data << (Word) reason.length();
	m_data << reason;
	m_data << (Word) 0x0000;
}

CliAuthorizeSNAC::~CliAuthorizeSNAC() { }

}
