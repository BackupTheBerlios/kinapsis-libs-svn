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
#include "buffer.h"
#include "msocket.h"
#include "connection.h"
#include "contact.h"
#include "client.h"
#include <curl/curl.h>
#include <qobject.h>

namespace libimmsnp {


class ParserSB :public QObject {
Q_OBJECT
public:
	ParserSB(msocket*, Client*, int);
	int hasData(){return m_buf.len();}
	void feed(QString);
	bool isParsing() {return m_isParsing;}
	void parse();
	void parseIro();
	void parseAns();
	void parseMsg();
	void parseBye();
private:
	int m_idtr;
	Buffer m_buf;
	Client* m_client;
	msocket* m_socket;
	bool m_isParsing;
	bool m_hasCommand;
	int m_chatId;

signals:
       void chatArrivedMessage (int, QString, QString);
       void chatInfo (int, QString, QString);
       void chatIsTyping (int, QString);

};
}
	
#endif // _PARSERSB_H_
