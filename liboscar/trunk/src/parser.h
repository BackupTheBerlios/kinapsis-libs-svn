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


#ifndef _PARSER_H_
#define _PARSER_H_

#include "buffer.h"
#include <qobject.h>
#include <qstring.h>

namespace liboscar {

	class Buffer;

class Parser : public QObject {
Q_OBJECT

public:
	Parser();

	void add(QString data);
	virtual ~Parser();

public slots:
	void parse();

private:
	Word getNextSeqNumber();
	void parseCh1(Buffer& buf);
	void parseCh2(Buffer& buf);
	void parseCh4(Buffer& buf);
	void parseCh5(Buffer& buf);

	Buffer m_buf;
	Word m_seq; /* FLAP's sequence number */

};

}

#endif // _PARSER_H_
