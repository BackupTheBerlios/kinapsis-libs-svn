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
	m_status = STATUS_ONLINE;
}

UserInfo::~UserInfo() {
	delete m_dc;
}
	
UIN UserInfo::getUin(){
	return m_uin;
}

PresenceStatus UserInfo::getStatus(){
	return m_status;
}

DWord UserInfo::getCreateTime() {
	return m_createtime;
}

DWord UserInfo::getSignOnTime() {
	return m_signontime;
}

DWord UserInfo::getIdleTime() {
	return m_idletime;
}

DWord UserInfo::getCreationTime() {
	return m_creationtime;
}

DWord UserInfo::getOnlineTime() {
	return m_onlinetime;
}

UserClass UserInfo::getUserClass() {
	return m_class;
}

void UserInfo::setStatus(PresenceStatus status) {
	m_status = status;
}

void UserInfo::parse(Buffer& b) {

	Word count, type, len, w;
	UnformattedTLV tlv(TLV_TYPE_GENERIC);
	CapabilitiesTLV *ct = 0;
	StatusTLV st;

//	b.removeFromBegin();
//	b.gotoBegin();

	m_uin.parse(b);

	b >> m_warn;
	b >> count;

	while (count--) {
		b >> type;
		b >> len;
		switch (type) {
			case 0x0001:
				tlv.parseData(b, len);
				tlv.data() >> w; m_class = (UserClass) w;
				if (w & 0x0020)
					m_status = STATUS_AWAY;
				break;
			case 0x0002:
				tlv.parseData(b, len);
				tlv.data() >> m_createtime;
				break;
			case 0x0003:
				tlv.parseData(b, len);
				tlv.data() >> m_signontime;
				break;
			case 0x0004:
				tlv.parseData(b, len);
				tlv.data() >> m_idletime;
				break;
			case 0x0005:
				tlv.parseData(b, len);
				tlv.data() >> m_createtime;
				break;
			case 0x0006:
				st.parse(b);
				m_status = st.getStatus();
				break;
			case 0x000a:
				tlv.parseData(b, len);
				tlv.data() >> m_extip;
				break;
			case 0x000c:
				m_dc = new DirectConnectionTLV();
				m_dc->parse(b);
				break;
			case 0x000d:
				ct = new CapabilitiesTLV(len);
				ct->parse(b);
				m_cap = ct->getCapabilities();
				delete ct;
				break;
			case 0x000f:
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
