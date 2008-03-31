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

#include "connectionlistener.h"
#include "rosterlistener.h"
#include "presencelistener.h"
#include "chatlistener.h"

#include "command.h"
#include "cmdswichboardserver.h"
#include "msocket.h"

#include "cmdlogin.h"
#include "roster.h"

#include "cmdp2p.h"

#include <QObject>
#include <QStringList>

namespace libimmsnp {

class ParserNS;
class ParserSB;

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
	void sendChat(int chatId, Command& msg);
	void addContact(Contact& contact, Group& group = *(new Group()));
	void delContact(Contact& contact);
	void blockContact(Contact& contact);
	void deblockContact(Contact& contact);
	void addGroup(Group& group);
	void delGroup(Group& group);

	void changeStatus (State newStatus);

	QString getClientName () {return QString("libMsn");}
	QString getClientVer () {return QString("0.1beta");}
	QString getClientIp () {return m_ip;}
	QString getClientPort () {return m_port;}
	int getClientIsLogging () {return 1;}

        // Listener's connections
        void addConnectionListener (ConnectionListener* cl);
        void delConnectionListener (ConnectionListener* cl);

	void addRosterListener (RosterListener *rl);
	void delRosterListener (RosterListener *rl);

	void addPresenceListener(PresenceListener *pl);
	void delPresenceListener(PresenceListener *pl);
	
	void addChatListener(ChatListener *chl);
	void delChatListener(ChatListener *chl);

	virtual ~Client();

public slots:
	void clientIpPort(QString ip, QString port);
	void connected ();
	void disconnected (ConnectionError e);

	void newGroupArrived (Group* g);
	void newContactArrived (Contact* c);
	void statusChanged(QString passport, State status, QString displayName, QString capabilities);
	void personalMessage (QString passport, QString personalMsg);
	void hasBlog (QString passport);
	void groupDeleted (QString groupId);

	void chatRequest(QString address, int port, QString contact, QString fName, QString ticket, QString sessid);
	void chatArrivedMessage (int chatId, QString msnPassport, MSG chatMsg);
	void chatInfo(int chatId, QString chatMsnClient, QString chatIsLogging);
	void chatIsTyping(int chatId, QString chatMsnPassport);
	void chatLeavedTheRoom (int chatId, QString chatMsnPassport);

        void incomingFileTransfer (P2P* msg, int chatId);
       	void fileTransferProgress(QByteArray ftId,int received, int total);
       	void fileTransferFinished(QByteArray ftId);
	
//	void contactDisconnected (QString msnPassport);
//	void chatCreated (QString hostPort, QString ticket);
//
//	void initChatSB(QString, QString);
//	void newChat(int, QString);
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
	void notifyChatArrivedMessage (int, QString, MSG);
	
private:
	ParserNS* m_parser;
	msocket* m_mainSocket;
	QString m_msnPassport;
	QString m_msnPass;
	int m_idtr;
	int m_chatCount;
	State m_initialStatus;
	Roster* m_roster;
	QString m_ip;
	QString m_port;

	typedef QMap<int, ParserSB*> ChatMap;
	ChatMap m_chatList;

	QStringList m_destPassport;
};
}
#endif // _CLIENT_H_
