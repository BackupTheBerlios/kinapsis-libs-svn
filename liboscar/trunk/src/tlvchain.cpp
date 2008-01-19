/***************************************************************************
 *   Copyright (C) 2006 by Luis Cidoncha                                   *
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


#include "tlvchain.h"

namespace liboscar {

TLVChain::TLVChain() { }

TLVChain::~TLVChain(){
	// FIXME: segfaults
/*	TLVMap::iterator i;
	TLV* tlv;

	if (m_map.size()){
		for (i = m_map.begin(); i != m_map.end(); i++){
			tlv = i.value();
			if (tlv)
				delete tlv;
		}
	}
	*/
}
	
void TLVChain::addTLV(TLV* tlv) {
	m_map[tlv->getType()] = tlv;
}

bool TLVChain::exists(Word type) {
	return m_map.contains(type);
}

void TLVChain::addItem(Buffer &b){
	UnformattedTLV *tlv;

	tlv = new UnformattedTLV(TLV_TYPE_GENERIC);
	tlv->parse(b);
	m_map[tlv->getType()] = tlv;
}

int TLVChain::len(){
	return m_map.size();
}

void TLVChain::parseLen(Buffer& b) {
	Word len;
	UnformattedTLV* tlv(TLV_TYPE_GENERIC);

	b >> len;

	while (len){
		b.removeFromBegin();
		tlv->parse(b);
		this->addTLV(tlv);
		len -= (tlv->data().len() + 4);
	}
	b.removeFromBegin();
}

void TLVChain::parse(Buffer& b, bool withcount) {

	if (!b.len())
		return;
	
	if (withcount){
		Word count;
		unsigned int i;

		b >> count;
		for (i=0; i < count; i++)
			addItem(b); 
	} 
	else { 
		while (b.len()) addItem(b); 
	} 
}
	
TLV * & TLVChain::operator[] (Word type) { return m_map[type]; }

Buffer& TLVChain::pack() {
	QMapIterator<Word, TLV*> i(m_map);
	m_data.gotoBegin();

	while (i.hasNext()) {
		i.next();
		m_data << i.value()->pack();
	}

	return m_data;
}

}
