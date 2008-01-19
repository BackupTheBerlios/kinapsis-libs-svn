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


#include "tlv.h"

namespace liboscar {

TLV::TLV(const Word type){
	m_type = type;
	m_length = 0;
}

void TLV::setType (const Word type){
	m_type = type;
}

void TLV::setLength (const Word length){
	m_length = length;
}

Word TLV::getLength (){
	return m_length;
}

Buffer& TLV::data(){
	return m_data;
}

Word TLV::getType(){
	return m_type;
}

Buffer& TLV::pack(){

	this->specPack();
	m_data.setBigEndian();
	m_data.prepend((Word) m_data.len());
	m_data.prepend(m_type);

	return m_data;
}

TLV::~TLV(){ }
	

UnformattedTLV::UnformattedTLV(Word type) 
	: TLV (type) { }

UnformattedTLV::~UnformattedTLV(){ }

void UnformattedTLV::parse(Buffer &b){

	unsigned int i;
	Byte by;

	m_data.wipe();
	m_data.gotoBegin();

	b >> m_type;
	b >> m_length;

	for (i=0; i < m_length; i++){
		b >> by;
		m_data << (Byte) by;
	}
	b.removeFromBegin();
	m_data.gotoBegin();
}
	
void UnformattedTLV::parseData(Buffer &b, Word len){

	unsigned int i;
	Byte by;

	m_data.wipe();
	m_data.gotoBegin();

	for (i=0; i < len; i++){
		b >> by;
		m_data << (Byte) by;
	}
	b.removeFromBegin();
	m_data.gotoBegin();
}
	
Word UnformattedTLV::len(){
	return m_length;
}

}
