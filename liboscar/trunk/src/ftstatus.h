/***************************************************************************
 *   Copyright (C) 2008 by Luis Cidoncha                                   *
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


#ifndef _FTSTATUS_H_
#define _FTSTATUS_H_

#include "liboscar.h"
#include "uin.h"
#include "ftdata.h"

namespace liboscar {

	enum StatusConnFT {
		PENDING, // Not accepted
		DIRECT,
		REDIRECT,
		PROXY1,
		PROXY2,
		PROXY3
	};

class FTStatus {

public:
	FTStatus();
	virtual ~FTStatus();

	void setUin(UIN);
	UIN getUin();

	void setFTData(FTData);
	FTData getFTData();

	void setStatus(StatusConnFT);
	StatusConnFT getStatus();
	
private:
	UIN m_uin;

	FTData m_data;

	StatusConnFT m_status;
};


}

#endif // _FTSTATUS_H_
