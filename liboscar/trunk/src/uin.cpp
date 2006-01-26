/***************************************************************************
 *   Copyright (C) 2005 by Luis Cidoncha                                   *
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


#include "uin.h"

namespace liboscar {

UIN::UIN(){
	UIN("");
}

UIN::UIN(const unsigned int uin){
	setUin(uin);
}

UIN::UIN(const QString &uin){
	this->setUin(uin);
}

QString UIN::getId(){
	return m_uin;
}

void UIN::setUin(const QString uin){
	m_uin = uin;
}

void UIN::setUin(const unsigned int uin){
	QString s;
	s = QString::number(uin);
	this->setUin(s);
}

void UIN::appendUin(Buffer &b){
	b << (Byte) m_uin.length();
	b << m_uin;
}

void UIN::parse(Buffer &b){
	Byte len, by;
	unsigned int i;

	b >> len;

	for (i=0; i < len; i++){
		b >> by;
		m_uin.append(by);
	}
	b.removeFromBegin();
}

UIN::~UIN() { }


}
