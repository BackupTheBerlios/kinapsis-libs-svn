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


#include "loginst1process.h"
#include "flap.h"
#include "passwordtlv.h"

namespace liboscar {

LoginSt1Process::LoginSt1Process(Service* s, UIN uin, QString pass) { 
	m_parent = s;
	m_uin = uin;
	m_pass = pass;
}

LoginSt1Process::~LoginSt1Process() { }

//
// SLOTS
//

void LoginSt1Process::recvHello() {
	FLAP f(0x01, Connection::getNextSeqNumber(), 0); // Unknown length
	f.data() << (DWord) 0x00000001;

	UnformattedTLV* tlv;
	PasswordTLV* pt;
	
	// UIN
	tlv = new UnformattedTLV(TLV_TYPE_UIN);
	tlv->data() << m_uin.getId();
	f.addTLV(tlv);

	// Password
	pt = new PasswordTLV();
	pt->setPassword(m_pass);
	f.addTLV(pt);
	
	// Version
	tlv = new UnformattedTLV(TLV_TYPE_VERSION);
	tlv->data() << TLV_VERSION_ICQ2003B;
	f.addTLV(tlv); 

	// ClientId
	tlv = new UnformattedTLV(TLV_TYPE_CLIENTID);
	tlv->data() << (Word) 0x010a;
	f.addTLV(tlv); 

	// Versionmajor
	tlv = new UnformattedTLV(TLV_TYPE_VERMAJOR);
	tlv->data() << TLV_VERMAJOR;
	f.addTLV(tlv); 

	// Versionminor
	tlv = new UnformattedTLV(TLV_TYPE_VERMINOR);
	tlv->data() << TLV_VERMINOR_ICQ2003B;
	f.addTLV(tlv); 

	// Lesser
	tlv = new UnformattedTLV(TLV_TYPE_LESSER);
	tlv->data() << TLV_LESSER;
	f.addTLV(tlv); 

	// Build
	tlv = new UnformattedTLV(TLV_TYPE_BUILD);
	tlv->data() << TLV_BUILD_ICQ2003B;
	f.addTLV(tlv); 

	// Distrib
	tlv = new UnformattedTLV(TLV_TYPE_DISTRIBUTION);
	tlv->data() << TLV_DISTRIBUTION;
	f.addTLV(tlv); 

	// Language
	tlv = new UnformattedTLV(TLV_TYPE_LANGUAGE);
	tlv->data() << (Word) 0x656e; // en
	f.addTLV(tlv); 

	// Country
	tlv = new UnformattedTLV(TLV_TYPE_COUNTRY);
	tlv->data() << (Word) 0x7573; // us
	f.addTLV(tlv); 

	m_parent->send(f.pack());
	
}

void LoginSt1Process::recvBOS(QString server, QString port, QByteArray cookie) {
	emit stage1finished(server, port, cookie);
}

}

#include "loginst1process.moc"
