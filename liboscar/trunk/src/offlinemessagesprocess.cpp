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


#include "offlinemessagesprocess.h"
#include "flap.h"

namespace liboscar {

OfflineMessagesProcess::OfflineMessagesProcess(Client* c) { 
	m_parent = c;
}

OfflineMessagesProcess::~OfflineMessagesProcess() { }

//
// ACTIONS
//

bool OfflineMessagesProcess::requestOfflineMessages() {
	if (m_parent->getType() == ICQ){
		FLAP* f = new FLAP(0x02, Connection::getNextSeqNumber(), 0);
		CliMetaReqOfflineSNAC *cmr = new CliMetaReqOfflineSNAC(m_parent->getUIN().getId().toUInt());
		f->addSNAC(cmr);
		m_parent->send(f->pack());
		delete f;
		return true;
	}
	else
		return false;
}

//
// SLOTS
//

void OfflineMessagesProcess::processMetaReply(SrvMetaReplySNAC mrs){
	FLAP *f;
	CliMetaReqOfflineDeleteSNAC *cmrod;
	switch (mrs.getType()){
		case OFFLINE_MESSAGE:
			emit newOfflineMessage(mrs.getMessage());
			break;
		case END_OFFLINE_MESSAGES:
			f = new FLAP(0x02, Connection::getNextSeqNumber(), 0);
			cmrod = new CliMetaReqOfflineDeleteSNAC(m_parent->getUIN().getId().toUInt());
			f->addSNAC(cmrod);
			m_parent->send(f->pack());
			delete f;
			break;
		case META_INFO_RESPONSE:
			// TODO: ok, we will NO handle this here, but in another
			// process
			break;
	}
}

}

#include "offlinemessagesprocess.moc"
