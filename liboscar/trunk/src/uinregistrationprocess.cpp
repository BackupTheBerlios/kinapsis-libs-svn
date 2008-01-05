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


#include "uinregistrationprocess.h"
#include "flap.h"
#include "snac_newuser.h"

namespace liboscar {

UinRegistrationProcess::UinRegistrationProcess(Service* s, QString pass) { 
	m_parent = s;
	m_pass = pass;
}

UinRegistrationProcess::~UinRegistrationProcess() { }

//
// SLOTS
//

void UinRegistrationProcess::recvHello() {
	
	FLAP f(0x02, Connection::getNextSeqNumber(), 0);

	CliReqUINSNAC* crus = new CliReqUINSNAC(m_pass);

	f.addSNAC(crus);

	m_parent->send(f.pack());

}

void UinRegistrationProcess::recvUin(UIN uin){
	emit notifyNewUin(uin);
}

}

#include "uinregistrationprocess.moc"
