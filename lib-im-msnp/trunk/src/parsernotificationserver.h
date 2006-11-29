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
#include "connection.h"
#include <curl/curl.h>
#include <qobject.h>

namespace libimmsnp {

class Client;

class ParserNS:public QObject {
Q_OBJECT

public:
	ParserNS (QString, QString, Client*);
	int hasData(){return m_buf.len();}
	void feed (Buffer);
	void feed (QString);
	void parseVer();
	void parseCvr();
	std::string httpsReq (std::string url, std::string headers);
	void parseXfr();
	void parseUsr();
	void parseMsg();
	void parseSbs();
	void parseSyn();
	void parseGtc();
	void parseBlp();
	void parsePrp();
	void parseLsg();
	void parseLst();
	void parseChg();
	void parseChl();
	void parseQry();
	bool isParsing() {return m_isParsing;}
	void parseNln();
	void parseUbx();
	void parseBpr();
	void parseFln();
	void parse ();

private:
	int m_idtr;
	QString m_ticket;
	Buffer m_buf;
	QString m_msnPassport;
	QString m_msnPass;
	int m_connectionSteps;
	int m_groups;
	int m_contacts;
	QString m_initialStatus;
	Client* m_client;
	msocket* m_socket;
	bool m_isParsing;
	bool m_hasCommand;
	QString m_prevContact;


signals:
	void mainSocket(msocket*);
	void connected ();

};
}
	
#endif // _PARSERNS_H_
