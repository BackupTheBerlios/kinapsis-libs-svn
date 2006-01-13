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

UIN SrvRecvMsg::getUin() {
	return m_info.getUin();
}

QString SrvRecvMsg::getMessage() {
	return m_msg;
}

void SrvRecvMsg::parse(Buffer &b) {

	DWord dw;
	Word w;
	Word len, type;
	Byte by;
	UnformattedTLV tlv(TLV_TYPE_GENERIC);

	b >> dw; // TODO: get stamp to receive ACKs
	b >> dw;

	b >> w;
	m_format = (MessageFormat) w;

	m_info.parse(b);

	// Fixed part parsed. Let's go to format specifics
	
	b >> dw; //TLV header
	switch (m_format) {
		case 0x0001:
			tlv.parse(b); // Capabilities

			b >> type;
			b >> len;

			len -= 4; // Two words

			b >> w;
			switch (w){
				case 0x0000:
					m_encoding = ASCII;
					break;
				case 0x0002:
					m_encoding = UCS2BE;
					break;
				case 0x0003:
					m_encoding = LOCAL;
					break;
			}
			b >> w;

			if (m_encoding == UCS2BE){
				len /=2; // Reading Words

				Word strucs2[len];
				int i = 0;

				while (len--)
					b >> strucs2[i++];

				m_msg = QString::fromUcs2(strucs2);
			}
			else {
				while (len--){
					b >> by;
					m_msg.append(by);
				}
			}
			qDebug("Message: " + m_msg);
			b.removeFromBegin();
			break;
		case 0x0002:
			// TODO: support type2 messages
			// XXX: make message class?
			break;
		case 0x0004:
			m_encoding = ASCII;
			b >> type;
			b >> len;
			b >> dw; // UIN in hex

			b >> by; // TODO: msg type and flags
			b >> by;

			b.setLittleEndian();
			b >> len;
			b.setBigEndian();

			while (len--){
				b >> by;
				m_msg.append(by);
			}
			b.removeFromBegin();
			break;
		default:
			qDebug("Unknown message format.");
			break;
	}
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

	m_format = (MessageFormat) w;

	m_uin.parse(b);
}


	// 
	// Client's SNACs
	//
	

	// CliSetICBM SNAC
CliSetICBMSNAC::CliSetICBMSNAC()
	: SNAC_ICBM(ICBM_CLI_SETICBM, true) {

	m_data << (Word) 0x0000;
	m_data << (Word) 0x0000;
	m_data << (Word) 0x0003;
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
CliSendMsgSNAC::CliSendMsgSNAC(UIN uin, QString message)
	: SNAC_ICBM(ICBM_CLI_SENDMSG, false) {

	UnformattedTLV *tlv;
	UnformattedTLV *tlvm;
	
	// FIXME: message types (make message class?)
	m_data << (DWord) 0x67c31501;
	m_data << (DWord) 0x3d3a3721;

	m_data << (Word) 0x0001; // Type-1
	uin.appendUin(m_data);

	tlvm = new UnformattedTLV(TLV_TYPE_PASSWORD);

	tlv = new UnformattedTLV(TLV_TYPE_CAPABILITIES2);
	tlv->data() << (Word) 0x0106;

	tlvm->data() << tlv->pack();
	delete tlv;

	tlv = new UnformattedTLV(TLV_TYPE_MESSAGE);
	tlv->data() << (Word) 0x0000; // ASCII
	tlv->data() << (Word) 0x0000; // Unknown
	tlv->data() << message;

	tlvm->data() << tlv->pack();
	delete tlv;

	addTLV(tlvm);
	tlv = new UnformattedTLV(TLV_TYPE_COOKIE);
	addTLV(tlv);
}

CliSendMsgSNAC::~CliSendMsgSNAC() { }

	// CliAckMsg SNAC
CliAckMsgSNAC::CliAckMsgSNAC()
	: SNAC_ICBM(ICBM_CLI_ACKMSG, true) {
	
	// TODO: all (we need message handling first)
}

CliAckMsgSNAC::~CliAckMsgSNAC() { }

}
