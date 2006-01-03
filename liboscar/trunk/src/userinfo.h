/***************************************************************************
 *   Copyright (C) 2006 by Luis Cidoncha                                   *
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


#ifndef _USERINFO_H_
#define _USERINFO_H_

#include "uin.h"
#include "buffer.h"
#include "capabilities.h"
#include "directconnectiontlv.h"

namespace liboscar {

	class UIN;
	class Buffer;
	class Capabilities;
	class DirectConnectionTLV;

class UserInfo {

public:
	UserInfo();
	virtual ~UserInfo();
	
	// TODO: gets
	
	UIN getUin();

	void parse(Buffer& b);

private:
	UIN m_uin;
	Word m_warn;
	DirectConnectionTLV *m_dc;
	UserClass m_class;

	DWord m_createtime;
	DWord m_signontime;
	DWord m_idletime;
	DWord m_creationtime;
	DWord m_onlinetime;

	DWord m_extip;

	PresenceStatus m_status;

	Capabilities m_cap;
};


}

#endif // _USERINFO_H_
