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


#include "oscarconnectionresult.h"

namespace liboscar {

OscarConnectionResult::OscarConnectionResult(){
	OscarConnectionResult(CONN_NO_ERROR);
}

OscarConnectionResult::OscarConnectionResult(ConnectionError error, DisconnectReason rea){
	m_error = error;
	m_reason = rea;
}

OscarConnectionResult::~OscarConnectionResult(){
}

void OscarConnectionResult::fromOCR(OscarConnectionResult cr){
	m_error = cr.errorReason();
	m_reason = cr.getUnexpectedDisconnectReason();
}

bool OscarConnectionResult::successful(){
	return (m_error == CONN_NO_ERROR);
}

ConnectionError OscarConnectionResult::errorReason(){
	return m_error;
}

DisconnectReason OscarConnectionResult::getUnexpectedDisconnectReason(){
	return m_reason;
}

}
