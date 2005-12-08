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


#ifndef _FLAP_H_
#define _FLAP_H_

#include "buffer.h"

namespace liboscar {

	class Buffer;

class FLAP {

public:
	FLAP();
	FLAP(const Byte channel, const Word sequence, const Word length);
	virtual ~FLAP();

	void setChannel(const Byte channel);
	void setSequence(const Word sequence);
	void setLength(const Word length);

	Buffer& data();
	/* TODO: rehacer para SNACs y TLVs */ /* Nada de Buffer m_data */
	Buffer& pack();

private:

	Byte m_chan;
	Word m_seq;
	Word m_length;
	Buffer m_data;
};


}

#endif // _FLAP_H_
