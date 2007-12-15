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
#include "tlv.h"
#include <qlist.h>

namespace liboscar {

	class Buffer;
	class TLV;

	// SNACs wired values
	
	const Word SNAC_FAM_SERVICE = 0x0001;
	const Word SNAC_FAM_LOCATION = 0x0002;
	const Word SNAC_FAM_CONTACT = 0x0003;
	const Word SNAC_FAM_ICBM = 0x0004;
	const Word SNAC_FAM_BOS = 0x0009;
	const Word SNAC_FAM_INTERVAL = 0x000b;
	const Word SNAC_FAM_ROSTER = 0x0013;
	const Word SNAC_FAM_ICQ = 0x0015;
	const Word SNAC_FAM_NEWUSER = 0x0017;

class SNAC {

public:
	SNAC(Word family = 0, Word command = 0, bool raw = true);
	virtual ~SNAC();
	
	Word getFamily();
	Word getCommand();

	void setFlags (const Word flags);
	void setReference (const DWord reference);
	void setRaw (const bool raw);

	void addTLV(TLV *tlv);
	bool delTLV(TLV *tlv);
	QList<TLV *> getTLVs();

	Buffer& data();
	Buffer& pack(); /* pack the SNAC for sending */
	virtual void parse(Buffer& b) = 0; /* parse the Buffer content to the SNAC */

protected:

	Buffer m_data;
	QList<TLV *> m_tlvs;

private:
	void initValues();

	Word m_family;
	Word m_command;
	Word m_flags;
	DWord m_reference;

	bool m_raw;

	static DWord snac_sequence;
};


}

#endif // _SNAC_H_
