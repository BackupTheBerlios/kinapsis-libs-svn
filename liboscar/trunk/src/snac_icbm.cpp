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


#include "snac_icbm.h"
#include "tlv.h"

namespace liboscar {

	// SNAC_ICBM main class
SNAC_ICBM::SNAC_ICBM(Word command, bool raw) 
	: SNAC(SNAC_FAM_ICBM, command, raw) { }

SNAC_ICBM::~SNAC_ICBM(){ }
	
	// SvrICBMErr SNAC
SrvICBMErrSNAC::SrvICBMErrSNAC() 
	: SNAC_ICBM(ICBM_SRV_ICBM_ERR, true) { }

SrvICBMErrSNAC::~SrvICBMErrSNAC() { }

ICBMError SrvICBMErrSNAC::getError(){
	return m_err;
}

void SrvICBMErrSNAC::parse(Buffer& b) {
	Word err;

	b >> err;
	switch (err) {
		case 0x0004:
			m_err = USER_OFFLINE;
			break;
		case 0x0009:
			m_err = CLIENT_NOT_SUPPORT_TYPE_2;
			break;
		case 0x000e:
			m_err = MALFORMED_PACKET;
			break;
	}

	b.removeFromBegin();
}

	// SvrReplyICBM SNAC
SrvReplyICBMSNAC::SrvReplyICBMSNAC()
	: SNAC_ICBM(ICBM_SRV_REPLYICBM, true) { }

SrvReplyICBMSNAC::~SrvReplyICBMSNAC() { }

void SrvReplyICBMSNAC::parse(Buffer &b) {
	Word data;

	b >> data; // Unknown
	b >> data; // Unknown
	b >> data; // Unknown
	b >> data; // Unknown
	b >> data; // Unknown
	b >> data; // Unknown
	b >> data; // Unknown
	b >> data; // Unknown

	b.removeFromBegin();
}

	// SvrRecvMsg SNAC
SrvRecvMsg::SrvRecvMsg() 
	: SNAC_ICBM(ICBM_SRV_RECVMSG, true) { }

SrvRecvMsg::~SrvRecvMsg() { }

Message SrvRecvMsg::getMessage() {
	return m_msg;
}

void SrvRecvMsg::parse(Buffer &b) {
	m_msg.parse(b);
}

	// SvrMissedICBM SNAC
SrvMissedICBMSNAC::SrvMissedICBMSNAC()
	: SNAC_ICBM(ICBM_SRV_MISSEDICBM, true) { }

SrvMissedICBMSNAC::~SrvMissedICBMSNAC() { }

UIN SrvMissedICBMSNAC::getUin() {
	return m_info.getUin();
}

Word SrvMissedICBMSNAC::getNumberLost() {
	return m_nlost;
}

LostReason SrvMissedICBMSNAC::getReason() {
	return m_reason;
}


void SrvMissedICBMSNAC::parse(Buffer &b) {
	Word w;
	UnformattedTLV tlv(TLV_TYPE_GENERIC);

	b >> w;
	
	m_info.parse(b);

	b >> m_nlost;
	b >> w;

	switch (w){
		default:
		case 0x00:
			m_reason = MESSAGE_INVALID;
			break;
		case 0x01:
			m_reason = MESSAGE_TOO_LARGE;
			break;
		case 0x02:
			m_reason = MESSAGE_RATE_EXCEEDED;
			break;
		case 0x03:
			m_reason = SENDER_TOO_EVIL;
			break;
		case 0x04:
			m_reason = CLIENT_TOO_EVIL;
			break;
	}

}

	// SvrAckMsg SNAC
SrvAckMsgSNAC::SrvAckMsgSNAC()
	: SNAC_ICBM(ICBM_SRV_SRVACKMSG, true) { }

SrvAckMsgSNAC::~SrvAckMsgSNAC() { }

void SrvAckMsgSNAC::parse(Buffer &b) { 
	DWord dw;
	Word w;

	b >> dw;
	b >> dw; // TODO: pick for ACK

	b >> w;

	m_format = w;

	m_uin.parse(b);
}

SrvCliTypingSNAC::SrvCliTypingSNAC() 
	: SNAC_ICBM(ICBM_SRV_CLI_TYPING, true) { }

SrvCliTypingSNAC::SrvCliTypingSNAC(UIN uin, IsTypingType type) 
	: SNAC_ICBM(ICBM_SRV_CLI_TYPING, true) {

	m_data << (DWord) 0x00000000;
	m_data << (DWord) 0x00000000;
	m_data << (Word) 0x0001; // always Ch1
	uin.appendUin(m_data);
	m_data << (Word) type;
}

SrvCliTypingSNAC::~SrvCliTypingSNAC() { }

IsTypingType SrvCliTypingSNAC::getType(){
	return m_type;
}

UIN SrvCliTypingSNAC::getUin(){
	return m_uin;
}

void SrvCliTypingSNAC::parse(Buffer &b) { 
	if (b.len()<=12)
		return;

	Word w;
	b.advance(10); // id and channel
	
	m_uin.parse(b);
	b >> w; m_type = (IsTypingType) w;
}


	// 
	// Client's SNACs
	//
	

	// CliSetICBM SNAC
CliSetICBMSNAC::CliSetICBMSNAC()
	: SNAC_ICBM(ICBM_CLI_SETICBM, true) {

	m_data << (Word) 0x0000;
	m_data << (DWord) 0x0000000b;
	m_data << (Word) 0x1f40;
	m_data << (Word) 0x03e7;
	m_data << (Word) 0x03e7;
	m_data << (Word) 0x0000;
	m_data << (Word) 0x0000;

}

CliSetICBMSNAC::~CliSetICBMSNAC() { }

	// CliReqICBM SNAC
CliReqICBMSNAC::CliReqICBMSNAC()
	: SNAC_ICBM(ICBM_CLI_REQICBM, true) { }

CliReqICBMSNAC::~CliReqICBMSNAC() { }

	// CliSendMsg SNAC
CliSendMsgSNAC::CliSendMsgSNAC(Message message)
	: SNAC_ICBM(ICBM_CLI_SENDMSG, false) {

	m_data << message.pack();
}

CliSendMsgSNAC::~CliSendMsgSNAC() { }

	// CliAckMsg SNAC
CliAckMsgSNAC::CliAckMsgSNAC()
	: SNAC_ICBM(ICBM_CLI_ACKMSG, true) {
	
	// TODO: all (we need message handling first)
}

CliAckMsgSNAC::~CliAckMsgSNAC() { }

Message CliAckMsgSNAC::getMessage(){
	return m_msg;
}

void CliAckMsgSNAC::parse(Buffer &b) {

	DWord dw;
	Word w, channel;
	Byte by;
	UIN uin;
	QString m;

	b >> dw; b >> dw;

	b >> channel;
	m_msg.setFormat(channel);

	uin.parse(b);
	m_msg.setUin(uin);

	b >> m_reason;
	
	switch (channel){
		case 0x0001:
			// TODO
			break;
		case 0x0002:
			b.setLittleEndian();
			b >> w; b.advance(w);
			b >> w; b.advance(w);
			b >> by; m_msg.setType(Message::byteToType(by));
			b >> by; m_msg.setFlags(Message::byteToFlags(by));
			b.advance(4);
			b.readString(m); 
			m.truncate(m.length() - 1); // Null terminated string
			m_msg.setText(m);
			b.removeFromBegin();
			break;
		default:
			qDebug("Unknown channel in SNAC 0x04,0x0B"); 
			break;
	}

}


}
