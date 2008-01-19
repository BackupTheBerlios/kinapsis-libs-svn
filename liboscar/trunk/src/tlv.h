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
	
	const Word TLV_TYPE_GENERIC = 0x0000;

	const Word TLV_TYPE_UIN = 0x0001;
	const Word TLV_TYPE_FTCAP = 0x0001;
	const Word TLV_TYPE_PASSWORD = 0x0002;
	const Word TLV_TYPE_VERSION = 0x0003;
	const Word TLV_TYPE_DESCRIPTION = 0x0004;
	const Word TLV_TYPE_SERVER = 0x0005;
	const Word TLV_TYPE_CAPABILITIES = 0x0005;
	const Word TLV_TYPE_COOKIE = 0x0006;
	const Word TLV_TYPE_STATUS = 0x0006;

	const Word TLV_TYPE_ERROR = 0x0008;

	const Word TLV_TYPE_C2C = 0x000c;

	const Word TLV_TYPE_COUNTRY = 0x000e;
	const Word TLV_TYPE_LANGUAGE = 0x000f;

	const Word TLV_TYPE_DISTRIBUTION = 0x0014;

	const Word TLV_TYPE_CLIENTID = 0x0016;
	const Word TLV_TYPE_VERMAJOR = 0x0017;
	const Word TLV_TYPE_VERMINOR = 0x0018;
	const Word TLV_TYPE_LESSER = 0x0019;
	const Word TLV_TYPE_BUILD = 0x001a;

	const Word TLV_TYPE_AVATAR = 0x001d;

	const Word TLV_TYPE_AUTH = 0x0066;

	const Word TLV_TYPE_MESSAGE = 0x0101;
	const Word TLV_TYPE_NICK = 0x0131;
	const Word TLV_TYPE_MAIL = 0x0137;
	const Word TLV_TYPE_SMS = 0x013a;
	const Word TLV_TYPE_COMMENT = 0x013c;
	const Word TLV_TYPE_CAPABILITIES2 = 0x0501;

	const Word TLV_VERMAJOR = 0x0002;

	const Word TLV_VERMINOR_ICQ2001B = 0x000f;
	const Word TLV_VERMINOR_ICQ2002A = 0x0025;
	const Word TLV_VERMINOR_ICQ2003B = 0x0038;

	const Word TLV_LESSER = 0x0001;

	const Word TLV_BUILD_ICQ2001B = 0x0e36;
	const Word TLV_BUILD_ICQ2002A = 0x0e90;
	const Word TLV_BUILD_ICQ2003B = 0x0f4c;

	const DWord TLV_DISTRIBUTION = 0x00000055;

	const QString TLV_VERSION_ICQ2001B = "ICQ Inc. - Product of ICQ (TM).2001b.5.15.1.3638.85";
	const QString TLV_VERSION_ICQ2002A = "ICQ Inc. - Product of ICQ (TM).2002a.5.37.1.3728.85";
	const QString TLV_VERSION_ICQ2003B = "ICQ Inc. - Product of ICQ (TM).2003b.5.56.1.3916.85";

class TLV {

public:
	TLV(const Word type = 0);
	virtual ~TLV();
	
	void setType (const Word type);
	void setLength (const Word length);
	Word getLength ();
	Word getType();

	Buffer& data();
	Buffer& pack(); /* pack the TLV for sending */

	virtual void specPack() = 0;
	virtual void parse(Buffer& b) = 0;

protected:
	Buffer m_data;
	Word m_type;
	Word m_length;
};

class UnformattedTLV : public TLV {

public:
	UnformattedTLV(const Word type);
	virtual ~UnformattedTLV();
	
	Word len();

	void specPack() { return ; };
	void parse(Buffer& b);
	void parseData(Buffer& b, Word len);
};


}

#endif // _TLV_H_
