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
	ConnectionResult(false, UNKNOWN_ERROR);
}

ConnectionResult::ConnectionResult(bool successful, SocketError error){
	m_successful = successful;
	m_error = error;
}

ConnectionResult::~ConnectionResult(){
}

void ConnectionResult::fromCR(ConnectionResult cr){
	m_successful = cr.successful();
	m_error = cr.errorReason();
}

bool ConnectionResult::successful(){
	return m_successful;
}

SocketError ConnectionResult::errorReason(){
	if (!m_successful)
		return m_error;
	else
		return SOCK_NO_ERROR;
}

}
