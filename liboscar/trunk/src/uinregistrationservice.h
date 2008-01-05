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


#ifndef _UINREGISTRATIONSERVICE_H_
#define _UINREGISTRATIONSERVICE_H_

#include "service.h"
#include "uinregistrationprocess.h"
#include "uinregistrationlistener.h"

namespace liboscar {

class UinRegistrationService : public Service {
Q_OBJECT

public:
	UinRegistrationService(QString pass);
	virtual ~UinRegistrationService();

	void addUINRegistrationListener(UINRegistrationListener *ul);
	void delUINRegistrationListener(UINRegistrationListener *ul);
	
private:
	void create();
	void registerMeta();

	UinRegistrationProcess* m_up;

	QString m_pass;
};


}

#endif // _UINREGISTRATIONSERVICE_H_
