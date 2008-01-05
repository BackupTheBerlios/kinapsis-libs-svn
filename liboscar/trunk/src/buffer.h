/***************************************************************************
 *   Copyright (C) 2005-2008 by Luis Cidoncha                              *
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
#include <qlinkedlist.h>
#include <qstring.h>
#include <qbytearray.h>
#include <qmetatype.h>

namespace liboscar {

class Buffer {

public:
	Buffer();
	//Buffer(const Buffer&);

	Buffer& operator<<(Byte);
	Buffer& operator<<(Word);
	Buffer& operator<<(DWord);
	Buffer& operator<<(QWord);
	Buffer& operator<<(const QString&);
	Buffer& operator<<(Buffer&);
	Buffer& operator<<(QByteArray&);

	Buffer& operator>>(Byte &);
	Buffer& operator>>(Word &);
	Buffer& operator>>(DWord &);
	Buffer& operator>>(QWord &);
	void readString(QString &);
	void readString(QString &, Word);

	void prepend(Byte);
	void prepend(Word);
	void prepend(DWord);

	void remove(unsigned int num = 1);
	void removeFromBegin();
	void gotoBegin();
	void gotoEnd();
	void setPosition(unsigned int pos);
	void advance(int pos);

	void setLength(unsigned int length);

	void setLittleEndian();
	void setBigEndian();

	void wipe();

	void copy(Byte * bb);
	QByteArray toByteArray();

	unsigned int len();

	virtual ~Buffer();

private:
	typedef QLinkedList<Byte>::iterator BIterator;

	bool m_lendian;

	Byte getByte();
	Word getWord();
	DWord getDWord();

	QLinkedList<Byte> m_data;
	BIterator m_it;

};

}

Q_DECLARE_METATYPE(liboscar::Buffer)


#endif // _BUFFER_H_
