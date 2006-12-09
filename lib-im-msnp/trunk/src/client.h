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
#include "parserswichboardserver.h"
#include "contact.h"
#include "roster.h"
#include "connection.h"
#include "connectionlistener.h"
#include "msocket.h"
#include "buffer.h"
#include "command.h"
#include "chat.h"
#include "rosterlistener.h"
#include "presencelistener.h"
#include "chatlistener.h"

#include <sstream>
#include <string>
#include <qobject.h>
#include <qmap.h>

namespace libimmsnp {
class Chat;
class ChatListener;
class Client : public  QObject{
	
Q_OBJECT
	
public:
	Client(QString msnPassport, QString msnPass, State initialStatus);
	void connect ();
	void run ();
	virtual ~Client();
	int getIdtr();
	void send(Command& c, int chat = 0);
	void disconnect();
	void makeConnection (QString ip, int port);
	void startConnection();
	void sendChat(int chatId, QString msg);
	void initChat(QString destPassport);
	void closeChat(int chatId);

	void changeStatus (State newStatus);
	void changeNick(QString nick);

	// Listener's connections
	void addConnectionListener (ConnectionListener* cl);
	void delConnectionListener (ConnectionListener* cl);
	
	void addRosterListener (RosterListener *rl);
	void delRosterListener (RosterListener *rl);

	void addPresenceListener(PresenceListener *pl);
	void delPresenceListener(PresenceListener *pl);
	
	void addChatListener(ChatListener *chl);
	void delChatListener(ChatListener *chl);

signals:
	void notifyConnect();
	void notifyDisconnect(ConnectionError e);
	void notifyNewContact(Contact*);
	void notifyNewGroup(Group*);
	void notifyPresence(Contact*);
	void notifyPersonalMessage(Contact*);

	void notifyNewChat (int, QString);
	void notifyChatLeavedTheRoom (int, QString);
	void notifyChatIsTyping(int, QString);
	void notifyChatInfo (int, QString, QString);
	void notifyChatArrivedMessage (int, QString, QString);


public slots:
	void connected ();
	void disconnected (ConnectionError e);
	void newGroupArrived (Group* g);

	void newContactArrived (Contact* c);
	void statusChanged(QString passport, State status, QString displayName, QString capabilities);
	void personalMessage (QString passport, QString personalMsg);

	void chatRequest(QString ipPort, QString passport, QString ticket, QString sessionId);
	void chatArrivedMessage (int chatId, QString msnPassport, QString chatMsg);
	void chatInfo(int chatId, QString chatMsnClient, QString chatIsLogging);
	void chatIsTyping(int chatId, QString chatMsnPassport);
	void chatLeavedTheRoom (int chatId, QString chatMsnPassport);
//	void contactDisconnected (QString msnPassport);
//	void chatCreated (QString hostPort, QString ticket);
//
	void initChatSB(QString, QString);
	void newChat(int, QString);
	
private:
	typedef QMap<int, Chat*> ChatMap;

	ParserNS* m_parser;
	Connection* m_conn;
	msocket* m_mainSocket;
	QString m_msnPassport;
	QString m_msnPass;
	int m_idtr;
	Roster* m_roster;
	State m_initialStatus;
	int m_chatCount;
	ChatMap m_chatList;
	QString m_destPassport;
};
}
#endif // _CLIENT_H_
