/***************************************************************************
 *   Copyright (C) 2008 by Luis Cidoncha                                   *
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


#ifndef _CONNECTIONRESULT_H_
#define _CONNECTIONRESULT_H_

#include "liboscar.h"

namespace liboscar {

class ConnectionResult {

public:
	ConnectionResult();
	ConnectionResult(SocketError error, DisconnectReason reason);
	virtual ~ConnectionResult();

	void fromCR(ConnectionResult cr);

	bool successful();
	SocketError errorReason();

	DisconnectReason getUnexpectedDisconnectReason();

private:
	SocketError m_error;
	DisconnectReason m_reason;
};

}

Q_DECLARE_METATYPE(liboscar::ConnectionResult)

#endif // _CONNECTIONRESULT_H_
