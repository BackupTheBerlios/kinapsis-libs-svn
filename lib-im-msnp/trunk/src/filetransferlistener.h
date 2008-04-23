/*
  Copyright (c) 2006 by Pedro Arnedo <vaticano666@gmail.com>
  This file is part of the lib-im-msnp library.
  https://developer.berlios.de/projects/kinapsis-libs/

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/

#ifndef _FILETRANSFERLISTENER_H_
#define _FILETRANSFERLISTENER_H_

#include <qobject.h>
#include "libimmsnp.h"
#include "transfer.h"

namespace libimmsnp {

class FileTransferListener : public QObject{
Q_OBJECT
public:
	FileTransferListener();
	virtual ~FileTransferListener();
	virtual void incomingFileTransfer (int chatId, Transfer* c) = 0;
	virtual void fileTransferProgress(int chatId, int ftId, int received, int total) = 0;
	virtual void fileTransferFinished (int chatId, int ftId) = 0;
	virtual void fileTransferCanceled (int chatId, int ftId) = 0;

public slots:
	void incomingFileTransferSlot (int chatId, Transfer* c);
	void fileTransferProgressSlot(int chatId, int ftId, int received, int total);
	void fileTransferFinishedSlot (int chatId, int ftId);
	void fileTransferCanceledSlot (int chatId, int ftId);
};


}

#endif // _FILETRANSFERLISTENER_H_
