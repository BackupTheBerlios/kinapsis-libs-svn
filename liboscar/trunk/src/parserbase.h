/***************************************************************************
 *   Copyright (C) 2006-2008 by Luis Cidoncha                              *
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


#ifndef _PARSER_H_
#define _PARSER_H_

#include "buffer.h"
#include <qobject.h>
#include <qstring.h>
#include <qbytearray.h>

namespace liboscar {

	class Buffer;

class Parser : public QObject {
Q_OBJECT

public:
	Parser();

	void add(QByteArray data);
	virtual ~Parser();

public slots:
	virtual void parse() = 0;

protected:
	Buffer m_buf;
};

}

#endif // _PARSER_H_
