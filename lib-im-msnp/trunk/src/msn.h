#include "libimmsnp.h"
#include "msocket.h"
#include "client.h"
#include "cmdswichboardserver.h"

#include <qthread.h>
#include <QCoreApplication> 

namespace libimmsnp {

class Client;

class MsnTest  : public QThread, ConnectionListener, RosterListener, PresenceListener, ChatListener {

public:
        MsnTest ();
	//void Connect ();
	void run();
	void initChat (QString chatFriend);
        void onConnect();
        void onDisconnect(ConnectionError);
	void setApp(QCoreApplication* a)   { mainLoop = a; }; 

	void onNewContact(Contact* c);
	void onNewGroup(Group*);
	void presenceChanged(Contact* c);
	void personalMessage (Contact* c);

	void newChat (int chatId, QString msnPassport);
	void chatLeavedTheRoom(int chatId, QString chatMsnPassport);
	void chatIsTyping(int chatId, QString chatMsnPassport);
	void chatInfo(int chatId, QString chatMsnClient, QString chatIsLogging);
	void chatArrivedMessage(int chatId, QString chatMsnPassport, QString chatMsg);

	~MsnTest();
private:
	Client*  m_client;
        int m_connected;
	QCoreApplication*  mainLoop;

};
}

