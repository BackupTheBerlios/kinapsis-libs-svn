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
#include "ftdata.h"
#include <qdatetime.h>
#include <qmetatype.h>

namespace liboscar {

class Message {

public:
	Message();
	virtual ~Message();

	void fromMessage(Message &m);

	QString getText();
	void setText(QString message);
	
	UIN getUin();
	void setUin(UIN uin);

	Word getFormat();
	void setFormat(Word format);

	UserInfo getInfo();

	MessageEncoding getEncoding();
	void setEncoding(MessageEncoding encoding);

	MessageType getType();
	void setType(MessageType type);

	MessageFlags getFlags();
	void setFlags(MessageFlags flags);

	MessageRequest getRequest();
	void setRequest(MessageRequest req);

	QDateTime getTime();
	void setTime(QDateTime time);

	Word getReqNumber();
	void setReqNumber(Word);

	void parse(Buffer &b);

	Buffer& pack();

	QWord getCookie();

	void setCookie(QWord c);

	void setCh2Cookie(Word cookie);
	Word getCh2Cookie();

	FTData getFTData();

	static MessageType byteToType(Byte b);
	static MessageFlags byteToFlags(Byte b);

private:
	void parseCh1(Buffer& b);
	void parseCh2(Buffer& b);
	void parseCh4(Buffer& b);
	void parse2711(Buffer& b);
	void parse2711file(Buffer& b);

	void packCh2();

	Byte typeToByte(MessageType t);
	Byte flagsToByte(MessageFlags f);

	Word m_format;
	QString m_msg;
	UIN m_uin;

	FTData m_ftd;

	UserInfo m_info;
	
	MessageEncoding m_encoding;
	MessageType m_type;
	MessageFlags m_flags;

	QWord m_cookie;

	Word m_ch2cookie;
	MessageRequest m_ch2req;
	Word m_rnum;

	QDateTime m_time;

	Buffer m_data;
};


}

Q_DECLARE_METATYPE(liboscar::Message)

#endif // _MESSAGE_H_
