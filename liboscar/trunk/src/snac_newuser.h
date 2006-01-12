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


#ifndef _SNAC_NEWUSER_H_
#define _SNAC_NEWUSER_H_

#include "snac.h"
#include "uin.h"

namespace liboscar {

	const Word NEWUSER_SRV_REGREFUSED = 0x0001; 
	const Word NEWUSER_CLI_REQLOGIN = 0x0002; 
	const Word NEWUSER_SRV_REPLYLOGIN = 0x0003; 
	const Word NEWUSER_CLI_REQUIN = 0x0004; 
	const Word NEWUSER_SRV_REPLYUIN = 0x0005; 
	const Word NEWUSER_CLI_REQMD5 = 0x0006; 
	const Word NEWUSER_SRV_REPLYMD5 = 0x0007; 
	const Word NEWUSER_SRV_REQSECUREID = 0x000a; 
	const Word NEWUSER_CLI_REPLYSECUREID = 0x000b; 

class SNAC_NewUser : public SNAC {

public:
	SNAC_NewUser(Word command = 0, bool raw = true);
	virtual ~SNAC_NewUser();

	virtual void parse(Buffer& b) = 0;
};

class SrvRegRefusedSNAC : public SNAC_NewUser {

public:
	SrvRegRefusedSNAC();
	virtual ~SrvRegRefusedSNAC();

	void parse(Buffer &b);
};

class SrvReplyLoginSNAC : public SNAC_NewUser {

public:
	SrvReplyLoginSNAC();
	virtual ~SrvReplyLoginSNAC();

	void parse(Buffer &b);
};

class SrvReplyUINSNAC : public SNAC_NewUser {

public:
	SrvReplyUINSNAC();
	virtual ~SrvReplyUINSNAC();

	UIN getUin();

	void parse(Buffer &b);
private:
	DWord m_uin;
};

class SrvReplyMD5SNAC : public SNAC_NewUser {

public:
	SrvReplyMD5SNAC();
	virtual ~SrvReplyMD5SNAC();

	void parse(Buffer &b);
};

class SrvReqSecureIdSNAC : public SNAC_NewUser {

public:
	SrvReqSecureIdSNAC();
	virtual ~SrvReqSecureIdSNAC();

	void parse(Buffer &b);
};

class CliReqLoginSNAC : public SNAC_NewUser {

public:
	CliReqLoginSNAC();
	virtual ~CliReqLoginSNAC();

	void parse(Buffer &b) {return ; };
};

class CliReqUINSNAC : public SNAC_NewUser {

public:
	CliReqUINSNAC(QString password);
	virtual ~CliReqUINSNAC();

	void parse(Buffer &b) {return ; };
};

class CliReqMD5SNAC : public SNAC_NewUser {

public:
	CliReqMD5SNAC();
	virtual ~CliReqMD5SNAC();

	void parse(Buffer &b) {return ; };
};

class CliReplySecureIdSNAC : public SNAC_NewUser {

public:
	CliReplySecureIdSNAC();
	virtual ~CliReplySecureIdSNAC();

	void parse(Buffer &b) {return ; };
};


}
#endif // _SNAC_NEWUSER_H_
