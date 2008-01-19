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


#ifndef _CONTACT_H_
#define _CONTACT_H_

#include "uin.h"
#include "sblitem.h"

namespace liboscar {

	class SBLItem;

class Contact {

public:
	Contact();
	Contact(Contact *);
	Contact(SBLItem *, QString groupname);
	Contact(UIN uin, QString nick, QString group);
	virtual ~Contact();
	
/*	void setUin(UIN uin);
	void setId(Word id);
	void setNickname(QString nick);
	void setGroup(QString group);
	void setAuth(bool value);
	void setVisible(bool value); */

	UIN getUin();
	Word getId();
	QString getNickname();
	QString getGroup();
	bool getAuth();
	bool getVisible();

private:
	UIN m_uin;
	Word m_id;
	QString m_nick;
	QString m_group;
	// FIXME: maybe this don't work ATM
	bool m_auth;
	bool m_visible;
};


}

Q_DECLARE_METATYPE(liboscar::Contact)

#endif // _CONTACT_H_
