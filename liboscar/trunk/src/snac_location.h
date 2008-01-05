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


#ifndef _SNAC_LOCATION_H_
#define _SNAC_LOCATION_H_

#include "snac.h"
#include "uin.h"
#include "userinfo.h"
#include "capabilities.h"
#include <qmetatype.h>

namespace liboscar {

	const Word LOCATION_SRV_LOCATION_ERR = 0x0001; 
	const Word LOCATION_CLI_REQLOCATION = 0x0002; 
	const Word LOCATION_SRV_REPLYLOCATION = 0x0003; 
	const Word LOCATION_CLI_SETUSERINFO = 0x0004; 
	const Word LOCATION_CLI_REQUSERINFO = 0x0005; 
	const Word LOCATION_SRV_USERINFO = 0x0006; 

class SNAC_Location : public SNAC {

public:
	SNAC_Location(Word command = 0, bool raw = true);
	virtual ~SNAC_Location();

	virtual void parse(Buffer& b) = 0;
};

class SrvLocationErrSNAC : public SNAC_Location {

public:
	SrvLocationErrSNAC();
	virtual ~SrvLocationErrSNAC();

	bool haveSubError();
	SNACError getError();
	DisconnectReason getSubError();

	void parse(Buffer& b);
private:
	bool m_haveopt;
	DisconnectReason m_opt;
	SNACError m_err;
};

class SrvReplyLocationSNAC : public SNAC_Location {

public:
	SrvReplyLocationSNAC();
	virtual ~SrvReplyLocationSNAC();

	Word getMaxCap();

	void parse(Buffer &b);
private:
	Word m_maxcap;
};

class SrvUserInfoSNAC : public SNAC_Location {

public:
	SrvUserInfoSNAC();
	virtual ~SrvUserInfoSNAC();

	InfoRequestType getType();
	QString getAwayMessage();
	QString getProfile();
	UserInfo getUserOnlineInfo();

	void parse(Buffer &b);
private:
	InfoRequestType m_type;
	UserInfo m_info;
	QString m_away;
	QString m_profile;
};

class CliReqLocationSNAC : public SNAC_Location {

public:
	CliReqLocationSNAC();
	virtual ~CliReqLocationSNAC();

	void parse(Buffer &b) {return ; };
};

class CliSetUserInfoSNAC : public SNAC_Location {

public:
	CliSetUserInfoSNAC(Capabilities cap, QString awaymessage);
	virtual ~CliSetUserInfoSNAC();

	void parse(Buffer &b) {return ; };
};

class CliReqUserInfoSNAC : public SNAC_Location {

public:
	CliReqUserInfoSNAC(UIN uin, InfoRequestType type);
	virtual ~CliReqUserInfoSNAC();

	void parse(Buffer &b) {return ; };
};


}

Q_DECLARE_METATYPE(liboscar::SrvReplyLocationSNAC)
Q_DECLARE_METATYPE(liboscar::SrvUserInfoSNAC)

#endif // _SNAC_LOCATION_H_
