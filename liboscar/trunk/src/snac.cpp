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


#include "snac.h"

namespace liboscar {

SNAC::SNAC() { 
	m_family = 0;
	m_command = 0;
	m_flags = 0;
	m_reference = 0;
}

SNAC::SNAC(const Word family, const Word command, const Word flags, const DWord reference){

	m_family = family;
	m_command = command;
	m_flags = flags;
	m_reference = reference;
}

void SNAC::setFamily (const Word family){
	m_family = family;
}

void SNAC::setCommand (const Word command){
	m_command = command;
}

void SNAC::setFlags (const Word flags){
	m_flags = flags;
}

void SNAC::setReference (const DWord reference){
	m_reference = reference;
}

Buffer& SNAC::pack(){ return m_data; }

void SNAC::parse(Buffer& b){ }

SNAC::~SNAC(){ }
	

}
