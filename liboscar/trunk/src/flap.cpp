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


#include "flap.h"

namespace liboscar {

FLAP::FLAP() { 
	m_chan = 0;
	m_length = 0;
	m_seq = 0;
	initValues();
}

FLAP::FLAP(const Byte channel, const Word sequence, const Word length){
	m_chan = channel;
	m_length = length;
	m_seq = sequence;
	initValues();
}

void FLAP::initValues(){
	m_snac = 0;
}

void FLAP::setChannel (const Byte channel){
	m_chan = channel;
}

void FLAP::setSequence (const Word sequence){
	m_seq = sequence;
}

void FLAP::setLength (const Word length){
	m_length = length;
}

void FLAP::addTLV(TLV *tlv){
	if (m_snac){
		delete m_snac;
		m_snac = 0;
	}
	m_tlvs.append(tlv);
}

bool FLAP::delTLV(TLV *tlv){
	for (int i = 0; i < m_tlvs.size(); ++i) {
	     if (m_tlvs.at(i) == tlv){
		     m_tlvs.removeAt(i);
		     return true; //found
	     }
	}
	return false; // not found
}

QList<TLV*> FLAP::getTLVs(){
	return m_tlvs;
}

void FLAP::addSNAC(SNAC *snac){
	if (m_snac)
		delete m_snac;
	else if (m_tlvs.isEmpty() == false)
		m_tlvs.clear();
	m_snac = snac;
}

bool FLAP::delSNAC(){
	if (m_snac){
		delete m_snac;
		m_snac = 0;
		return true;
	}
	return false;
}

SNAC* FLAP::getSNAC(){
	return m_snac; /*XXX: dangerous */
}


Buffer& FLAP::data(){
	return m_data;
}

Buffer& FLAP::pack(){

//	m_data.wipe();
	if (m_snac)
		m_data << m_snac->pack();
	else {
		for (int i = 0; i < m_tlvs.size(); i++)
			m_data << m_tlvs[i]->pack();
	}

	m_data.prepend((Word)m_data.len()); //FIXME: think the length issue 
	m_data.prepend(m_seq);
	m_data.prepend(m_chan);
	m_data.prepend((Byte) 0x2a); // FLAP magic

	return m_data;
}

FLAP::~FLAP(){ 
	qDeleteAll(m_tlvs); // delete the TLVs
	if (m_snac)
		delete m_snac;
}
	

}
