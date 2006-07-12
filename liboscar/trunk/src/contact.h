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


#ifndef _CONTACT_H_
#define _CONTACT_H_

#include "uin.h"

namespace liboscar {

class Contact {

public:
	Contact();
	Contact(UIN uin, QString nick, QString group);
	virtual ~Contact();
	
	void setUin(UIN uin);
	void setNickname(QString nick);
	void setGroup(QString group);

	UIN getUin();
	QString getNickname();
	QString getGroup();

private:
	UIN m_uin;
	QString m_nick;
	QString m_group;
};


}

#endif // _CONTACT_H_
