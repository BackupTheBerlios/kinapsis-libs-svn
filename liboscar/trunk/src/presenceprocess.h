/***************************************************************************
 *   Copyright (C) 2007 by Luis Cidoncha                                   *
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


#ifndef _PRESENCEPROCESS_H_
#define _PRESENCEPROCESS_H_

#include "liboscar.h"
#include "client.h"
#include "uin.h"
#include <qobject.h>

namespace liboscar {

class PresenceProcess : public QObject {
Q_OBJECT

public:
	PresenceProcess(Client* c);
	virtual ~PresenceProcess();

public slots:
	void statusChanged(UIN uin, PresenceStatus status);
	void newAwayMessage(SrvUserInfoSNAC suis);

signals:
	void notifyPresence(UIN uin, PresenceStatus status);
	void notifyAwayMessage(UIN uin, QString awaymsg);

private:
	Client* m_parent;

};

}

#endif // _PRESENCEPROCESS_H_
