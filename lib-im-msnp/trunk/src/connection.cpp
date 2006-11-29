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

#include "connection.h"
#include "parsernotificationserver.h"
#include <qobject.h>

namespace libimmsnp {

Connection::Connection (msocket* s, ParserNS* p){
	m_socket = s;
	m_parser = p;
}

void Connection::run (){
	QString data;
	int size;
	while ((size = (m_socket->recv(data))) != -1){
		//qDebug ("//////////Receiving");
		////qDebug ("%1").arg(m_parser->hasData());
		if (size == 0) {
			//qDebug ("Connection closed unexpectly. Host:" + QString (m_socket->getHost()));
			this->exit();
		}
		//while (!m_parser->hasData()){}
		//qDebug ("//////////Filling the Parser");
		m_parser->feed (data);
		while (m_parser->isParsing()){}
		m_parser->parse();
		//qDebug ("//////////Out");
		data = "";
	}
	//qDebug("________________End of Run");
	this->exit();
}

Connection::~Connection (){
}
}
