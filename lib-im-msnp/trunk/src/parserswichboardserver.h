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

#ifndef _PARSERSB_H_
#define _PARSERSB_H_

#include "libimmsnp.h"

#include "msocket.h"
#include "client.h"
#include "command.h"
#include "cmdswichboardserver.h"
#include "cmdp2p.h"
#include <qobject.h>
#include <qthread.h>

namespace libimmsnp {

class Client;

class ParserSB :public QThread {

Q_OBJECT

public:
	ParserSB(QString, int, int, QString msnPassport, QString ticket, QString sessid, Client* c);
	void run();

	int hasData() {return m_buf.size();}

	void send (Command& c);
	void feed(QByteArray);
	void parse();

	void parseIro();
	void parseAns();

	void acceptFileTransfer (P2P*, QByteArray);

	void parseMsg();
	void parseUsr();
	void parseCal();
	void parseAck();
	void parseJoi();
	void parseBye();

	void parseError();

	int nextIdtr (){return m_idtr++;};
	
	~ParserSB(){printf ("MSN::ParserSB::log ## Parser deleted\n");}

signals:
       void chatArrivedMessage (int, QString, MSG);
       void chatInfo (int, QString, QString);
       void chatIsTyping (int, QString);
       void chatLeavedTheRoom(int, QString);
       void newChat (int, QString);

       void incomingFileTransfer (P2P* , int);
       void fileTransferProgress(QByteArray ,int, int);
       void fileTransferFinished(QByteArray);

private:
	QString m_address;
	int m_port;
	int m_idtr;
	Client* m_client;
	msocket* m_socket;
	int m_chatId;
	QString m_msnPassport;
	QString m_ticket;
	QString m_sessid;

	typedef QMap<QByteArray, P2P*> P2PMap;
	P2PMap m_p2pList;

	QByteArray m_buf;

	bool m_isParsing;
	bool m_hasCommand;

	QString m_buddy;
	bool m_endChat;

};
}
	
#endif // _PARSERSB_H_
