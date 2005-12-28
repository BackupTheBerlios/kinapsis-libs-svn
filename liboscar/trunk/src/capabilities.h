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


#ifndef _CAPABILITIES_H_
#define _CAPABILITIES_H_

#include "liboscar.h"
#include <qvaluelist.h>

namespace liboscar {

	typedef enum CapName {
		// TODO: add more wired capbilities
		CAP_AIM_SERVERRELAY,
		CAP_UTF8,
		CAP_RTFMSGS,
		CAP_AIM_ISICQ,
		CAP_UNKNOWN
	};

	typedef struct {
		CapName name;
		Byte data[16];
	} Capability;

	typedef QValueList<Capability>::iterator CapIterator;

	// Wired Capabilities
	
	const Capability wiredCaps[]  = {
		{CAP_AIM_SERVERRELAY,
			{0x09, 0x46, 0x13, 0x49, 0x4C, 0x7F, 0x11, 0xD1, 0x82, 0x22,
				0x44, 0x45, 0x53, 0x54, 0x00, 0x00}},
		{CAP_UTF8,
			{0x09, 0x46, 0x13, 0x4e, 0x4C, 0x7F, 0x11, 0xD1, 0x82, 0x22,
				0x44, 0x45, 0x53, 0x54, 0x00, 0x00}},
		{CAP_RTFMSGS,
			{0x97, 0xB1, 0x27, 0x51, 0x24, 0x3C, 0x43, 0x34, 0xAD, 0x22,
				0xD6, 0xAB, 0xF7, 0x3F, 0x14, 0x92}},
		{CAP_AIM_ISICQ,
			{0x09, 0x46, 0x13, 0x44, 0x4C, 0x7F, 0x11, 0xD1, 0x82, 0x22,
				0x44, 0x45, 0x53, 0x54, 0x00, 0x00}},
	};

class Capabilities {

public:
	Capabilities();
	virtual ~Capabilities();

	void addCapability(Capability cap);
	void addCapability(CapName cap);

	void setDefault();

	unsigned int len();
	CapIterator begin();
	CapIterator next();


private:
	CapIterator m_it;
	QValueList<Capability> m_data;

};

}

#endif // _CAPABILITIES_H_
