/***************************************************************************
 *   Copyright (C) 2005 by Luis Cidoncha                                   *
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


#ifndef _DIRECTCONNECTIONTLV_H_
#define _DIRECTCONNECTIONTLV_H_

#include "tlv.h"

namespace liboscar {

	class TLV;

class DirectConnectionTLV : public TLV{

public:
	DirectConnectionTLV();
	virtual ~DirectConnectionTLV();
	
	void setInfo(DWord ip, Word port, FirewallConfiguration firewall);

	DWord getIp();
	Word getPort();
	FirewallConfiguration getFirewall();
	Word getTCPVersion();
	DWord getCookie();

	void specPack();
	void parse(Buffer& b);

private:
	DWord m_localip;
	Word m_port;
	FirewallConfiguration m_firewall;
	Word m_tcpv;
	DWord m_cookie;

};

}

#endif // _DIRECTCONNECTIONTLV_H_
