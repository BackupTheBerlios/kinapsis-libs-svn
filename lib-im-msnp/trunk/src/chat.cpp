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

// Thread class inspired in http://www.linuxselfhelp.com/HOWTO/C++Programming-HOWTO-18.html#ss18.2

#include "chat.h"
namespace libimmsnp {

	
	Chat::Chat (Parser* parser, int id, msocket* sock)
		: m_id(id), m_parser(parser), m_socket(sock),m_idtrChat(1){
	}

	Chat::Chat (const Chat& c) {
		m_id = c.m_id;
		m_parser = c.m_parser;
		m_socket = c.m_socket;
		m_idtrChat = c.m_idtrChat;
	}
	
	int Chat::Start()
	{
		pthread_t th1;
		//pthread_create(&th1,PTHREAD_CREATE_DETACHED,Run,NULL);
		pthread_create(&th1,NULL,Chat::EntryPoint,this);
		return 0;
	}

	/*static */
	void * Chat::EntryPoint(void * pthis)
	{
		   Chat* pt = (Chat*)pthis;
		   pt->Run();
	}

	
	
	Chat::~Chat(){
		delete m_socket;
		m_parser = 0;
		delete m_parser;
		m_socket = 0;
		std::cout << "Socket for chat with id: " << m_id << " is closed\n"; 
		//emit disconnect(m_id);
	}
	
	int Chat::Run(){
		int lenBuf;
		ParserOperation bufState;
		std::string buf; 
		while (1){
			if ((lenBuf = m_socket->recv (buf)) == -1) {
				delete m_parser;
				m_parser = 0;
				delete m_socket;
				m_socket = 0;
				//emit disconnect(m_id);
			}
			else {
				bufState = m_parser->feed(buf);
				if (bufState == PARSER_BYE) break; 
			}
		}
		delete this;
	}
	
	int Chat::Send (std::string msg){
		Commands msgCmd;
		std::string composedMsg = msgCmd.MSG(m_idtrChat++, msg);
		int lenSended;
		if ((lenSended = m_socket->send(composedMsg)) != composedMsg.length())
			std::cout << "### ERROR : SENDING MESSAGE `" + msg + "'\n";
		return lenSended;
		
	}
	
}
