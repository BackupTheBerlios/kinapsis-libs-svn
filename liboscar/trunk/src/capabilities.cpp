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


#include "capabilities.h"

namespace liboscar {

Capabilities::Capabilities() { }
Capabilities::~Capabilities() { }

void Capabilities::addCapability(Capability cap) {
	m_data.append(cap);
}

void Capabilities::addCapability(CapName cap) {
	m_data.append(wiredCaps[cap]);
}

void Capabilities::setDefault() {
	// Default ICQ2003 capabilities
	addCapability(CAP_AIM_SERVERRELAY);
	addCapability(CAP_UTF8);
	addCapability(CAP_RTFMSGS);
	addCapability(CAP_AIM_ISICQ);
	// XXX: tmp
	addCapability(CAP_CHAT_ROOM);
}

void Capabilities::setMaxCap(Word max) {
	m_maxcap = max;
}
unsigned int Capabilities::len() {
	return m_data.count();
}

CapIterator Capabilities::begin() {
	m_it = m_data.begin();
	return m_it;
}

CapIterator Capabilities::next() {
	m_it++;
	return m_it;
}



}
