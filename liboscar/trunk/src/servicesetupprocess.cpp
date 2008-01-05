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


#include "servicesetupprocess.h"
#include "flap.h"

namespace liboscar {

ServiceSetupProcess::ServiceSetupProcess(Client* c){ 
	m_parent = c;
	m_count = 0;
}

ServiceSetupProcess::~ServiceSetupProcess() { }

//
// SLOTS
//

void ServiceSetupProcess::requestServiceSetup() {
	// XXX: may need a pause between request
	requestLocation();
	requestBLM();
	requestICBM();
	requestPRM();
	requestSSI();
}

void ServiceSetupProcess::locationLimits(SrvReplyLocationSNAC srl){
	// Reply with our profile
	m_parent->getCapabilities().setMaxCap(srl.getMaxCap());

	FLAP *f = new FLAP(0x02, Connection::getNextSeqNumber(), 0);
	CliSetUserInfoSNAC *cuinfo = new CliSetUserInfoSNAC(m_parent->getCapabilities(), m_parent->getAwayMessage());
	f->addSNAC(cuinfo);
	m_parent->send(f->pack());

	done();
}

void ServiceSetupProcess::BLMLimits(SrvReplyBuddySNAC srb){
	done();
}

void ServiceSetupProcess::ICBMLimits(SrvReplyICBMSNAC sri){
	FLAP *f = new FLAP(0x02, Connection::getNextSeqNumber(), 0);
	CliSetICBMSNAC *cseti = new CliSetICBMSNAC;
	f->addSNAC(cseti);
	m_parent->send(f->pack());

	done();
}

void ServiceSetupProcess::PRMLimits(SrvReplyBOSSNAC srb){
	done();
}

void ServiceSetupProcess::SSILimits(SrvReplyListsSNAC srr){
	FLAP *f = new FLAP(0x02, Connection::getNextSeqNumber(), 0);
	CliCheckRosterSNAC *ccr = new CliCheckRosterSNAC(m_parent->getRoster());
	f->addSNAC(ccr);
	m_parent->send(f->pack());

	done();
}

//
// PRIVATE
//
void ServiceSetupProcess::requestLocation() {
	FLAP* f = new FLAP(0x02, Connection::getNextSeqNumber(), 0);
	CliReqLocationSNAC *creqloc = new CliReqLocationSNAC;
	f->addSNAC(creqloc);
	m_parent->send(f->pack());
}

void ServiceSetupProcess::requestBLM() {
	FLAP* f = new FLAP(0x02, Connection::getNextSeqNumber(), 0);
	CliReqBuddySNAC *creqb = new CliReqBuddySNAC;
	f->addSNAC(creqb);
	m_parent->send(f->pack());
}

void ServiceSetupProcess::requestICBM() {
	FLAP *f = new FLAP(0x02, Connection::getNextSeqNumber(), 0);
	CliReqICBMSNAC *creqi = new CliReqICBMSNAC;
	f->addSNAC(creqi);
	m_parent->send(f->pack());
}

void ServiceSetupProcess::requestPRM() {
	FLAP* f = new FLAP(0x02, Connection::getNextSeqNumber(), 0);
	CliReqBOSSNAC *creqbos = new CliReqBOSSNAC;
	f->addSNAC(creqbos);
	m_parent->send(f->pack());
}

void ServiceSetupProcess::requestSSI() {
	FLAP *f = new FLAP(0x02, Connection::getNextSeqNumber(), 0);
	CliReqListsSNAC *creql = new CliReqListsSNAC;
	f->addSNAC(creql);
	m_parent->send(f->pack());
}

void ServiceSetupProcess::done() {
	// easiest way to see if all the replies arrived
	if (m_count < NUM_OF_SERVICES-1)
		m_count++;
	else
		emit serviceSetupDone();
}

}
#include "servicesetupprocess.moc"
