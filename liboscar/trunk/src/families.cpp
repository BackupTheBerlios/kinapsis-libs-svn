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


#include "families.h"

namespace liboscar {

Families::Families() { }
Families::~Families() { }

void Families::addFamily(Word f) {
	Family fa;
	fa.id = f;
	fa.version = 0;
	addFamily (fa);
}

void Families::addFamily(Family f) {
	m_data.insert(f.id, f.version);
}

void Families::delFamily(Word f){
	FIterator it = m_data.find(f);
	if (it != m_data.end())
		m_data.erase(it); // delete ocurrence of "f".
}

void Families::updateVersion (Family family){
	// As QMap insert() looks for an existing entry and if it exists update
	// the value, we only have to call it here
	this->addFamily(family);
}

Word Families::getVersion(Word family){
	FIterator it;

	it = m_data.find(family);
	return *it;
}

Family Families::getFamily(Word family){
	FIterator it;
	Family f;

	it = m_data.find (family);
	f.id = family;
	f.version = *it;

	return f;
}



}
