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


#ifndef _UIN_H_
#define _UIN_H_

#include <qstring.h>
#include "buffer.h"

namespace liboscar {

class UIN {

public:
	UIN();
	UIN(const unsigned int uin);
	UIN(const QString& uin);
	virtual ~UIN();
	
	QString getUin();
	void setUin(const QString uin);
	void setUin(const unsigned int uin);

	void parse(Buffer &b);

private:
	QString m_uin;

};


}

#endif // _UIN_H_
