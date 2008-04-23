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

#include "filetransferlistener.h"

namespace libimmsnp {

FileTransferListener::FileTransferListener() { }

FileTransferListener::~FileTransferListener() { }

void FileTransferListener::incomingFileTransferSlot (int chatId, Transfer* c){
       	this->incomingFileTransfer(chatId, c);
}
void FileTransferListener::fileTransferProgressSlot(int chatId, int ftId, int received, int total){
	this->fileTransferProgress(chatId, ftId, received, total);
}
void FileTransferListener::fileTransferFinishedSlot (int chatId, int ftId){
	this->fileTransferFinished(chatId, ftId);
}

void FileTransferListener::fileTransferCanceledSlot (int chatId, int ftId){
	this->fileTransferCanceled(chatId, ftId);
}
}

#include "filetransferlistener.moc"
