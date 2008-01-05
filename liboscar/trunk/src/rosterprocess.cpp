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
	m_ocontact = 0;
}

RosterProcess::~RosterProcess() { }

//
// ACTIONS
//

bool RosterProcess::addContact(UIN uin, QString groupname, QString nickname,  bool reqAuth) {
	// XXX: maybe we can reuse code with del and modify. Do it later
	
	// FIXME: find group name, and if == "", then default group
	Contact* c = new Contact(uin, groupname, nickname);
	SBLItem it(c);

	startEdit();

	FLAP f(0x02, Connection::getNextSeqNumber(), 0);
	CliRosterAddSNAC *s = new CliRosterAddSNAC(it);
	f.addSNAC(s);
	m_parent->send(f.pack());

	endEdit();

	m_op = ROSTEROP_ADD_CONTACT;
	m_ocontact = c;
	return true;
}

bool RosterProcess::delContact(UIN uin) {
	Contact* c = m_parent->getRoster()->findContactByUin(uin);
	
	if (!c) // not found
		return false;

	SBLItem it(c);

	startEdit();

	FLAP f(0x02, Connection::getNextSeqNumber(), 0);
	CliRosterDeleteSNAC *s = new CliRosterDeleteSNAC(it);
	f.addSNAC(s);
	m_parent->send(f.pack());

	endEdit();

	m_op = ROSTEROP_DEL_CONTACT;
	m_ocontact = c;
	return true;
}

bool RosterProcess::authorize(UIN uin, QString message, bool ack) {
	FLAP f(0x02, Connection::getNextSeqNumber(), 0);
	CliAuthorizeSNAC *s = new CliAuthorizeSNAC(uin, ack, message);
	f.addSNAC(s);
	m_parent->send(f.pack());
	return true;
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
			addContact(m_ocontact->getUin(), "", "", true); // Ask for auth FIXME: save group and nick
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
		case ROSTEROP_ADD_CONTACT:
			// Added a contact successfully acked. Report
			m_parent->getRoster()->addContact(m_ocontact);
			m_ocontact = 0;
			break;
		case ROSTEROP_DEL_CONTACT:
			// Deleted a contact successfully acked. Report
			m_parent->getRoster()->delContact(m_ocontact);
			m_ocontact = 0;
			break;
		case ROSTEROP_MOD_CONTACT:
			break;
		case ROSTEROP_ADD_GROUP:
			break;
		case ROSTEROP_DEL_GROUP:
			break;
		case ROSTEROP_MOD_GROUP:
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

}

#include "rosterprocess.moc"
