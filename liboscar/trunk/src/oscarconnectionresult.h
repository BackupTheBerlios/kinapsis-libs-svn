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


#ifndef _OSCARCONNECTIONRESULT_H_
#define _OSCARCONNECTIONRESULT_H_

#include "liboscar.h"

namespace liboscar {

class OscarConnectionResult {

public:
	OscarConnectionResult();
	OscarConnectionResult(bool succesful, ConnectionError error);
	virtual ~OscarConnectionResult();

	void fromOCR(OscarConnectionResult cr);

	bool successful();
	ConnectionError errorReason();

private:
	bool m_successful;
	ConnectionError m_error;
};

}

#endif // _OSCARCONNECTIONRESULT_H_
