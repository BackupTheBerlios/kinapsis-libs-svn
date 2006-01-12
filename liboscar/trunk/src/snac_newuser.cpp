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


#include "snac_newuser.h"
#include "tlv.h"

namespace liboscar {

	//
	// BIG TODO: incomplete in general
	//
	
	// SNAC_NewUser main class
SNAC_NewUser::SNAC_NewUser(Word command, bool raw) 
	: SNAC(SNAC_FAM_NEWUSER, command, raw) { }

SNAC_NewUser::~SNAC_NewUser(){ }
	
	//SrvRegRefusedSNAC
SrvRegRefusedSNAC::SrvRegRefusedSNAC() 
	: SNAC_NewUser(NEWUSER_SRV_REGREFUSED, true) { }

SrvRegRefusedSNAC::~SrvRegRefusedSNAC() { }

void SrvRegRefusedSNAC::parse(Buffer &b) { }

	//SrvReplyLoginSNAC
SrvReplyLoginSNAC::SrvReplyLoginSNAC() 
	: SNAC_NewUser(NEWUSER_SRV_REPLYLOGIN, true) { }

SrvReplyLoginSNAC::~SrvReplyLoginSNAC() { }

void SrvReplyLoginSNAC::parse(Buffer &b) { }

	//SrvReplyUINSNAC
SrvReplyUINSNAC::SrvReplyUINSNAC() 
	: SNAC_NewUser(NEWUSER_SRV_REPLYUIN, true) { }

SrvReplyUINSNAC::~SrvReplyUINSNAC() { }

UIN SrvReplyUINSNAC::getUin(){
	return UIN( (unsigned int) m_uin);
}

void SrvReplyUINSNAC::parse(Buffer &b) { 
	Word w;
	DWord dw;
	unsigned int i;
	// XXX: check reg cookie?
	b.setLittleEndian();
	for (i=0; i < 7; i++)
		b >> w;
	for (i=0; i < 8; i++)
		b >> dw;

	b >> m_uin;
	b >> dw;
}

	//SrvReplyMD5SNAC
SrvReplyMD5SNAC::SrvReplyMD5SNAC() 
	: SNAC_NewUser(NEWUSER_SRV_REPLYMD5, true) { }

SrvReplyMD5SNAC::~SrvReplyMD5SNAC() { }

void SrvReplyMD5SNAC::parse(Buffer &b) { }

	//SrvReqSecureIdSNAC
SrvReqSecureIdSNAC::SrvReqSecureIdSNAC() 
	: SNAC_NewUser(NEWUSER_SRV_REQSECUREID, true) { }

SrvReqSecureIdSNAC::~SrvReqSecureIdSNAC() { }

void SrvReqSecureIdSNAC::parse(Buffer &b) { }

	// 
	// Client's SNACs
	//
	
	//CliReqLoginSNAC
CliReqLoginSNAC::CliReqLoginSNAC()
	: SNAC_NewUser(NEWUSER_CLI_REQLOGIN, false) { }

CliReqLoginSNAC::~CliReqLoginSNAC() { }

	//CliReqUINSNAC
CliReqUINSNAC::CliReqUINSNAC(QString password)
	: SNAC_NewUser(NEWUSER_CLI_REQUIN, false) { 
	UnformattedTLV *tlv;	

	tlv = new UnformattedTLV(TLV_TYPE_UIN);

	tlv->data() << (DWord) 0x00000000;
	tlv->data() << (Word) 0x2800;
	tlv->data() << (Word) 0x0300;
	tlv->data() << (DWord) 0x00000000;
	tlv->data() << (DWord) 0x00000000;
	tlv->data() << (DWord) 0x3c380000; // FIXME: do a random?
	tlv->data() << (DWord) 0x3c380000;
	tlv->data() << (DWord) 0x00000000;
	tlv->data() << (DWord) 0x00000000;
	tlv->data() << (DWord) 0x00000000;
	tlv->data() << (DWord) 0x00000000;
	tlv->data().setLittleEndian();
	tlv->data() << (Word) password.length();
	tlv->data().setBigEndian();
	tlv->data() << password;
	tlv->data() << (DWord) 0x3c380000;
	tlv->data() << (DWord) 0x00000000;
	tlv->data() << (Word) 0xd101; //FIXME random?
}

CliReqUINSNAC::~CliReqUINSNAC() { }

	//CliReqMD5SNAC
CliReqMD5SNAC::CliReqMD5SNAC()
	: SNAC_NewUser (NEWUSER_CLI_REQMD5, false) { }

CliReqMD5SNAC::~CliReqMD5SNAC() { }

	//CliReplySecureIdSNAC
CliReplySecureIdSNAC::CliReplySecureIdSNAC()
	: SNAC_NewUser(NEWUSER_CLI_REPLYSECUREID, true) { }

CliReplySecureIdSNAC::~CliReplySecureIdSNAC() { }


}
