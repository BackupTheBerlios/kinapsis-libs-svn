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


#ifndef _OFTHEADER_H_
#define _OFTHEADER_H_

#include "liboscar.h"
#include "buffer.h"

namespace liboscar {

	enum OFTType {
		OFTPROMPT = 0x0101,
		OFTACK = 0x0202,
		OFTDONE = 0x0204,
		OFTRECRES = 0x0205,
		OFTSENRES = 0x0102,
		OFTRESACK = 0x0207
	};

	enum OFTFlags {
		OFTFLAGNEGO = 0x20,
		OFTFLAGDONE = 0x01
	};

class OFTHeader {

public:
	OFTHeader();
	virtual ~OFTHeader();

	void setType(Word w);
	void setType(OFTType t);
	void setCookie(QWord qw);
	void setTotalFiles(Word w);
	void setFilesLeft(Word w);
	void setTotalParts(Word w);
	void setPartsLeft(Word w);
	void setTotalSize(DWord dw);
	void setSizeLeft(DWord dw);
	void setChecksum(DWord dw);
	void setBytesReceived(DWord dw);
	void setReceivedChk(DWord dw);
	void setModTime(DWord dw);
	void setFlags(Byte b);
	void setEncoding(Word w);
	void setFilename(QString s);

	OFTType getType();
	QWord getCookie();
	Word getTotalFiles();
	Word getFilesLeft();
	Word getTotalParts();
	Word getPartsLeft();
	DWord getTotalSize();
	DWord getSizeLeft();
	DWord getChecksum();
	DWord getBytesReceived();
	DWord getReceivedChk();
	OFTFlags getFlags();
	MessageEncoding getEncoding();
	QString getFilename();

	Buffer& pack();

private:
	OFTType m_type;
	QWord m_cookie;

	Word m_totfiles;
	Word m_leftfiles;
	Word m_totparts;
	Word m_leftparts;
	DWord m_totsize;
	DWord m_leftsize;

	DWord m_modtime;

	DWord m_chk;
	DWord m_recvchk;
	
	DWord m_bytesrecv;

	OFTFlags m_flags;
	MessageEncoding m_enc;
	QString m_fname;

	Buffer m_buf;
};


}

#endif // _OFTHEADER_H_
