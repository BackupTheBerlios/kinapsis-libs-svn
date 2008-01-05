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


#ifndef _SNAC_ICBM_H_
#define _SNAC_ICBM_H_

#include "snac.h"
#include "message.h"
#include <qmetatype.h>

namespace liboscar {

	const Word ICBM_SRV_ICBM_ERR = 0x0001; 
	const Word ICBM_CLI_SETICBM = 0x0002; 
	const Word ICBM_CLI_REQICBM = 0x0004; 
	const Word ICBM_SRV_REPLYICBM = 0x0005; 
	const Word ICBM_CLI_SENDMSG = 0x0006; 
	const Word ICBM_SRV_RECVMSG = 0x0007; 
	const Word ICBM_SRV_MISSEDICBM = 0x000a; 
	const Word ICBM_CLI_ACKMSG = 0x000b; 
	const Word ICBM_SRV_SRVACKMSG = 0x000c; 
	const Word ICBM_SRV_CLI_TYPING = 0x0014; 

class SNAC_ICBM : public SNAC {

public:
	SNAC_ICBM(Word command = 0, bool raw = true);
	virtual ~SNAC_ICBM();

	virtual void parse(Buffer& b) = 0;
};

class SrvICBMErrSNAC : public SNAC_ICBM {

public:
	SrvICBMErrSNAC();
	virtual ~SrvICBMErrSNAC();

	ICBMError getError();

	void parse(Buffer& b);
private:
	ICBMError m_err;
};

class SrvReplyICBMSNAC : public SNAC_ICBM {

public:
	SrvReplyICBMSNAC();
	virtual ~SrvReplyICBMSNAC();

	void parse(Buffer &b);
};

class SrvRecvMsg : public SNAC_ICBM {

public:
	SrvRecvMsg();
	virtual ~SrvRecvMsg();
	Message getMessage();
	void parse(Buffer &b);
private:
	Message m_msg;
};

class SrvMissedICBMSNAC : public SNAC_ICBM {

public:
	SrvMissedICBMSNAC();
	virtual ~SrvMissedICBMSNAC();

	UIN getUin();
	Word getNumberLost();
	LostReason getReason();

	void parse(Buffer &b);
private:
	UserInfo m_info;
	Word m_nlost;
	LostReason m_reason;
};

class SrvAckMsgSNAC : public SNAC_ICBM {

public:
	SrvAckMsgSNAC();
	virtual ~SrvAckMsgSNAC();

	void parse(Buffer &b);
private:
	UIN m_uin;
	Word m_format;

};

class SrvCliTypingSNAC : public SNAC_ICBM {
public:
	SrvCliTypingSNAC();
	SrvCliTypingSNAC(UIN uin, IsTypingType type);
	virtual ~SrvCliTypingSNAC();

	IsTypingType getType();
	UIN getUin();

	void parse(Buffer &b);
private:
	UIN m_uin;
	IsTypingType m_type;
};

class CliSetICBMSNAC : public SNAC_ICBM {

public:
	CliSetICBMSNAC();
	virtual ~CliSetICBMSNAC();

	void parse(Buffer &b) {return ; };
};

class CliReqICBMSNAC : public SNAC_ICBM {

public:
	CliReqICBMSNAC();
	virtual ~CliReqICBMSNAC();

	void parse(Buffer &b) {return ; };
};

class CliSendMsgSNAC : public SNAC_ICBM {

public:
	CliSendMsgSNAC(Message message);
	virtual ~CliSendMsgSNAC();

	void parse(Buffer &b) {return ; };
};

class CliAckMsgSNAC : public SNAC_ICBM {

public:
	CliAckMsgSNAC();
	virtual ~CliAckMsgSNAC();

	Message getMessage();

	void parse(Buffer &b);
private:
	Message m_msg;
	Word m_reason;
};

}

Q_DECLARE_METATYPE(liboscar::SrvReplyICBMSNAC)

#endif // _SNAC_ICBM_H_
