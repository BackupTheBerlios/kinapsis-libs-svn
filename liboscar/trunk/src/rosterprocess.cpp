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


#include "rosterprocess.h"
#include "sblitem.h"
#include "flap.h"
#include "snac_roster.h"
#include "connection.h"
#include "client.h"
#include <QtDebug>

namespace liboscar {

RosterProcess::RosterProcess(Client* c) { 
	m_parent = c;
	m_op = ROSTEROP_NONE;
	m_oitem = 0;
}

RosterProcess::~RosterProcess() { }

//
// ACTIONS
//

bool RosterProcess::addContact(UIN uin, QString groupname, QString nickname,  bool reqAuth) {
	// XXX: maybe we can reuse code with del and modify. Do it later
	
	// FIXME: find group name, and if == "", then default group
	SBLItem *it = new SBLItem;
	UnformattedTLV* tlv;

	it->setUin(uin);
	it->setItemId(m_parent->getRoster()->getNextId());
	it->setGroupId(m_parent->getRoster()->findItemByName(groupname, ITEM_GROUP)->getGroupId());
	it->setType(ITEM_BUDDY);

	tlv = new UnformattedTLV(TLV_TYPE_NICK);
	tlv->data() << nickname;
	it->addTLV(tlv);

	if (reqAuth) {
		tlv = new UnformattedTLV(TLV_TYPE_AUTH);
		tlv->data() << nickname;
		it->addTLV(tlv);
	}

	addItem(it);

	return true;
}

bool RosterProcess::delContact(UIN uin) {
	SBLItem* it= m_parent->getRoster()->findItemByUin(uin, ITEM_BUDDY);
	
	if (!it) // not found
		return false;

	delItem(it);
	return true;
}

bool RosterProcess::authorize(UIN uin, QString message, bool ack) {
	FLAP f(0x02, Connection::getNextSeqNumber(), 0);
	CliAuthorizeSNAC *s = new CliAuthorizeSNAC(uin, ack, message);
	f.addSNAC(s);
	m_parent->send(f.pack());
	return true;
}

bool RosterProcess::setVisibleStatus(UIN uin, bool in) {
	SBLItem *it = m_parent->getRoster()->findItemByUin(uin, ITEM_PERMIT);

	if (!it && in) {
		// Add UIN to visible list
		it = new SBLItem();
		it->setUin(uin);
		it->setGroupId(0);
		it->setItemId(m_parent->getRoster()->getNextId());
		it->setType(ITEM_PERMIT);

		this->addItem(it);
		return true;
	}
	else if (it && !in) {
		this->delItem(it);
		return true;
	}
	else 
		return false;
}

bool RosterProcess::setInvisibleStatus(UIN uin, bool in) {
	SBLItem *it = m_parent->getRoster()->findItemByUin(uin, ITEM_DENY);

	if (!it && in) {
		// Add UIN to invisible list
		it = new SBLItem();
		it->setUin(uin);
		it->setGroupId(0);
		it->setItemId(m_parent->getRoster()->getNextId());
		it->setType(ITEM_DENY);

		this->addItem(it);
		return true;
	}
	else if (it && !in) {
		this->delItem(it);
		return true;
	}
	else 
		return false;
}

//
// SLOTS (reactions)
//

void RosterProcess::rosterArrivedSlot(Buffer buf) {

	Buffer b;

	b.wipe();
	b << buf;
	b.gotoBegin();

	m_parent->getRoster()->parse(b); // parse it!

	// Ack the roster
	FLAP *f = new FLAP(0x02, Connection::getNextSeqNumber(), 0);
	CliRosterAckSNAC *cra = new CliRosterAckSNAC;
	f->addSNAC(cra);
	m_parent->send(f->pack());

	emit rosterAcked();
}

void RosterProcess::handleUpdateAck(RosterModifyAck a) {

	switch (a){
		case ACK_OK:
			reportSuccess();
			break;
		case ACK_REQ_AUTH:
			addContact(m_oitem->getUin(), "", "", true); // Ask for auth FIXME: save group and nick
			break;
		default:
			qDebug("RosterProcess: Server error during operation: %d", a);
			// TODO: report error to the user and do error messages
			break;
	}
}

void RosterProcess::authReq(UIN uin, QString reason){
	// Someone want to be authorized by us. ask the user
	emit notifyAuthRequest(uin, reason);
}

void RosterProcess::addedMe(UIN uin){
	// TODO: do something? may, test before
	qDebug() << "RosterProcess: Someone added me:" << uin.getId();
}

//
//PRIVATE
//

void RosterProcess::reportSuccess(){
	switch (m_op) {
		case ROSTEROP_ADD_ITEM:
			// Added a item successfully acked. Add to roster
			m_parent->getRoster()->addItem(m_oitem);
			m_oitem = 0;
			break;
		case ROSTEROP_DEL_ITEM:
			// Deleted a item successfully acked. Del from roster
			m_parent->getRoster()->delItem(m_oitem);
			m_oitem = 0;
			break;
		default:
			break;
	}
	m_op = ROSTEROP_NONE; // finished
}

void RosterProcess::startEdit() { 
	FLAP f(0x02, Connection::getNextSeqNumber(), 0);
	CliAddStartSNAC *s = new CliAddStartSNAC();
	f.addSNAC(s);
	m_parent->send(f.pack());
}

void RosterProcess::endEdit() { 
	FLAP f(0x02, Connection::getNextSeqNumber(), 0);
	CliAddEndSNAC *s = new CliAddEndSNAC();
	f.addSNAC(s);
	m_parent->send(f.pack());
}

void RosterProcess::addItem(SBLItem* it) {
	startEdit();

	FLAP f(0x02, Connection::getNextSeqNumber(), 0);
	CliRosterAddSNAC *s = new CliRosterAddSNAC(it);
	f.addSNAC(s);
	m_parent->send(f.pack());

	endEdit();

	m_oitem = it;
	m_op = ROSTEROP_ADD_ITEM;
}

void RosterProcess::delItem(SBLItem* it) {
	startEdit();

	FLAP f(0x02, Connection::getNextSeqNumber(), 0);
	CliRosterDeleteSNAC *s = new CliRosterDeleteSNAC(it);
	f.addSNAC(s);
	m_parent->send(f.pack());

	endEdit();

	m_oitem = it;
	m_op = ROSTEROP_DEL_ITEM;
}

}
#include "rosterprocess.moc"
