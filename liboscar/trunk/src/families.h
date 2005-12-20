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


#ifndef _FAMILIES_H_
#define _FAMILIES_H_

#include "liboscar.h"
#include <qmap.h>

namespace liboscar {

	typedef struct {
		Word id;
		Word version;
	} Family;

class Families {

public:
	Families();
	virtual ~Families();

	void addFamily(Word f);
	void addFamily(Family f);

	void delFamily(Word f);

	void updateVersion (Family family);

	Word getVersion(Word family);
	Family getFamily(Word family);

private:

	typedef QMap<Word, Word>::iterator FIterator;

	QMap<Word, Word> m_data;
	FIterator m_it;

};

}

#endif // _FAMILIES_H_
