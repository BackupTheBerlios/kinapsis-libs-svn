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

#ifndef _CHATLISTENER_H_
#define _CHATLISTENER_H_

#include <qobject.h>
#include "cmdswichboardserver.h"
namespace libimmsnp {

class ChatListener : public QObject{
Q_OBJECT
public:
	ChatListener();
	virtual ~ChatListener();

	virtual void newChat (int chatId, QString msnPassport) = 0;
	virtual void chatLeavedTheRoom(int chatId, QString chatMsnPassport) = 0;
	virtual void chatIsTyping(int chatId, QString chatMsnPassport) = 0;
	virtual void chatInfo(int chatId, QString chatMsnClient, QString chatIsLogging) = 0;
	virtual void chatArrivedMessage(int chatId, QString chatMsnPassport, QString chatMsg) = 0;


public slots:
	void newChatSlot (int chatId, QString msnPassport);
	void chatLeavedTheRoomSlot(int chatId, QString chatMsnPassport);
	void chatIsTypingSlot(int chatId, QString chatMsnPassport);
	void chatInfoSlot(int chatId, QString chatMsnClient, QString chatIsLogging);
	void chatArrivedMessageSlot (int chatId, QString chatMsnPassport, QString chatMsg);
};

}

#endif // _CHATLISTENER_H_
