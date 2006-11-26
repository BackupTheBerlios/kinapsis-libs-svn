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

#ifndef _PARSER_H_
#define _PARSER_H_

#include "libimmsnp.h"
#include "roster.h"
#include "msocket.h"
#include "contact.h"
#include <qobject.h>
#include <qstringlist.h>
#include <string>
#include <iostream>

namespace libimmsnp {
class Parser :public QObject {
Q_OBJECT
	
public:
	Parser(std::string msnPassport, std::string msnPass);
	Parser(std::string msnPassport, std::string msnPass, int chatId);
	ParserOperation makeRoaster (msocket* s, std::string bufSyn);
	ParserOperation feed (std::string cmd);
	std::string getMsnPassport(){ return m_msnPassport;};
	std::string getMsnPass(){ return m_msnPass;};

signals:
	void newGroupArrived (Group*);
	void newContactArrived (Contact*);
	void statusChanged (QString, PresenceStatus, QString, QString); 
	void personalMessage (QString, QString);
	void chatRequest (QString, QString, QString, QString);
	void chatArrivedMessage (int, QString, QString);
	void chatInfo (int, QString, QString);
	void chatIsTyping (int, QString);
	void chatLeavedTheRoom (int, QString);
	void contactDisconnected (QString);
	void chatCreated (QString, QString);

private:
	QString m_cmd;
	int m_processingCmd;
	int m_chatId;
	std::string m_msnPassport;
	std::string m_msnPass;
};
}

#endif // _PARSER_H_
