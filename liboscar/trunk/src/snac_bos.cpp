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


#include "snac_bos.h"
#include "uin.h"
#include "tlv.h"

namespace liboscar {

	// SNAC_BOS main class
SNAC_BOS::SNAC_BOS(Word command, bool raw) 
	: SNAC(SNAC_FAM_BOS, command, raw) { }

SNAC_BOS::~SNAC_BOS(){ }
	
	// SvrReplyBOS SNAC
SrvReplyBOSSNAC::SrvReplyBOSSNAC()
	: SNAC_BOS(BOS_SRV_REPLYBOS, true) { }

SrvReplyBOSSNAC::~SrvReplyBOSSNAC() { }

void SrvReplyBOSSNAC::parse(Buffer &b) {
	UnformattedTLV tlv(TLV_TYPE_GENERIC);

	tlv.parse(b); // Unknown
	tlv.parse(b);
}

	// 
	// Client's SNACs
	//
	
	// CliAddVisible SNAC
CliAddVisibleSNAC::CliAddVisibleSNAC(UIN uin)
	: SNAC_BOS(BOS_CLI_ADDVISIBLE, true) {

	uin.appendUin(m_data);
}

CliAddVisibleSNAC::~CliAddVisibleSNAC() { }

	// CliRemVisible SNAC
CliRemVisibleSNAC::CliRemVisibleSNAC(UIN uin)
	: SNAC_BOS(BOS_CLI_REMVISIBLE, true) {

	uin.appendUin(m_data);
}

CliRemVisibleSNAC::~CliRemVisibleSNAC() { }

	// CliAddInvisible SNAC
CliAddInvisibleSNAC::CliAddInvisibleSNAC(UIN uin)
	: SNAC_BOS(BOS_CLI_ADDINVISIBLE, true) {

	uin.appendUin(m_data);
}

CliAddInvisibleSNAC::~CliAddInvisibleSNAC() { }

	// CliRemInvisible SNAC
CliRemInvisibleSNAC::CliRemInvisibleSNAC(UIN uin)
	: SNAC_BOS(BOS_CLI_REMINVISIBLE, true) {

	uin.appendUin(m_data);
}

CliRemInvisibleSNAC::~CliRemInvisibleSNAC() { }

}
