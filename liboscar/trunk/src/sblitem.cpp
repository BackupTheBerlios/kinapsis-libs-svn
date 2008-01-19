/***************************************************************************
 *   Copyright (C) 2006-2008 by Luis Cidoncha                              *
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


#include "sblitem.h"

namespace liboscar {

SBLItem::SBLItem() { }

void SBLItem::setUin(UIN uin) {
	m_uin = uin;
}

void SBLItem::setGroupId(Word group){
	m_groupid = group;
}

void SBLItem::setItemId(Word item){
	m_itemid = item;
}

void SBLItem::setType(SBLType type){
	m_type = type;
}

UIN SBLItem::getUin() {
	return m_uin;
}

Word SBLItem::getGroupId(){
	return m_groupid;
}

Word SBLItem::getItemId(){
	return m_itemid;
}

SBLType SBLItem::getType(){
	return m_type;
}

void SBLItem::addTLV(TLV* tlv) {
	m_tlvs.addTLV(tlv);
}

TLVChain& SBLItem::getTLVs(){
	return m_tlvs;
}

void SBLItem::parse(Buffer& b) {
	Word w,len ;

	m_uin.parse(b);
	b.removeFromBegin();

	b >> w;
	this->setGroupId(w);
	b >> w;
	this->setItemId(w);
	b >> w;
	this->setType((SBLType) w);

	m_tlvs.parseLen(b);
}

Buffer& SBLItem::pack(){ 

	m_data << m_tlvs.pack();
	
	m_data.prepend((Word) m_type);
	m_data.prepend(m_itemid);
	m_data.prepend(m_groupid);
	m_data.prepend(m_uin.getId());
	
	return m_data;
}

SBLItem::~SBLItem(){ }
	

}
