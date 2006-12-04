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

//#include "../connection.h"
//#include "../parser.h"
#include "../parsernotificationserver.h"
#include "../client.h"
#include "../buffer.h"
#include "../msocket.h"
#include "../libimmsnp.h"

#include <string>
#include <iostream>
#include <qobject.h>
#include <qstring.h>
#include <qthread.h>

namespace libimmsnp {

class MsnTest : public QThread, ConnectionListener, RosterListener, PresenceListener, ChatListener{

public:
	MsnTest ();
	~MsnTest();

	void onConnect();
	void onDisconnect(ConnectionError);
	void run();
	int getConnected() {return m_connected;};
//	int getChatId () {return m_chatid;};
//
	void onNewContact(Contact* c);
	void onNewGroup(Group*);
	void presenceChanged(QString c, State status, QString msnPersMsg, QString msnCapabilities);
	void personalMessage (QString c, QString msg);
	void hasBlog(QString c);
//	void contactDisconnected(QString msnPassport);
//
	void newChat (int chatId, QString msnPassport);
//	void chatLeavedTheRoom(int chatId, QString chatMsnPassport);
	void chatIsTyping(int chatId, QString chatMsnPassport);
	void chatInfo(int chatId, QString chatMsnClient, QString chatIsLogging);
	void chatArrivedMessage(int chatId, QString chatMsnPassport, QString chatMsg);
//
//	void sendTxtMsg (int chatCount, QString chatMsg);
	void changeStatus (State newStatus);
	void changeNick (QString nick);
//	void initNewChat (std::string chatFriend);

private:
	std::string m_initialStatus;
	int m_connected;
	int m_chatid;
};
}
