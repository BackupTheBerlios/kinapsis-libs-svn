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


#include "capabilitiestlv.h"

namespace liboscar {


CapabilitiesTLV::CapabilitiesTLV(Word len)
	:TLV(TLV_TYPE_CAPABILITIES){
	m_length = len;
}

CapabilitiesTLV::~CapabilitiesTLV() { }
	
void CapabilitiesTLV::setCapabilities(Capabilities cap) {
	m_cap = cap;
}

Capabilities CapabilitiesTLV::getCapabilities() {
	return m_cap;
}

void CapabilitiesTLV::specPack() {
	unsigned int i, j;
	CapIterator it;
	Capability cdat;

	it = m_cap.begin();

	for ( i=0; i < m_cap.len(); i++) {
		cdat = *it;
		for (j=0; j < 16; j++)
			m_data << cdat.data[j];
		it = m_cap.next();
	}
}

void CapabilitiesTLV::parse(Buffer& b) {

	unsigned int num = m_length / 16; // Number of capabilities
	unsigned int i, j;
	Capability c;
	Byte by;

	for (i = 0; i < num; i++){
		c.name = CAP_UNKNOWN;
		for (j=0; j < 16; j++){
			b >> by;
			c.data[j] = by;
		}
		m_cap.addCapability(c);
	}
	b.removeFromBegin();
}


}
