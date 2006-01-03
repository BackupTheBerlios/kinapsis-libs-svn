/***************************************************************************
 *   Copyright (C) 2006 by Luis Cidoncha                                   *
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


#ifndef _SNAC_BOS_H_
#define _SNAC_BOS_H_

#include "snac.h"

namespace liboscar {

	const Word BOS_CLI_REQBOS = 0x0002; 
	const Word BOS_SRV_REPLYBOS = 0x0003; 
	const Word BOS_CLI_ADDVISIBLE = 0x0005; 
	const Word BOS_CLI_REMVISIBLE = 0x0006; 
	const Word BOS_CLI_ADDINVISIBLE = 0x0007; 
	const Word BOS_CLI_REMINVISIBLE = 0x0008; 

	class UIN;
class SNAC_BOS : public SNAC {

public:
	SNAC_BOS(Word command = 0, bool raw = true);
	virtual ~SNAC_BOS();

	virtual void parse(Buffer& b) = 0;
};

class SrvReplyBOSSNAC : public SNAC_BOS {

public:
	SrvReplyBOSSNAC();
	virtual ~SrvReplyBOSSNAC();

	void parse(Buffer &b);
};

class CliAddVisibleSNAC : public SNAC_BOS {

public:
	CliAddVisibleSNAC(UIN uin);
	virtual ~CliAddVisibleSNAC();

	void parse(Buffer &b) {return ; };
};

class CliRemVisibleSNAC : public SNAC_BOS {

public:
	CliRemVisibleSNAC(UIN uin);
	virtual ~CliRemVisibleSNAC();

	void parse(Buffer &b) {return ; };
};

class CliAddInvisibleSNAC : public SNAC_BOS {

public:
	CliAddInvisibleSNAC(UIN uin);
	virtual ~CliAddInvisibleSNAC();

	void parse(Buffer &b) {return ; };
};

class CliRemInvisibleSNAC : public SNAC_BOS {

public:
	CliRemInvisibleSNAC(UIN uin);
	virtual ~CliRemInvisibleSNAC();

	void parse(Buffer &b) {return ; };
};

}
#endif // _SNAC_BOS_H_
