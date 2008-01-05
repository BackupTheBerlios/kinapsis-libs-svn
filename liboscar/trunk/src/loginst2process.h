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


#ifndef _LOGINST2PROCESS_H_
#define _LOGINST2PROCESS_H_

#include "liboscar.h"
#include "snac_service.h"
#include "service.h"
#include <qobject.h>
#include <qbytearray.h>

namespace liboscar {

class LoginSt2Process : public QObject {
Q_OBJECT

public:
	LoginSt2Process(Service* s);
	virtual ~LoginSt2Process();

	void setCookie(QByteArray cookie);

public slots:
	void recvHello();
	void recvFamilies(SrvFamiliesSNAC sf);
	void recvVersions(SrvVersionsSNAC svs);
	void recvRates(SrvRatesSNAC sra);

signals:
	void stage2finished();

private:
	void requestRates();
	void requestVersions();

	Service* m_parent;
	QByteArray m_cookie;
};

}

#endif // _LOGINST2PROCESS_H_
