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


#include "servertlv.h"

namespace liboscar {

ServerTLV::ServerTLV() : TLV(TLV_TYPE_SERVER) {
	m_server = "";
	m_port = "";
}

ServerTLV::~ServerTLV() { }

void ServerTLV::setServer (QString server){
	m_server = server;
}

void ServerTLV::setPort (QString port){
	m_port = port;
}

QString ServerTLV::getServer(){
	return m_server;
}

QString ServerTLV::getPort(){
	return m_port;
}

void ServerTLV::specPack(){
	m_data << m_server;
	m_data << (Byte) 0x3a;
	m_data << m_port;
}

void ServerTLV::parse(Buffer& b){
	
	Byte by;
	unsigned int i=0;

	b >> by;
	while (by != 0x3a){
		m_server.append(by);
		b >> by;
	}
	for (i=0; i < 4; i++) {
		b >> by;
		m_port.append(by);
	}
}


}
