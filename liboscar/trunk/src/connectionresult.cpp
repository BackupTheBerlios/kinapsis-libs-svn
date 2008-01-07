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


#include "connectionresult.h"

namespace liboscar {

ConnectionResult::ConnectionResult(){
	ConnectionResult(UNKNOWN_ERROR, NO_ERROR);
}

ConnectionResult::ConnectionResult(SocketError error, DisconnectReason reason){
	m_error = error;
	m_reason = reason;
}

ConnectionResult::~ConnectionResult(){
}

void ConnectionResult::fromCR(ConnectionResult cr){
	m_error = cr.errorReason();
	m_reason = cr.getUnexpectedDisconnectReason();
}

bool ConnectionResult::successful(){
	return (m_error == SOCK_NO_ERROR);
}

SocketError ConnectionResult::errorReason(){
	return m_error;
}

DisconnectReason ConnectionResult::getUnexpectedDisconnectReason(){
	return m_reason;
}

}
