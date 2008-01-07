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


#ifndef _OFTPROXYCOMMAND_H_
#define _OFTPROXYCOMMAND_H_

#include "buffer.h"
#include "uin.h"
#include <qhostaddress.h>

namespace liboscar {

	enum OFTProxyCommandId {
		OPID_ERROR = 0x0001,
		OPID_INITSEND = 0x0002,
		OPID_ACK = 0x0003,
		OPID_INITRECV = 0x0004,
		OPID_READY = 0x0005
	};
		
	enum OFTProxyErrCode {
		OFT_PXY_ERR_BADREQ = 0x0d,
		OFT_PXY_ERR_TIMEOUT = 0x10
	};

class OFTProxyCommand {

public:
	OFTProxyCommand(OFTProxyCommandId id);
	virtual ~OFTProxyCommand();
	
	OFTProxyCommandId getId();

	Buffer& pack(); /* pack the command for sending */
	virtual void parse(Buffer& b) = 0; /* parse the Buffer content to the command */

protected:
	Buffer m_data;

private:
	OFTProxyCommandId m_id;
};

class OFTProxyError : public OFTProxyCommand {

public:
	OFTProxyError();
	virtual ~OFTProxyError();

	void parse(Buffer& b);

	OFTProxyErrCode getError();
private:
	OFTProxyErrCode m_err;
};

class OFTProxyInitSend : public OFTProxyCommand  {

public:
	OFTProxyInitSend(UIN uin, QWord cookie);
	virtual ~OFTProxyInitSend();

	void parse(Buffer& b) { return ; }
};

class OFTProxyAck : public OFTProxyCommand {

public:
	OFTProxyAck();
	virtual ~OFTProxyAck();

	void parse(Buffer& b);

	QHostAddress getProxyIP();
	Word getPort();
private:
	QHostAddress m_ip;
	Word m_port;
};

class OFTProxyInitRecv : public OFTProxyCommand {

public:
	OFTProxyInitRecv(UIN uin, Word port, QWord cookie);
	virtual ~OFTProxyInitRecv();

	void parse(Buffer& b) { return ; }
};

class OFTProxyReady : public OFTProxyCommand {

public:
	OFTProxyReady();
	virtual ~OFTProxyReady();

	void parse(Buffer& b);
};

}

#endif // _OFTPROXYCOMMAND_H_
