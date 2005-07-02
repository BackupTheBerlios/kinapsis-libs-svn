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


#ifndef _ISTYPINGLISTENER_H_
#define _ISTYPINGLISTENER_H_

#include "uin.h"
#include <qobject.h>

namespace liboscar {

class IsTypingListener : public QObject{
Q_OBJECT
public:
	IsTypingListener();
	virtual ~IsTypingListener();

	virtual void isTypingEvent(UIN uin, IsTypingType type) = 0;

public slots:
	void isTypingEventSlot(UIN uin, IsTypingType type);
};


}

#endif // _ISTYPINGLISTENER_H_