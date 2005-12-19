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


#include "passwordtlv.h"

namespace liboscar {

	const Byte PasswordTable[] = { 0xf3, 0x26, 0x81, 0xc4, 0x39, 0x86, 0xdb, 0x92,
					0x71, 0xa3, 0xb9, 0xe6, 0x53, 0x7a, 0x95, 0x7c};

PasswordTLV::PasswordTLV() : TLV (TLV_TYPE_PASSWORD){
	m_pass = "";
}

PasswordTLV::~PasswordTLV() { }
	
void PasswordTLV::setPassword(QString password){
	m_pass = password;
}

QString PasswordTLV::getPassword(){
	return m_pass;
}

void PasswordTLV::specPack(){
	unsigned int i = 0;

	for (i=0; i < m_pass.length(); i ++)
		m_data << (Byte) (m_pass.ascii()[i] ^ PasswordTable[i%16]);
}

void PasswordTLV::parse(Buffer& b){
	return ;
}


}
