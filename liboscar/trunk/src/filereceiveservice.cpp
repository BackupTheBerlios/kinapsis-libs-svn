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


#include "filereceiveservice.h"
#include "oftparser.h"
#include "proxyparser.h"

namespace liboscar {

FileReceiveService::FileReceiveService(bool proxy) { 
	m_proxy = proxy;
}

FileReceiveService::~FileReceiveService() { }

void FileReceiveService::registerMeta() { }

void FileReceiveService::create() {
	if (!m_parser){ 
		if (m_proxy)
			m_parser = new ProxyParser();
		else
			m_parser = new OFTParser();
	}
}

}

#include "filereceiveservice.moc"
