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


#include "userinfo.h"
#include "tlv.h"
#include "capabilitiestlv.h"
#include "statustlv.h"

namespace liboscar {

UserInfo::UserInfo() {
	m_class = CLASS_UNCONFIRMED;
	m_dc = 0;
	m_warn = 0;
	m_createtime = m_signontime = m_idletime = m_createtime = m_onlinetime = 0;
	m_extip = 0;
	m_status = STATUS_OFFLINE;
}

UserInfo::~UserInfo() {
	delete m_dc;
}
	
UIN UserInfo::getUin(){
	return m_uin;
}

void UserInfo::parse(Buffer& b) {

	Word count, type, len;
	DWord dw;
	UnformattedTLV tlv(TLV_TYPE_GENERIC);
	CapabilitiesTLV *ct = 0;
	StatusTLV st;

	m_uin.parse(b);

	b >> m_warn;
	b >> count;

	while (count--) {
		b >> type;
		b >> len;
		switch (type) {
			case 0x01:
				tlv.parseData(b, len);
				tlv.data() >> dw; m_class = (UserClass) dw;
				break;
			case 0x02:
				tlv.parseData(b, len);
				tlv.data() >> m_createtime;
				break;
			case 0x03:
				tlv.parseData(b, len);
				tlv.data() >> m_signontime;
				break;
			case 0x04:
				tlv.parseData(b, len);
				tlv.data() >> m_idletime;
				break;
			case 0x05:
				tlv.parseData(b, len);
				tlv.data() >> m_createtime;
				break;
			case 0x06:
				st.parse(b);
				m_status = st.getStatus();
				break;
			case 0x0a:
				tlv.parseData(b, len);
				tlv.data() >> m_extip;
				break;
			case 0x0c:
				m_dc = new DirectConnectionTLV();
				m_dc->parse(b);
				break;
			case 0x0d:
				ct = new CapabilitiesTLV(len);
				ct->parse(b);
				m_cap = ct->getCapabilities();
				delete ct;
				break;
			case 0x0f:
				tlv.parseData(b, len);
				tlv.data() >> m_onlinetime;
				break;
			default: // Unknown data, forget it
				tlv.parseData(b, len);
				break;
		}
	}
}

}
