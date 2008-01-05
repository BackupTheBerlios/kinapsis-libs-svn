/***************************************************************************
 *   Copyright (C) 2006-2007 by Luis Cidoncha                              *
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


#ifndef _SNAC_ROSTER_H_
#define _SNAC_ROSTER_H_

#include "snac.h"
#include "uin.h"
#include "roster.h"
#include "sblitem.h"
#include "tlvchain.h"

namespace liboscar {

	const Word ROSTER_CLI_REQLISTS = 0x0002; 
	const Word ROSTER_SRV_REPLYLISTS = 0x0003; 
	const Word ROSTER_CLI_REQROSTER = 0x0004; 
	const Word ROSTER_CLI_CHECKROSTER = 0x0005; 
	const Word ROSTER_SRV_REPLYROSTER = 0x0006; 
	const Word ROSTER_CLI_ROSTERACK = 0x0007; 
	const Word ROSTER_CLI_ROSTERADD = 0x0008; 
	const Word ROSTER_CLI_ROSTERUPDATE = 0x0009; 
	const Word ROSTER_CLI_ROSTERDELETE = 0x000a; 
	const Word ROSTER_SRV_UPDATEACK = 0x000e; 
	const Word ROSTER_SRV_REPLYROSTEROK = 0x000f; 
	const Word ROSTER_CLI_ADDSTART = 0x0011; 
	const Word ROSTER_CLI_ADDEND = 0x0012; 
	const Word ROSTER_CLI_GRANTAUTH = 0x0014; 
	const Word ROSTER_SRV_FUTUREGRANTREC = 0x0015; 
	const Word ROSTER_CLI_DELYOURSELF = 0x0016; 
	const Word ROSTER_CLI_REQAUTH = 0x0018; 
	const Word ROSTER_SRV_AUTHREQ = 0x0019; 
	const Word ROSTER_CLI_AUTHORIZE = 0x001a; 
	const Word ROSTER_SRV_AUTHREPLY = 0x001b; 
	const Word ROSTER_SRV_ADDEDYOU = 0x001c; 

	enum RosterModifyAck {
		ACK_OK = 0x0000,
		ACK_NOT_FOUND = 0x0002,
		ACK_EXISTS = 0x0003,
		ACK_INVALID = 0x000a,
		ACK_LIMIT = 0x000c,
		ACK_ADDING_ICQ_TO_AIM = 0x000d,
		ACK_REQ_AUTH = 0x000e
	};

class SNAC_Roster : public SNAC {

public:
	SNAC_Roster(Word command = 0, bool raw = true);
	virtual ~SNAC_Roster();

	virtual void parse(Buffer& b) = 0;
};

class SrvReplyListsSNAC : public SNAC_Roster {

public:
	SrvReplyListsSNAC();
	virtual ~SrvReplyListsSNAC();

	void parse(Buffer &b);
	TLVChain getTLVs();

private:
	TLVChain tch;
};

class SrvReplyRosterSNAC : public SNAC_Roster {

public:
	SrvReplyRosterSNAC();
	virtual ~SrvReplyRosterSNAC();

	void parse(Buffer &b);
private:
	Roster m_roster;
};

class SrvUpdateAckSNAC : public SNAC_Roster {

public:
	SrvUpdateAckSNAC();
	virtual ~SrvUpdateAckSNAC();

	RosterModifyAck getAck();
	void parse(Buffer &b);
private:
	RosterModifyAck m_err;
};

class SrvReplyRosterOkSNAC : public SNAC_Roster {

public:
	SrvReplyRosterOkSNAC();
	virtual ~SrvReplyRosterOkSNAC();

	void parse(Buffer &b);
};

class SrvFutureGrantRecSNAC : public SNAC_Roster {

public:
	SrvFutureGrantRecSNAC();
	virtual ~SrvFutureGrantRecSNAC();

	UIN getUin();
	QString getReason();

	void parse(Buffer &b);
private:
	UIN m_uin;
	QString m_reason;
};

class SrvAuthReqSNAC : public SNAC_Roster {

public:
	SrvAuthReqSNAC();
	virtual ~SrvAuthReqSNAC();

	UIN getUin();
	QString getReason();
	void parse(Buffer &b);
private:
	UIN m_uin;
	QString m_reason;
};

class SrvAuthReplySNAC : public SNAC_Roster {

public:
	SrvAuthReplySNAC();
	virtual ~SrvAuthReplySNAC();

	UIN getUin();
	bool getGranted();
	QString getReason();
	void parse(Buffer &b);
private:
	UIN m_uin;
	bool m_granted;
	QString m_reason;
};

class SrvAddedYouSNAC : public SNAC_Roster {

public:
	SrvAddedYouSNAC();
	virtual ~SrvAddedYouSNAC();

	UIN getUin();
	void parse(Buffer &b);
private:
	UIN m_uin;
};

class CliReqListsSNAC : public SNAC_Roster {

public:
	CliReqListsSNAC();
	virtual ~CliReqListsSNAC();

	void parse(Buffer &b) {return ; };
};

class CliReqRosterSNAC : public SNAC_Roster {

public:
	CliReqRosterSNAC();
	virtual ~CliReqRosterSNAC();

	void parse(Buffer &b) {return ; };
};

class CliCheckRosterSNAC : public SNAC_Roster {

public:
	CliCheckRosterSNAC(Roster* roster);
	virtual ~CliCheckRosterSNAC();

	void parse(Buffer &b) {return ; };
};

class CliRosterAckSNAC : public SNAC_Roster {

public:
	CliRosterAckSNAC();
	virtual ~CliRosterAckSNAC();

	void parse(Buffer &b) {return ; };
};

class CliRosterAddSNAC : public SNAC_Roster {

public:
	CliRosterAddSNAC(SBLItem i);
	virtual ~CliRosterAddSNAC();

	void parse(Buffer &b) {return ; };
};

class CliRosterUpdateSNAC : public SNAC_Roster {

public:
	CliRosterUpdateSNAC(SBLItem i);
	virtual ~CliRosterUpdateSNAC();

	void parse(Buffer &b) {return ; };
};

class CliRosterDeleteSNAC : public SNAC_Roster {

public:
	CliRosterDeleteSNAC(SBLItem i);
	virtual ~CliRosterDeleteSNAC();

	void parse(Buffer &b) {return ; };
};

class CliAddStartSNAC : public SNAC_Roster {

public:
	CliAddStartSNAC();
	virtual ~CliAddStartSNAC();

	void parse(Buffer &b) {return ; };
};

class CliAddEndSNAC : public SNAC_Roster {

public:
	CliAddEndSNAC();
	virtual ~CliAddEndSNAC();

	void parse(Buffer &b) {return ; };
};

class CliGrantAuthSNAC : public SNAC_Roster {

public:
	CliGrantAuthSNAC(UIN uin);
	virtual ~CliGrantAuthSNAC();

	void parse(Buffer &b) {return ; };
};

class CliDelYourselfSNAC : public SNAC_Roster {

public:
	CliDelYourselfSNAC(UIN uin);
	virtual ~CliDelYourselfSNAC();

	void parse(Buffer &b) {return ; };
};

class CliReqAuthSNAC : public SNAC_Roster {

public:
	CliReqAuthSNAC(UIN uin, QString reason);
	virtual ~CliReqAuthSNAC();

	void parse(Buffer &b) {return ; };
};

class CliAuthorizeSNAC : public SNAC_Roster {

public:
	CliAuthorizeSNAC(UIN uin, bool granted, QString reason);
	virtual ~CliAuthorizeSNAC();

	void parse(Buffer &b) {return ; };
};

}
#endif // _SNAC_ROSTER_H_
