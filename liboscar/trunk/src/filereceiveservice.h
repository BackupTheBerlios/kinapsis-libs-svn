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


#ifndef _FILERECEIVESERVICE_H_
#define _FILERECEIVESERVICE_H_

#include "service.h"
#include "ftstatus.h"
#include "oftproxycommand.h"
#include "oftheader.h"
#include <qfile.h>

namespace liboscar {

class FileReceiveService : public Service {
Q_OBJECT

public:
	FileReceiveService(FTStatus);
	virtual ~FileReceiveService();

	void disconnect();

signals:
	void ftProgress(unsigned int, int, int);
	void fileEnded(unsigned int);
	void connectionSuccessful(unsigned int);

	void proxyAck(unsigned int, QHostAddress, Word);
	void proxyReady(unsigned int);

public slots:
	void handleConnect();
	void handleProxyError(OFTProxyError);
	void handleProxyAck(OFTProxyAck);
	void handleProxyReady(OFTProxyReady);
	void handleDCheader(OFTHeader);
	void handleDCdata(Buffer);

private:
	void create();
	void registerMeta();

	void sendProxyInitRecv();
	void sendProxyInitSend();

	void usePP();
	void useOP();

	FTStatus m_st;

	OFTHeader m_head;

	QFile* m_file;
};

}

#endif // _FILERECEIVESERVICE_H_
