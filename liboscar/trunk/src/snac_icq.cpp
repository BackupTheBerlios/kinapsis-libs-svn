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


#include "snac_icq.h"

namespace liboscar {

	// SNAC_ICQ main class
SNAC_ICQ::SNAC_ICQ(Word command, bool raw) 
	: SNAC(SNAC_FAM_ICQ, command, raw) { }

SNAC_ICQ::~SNAC_ICQ(){ }
	
	// SvrICQErr SNAC
SrvICQErrSNAC::SrvICQErrSNAC() 
	: SNAC_ICQ(ICQ_SRV_ICQ_ERR, true) { 
	m_error = 0;	
}

SrvICQErrSNAC::~SrvICQErrSNAC() { }

Word SrvICQErrSNAC::getError(){
	return m_error;
}

void SrvICQErrSNAC::parse(Buffer& b) {
	UnformattedTLV tlv(TLV_TYPE_GENERIC);

	b >> m_error; // error code

	for (unsigned int i = 0; i < 2; i++){
		if (b.len())
			tlv.parse(b);
	}

	b.removeFromBegin();
}


	// SrvMetaReply SNAC
SrvMetaReplySNAC::SrvMetaReplySNAC()
	: SNAC_ICQ(ICQ_SRV_METAREPLY, true) { }

SrvMetaReplySNAC::~SrvMetaReplySNAC() { }

MetaResponseType SrvMetaReplySNAC::getType(){
	return m_type;
}

Message SrvMetaReplySNAC::getMessage() {
	return m_msg;
}

void SrvMetaReplySNAC::parse(Buffer &b) {
	// TODO::
	
	Word seq, year, w;
	DWord dw;
	Byte m, d, h, mi, by;
	QDateTime time;
	QString str;

	b.setLittleEndian();
	b.advance(10); // some stuff we just don't care
	b >> w; m_type = (MetaResponseType) w;
	b >> seq; 

	switch (m_type){
		case OFFLINE_MESSAGE:
			b >> dw;
			m_msg.setUin(UIN(dw));
			b >> year;
			b >> m;
			b >> d;
			time.setDate(QDate(year, m, d));
			b >> h;
			b >> mi;
			time.setTime(QTime(h, mi));
			m_msg.setTime(time);
			b >> by; m_msg.setType(Message::byteToType(by));
			b >> by; m_msg.setFlags(Message::byteToFlags(by));
			b.readString(str); str.truncate(str.length() - 1); // Null-terminated 
			m_msg.setText(str);
			break;
		case END_OFFLINE_MESSAGES:
			b >> by;
			break;
		case META_INFO_RESPONSE:
			// TODO:
			break;
		default:
			qDebug("Unknown type in MetaInfo response. Discarding data.");
			b.wipe();
			break;
	}
	b.removeFromBegin();
	b.setBigEndian();
}

	// CliMetaReqOffline SNAC
CliMetaReqOfflineSNAC::CliMetaReqOfflineSNAC(DWord uin)
	: SNAC_ICQ(ICQ_CLI_METAREQ, true) { 

	UnformattedTLV tlv(TLV_TYPE_UIN);

	tlv.data().setLittleEndian();
	tlv.data() << (Word) 0x0008;
	tlv.data() << uin;
	tlv.data() << OFFLINE_MESSAGES;
	tlv.data() << (Word) 0x0002; // FIXME: request sequence
	m_data << tlv.pack();
}

CliMetaReqOfflineSNAC::~CliMetaReqOfflineSNAC() { }

	// CliMetaReqOfflineDelete SNAC
CliMetaReqOfflineDeleteSNAC::CliMetaReqOfflineDeleteSNAC(DWord uin)
	: SNAC_ICQ(ICQ_CLI_METAREQ, true) { 

	UnformattedTLV tlv(TLV_TYPE_UIN);

	tlv.data().setLittleEndian();
	tlv.data() << (Word) 0x0008;
	tlv.data() << uin;
	tlv.data() << DELETE_OFFLINE_MESSAGES;
	tlv.data() << (Word) 0x0002; // FIXME: request sequence
	m_data << tlv.pack();
}

CliMetaReqOfflineDeleteSNAC::~CliMetaReqOfflineDeleteSNAC() { }

	// CliMetaReqMetaInfo SNAC
CliMetaReqMetaInfoSNAC::CliMetaReqMetaInfoSNAC()
	: SNAC_ICQ(ICQ_CLI_METAREQ, true) { 
	// TODO: all
}

CliMetaReqMetaInfoSNAC::~CliMetaReqMetaInfoSNAC() { }


}
