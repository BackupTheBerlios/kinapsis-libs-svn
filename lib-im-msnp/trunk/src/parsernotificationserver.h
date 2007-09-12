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

#ifndef _PARSERNS_H_
#define _PARSERNS_H_

#include "libimmsnp.h"
#include "buffer.h"
#include "msocket.h"
#include "client.h"
#include "contact.h"
#include <qobject.h>

namespace libimmsnp {

class Client;

class ParserNS:public QObject {

Q_OBJECT

public:
	ParserNS (QString, QString, State, Client*, msocket*, QString);
	void init();

	int hasData() {return m_buf.size();}

	void feed (Buffer);
	void parse ();

	void parseVer();
	void parseCvr();
	void parseXfr();
	void parseGcf();
	void parseUsr();
	void parseMsg();
	void parseSyn();
	void parseGtc();
	void parseBlp();
	void parsePrp();
	void parseLsg();
	void parseLst();
	void parseChg();
	void parseChl();
	void parseQry();
	void parseNln();
	void parseUbx();
	void parseBpr();
	void parseFln();
	void parseRng();
	void parseNot();
	void parseOut();
	void parseRem();
	void parseAdc();
	void parseRmg();
	
	void parseError();

	virtual ~ParserNS() {qDebug ("MSN::ParserNS::log ## Parser deleted");}

signals:
//	void mainSocket(msocket*);
	void connected ();
	void disconnected(ConnectionError);
	void newGroupArrived (Group*);
	void newContactArrived (Contact*);
	void statusChanged (QString, State, QString, QString); 
	void personalMessage (QString, QString);
	void hasBlog (QString);
	void chatRequest (QString, int, QString, QString, QString, QString);
	void groupDeleted(QString);
//	void initChatSB (QString, QString);

private:

	int m_idtr;
	QString m_msnPassport;
	QString m_msnPass;
	QString m_ticket;
	msocket* m_socket;
	Buffer m_buf;
	State m_initialStatus;
	Client* m_client;
	QString m_protocol;
	QString m_prevContact;

	int m_contacts;
	int m_groups;

	bool m_hasCommand;
	bool m_isParsing;

};
}
	
#endif // _PARSERNS_H_
