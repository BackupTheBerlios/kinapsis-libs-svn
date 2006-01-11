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


#include "statustlv.h"

namespace liboscar {

StatusTLV::StatusTLV() : TLV(TLV_TYPE_STATUS) {
	m_status = STATUS_ONLINE;
}

StatusTLV::~StatusTLV(){ }

	
void StatusTLV::setStatus(PresenceStatus status){
	m_status = status;
}

PresenceStatus StatusTLV::getStatus(){
	return m_status;
}


void StatusTLV::specPack(){
	switch(m_status){
		case STATUS_OFFLINE:
			m_data << (DWord) 0xffffffff;
			break;
		case STATUS_INVISIBLE:
			m_data << (DWord) 0x00000100;
			break;
		case STATUS_DND:
			m_data << (DWord) 0x00000013;
			break;
		case STATUS_OCUPPIED:
			m_data << (DWord) 0x00000011;
			break;
		case STATUS_NA:
			m_data << (DWord) 0x00000005;
			break;
		case STATUS_AWAY:
			m_data << (DWord) 0x00000001;
			break;
		case STATUS_FFC:
			m_data << (DWord) 0x00000020;
			break;
		default:
		case STATUS_ONLINE:
			m_data << (DWord) 0x00000000;
			break;
	}
}

void StatusTLV::parse(Buffer& b){
	Word f;
	Word s;

	// TODO: ignoring flags. We must handle them
	b >> f;
	b >> s;
	switch (s) {
		case 0xffff:
			m_status = STATUS_OFFLINE;
			break;
		case 0x0100:
			m_status = STATUS_INVISIBLE;
			break;
		case 0x0013:
		case 0x0002:
			m_status = STATUS_DND;
			break;
		case 0x0011:
			m_status = STATUS_OCUPPIED;
			break;
		case 0x0005:
			m_status = STATUS_NA;
			break;
		case 0x0001:
			m_status = STATUS_AWAY;
			break;
		case 0x0020:
			m_status = STATUS_FFC;
			break;
		default:
		case 0x0000:
			m_status = STATUS_ONLINE;
			break;

	}
}


}
