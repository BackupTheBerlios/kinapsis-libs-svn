#include "libimmsnp.h"
#include "msocket.h"
#include "client.h"

#include <qthread.h>
#include <QCoreApplication> 

namespace libimmsnp {

class Client;

class MsnTest  : public QThread, ConnectionListener {

public:
        MsnTest ();
	//void Connect ();
	void run();
	void initChat (QString chatFriend);
        void onConnect();
        void onDisconnect(ConnectionError);
	void setApp(QCoreApplication* a)   { mainLoop = a; }; 

	~MsnTest();
private:
	Client*  m_client;
        int m_connected;
	QCoreApplication*  mainLoop;

};
}

