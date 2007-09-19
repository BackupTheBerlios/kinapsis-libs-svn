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

#include "chatlistener.h"

namespace libimmsnp {

ChatListener::ChatListener() { }

ChatListener::~ChatListener() { }

void ChatListener::newChatSlot (int chatId, QString msnPassport){
	this-> newChat (chatId, msnPassport);
}

void ChatListener::chatLeavedTheRoomSlot(int chatId, QString passport){
	this->chatLeavedTheRoom(chatId, passport) ;
}

void ChatListener::chatIsTypingSlot(int chatId, QString chatMsnPassport){
	this->chatIsTyping(chatId, chatMsnPassport) ;
}

void ChatListener::chatInfoSlot (int chatId, QString chatMsnClient, QString chatIsLogging){
	this->chatInfo(chatId, chatMsnClient, chatIsLogging);
}

void ChatListener::chatArrivedMessageSlot (int chatId, QString chatMsnPassport, QString chatMsg){
	this->chatArrivedMessage(chatId, chatMsnPassport, chatMsg);
}

}

#include "chatlistener.moc"
