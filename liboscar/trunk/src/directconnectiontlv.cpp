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


#include "directconnectiontlv.h"

namespace liboscar {

DirectConnectionTLV::DirectConnectionTLV() : TLV (TLV_TYPE_C2C){ }

DirectConnectionTLV::~DirectConnectionTLV(){
}

void DirectConnectionTLV::setInfo(DWord ip, Word port, FirewallConfiguration firewall){
	m_localip = ip;
	m_port = port;
	m_firewall = firewall;
	m_tcpv = 7;
	m_cookie = 0;
}

DWord DirectConnectionTLV::getIp(){
	return m_localip;
}

Word DirectConnectionTLV::getPort(){
	return m_port;
}

FirewallConfiguration DirectConnectionTLV::getFirewall(){
	return m_firewall;
}

Word DirectConnectionTLV::getTCPVersion(){
	return m_tcpv;
}

DWord DirectConnectionTLV::getCookie(){
	return m_cookie;
}

	
void DirectConnectionTLV::specPack(){
	m_data << m_localip;
	m_data << (Word) 0x0000;
	m_data << m_port;
	switch (m_firewall){
		case FIREWALL:
			m_data << (Byte) 0x01;
			break;
		case SOCKS_PROXY:
			m_data << (Byte) 0x02;
			break;
		default:
		case NORMAL:
			m_data << (Byte) 0x04;
			break;
	}
	m_data << m_tcpv;
	m_data << m_cookie;

	m_data << (Word) 0x0000;
	m_data << (Word) 0x0050;
	m_data << (Word) 0x0000;

	m_data << (Word) 0x0003;

	m_data << (DWord) 0x3ba8dbaf;
	m_data << (DWord) 0x3beb5373;
	m_data << (DWord) 0x3beb5262;

	m_data << (Word) 0x0000;
}

void DirectConnectionTLV::parse(Buffer& b){

	Word w;
	DWord dw;
	Byte by;
	Word count;

	b >> dw; m_localip = dw;

	b >> w;
	b >> w; m_port = w;

	b >> by;

	switch (by){
		case 0x01:
			m_firewall = FIREWALL;
			break;
		case 0x02:
			m_firewall = SOCKS_PROXY;
			break;
		default:
		case 0x04:
			m_firewall = NORMAL;
			break;
	}

	b >> w; m_tcpv = w;
	b >> dw; m_cookie = dw;

	b >> w;
	b >> w;
	b >> w;
	b >> w; count = w;

	while (count--)
		b >> dw; // get timestamps
	b >> w;
}



}
