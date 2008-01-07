/***************************************************************************
 *   Copyright (C) 2008 by Luis Cidoncha                                   *
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


#include "oftproxycommand.h"
#include "ftcaptlv.h"

namespace liboscar {

	//
	// OFTProxyCommand
	//
	
OFTProxyCommand::OFTProxyCommand(OFTProxyCommandId id) { 
	m_id = id;
}

OFTProxyCommand::~OFTProxyCommand(){ }
	

Buffer& OFTProxyCommand::pack(){

	m_data.prepend((Word) 0x0000); // Flags
	m_data.prepend((DWord) 0x00000000); // Unknown
	m_data.prepend((Word) m_id); // Command type
	m_data.prepend((Word) 0x044A); // Version
	m_data.prepend((Word) (m_data.len() + 2)); // Lenght (actual lenght + 2 bytes for the "lenght" word)

	return m_data;
}

	// 
	// OFTProxyError
	//
	
OFTProxyError::OFTProxyError() : OFTProxyCommand(OPID_ERROR) { }

OFTProxyError::~OFTProxyError() { }

void OFTProxyError::parse(Buffer& b) {
	Byte by;

	b >> by;
	m_err = (OFTProxyErrCode) by;
}

OFTProxyErrCode OFTProxyError::getError(){
	return m_err;
}

	// 
	// OFTProxyInitSend
	//
	
OFTProxyInitSend::OFTProxyInitSend(UIN uin, QWord cookie) : OFTProxyCommand(OPID_INITSEND) {
	FTCapTLV tlv;

	uin.appendUin(m_data); //pack
	m_data << cookie;
	m_data << tlv.pack();
}

OFTProxyInitSend::~OFTProxyInitSend() { }

	// 
	// OFTProxyAck
	//

OFTProxyAck::OFTProxyAck() : OFTProxyCommand(OPID_ACK) {

}
OFTProxyAck::~OFTProxyAck() { }

void OFTProxyAck::parse(Buffer& b) {
	DWord dw;

	b >> m_port;
	b >> dw;
	m_ip.setAddress(dw);
}

QHostAddress OFTProxyAck::getProxyIP() {
	return m_ip;
}

Word OFTProxyAck::getPort() {
	return m_port;
}

	// 
	// OFTProxyInitRecv
	//
	
OFTProxyInitRecv::OFTProxyInitRecv(UIN uin, Word port, QWord cookie) : OFTProxyCommand(OPID_INITRECV) {
	FTCapTLV tlv;

	uin.appendUin(m_data);
	m_data << port;
	m_data << cookie;
	m_data << tlv.pack();
}

OFTProxyInitRecv::~OFTProxyInitRecv() { }

	// 
	// OFTProxyReady
	//
	
OFTProxyReady::OFTProxyReady() : OFTProxyCommand(OPID_READY) { }

OFTProxyReady::~OFTProxyReady(){ }

void OFTProxyReady::parse(Buffer& b) {
	return ; // it's just a header :-)
}

}
