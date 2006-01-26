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


#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "uin.h"
#include "buffer.h"
#include "userinfo.h"

namespace liboscar {

class Message {

public:
	Message();
	virtual ~Message();

	QString getMessage();
	void setMessage(QString message);
	
	UIN getUin();
	void setUin(UIN uin);

	Word getFormat();
	void setFormat(Word format);

	UserInfo getInfo();

	MessageEncoding getEncoding();
	void setEncoding(MessageEncoding encoding);

	MessageType getType();
	void setType(MessageType type);

	MessageRequest getRequest();
	void setRequest(MessageRequest req);

	void parse(Buffer &b);

	Buffer& pack();

private:
	void parseCh1(Buffer& b);
	void parseCh2(Buffer& b);
	void parseCh4(Buffer& b);
	void parse2711(Buffer& b);

	MessageType byteToType(Byte b);
	MessageFlags byteToFlags(Byte b);

	Word m_format;
	QString m_msg;
	UIN m_uin;

	UserInfo m_info;
	
	MessageEncoding m_encoding;
	MessageType m_type;
	MessageFlags m_flags;

	DWord m_cookiehigh;
	DWord m_cookielow;

	Word m_ch2cookie;
	MessageRequest m_ch2req;

	Buffer m_data;
};


}

#endif // _MESSAGE_H_
