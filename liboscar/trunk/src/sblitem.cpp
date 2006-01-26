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


#include "sblitem.h"

namespace liboscar {

SBLItem::SBLItem() { 
	m_tlvs.setAutoDelete(true);
}

void SBLItem::setName(QString name) {
	m_name = name;
}

void SBLItem::setGroupId(Word group){
	m_groupid = group;
}

void SBLItem::setItemId(Word item){
	m_itemid = item;
}

void SBLItem::setTypeBuddy(bool reqAuth, QString nick, QString mail, QString smsnumber, QString comment) {

	UnformattedTLV *tlv;

	m_type = ITEM_BUDDY;

	if (reqAuth){
		tlv = new UnformattedTLV(TLV_TYPE_AUTH);
		addTLV(tlv);
	}

	if (nick != "") {
		tlv = new UnformattedTLV(TLV_TYPE_NICK);
		tlv->data() << (Word) nick.length();
		tlv->data() << nick;
		addTLV(tlv);
	}

	if (comment != "") {
		tlv = new UnformattedTLV(TLV_TYPE_COMMENT);
		tlv->data() << (Word) comment.length();
		tlv->data() << comment;
		addTLV(tlv);
	}

	if (smsnumber != "") {
		tlv = new UnformattedTLV(TLV_TYPE_SMS);
		tlv->data() << (Word) smsnumber.length();
		tlv->data() << smsnumber;
		addTLV(tlv);
	}

	if (mail != "") {
		tlv = new UnformattedTLV(TLV_TYPE_MAIL);
		tlv->data() << (Word) mail.length();
		tlv->data() << mail;
		addTLV(tlv);
	}

}

void SBLItem::setType(SBLType type){
	m_type = type;
	// TODO: add TLV's
}


QString SBLItem::getName() {
	return m_name;
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


void SBLItem::addTLV(TLV *tlv){
	m_tlvs.append(tlv);
}

bool SBLItem::delTLV(TLV *tlv){
	return m_tlvs.remove(tlv);
}

QPtrList<TLV> SBLItem::getTLVs(){
	return m_tlvs;
}

void SBLItem::parse(Buffer& b) {
	// TODO: parse
}

Buffer& SBLItem::pack(){ 

	TLV *t;
	
	for (t = m_tlvs.first(); t; t = m_tlvs.next())
		m_data << t->pack();
	
	// TODO: prepend data
	
	return m_data;
}

SBLItem::~SBLItem(){ }
	

}
