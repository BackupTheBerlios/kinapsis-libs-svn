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


#ifndef _SBLITEM_H_
#define _SBLITEM_H_

#include "tlv.h"
#include "buffer.h"
#include "contact.h"
#include <qptrlist.h>

namespace liboscar {

	enum SBLType {
		ITEM_BUDDY,
		ITEM_GROUP,
		ITEM_PERMIT,
		ITEM_DENY,
		ITEM_SETTINGS,
		ITEM_PRESENCE,
		ITEM_UNKNOWN,
		ITEM_IGNORE,
		ITEM_LASTUPDATE,
		ITEM_NONICQ,
		ITEM_IMPORTTIME,
		ITEM_AVATAR,
		ITEM_LINKEDLIST,
		ITEM_LINKED
	};

class SBLItem {

public:
	SBLItem();
	SBLItem(Contact* c);
	virtual ~SBLItem();
	
	void setName(QString name);
	void setGroupId(Word group);
	void setItemId(Word item);
	void setType(SBLType type);
	void setTypeBuddy(bool reqAuth = false, QString nick = "", QString mail = "", QString smsnumber = "", QString comment = "");

	QString getName();
	Word getGroupId();
	Word getItemId();
	SBLType getType();

	void addTLV(TLV *tlv);
	bool delTLV(TLV *tlv);
	QPtrList<TLV> getTLVs();

	void parse(Buffer& b);
	Buffer& pack();

private:
	Buffer m_data;

	QString m_name;
	Word m_groupid;
	Word m_itemid;
	SBLType m_type;

	QPtrList<TLV> m_tlvs;
};


}

#endif // _SBLITEM_H_
