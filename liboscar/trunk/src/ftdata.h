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


#ifndef _FTDATA_H_
#define _FTDATA_H_

#include <qstring.h>
#include "liboscar.h"

namespace liboscar {

class FTData {

public:
	FTData();
	virtual ~FTData();

	void setMessage(QString m);
	QString getMessage();

	void setLocale(QString l);
	QString getLocale();

	void setEncoding(QString e);
	QString getEncoding();

	void setFileEncoding(QString f);
	QString getFileEncoding();

	void setProxyUsed(bool u);
	bool getProxyUsed();

	void setProxyIP(DWord i);
	DWord getProxyIP();

	void setClientIP(DWord i);
	DWord getClientIP();

	void setExternalIP(DWord i);
	DWord getExternalIP();

	void setListeningPort(QString p);
	QString getListeningPort();

	void setMultiple(bool m);
	bool getMultiple();

	void setFileCount(Word c);
	Word getFileCount();

	void setFileSize(DWord s);
	DWord getFileSize();

	void setFileName(QString n);
	QString getFileName();

private:
	QString m_msg;
	QString m_loc;
	QString m_enc;
	QString m_fileenc;

	bool m_proxyused;

	DWord m_clientip;
	DWord m_proxyip;
	DWord m_externalip;
	QString m_listenport;

	bool m_mul;

	Word m_count;
	DWord m_size;

	QString m_filename;
};


}

#endif // _FTDATA_H_
