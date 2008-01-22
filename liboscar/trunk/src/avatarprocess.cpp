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


#include "avatarprocess.h"
#include "snac_ssbi.h"
#include "flap.h"

namespace liboscar {

AvatarProcess::AvatarProcess(Service* s) { 
	m_parent = s;
}

AvatarProcess::~AvatarProcess() { }

void AvatarProcess::requestIconFor(UIN uin, QByteArray md5) {
	FLAP f(0x02, Connection::getNextSeqNumber(), 0);
	CliICQRequestIconSNAC *iis = 0;
	CliAIMRequestIconSNAC *ais = 0;

	if (m_parent->getType() == ICQ) {
		iis = new CliICQRequestIconSNAC(uin, md5);
		f.addSNAC(iis);
	}
	else {
		ais = new CliAIMRequestIconSNAC(uin, md5);
		f.addSNAC(ais);
	}

	m_parent->send(f.pack());

	if (iis) delete iis;
	if (ais) delete ais;
}

void AvatarProcess::uploadIcon(QString fname) {
	// This is the quick way that Kopete and Pidgin use.
	// The long way found in Alexandr Shutko's doc is not
	// being implemented.
	
	FLAP f(0x02, Connection::getNextSeqNumber(), 0);
	CliUploadIconSNAC* uis = new CliUploadIconSNAC(fname);
	f.addSNAC(uis);
	m_parent->send(f.pack());
	delete uis;
}

//
// SLOTS
//

void AvatarProcess::userOnlineInfo(UserInfo ui) {
	if (ui.getAvatarInfo())
		if (ui.getAvatarInfo()->hasIconInfo())
			emit contactIconHash(ui.getUin(), ui.getAvatarInfo()->getMD5Hash());
}

void AvatarProcess::ownIconAck(QByteArray h) {
	// XXX: report to user?
	qDebug("[AvatarProcess]: server ACKed our icon");
}

void AvatarProcess::iconInfo(UIN uin, QByteArray md5, QByteArray icon) {
	emit iconInfoArrived(uin, md5, icon);
}

}
#include "avatarprocess.moc"
