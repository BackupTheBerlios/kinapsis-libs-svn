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


#include "snac.h"

namespace liboscar {

SNAC::SNAC(Word family, Word command, bool raw) { 
	m_family = family;
	m_command = command;
	m_flags = 0;
	m_reference = 0;
	m_raw = raw;
}

void SNAC::initValues(){
	m_tlvs.setAutoDelete(true);
}

Word SNAC::getFamily (){
	return m_family;
}

Word SNAC::getCommand (){
	return m_command;
}

void SNAC::setFlags (const Word flags){
	m_flags = flags;
}

void SNAC::setReference (const DWord reference){
	m_reference = reference;
}

void SNAC::setRaw (const bool raw){
	m_raw = raw;
}

void SNAC::addTLV(TLV *tlv){
	m_tlvs.append(tlv);
}

bool SNAC::delTLV(TLV *tlv){
	return m_tlvs.remove(tlv);
}

QPtrList<TLV> SNAC::getTLVs(){
	return m_tlvs;
}

Buffer& SNAC::data(){ 
	return m_data;
}

Buffer& SNAC::pack(){ 

	if (!m_raw){
		TLV *t;
	
		for (t = m_tlvs.first(); t; t = m_tlvs.next())
			m_data << t->pack();
	}
	
	m_data.prepend(m_reference);
	m_data.prepend(m_flags);
	m_data.prepend(m_command);
	m_data.prepend(m_family);

	return m_data;
}

SNAC::~SNAC(){ }
	

}
