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


#ifndef _TLV_H_
#define _TLV_H_

#include "buffer.h"
#include <qstring.h>

namespace liboscar {

	class Buffer;

	// TLV wired values
	
	const Word TLV_TYPE_UIN = 0x0001;
	const Word TLV_TYPE_PASSWORD = 0x0002;
	const Word TLV_TYPE_VERSION = 0x0003;
	const Word TLV_TYPE_DESCRIPTION = 0x0004;
	const Word TLV_TYPE_SERVER = 0x0005;
	const Word TLV_TYPE_COOKIE = 0x0006;

	const Word TLV_TYPE_ERROR = 0x0008;

	const Word TLV_TYPE_COUNTRY = 0x000e;
	const Word TLV_TYPE_LANGUAGE = 0x000f;

	const Word TLV_TYPE_DISTRIBUTION = 0x0014;

	const Word TLV_TYPE_CLIENTID = 0x0016;
	const Word TLV_TYPE_VERMAJOR = 0x0017;
	const Word TLV_TYPE_VERMINOR = 0x0018;
	const Word TLV_TYPE_LESSER = 0x0019;
	const Word TLV_TYPE_BUILD = 0x001a;


	const Byte TLV_VERMINOR_ICQ2001B = 0x0f;
	const Byte TLV_VERMINOR_ICQ2002A = 0x25;

	const Word TLV_BUILD_ICQ2001B = 0x0e36;
	const Word TLV_BUILD_ICQ2002A = 0x0e90;

	const QString TLV_VERSION_ICQ2001B = "ICQ Inc. - Product of ICQ (TM).2001b.5.15.1.3638.85";
	const QString TLV_VERSION_ICQ2002A = "ICQ Inc. - Product of ICQ (TM).2002a.5.37.1.3728.85";

	const Byte PasswordTable[] = { 0xf3, 0x26, 0x81, 0xc4, 0x39, 0x86, 0xdb, 0x92,
					0x71, 0xa3, 0xb9, 0xe6, 0x53, 0x7a, 0x95, 0x7c};

class TLV {

public:
	TLV();
	TLV(const Word type, const Word length);
	virtual ~TLV();
	
	void setType (const Word type);
	void setLength (const Word length);
	Buffer& data();
	Buffer& pack(); /* pack the TLV for sending */

private:
	Word m_type;
	Word m_length;
	Buffer m_data;

};


}

#endif // _TLV_H_
