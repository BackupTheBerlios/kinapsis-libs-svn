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

namespace libimmsnp {

Connection::Connection (msocket* s, ParserNS* p, int iter){
	m_socket = s;
	m_parser = p;
	m_iterations = iter;
	m_disconnect = false;
}

void Connection:: run (){
	QString data;
	int size;
	printf("MSN::Log::Connection ## Start Run\n");
	while ((size = (m_socket->recv(data))) != -1){
		printf ("MSN::Log::Connection ## Receiving: %i\n",size);
		if (size == 0) {
			printf ("MSN::Log::Connection ## Connection closed unexpectly. Host:%s\n",QString (m_socket->getHost()).latin1());
			break;
		}
		m_parser->feed (data);

		if (m_iterations > 0) {
			m_iterations--;
			printf("MSN::Log::Connection ## itera:%i\n",m_iterations);
		}

		if (m_iterations == 0) {
			delete m_socket;
			m_socket = 0;
			m_parser->parse();
			break;
		}

		if (!m_parser->isParsing()){m_parser->parse();}
		data = "";
	}
	printf("MSN::Log::Connection ## END Run\n");
	if (m_disconnect) emit disconnected (ConnUserDisconnected);
}

Connection::~Connection (){
	// TODO:clean the parser
	delete m_parser;
	printf("MSN::Log::Connection ## Connection Deleted\n");
}
}
#include "connection.moc"
