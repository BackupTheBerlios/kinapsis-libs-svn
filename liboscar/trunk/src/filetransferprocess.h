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
#include "filereceiveservice.h"
#include "ftstatus.h"
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

public slots:
	void messageArrived(Message);

	void ftconnEnded(ConnectionResult);
	void ftconnSuccessful();
	void ftconnProgress(int);

signals:
	void notifyNewFTRequest(QWord cookie, UIN uin, QString filename);

private:
	Service* m_parent;
	FTMap m_transfers;

	FileReceiveService* m_frs;
};

}

#endif // _FILETRANSFERPROCESS_H_
