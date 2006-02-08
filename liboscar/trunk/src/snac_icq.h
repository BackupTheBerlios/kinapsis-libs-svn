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


#ifndef _SNAC_ICQ_H_
#define _SNAC_ICQ_H_

#include "snac.h"
#include "message.h"

namespace liboscar {

	const Word OFFLINE_MESSAGES = 0x003c;
	const Word DELETE_OFFLINE_MESSAGES = 0x003e;
	const Word META_INFO = 0x07D0;

	enum MetaResponseType {
		OFFLINE_MESSAGE = 0x0041,
		END_OFFLINE_MESSAGES = 0x0042,
		META_INFO_RESPONSE = 0x07Da
	};

	const Word ICQ_SRV_ICQ_ERR = 0x0001; 
	const Word ICQ_CLI_METAREQ = 0x0002; 
	const Word ICQ_SRV_METAREPLY = 0x0003; 

class SNAC_ICQ : public SNAC {

public:
	SNAC_ICQ(Word command = 0, bool raw = true);
	virtual ~SNAC_ICQ();

	virtual void parse(Buffer& b) = 0;
};

class SrvICQErrSNAC : public SNAC_ICQ {

public:
	SrvICQErrSNAC();
	virtual ~SrvICQErrSNAC();

	Word getError();

	void parse(Buffer& b);

private:
	Word m_error;
};

class SrvMetaReplySNAC : public SNAC_ICQ {

public:
	SrvMetaReplySNAC();
	virtual ~SrvMetaReplySNAC();

	MetaResponseType getType();
	Message getMessage();

	void parse(Buffer &b);
private:
	MetaResponseType m_type;
	Message m_msg;
};

class CliMetaReqOfflineSNAC : public SNAC_ICQ {

public:
	CliMetaReqOfflineSNAC(DWord uin);
	virtual ~CliMetaReqOfflineSNAC();

	void parse(Buffer &b) {return ; };
};
	
class CliMetaReqOfflineDeleteSNAC : public SNAC_ICQ {

public:
	CliMetaReqOfflineDeleteSNAC(DWord uin);
	virtual ~CliMetaReqOfflineDeleteSNAC();

	void parse(Buffer &b) {return ; };
};
	
class CliMetaReqMetaInfoSNAC : public SNAC_ICQ {

public:
	CliMetaReqMetaInfoSNAC();
	virtual ~CliMetaReqMetaInfoSNAC();

	void parse(Buffer &b) {return ; };
};

}
#endif // _SNAC_ICQ_H_
