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


#ifndef _FILETRANSFERPROCESS_H_
#define _FILETRANSFERPROCESS_H_

#include "liboscar.h"
#include "service.h"
#include "message.h"
#include "ftstatus.h"
#include "uin.h"
#include <qobject.h>
#include <qmap.h>

namespace liboscar {

	typedef QMap<QWord, FTStatus> FTMap;

class FileTransferProcess : public QObject {
Q_OBJECT

public:
	FileTransferProcess(Service* s);
	virtual ~FileTransferProcess();

	void acceptFileTransfer(QWord cookie, bool accept=true);
	QWord sendFile(UIN, QString);

	void setUseProxy(bool);
	void setListeningPort(int);

	void setReceiveDirectory(QString);

	static DWord calculateChunkChecksum(Buffer, DWord);
	static DWord calculateChunkChecksum(const QByteArray, int, DWord);
	static DWord calculateFileChecksum(QString);

public slots:
	void messageArrived(Message);

	void ftconnProgress(unsigned int,int,int);
	void fileEnded(unsigned int);
	void ftconnSuccessful(unsigned int);
	void proxyAckArrived(unsigned int, QHostAddress, Word);
	void proxyReadyArrived(unsigned int);

	void handleServiceEnd(unsigned int, ConnectionResult);
signals:
	void notifyNewFTRequest(QWord cookie, UIN uin, QString filename);

	void ftConnected(QWord);
	void ftProgress(QWord, int, int);
	void ftEnded(QWord);

private:
	void sendRequest(QWord);
	void cancelFT(QWord);
	void acceptFT(FTStatus);
	void startFT(FTStatus&);
	void addNewReceive(FTStatus&);
	void addNewSend(FTStatus&);
	void clearFTService(QWord);

	QWord getCookieFromId(unsigned int);

	Service* m_parent;
	FTMap m_transfers; // running transfers

	// Config
	bool m_useproxy;
	int m_mylport;
	QString m_dir;

};

}

#endif // _FILETRANSFERPROCESS_H_
