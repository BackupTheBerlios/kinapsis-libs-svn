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


#ifndef _SNAC_CONTACT_H_
#define _SNAC_CONTACT_H_

#include "snac.h"
#include "uin.h"
#include "userinfo.h"
#include <qmetatype.h>

namespace liboscar {

	const Word CONTACT_SRV_CONTACT_ERR = 0x0001; 
	const Word CONTACT_CLI_REQBUDDY = 0x0002; 
	const Word CONTACT_SRV_REPLYBUDDY = 0x0003; 
	const Word CONTACT_CLI_ADDCONTACT = 0x0004; 
	const Word CONTACT_CLI_REMCONTACT = 0x0005; 
	const Word CONTACT_SRV_REFUSED = 0x000a; 
	const Word CONTACT_SRV_USERONLINE = 0x000b; 
	const Word CONTACT_SRV_USEROFFLINE = 0x000c; 

class SNAC_Contact : public SNAC {

public:
	SNAC_Contact(Word command = 0, bool raw = true);
	virtual ~SNAC_Contact();

	virtual void parse(Buffer& b) = 0;
};

class SrvContactErrSNAC : public SNAC_Contact {

public:
	SrvContactErrSNAC();
	virtual ~SrvContactErrSNAC();

	ContactError getError();

	void parse(Buffer& b);
private:
	ContactError m_err;
};

class SrvReplyBuddySNAC : public SNAC_Contact {

public:
	SrvReplyBuddySNAC();
	virtual ~SrvReplyBuddySNAC();

	Word getMaxUins();
	Word getMaxWatchers();

	void parse(Buffer &b);

private:
	Word m_maxuins;
	Word m_maxwatchers;
};

class SrvRefusedSNAC : public SNAC_Contact {

public:
	SrvRefusedSNAC();
	virtual ~SrvRefusedSNAC();

	UIN getUin();

	void parse(Buffer &b);
private:
	UIN m_uin;
};

class SrvUserOnlineSNAC : public SNAC_Contact {

public:
	SrvUserOnlineSNAC();
	virtual ~SrvUserOnlineSNAC();
	UIN getUin();
	PresenceStatus getStatus();
	UserInfo getUserInfo();
	void parse(Buffer &b);
private:
	UserInfo m_info;
};

class SrvUserOfflineSNAC : public SNAC_Contact {

public:
	SrvUserOfflineSNAC();
	virtual ~SrvUserOfflineSNAC();

	UIN getUin();

	void parse(Buffer &b);
private:
	UserInfo m_info;
};

class CliReqBuddySNAC : public SNAC_Contact {

public:
	CliReqBuddySNAC();
	virtual ~CliReqBuddySNAC();

	void parse(Buffer &b) {return ; };
};

class CliAddContactSNAC : public SNAC_Contact {

public:
	CliAddContactSNAC(UIN uin);
	virtual ~CliAddContactSNAC();

	void parse(Buffer &b) {return ; };
};

class CliRemContactSNAC : public SNAC_Contact {

public:
	CliRemContactSNAC(UIN uin);
	virtual ~CliRemContactSNAC();

	void parse(Buffer &b) {return ; };
};

}

Q_DECLARE_METATYPE(liboscar::SrvReplyBuddySNAC)

#endif // _SNAC_CONTACT_H_
