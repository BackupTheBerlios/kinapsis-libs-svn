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


#include "uinregistrationservice.h"
#include "oscarparser.h"
#include <qmetatype.h>

namespace liboscar {

UinRegistrationService::UinRegistrationService(QString pass) { 
	m_pass = pass;
	m_up = new UinRegistrationProcess(this, m_pass);
}

UinRegistrationService::~UinRegistrationService() { }

void UinRegistrationService::registerMeta() {
	qRegisterMetaType<UIN>("UIN");
}

void UinRegistrationService::addUINRegistrationListener(UINRegistrationListener *ul) {
	QObject::connect(m_up, SIGNAL(notifyNewUin(UIN)), ul, SLOT(newUinSlot(UIN)));
}

void UinRegistrationService::delUINRegistrationListener(UINRegistrationListener *ul) {
	QObject::disconnect(m_up, 0, ul, 0);
}

void UinRegistrationService::create() {
	if (!m_parser)
		m_parser = new OscarParser();

	// Parser && UinRegistrationProcess conn
	QObject::connect(m_parser, SIGNAL(recvHello()), m_up, SLOT(recvHello()));
	QObject::connect(m_parser, SIGNAL(newUin()), m_up, SLOT(recvUin()));
}

}

#include "uinregistrationservice.moc"
