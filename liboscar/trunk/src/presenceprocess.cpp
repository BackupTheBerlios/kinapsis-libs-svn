/***************************************************************************
 *   Copyright (C) 2007 by Luis Cidoncha                                   *
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


#include "presenceprocess.h"
#include "flap.h"
#include "message.h"

namespace liboscar {

PresenceProcess::PresenceProcess(Client* c) { 
	m_parent = c;
}

PresenceProcess::~PresenceProcess() { }

//
// SLOTS
//

void PresenceProcess::statusChanged(UIN uin, PresenceStatus status){

	if (status == STATUS_DND || status == STATUS_AWAY ||
			status == STATUS_NA || status == STATUS_OCUPPIED){
		if (m_parent->getType() == AIM){
			// Request away message
			FLAP f(0x02, Connection::getNextSeqNumber(), 0);
			CliReqUserInfoSNAC *s = new CliReqUserInfoSNAC(uin, AWAY_MESSAGE);
			f.addSNAC(s);
			m_parent->send(f.pack());
		}
		else {
			Message m;
			m.setFormat(0x0002);
			m.setUin(uin);
			switch(status){
				case STATUS_DND:
					m.setType(TYPE_AUTODND);
					break;
				default:
				case STATUS_AWAY:
					m.setType(TYPE_AUTOAWAY);
					break;
				case STATUS_NA:
					m.setType(TYPE_AUTONA);
					break;
				case STATUS_OCUPPIED:
					m.setType(TYPE_AUTOBUSY);
					break;
			}
			m.setRequest(REQUEST);
			m_parent->sendMessage(m);
		}
	}
	emit notifyPresence(uin, status);
}

void PresenceProcess::newAwayMessage(SrvUserInfoSNAC suis) {
	if (suis.getType() == AWAY_MESSAGE)
		emit notifyAwayMessage(suis.getUserOnlineInfo().getUin(), suis.getAwayMessage());
}

}

#include "presenceprocess.moc"
