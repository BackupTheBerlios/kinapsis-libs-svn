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


#ifndef _SNAC_INTERVAL_H_
#define _SNAC_INTERVAL_H_

#include "snac.h"

namespace liboscar {

	const Word INTERVAL_SRV_SETINTERVAL = 0x0002; 

class SNAC_Interval : public SNAC {

public:
	SNAC_Interval(Word command = 0, bool raw = true);
	virtual ~SNAC_Interval();

	virtual void parse(Buffer& b) = 0;
};

class SrvSetIntervalSNAC : public SNAC_Interval {

public:
	SrvSetIntervalSNAC();
	virtual ~SrvSetIntervalSNAC();

	Word getInterval();

	void parse(Buffer &b);
private:
	Word m_interval;
};


}
#endif // _SNAC_INTERVAL_H_
