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
	qDebug ("HOLAAA");
	m_socket = s;
	m_parser = p;
}

void Connection::run (){
	qDebug ("HOLAAA2");
	QString data;
	while ((m_socket->recv(data)) != -1){
		m_parser->feed (data);
		m_parser->parse();
	}
}

Connection::~Connection (){
}
}
