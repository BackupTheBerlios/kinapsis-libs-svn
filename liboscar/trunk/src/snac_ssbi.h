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


#ifndef _SNAC_SSBI_H_
#define _SNAC_SSBI_H_

#include "snac.h"
#include "message.h"
#include <qmetatype.h>

namespace liboscar {

	const Word SSBI_SRV_SSBI_ERR = 0x0001; 
	const Word SSBI_CLI_UPLOAD = 0x0002; 
	const Word SSBI_SRV_UPLOADACK = 0x0003; 
	const Word SSBI_CLI_AIMREQUEST = 0x0004; 
	const Word SSBI_SRV_AIMRESPONSE = 0x0005; 
	const Word SSBI_CLI_ICQREQUEST = 0x0006; 
	const Word SSBI_SRV_ICQRESPONSE = 0x0007; 

class SNAC_SSBI : public SNAC {

public:
	SNAC_SSBI(Word command = 0, bool raw = true);
	virtual ~SNAC_SSBI();

	virtual void parse(Buffer& b) = 0;
};

class SrvSSBIErrSNAC : public SNAC_SSBI {

public:
	SrvSSBIErrSNAC();
	virtual ~SrvSSBIErrSNAC();

	Word getError();

	void parse(Buffer& b);

private:
	Word m_error;
};

class SrvUploadAckSNAC : public SNAC_SSBI {

public:
	SrvUploadAckSNAC();
	virtual ~SrvUploadAckSNAC();

	QByteArray getHash();

	void parse(Buffer& b);
private:
	QByteArray m_md5;
};

class SrvAIMIconResponseSNAC : public SNAC_SSBI {

public:
	SrvAIMIconResponseSNAC();
	virtual ~SrvAIMIconResponseSNAC();

	UIN getUin();
	QByteArray getHash();
	QByteArray getIcon();

	void parse(Buffer&);
private:
	UIN m_uin;
	QByteArray m_md5;
	QByteArray m_icon;
};

class SrvICQIconResponseSNAC : public SNAC_SSBI {

public:
	SrvICQIconResponseSNAC();
	virtual ~SrvICQIconResponseSNAC();

	UIN getUin();
	QByteArray getHash();
	QByteArray getIcon();

	void parse(Buffer&);
private:
	UIN m_uin;
	QByteArray m_md5;
	QByteArray m_icon;
};

class CliUploadIconSNAC : public SNAC_SSBI {

public:
	CliUploadIconSNAC(QString fname);
	virtual ~CliUploadIconSNAC();

	void parse(Buffer &b) {return ; };
};
	
class CliAIMRequestIconSNAC : public SNAC_SSBI {

public:
	CliAIMRequestIconSNAC(UIN, QByteArray);
	virtual ~CliAIMRequestIconSNAC();

	void parse(Buffer &b) {return ; };
};
	
class CliICQRequestIconSNAC : public SNAC_SSBI {

public:
	CliICQRequestIconSNAC(UIN, QByteArray);
	virtual ~CliICQRequestIconSNAC();

	void parse(Buffer &b) {return ; };
};
	
}

#endif // _SNAC_SSBI_H_
