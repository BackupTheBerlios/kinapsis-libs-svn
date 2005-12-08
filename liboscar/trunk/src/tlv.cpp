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


#include "tlv.h"

namespace liboscar {

TLV::TLV() { 
	m_type = 0;
	m_length = 0;
}

TLV::TLV(const Word type, const Word length){
	m_type = type;
	m_length = length;
}

void TLV::setType (const Word type){
	m_type = type;
}

void TLV::setLength (const Word length){
	m_length = length;
}

Buffer& TLV::data(){
	return m_data;
}

Buffer& TLV::pack(){
	/* Cut the extra data */
	if (m_data.len() > m_length){
		m_data.setPosition(m_length);
		m_data.remove(m_data.len() - m_length);
	}

	m_data.prepend(m_length);
	m_data.prepend(m_type);

	return m_data;
}

TLV::~TLV(){ }
	

}
