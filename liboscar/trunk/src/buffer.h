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


#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "liboscar.h"
#include <qvaluelist.h>
#include <qobject.h>

namespace liboscar {

class Buffer : public QObject {
Q_OBJECT

public:
	Buffer();

	Buffer& operator<<(Byte);
	Buffer& operator<<(Word);
	Buffer& operator<<(DWord);
	Buffer& operator<<(const QString&);
	Buffer& operator<<(Buffer&);

	Buffer& operator>>(Byte &);
	Buffer& operator>>(Word &);
	Buffer& operator>>(DWord &);

	void prepend(Byte);
	void prepend(Word);
	void prepend(DWord);

	void remove(unsigned int num = 1);
	void removeFromBegin();
	void gotoBegin();
	void gotoEnd();
	void setPosition(unsigned int pos);
	void advance(unsigned int pos);

	void setLength(unsigned int length);

	void wipe();

	void copy(Byte * bb);

	unsigned int len();

	virtual ~Buffer();

signals:
	void dataAvailable();

private:
	typedef QValueList<Byte>::iterator BIterator;

	Byte getByte();
	Word getWord();

	QValueList<Byte> m_data;
	BIterator m_it;

};

}

#endif // _BUFFER_H_
