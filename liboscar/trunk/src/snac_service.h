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


#ifndef _SNAC_SERVICE_H_
#define _SNAC_SERVICE_H_

#include "snac.h"
#include "families.h"

namespace liboscar {

	const Word SERVICE_SRV_SERVICE_ERR = 0x0001; 
	const Word SERVICE_CLI_READY = 0x0002; 
	const Word SERVICE_SRV_FAMILIES = 0x0003; 
	const Word SERVICE_CLI_NEWSERVICE = 0x0004; 
	const Word SERVICE_SRV_REDIRECT = 0x0005; 
	const Word SERVICE_CLI_RATESREQUEST = 0x0006; 
	const Word SERVICE_SRV_RATES = 0x0007; 
	const Word SERVICE_CLI_ACKRATES = 0x0008; 
	const Word SERVICE_SRV_RATEEXCEEDED = 0x000a; 
	const Word SERVICE_SRV_SERVERPAUSE = 0x000b; 
	const Word SERVICE_CLI_ACKSERVERPAUSE = 0x000c; 
	const Word SERVICE_CLI_REQINFO = 0x000e;
	const Word SERVICE_SRV_REPLYINFO = 0x000f;
	const Word SERVICE_CLI_SETIDLE = 0x0011;
	const Word SERVICE_SRV_MIGRATIONREQ = 0x0012;
	const Word SERVICE_SRV_MOTD = 0x0013;
	const Word SERVICE_CLI_FAMILIES = 0x0017;
	const Word SERVICE_SRV_FAMILIES2 = 0x0018;
	const Word SERVICE_CLI_SETSTATUS = 0x001e;

class SNAC_Service : public SNAC {

public:
	SNAC_Service(Word command = 0, bool raw = true);
	virtual ~SNAC_Service();

	virtual void parse(Buffer& b) = 0;
};

class SrvServiceErrSNAC : public SNAC_Service {

public:
	SrvServiceErrSNAC();
	virtual ~SrvServiceErrSNAC();

	bool haveSubError();
	SNACError getError();
	DisconnectReason getSubError();

	void parse(Buffer& b);
private:
	bool m_haveopt;
	DisconnectReason m_opt;
	SNACError m_err;
};

class SrvFamiliesSNAC : public SNAC_Service {

public:
	SrvFamiliesSNAC(Families* fam);
	virtual ~SrvFamiliesSNAC();

	void parse(Buffer &b);
private:
	Families* m_fam;
};

class SrvRedirectSNAC : public SNAC_Service {

public:
	SrvRedirectSNAC();
	virtual ~SrvRedirectSNAC();

	void parse(Buffer &b);
};

class SrvRatesSNAC : public SNAC_Service {

public:
	SrvRatesSNAC();
	virtual ~SrvRatesSNAC();

	void parse(Buffer &b);
};

class SrvRateExceededSNAC : public SNAC_Service {

public:
	SrvRateExceededSNAC();
	virtual ~SrvRateExceededSNAC();

	void parse(Buffer &b);
};

class SrvServerPauseSNAC : public SNAC_Service {

public:
	SrvServerPauseSNAC();
	virtual ~SrvServerPauseSNAC();

	void parse(Buffer &b);
};

class SrvReplyInfoSNAC : public SNAC_Service {

public:
	SrvReplyInfoSNAC();
	virtual ~SrvReplyInfoSNAC();

	void parse(Buffer &b);
};

class SrvMigrationReqSNAC : public SNAC_Service {

public:
	SrvMigrationReqSNAC();
	virtual ~SrvMigrationReqSNAC();

	void parse(Buffer &b);
};

class SrvMOTDSNAC : public SNAC_Service {

public:
	SrvMOTDSNAC();
	virtual ~SrvMOTDSNAC();

	void parse(Buffer &b);
};

class SrvFamilies2SNAC : public SNAC_Service {

public:
	SrvFamilies2SNAC(Families *fam);
	virtual ~SrvFamilies2SNAC();

	void parse(Buffer &b);
private:
	Families *m_fam;
};


class CliReadySNAC : public SNAC_Service {

public:
	CliReadySNAC();
	virtual ~CliReadySNAC();

	void parse(Buffer &b) {return ; };
};

class CliNewServiceSNAC : public SNAC_Service {

public:
	CliNewServiceSNAC(Word family);
	virtual ~CliNewServiceSNAC();

	void parse(Buffer &b) {return ; };

	Word getFamily();

private:
	Word m_family;
};

class CliRatesRequestSNAC : public SNAC_Service {

public:
	CliRatesRequestSNAC();
	virtual ~CliRatesRequestSNAC();

	void parse(Buffer &b) {return ; };
};

class CliAckRatesSNAC : public SNAC_Service {

public:
	CliAckRatesSNAC();
	virtual ~CliAckRatesSNAC();

	void parse(Buffer &b) {return ; };
};

class CliAckServerPauseSNAC : public SNAC_Service {

public:
	CliAckServerPauseSNAC();
	virtual ~CliAckServerPauseSNAC();

	void parse(Buffer &b) {return ; };
};

class CliReqInfoSNAC : public SNAC_Service {

public:
	CliReqInfoSNAC();
	virtual ~CliReqInfoSNAC();

	void parse(Buffer &b) {return ; };
};

class CliSetIdleSNAC : public SNAC_Service {

public:
	CliSetIdleSNAC(bool allow);
	virtual ~CliSetIdleSNAC();

	void parse(Buffer &b) {return ; };
	bool getAllow();

private:
	bool m_allow;
};

class CliFamiliesSNAC : public SNAC_Service {

public:
	CliFamiliesSNAC();
	virtual ~CliFamiliesSNAC();

	void parse(Buffer &b) {return ; };
};

class CliSetStatusSNAC : public SNAC_Service {

public:
	CliSetStatusSNAC(PresenceStatus status, DWord ip, Word port, FirewallConfiguration firewall);
	virtual ~CliSetStatusSNAC();

	void parse(Buffer &b) {return ; };
};

}

#endif // _SNAC_SERVICE_H_
