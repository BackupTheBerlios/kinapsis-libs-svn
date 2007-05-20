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

#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "libimmsnp.h"
#include "parsernotificationserver.h"
#include "command.h"
#include "msocket.h"
#include "buffer.h"
#include "cmdlogin.h"
#include "roster.h"

#include <QObject>
#include <QStringList>

namespace libimmsnp {

class ParserNS;

class Client : public  QObject{
	
Q_OBJECT
	
public:
	Client(QString msnPassport, QString msnPass, State initialStatus);
	void connect ();
	void disconnect();
	void initChat(QString);
	int nextIdtr();
	int nextChatCount();
	QString nextChatPassport();
	void send(Command& c, int chat = -1);

	virtual ~Client();

public slots:
	void connected ();
//	void disconnected (ConnectionError e);
//	void newGroupArrived (Group* g);
//
//	void newContactArrived (Contact* c);
//	void statusChanged(QString passport, State status, QString displayName, QString capabilities);
//	void personalMessage (QString passport, QString personalMsg);

//	void chatRequest(QString ipPort, QString passport, QString ticket, QString sessionId);
//	void chatArrivedMessage (int chatId, QString msnPassport, QString chatMsg);
//	void chatInfo(int chatId, QString chatMsnClient, QString chatIsLogging);
//	void chatIsTyping(int chatId, QString chatMsnPassport);
//	void chatLeavedTheRoom (int chatId, QString chatMsnPassport);
//	void contactDisconnected (QString msnPassport);
//	void chatCreated (QString hostPort, QString ticket);
//
//	void initChatSB(QString, QString);
//	void newChat(int, QString);
	
private:
	ParserNS* m_parser;
	msocket* m_mainSocket;
	QString m_msnPassport;
	QString m_msnPass;
	int m_idtr;
	int m_chatCount;
	State m_initialStatus;
	Roster* m_roster;

	typedef QMap<int, msocket*> ChatMap;
	ChatMap m_chatList;

	QStringList m_destPassport;
};
}
#endif // _CLIENT_H_
