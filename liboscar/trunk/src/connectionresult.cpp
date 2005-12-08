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


#include "connectionresult.h"

namespace liboscar {

ConnectionResult::ConnectionResult(){
	ConnectionResult(true, CONN_ERR_USER_REQUEST);
}
ConnectionResult::ConnectionResult(bool successful, ConnectionError error){
	m_successful = successful;
	m_error = error;
}

ConnectionResult::~ConnectionResult(){
}

bool ConnectionResult::successful(){
	return m_successful;
}

ConnectionError ConnectionResult::errorReason(){
	if (!m_successful)
		return m_error;
	else
		return (ConnectionError)-1;
}


}
