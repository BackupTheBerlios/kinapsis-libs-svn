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

#include "parserswichboardserver.h"
#include <qstringlist.h>

namespace libimmsnp {
ParserSB::ParserSB(msocket* s,Client* c){
	m_idtr = 0;
	m_socket = s;
	m_client = c;
	m_hasCommand = false;
	m_isParsing = false;
}

void ParserSB::feed (QString s){
	m_hasCommand = true;
	m_buf << s;
}

void ParserSB::parseIro (){
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_buf.advance (l);
		m_buf.removeFromBegin();
	}
	else m_hasCommand = false;
}

void ParserSB::parseAns (){
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_buf.advance (l);
		m_buf.removeFromBegin();
	}
	else m_hasCommand = false;
}

void ParserSB::parseMsg (){
	//MSG vaticano666@hotmail.com Cambiando_nick 139\r\nMIME-Version: 1.0\r\nContent-Type: text/plain; charset=UTF-8\r\nUser-Agent: Gaim/1.5.0\r\nX-MMS-IM-Format: FN=arial; EF=; CO=000000; PF=0\r\n\r\nholaMSG vaticano666@hotmail.com Cambiando_nick 96\r\nMIME-Version: 1.0\r\nContent-Type: text/x-clientcaps\r\n\r\nClient-Name: Gaim/1.5.0\r\nChat-Logging: Y\r\n
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_buf.advance (l);
		QStringList fields = QStringList::split(" ",s);
		QString passport = fields[0];
		QString nickCaller = fields[1];
		int payload = fields[2].replace("\r\n","").toInt();
		printf ("Msg form%s nick:%s payload:%i\n",passport.latin1(),nickCaller.latin1(),payload);
		printf ("###IniData:%s\n",s.replace("\r\n","\\r\\n").latin1());
		QString dataPayload;
		if ((m_buf.getNChar (dataPayload,payload)) == payload){
			m_buf.advance (payload);
			m_buf.removeFromBegin();
			printf ("MSG Data:%s\n", dataPayload.replace("\r\n","\\r\\n").latin1());
		}
		else m_hasCommand = false;
	}
	else m_hasCommand = false;
}

void ParserSB::parseBye (){
	// 
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_buf.advance (l);
		m_buf.removeFromBegin();
		QStringList fields = QStringList::split(" ",s);
		QString passport = fields[0].replace("\r\n","");
		printf ("Contact:%s has left the room\n");
	}
	else m_hasCommand = false;
}

void ParserSB::parse (){
	m_isParsing = true;
	QString cmd;
	int idtr;
	int lIdtr;

	QString d1;
	m_buf.data(d1);
	while (m_buf.len() && m_hasCommand){
		/* We have more data in the buffer */
		m_buf.gotoBegin();

		cmd = "";
		m_buf.getQString(cmd, 3);

		m_buf += 3; // The command

		m_buf.setPosition(3);
		if (cmd == "IRO"){
				printf ("MSN::Log::ParserSB ## Parsing IRO\n");
				//IRO 1 1 1 carlitos@hotmail.com USHER%20-%20weeeeee\r\n
				lIdtr = m_buf.getInt (idtr);
				m_buf.advance (1+lIdtr+1);
				// TODO : quitar de la lista de comprobacion
				parseIro();
		}
		else if (cmd == "ANS"){
				printf ("MSN::Log::ParserSB ## Parsing ANS\n");
				// ANS 1 OK\r\n
				lIdtr = m_buf.getInt (idtr);
				m_buf.advance (1+lIdtr+1);
				// TODO : quitar de la lista de comprobacion
				parseAns();
		}
		else if (cmd == "MSG"){
				printf ("MSN::Log::ParserSB ## Parsing MSG\n");
				// MSG passport nick 139\r\n
				m_buf.advance (1);
				// TODO : quitar de la lista de comprobacion
				parseMsg();
		}
		else if (cmd == "BYE"){
				//BYE passport\r\n
				printf ("MSN::Log::ParserSB ## Parsing MSG\n");
				m_buf.advance (1);
				parseBye();
		}
		else {	
			QString error;
			m_buf.data(error);
			printf ("MSN::ERROR::ParserSB ## Unknow Command: %s\n",error.replace('\n',"\\n").replace('\r',"\\r").latin1());
			break;			
		}
	QString d;
	m_buf.data(d);
	printf ("MSN::Log::ParserSB ## Buffer Data<%s>\n", d.replace('\n',"\\n").replace('\r',"\\r").latin1());
	}	
	m_isParsing = false;
}

}
