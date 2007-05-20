#include "libimmsnp.h"
#include "msocket.h"
#include "client.h"
#include <qthread.h>

namespace libimmsnp {

class Client;

class MsnTest  : public QThread {
public:
        MsnTest ();
	//void Connect ();
	void run();
	void initChat (QString chatFriend);
	~MsnTest();
private:
	Client*  m_client;

};
}

