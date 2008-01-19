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


#include "contactavatartlv.h"

namespace liboscar {

ContactAvatarTLV::ContactAvatarTLV() : TLV (TLV_TYPE_AVATAR){
	m_info = false;
}

ContactAvatarTLV::~ContactAvatarTLV(){ }

bool ContactAvatarTLV::hasIconInfo() {
	return m_info;
}

QByteArray ContactAvatarTLV::getMD5Hash() {
	return m_md5;
}

void ContactAvatarTLV::specPack() {
	//TODO: 
}

void ContactAvatarTLV::parse(Buffer& b){
	Word id;
	Byte flags, len;

	b >> id;
	b >> flags;
	b >> len;

	if (id == 0x0001){ // avatar info
		b.readArray(m_md5, len);
		m_info = true;
	}
	else
		b.advance(len); // we don't care

	b.removeFromBegin();
}



}
