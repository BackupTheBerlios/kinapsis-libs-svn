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


#include "loginst2process.h"
#include "flap.h"

namespace liboscar {

LoginSt2Process::LoginSt2Process(Service* s) { 
	m_parent = s;
}

LoginSt2Process::~LoginSt2Process() { }

void LoginSt2Process::setCookie(QByteArray cookie){
	m_cookie = cookie;
}

//
// SLOTS
//

void LoginSt2Process::recvHello() {
	qDebug("recvHello");
	FLAP f(0x01, Connection::getNextSeqNumber(), 0);
	f.data() << (DWord) 0x00000001;
	UnformattedTLV* tlv = new UnformattedTLV(TLV_TYPE_COOKIE);
	tlv->data() << m_cookie;
	f.addTLV(tlv);
	m_parent->send(f.pack());
}

void LoginSt2Process::recvFamilies(SrvFamiliesSNAC sf) {
	// TODO: Process this
	requestVersions();
}

void LoginSt2Process::recvVersions(SrvVersionsSNAC svs) {
	// TODO: Process this
	requestRates();
}

void LoginSt2Process::recvRates(SrvRatesSNAC sra) {
	// Ack Rates
	FLAP *f = new FLAP(0x02, Connection::getNextSeqNumber(), 0);
	CliAckRatesSNAC *clia = new CliAckRatesSNAC;
	f->addSNAC(clia);
	m_parent->send(f->pack());
	emit stage2finished();
}

//
// REACTIONS
//

void LoginSt2Process::requestRates(){
	FLAP f(0x02, Connection::getNextSeqNumber(), 0);
	CliRatesRequestSNAC *clir = new CliRatesRequestSNAC;
	f.addSNAC(clir);
	m_parent->send(f.pack());
}

void LoginSt2Process::requestVersions(){
	FLAP f(0x02, Connection::getNextSeqNumber(), 0);
	CliFamiliesSNAC *clis = new CliFamiliesSNAC; // TODO: make this get Families info
	f.addSNAC(clis);
	m_parent->send(f.pack());
}

}

#include "loginst2process.moc"
