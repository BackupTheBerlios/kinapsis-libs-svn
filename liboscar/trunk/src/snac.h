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


#ifndef _SNAC_H_
#define _SNAC_H_

#include "buffer.h"

namespace liboscar {

	class Buffer;

	// SNACs wired values
	
	const Word SNAC_FAM_SERVICE = 0x0001;
	const Word SNAC_FAM_LOCATION = 0x0002;
	const Word SNAC_FAM_CONTACT = 0x0003;
	const Word SNAC_FAM_ICBM = 0x0004;
	const Word SNAC_FAM_BOS = 0x0009;
	const Word SNAC_FAM_INTERVAL = 0x000b;
	const Word SNAC_FAM_ROSTER = 0x0013;
	const Word SNAC_FAM_OLDICQ = 0x0015;
	const Word SNAC_FAM_NEWUSER = 0x0017;

class SNAC {

public:
	SNAC();
	SNAC(const Word family, const Word command, const Word flags, const DWord reference);
	virtual ~SNAC();
	
	void setFamily (const Word family);
	void setCommand (const Word command);
	void setFlags (const Word flags);
	void setReference (const DWord reference);

	virtual Buffer& pack(); /* pack the SNAC for sending */
	virtual void parse(Buffer& b); /* parse the Buffer content to the SNAC */

private:
	Word m_family;
	Word m_command;
	Word m_flags;
	DWord m_reference;

	Buffer m_data;
};


}

#endif // _SNAC_H_
