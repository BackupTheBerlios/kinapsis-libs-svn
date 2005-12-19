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


#include "errortlv.h"

namespace liboscar {

ErrorTLV::ErrorTLV() : TLV(TLV_TYPE_ERROR) {
	m_error = NO_ERROR;
}

void ErrorTLV::setError (DisconnectReason error) {
	m_error = error;
}

DisconnectReason ErrorTLV::getError() {
	return m_error;
}

void ErrorTLV::specPack() {

	switch (m_error) {
		case MULTIPLE_LOGINS:
			m_data << (Word) 0x0001;
			break;
		case BAD_PASSWORD:
			m_data << (Word) 0x0004;
			break;
		case NON_EXISTANT_UIN:
			m_data << (Word) 0x0007;
			break;
		case TOO_MANY_CLIENTS:
			m_data << (Word) 0x0015;
			break;
		case RATE_EXCEEDED:
			m_data << (Word) 0x0018;
			break;
		case OLD_VERSION:
			m_data << (Word) 0x001b;
			break;
		case RECONNECTING_TOO_FAST:
			m_data << (Word) 0x001d;
			break;
		case CANT_REGISTER:
			m_data << (Word) 0x001e;
			break;
		default:
		case NO_ERROR:
			m_data << (Word) 0x0000;
			break;
	}
}

void ErrorTLV::parse (Buffer& b) {
	Word err;

	b >> err;

	switch (err) {
		case 0x0001:
			m_error = MULTIPLE_LOGINS;
			break;
		case 0x0004:
		case 0x0005:
			m_error = BAD_PASSWORD;
			break;
		case 0x0007:
		case 0x0008:
			m_error = NON_EXISTANT_UIN;
			break;
		case 0x0015:
		case 0x0016:
			m_error = TOO_MANY_CLIENTS;
			break;
		case 0x0018:
			m_error = RATE_EXCEEDED;
			break;
		case 0x001b:
			m_error = OLD_VERSION;
			break;
		case 0x001d:
			m_error = RECONNECTING_TOO_FAST;
			break;
		case 0x001e:
			m_error = CANT_REGISTER;
			break;
		default:
		case 0x0000:
			m_error = NO_ERROR;
			break;
	}
}

ErrorTLV::~ErrorTLV(){ }
	

}
