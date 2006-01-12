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


#ifndef _MESSAGELISTENER_H_
#define _MESSAGELISTENER_H_

#include <qobject.h>
#include "uin.h"

namespace liboscar {

class MessageListener : public QObject{
Q_OBJECT
public:
	MessageListener();
	virtual ~MessageListener();

	virtual void incomingMessage(UIN uin, QString message) = 0;

public slots:
	void incomingSlot(UIN uin, QString message);
};


}

#endif // _MESSAGELISTENER_H_
