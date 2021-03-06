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

#include "ftstatus.h"

namespace liboscar {

FTStatus::FTStatus() { 
	m_service = 0;
}

FTStatus::~FTStatus() { }

void FTStatus::setUin(UIN uin){
	m_uin = uin;
}

UIN FTStatus::getUin(){
	return m_uin;
}

void FTStatus::setFTData(FTData d){
	m_data = d;
}

FTData& FTStatus::getFTData(){
	return m_data;
}

void FTStatus::setStatus(StatusConnFT s) {
	m_status = s;
}

StatusConnFT FTStatus::getStatus() {
	return m_status;
}

void FTStatus::setCookie(QWord c) {
	m_cookie = c;
}

QWord FTStatus::getCookie() {
	return m_cookie;
}

void FTStatus::setType(TypeConnFT t){
	m_type = t;
}

TypeConnFT FTStatus::getType() {
	return m_type;
}

void FTStatus::setService(Service* s) {
	m_service = s;
}

Service* FTStatus::getService(){
	return m_service;
}

}
