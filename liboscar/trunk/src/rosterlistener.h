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


#ifndef _ROSTERLISTENER_H_
#define _ROSTERLISTENER_H_

#include <qobject.h>
#include "contact.h"

namespace liboscar {

class RosterListener : public QObject{
Q_OBJECT
public:
	RosterListener();
	virtual ~RosterListener();

	virtual void onNewContact(Contact *c) = 0;
	virtual void onAuthRequest(UIN uin, QString reason) = 0;

public slots:
	void onNewContactSlot(Contact *c);
	void onAuthRequestSlot(UIN uin, QString reason);
};


}

#endif // _ROSTERLISTENER_H_
