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

#ifndef _CHAT_H_
#define _CHAT_H_

#include "libimmsnp.h"
#include "parser.h"
#include "msocket.h"
#include "commands.h"
#include <pthread.h>
#include <qobject.h>
#include <string>


namespace libimmsnp {
class Chat : public QObject {
Q_OBJECT
public:
	// Constructor
	Chat (Parser* parser, int id, msocket* sock);
	
	// Copy Constructor
	Chat (const Chat&);
	int getChatId (){ return m_id; };
	std::string getHost(){ return m_socket->getHost(); };
	
	int Start ();
	int Send (std::string msg);
	// Destructor
	virtual ~Chat();	
	
protected:
	static void * EntryPoint(void *);
	int Run();
private:
	//void* Run(void* p);
	int m_id;
	Parser* m_parser;
	msocket* m_socket;
	int m_idtrChat;
}; 
}

#endif // _CHAT_H_

