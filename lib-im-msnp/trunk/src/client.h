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
#include "contact.h"
#include "roster.h"
#include "connection.h"
#include "connectionlistener.h"
#include "msocket.h"
#include "buffer.h"
#include "command.h"
#include "rosterlistener.h"
#include "presencelistener.h"
//#include "chatlistener.h"
//#include "chat.h"

#include <sstream>
#include <string>
#include <qobject.h>
#include <qptrlist.h>

namespace libimmsnp {
class Client : public  QObject{
	
Q_OBJECT
	
public:
	Client(QString msnPassport, QString msnPass, QString initialStatus, QString msnHost = "messenger.hotmail.com", int msnPort = 1863);
	int connect ();
	void run ();
//	int sendTxtMsg (int chatCount, QString chatMsg);
//	void changeStatus (PresenceStatus newStatus);
//	int initNewChat (std::string chatFriend);
	virtual ~Client();
	int getIdtr();
	void send(Command& c);
	void makeConnection (QString ip, int port);
	void startConnection();

	// Listener's connections
	void addConnectionListener (ConnectionListener* cl);
	void delConnectionListener (ConnectionListener* cl);
	
	void addRosterListener (RosterListener *rl);
	void delRosterListener (RosterListener *rl);
//	
	void addPresenceListener(PresenceListener *pl);
	void delPresenceListener(PresenceListener *pl);
//		
//	void addChatListener(ChatListener *chl);
//	void delChatListener(ChatListener *chl);
	
signals:
	void notifyConnect();
	void notifyDisconnect();
	void notifyNewContact(Contact*);
	void notifyPresence(QString, State, QString, QString);
	void notifyPersonalMessage(QString, QString);
	void notifyHasBlog(QString);
//	void notifyContactDisconnected (QString);
//
//	void notifyNewChat (int, QString);
//	void notifyChatLeavedTheRoom (int, QString);
//	void notifyChatIsTyping(int, QString);
//	void notifyChatInfo (int, QString, QString);
//	void notifyChatArrivedMessage (int, QString, QString);

public slots:
	void mainSocket(msocket* mainSocket);
	void connected ();
	void disconnected ();
	void newGroupArrived (Group* g);

	void newContactArrived (Contact* c);
	void statusChanged(QString passport, State status, QString displayName, QString capabilities);
	void personalMessage (QString passport, QString personalMsg);
	void hasBlog (QString passport);
//	
//	void chatRequest(QString hostPort, QString msnPassport, QString ticket, QString sessid);
//	void chatArrivedMessage (int chatId, QString msnPassport, QString chatMsg);
//	void chatInfo(int chatId, QString chatMsnClient, QString chatIsLogging);
//	void chatIsTyping(int chatId, QString chatMsnPassport);
//	void chatLeavedTheRoom (int chatId, QString chatMsnPassport);
//	void contactDisconnected (QString msnPassport);
//	void chatCreated (QString hostPort, QString ticket);
	
private:
	ParserNS* m_parser;
	Connection* m_conn;
	msocket* m_mainSocket;
//	int m_chatCount;
	QString m_msnPassport;
	QString m_msnPass;
	int m_idtr;
	Roster* m_roster;
	QString m_initialStatus;
	QPtrList<QThread> m_threads;
};
}
#endif // _CLIENT_H_
