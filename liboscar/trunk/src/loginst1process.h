/***************************************************************************
 *   Copyright (C) 2008 by Luis Cidoncha                                   *
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


#ifndef _LOGINST1PROCESS_H_
#define _LOGINST1PROCESS_H_

#include "liboscar.h"
#include "tlvchain.h"
#include "service.h"
#include "uin.h"
#include <qobject.h>
#include <qbytearray.h>

namespace liboscar {

class LoginSt1Process : public QObject {
Q_OBJECT

public:
	LoginSt1Process(Service* s, UIN uin, QString pass);
	virtual ~LoginSt1Process();

public slots:
	void recvHello();
	void recvBOS(QString serv, QString port, QByteArray cookie);

signals:
	void stage1finished(QString bos, QString bosport, QByteArray cookie);

private:

	Service* m_parent;
	UIN m_uin;
	QString m_pass;
};

}

#endif // _LOGINST1PROCESS_H_
