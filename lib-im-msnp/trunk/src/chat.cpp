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

	Chat::Chat (ParserSB* parser, int id, msocket* sock)
		: m_parser(parser), m_id(id), m_socket(sock),m_idtr(1){
	}

	Chat::Chat (const Chat& c) {
		m_id = c.m_id;
		m_parser = c.m_parser;
		m_socket = c.m_socket;
		m_idtr = c.m_idtr;
	}
	
	int Chat::Start() {
		pthread_t th1;
		pthread_create(&th1,NULL,Chat::EntryPoint,this);
		return 0;
	}

	/*static */
	void * Chat::EntryPoint(void * pthis) {
		   Chat* pt = (Chat*)pthis;
		   pt->Run();
	}

	
	
	Chat::~Chat(){
		delete m_socket;
		m_parser = 0;
		delete m_parser;
		m_socket = 0;
		printf ("Socket for chat with id:%i is closed\n", m_id); 
		//emit disconnect(m_id);
	}
	
	int Chat::Run(){
		int lenBuf;
		QString data; 
		while (1){
			if ((lenBuf = m_socket->recv (data)) == -1) {
				delete m_parser;
				m_parser = 0;
				delete m_socket;
				m_socket = 0;
				//emit disconnect(m_id);
			}
			else {
				m_parser->feed(data);
				if (!m_parser->isParsing()){m_parser->parse();}
				data = "";
//				if (bufState == PARSER_BYE) break; 
			}
		}
		delete this;
	}
}
#include "chat.moc"
