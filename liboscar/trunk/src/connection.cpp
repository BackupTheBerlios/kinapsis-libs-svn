/***************************************************************************
 *   Copyright (C) 2005 by Luis Cidoncha                                   *
 *   luis.cidoncha@gmail.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "connection.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#define BUFLEN 1024

namespace liboscar {

Connection::Connection(const QString server, int port, Parser* parser){
	m_server = server;
	m_port = port;
	m_exit = false;
	m_parser = parser;
	m_socket = -1;
	QObject::connect(this, SIGNAL(dataReceived()), m_parser, SLOT(parse()));
	clear();
}

ConnectionStatus Connection::getStatus(){
	return m_status;
}

ConnectionStatus Connection::clear(){
	m_status = CONN_DISCONNECTED;
	if (m_socket != -1){
		close(m_socket);
		m_socket = -1;
	}
	return m_status;
}

ConnectionStatus Connection::connect(){

	if (m_socket != -1)
		return m_status;

	m_status = CONN_CONNECTING;

	struct hostent *host;
	struct sockaddr_in sockaddr;

	if ((host = gethostbyname(m_server.ascii())) == NULL)
		return clear();
	memcpy(&sockaddr.sin_addr, *host->h_addr_list, host->h_length);

	m_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (m_socket < 0)
		return clear();

	sockaddr.sin_port = ntohs(m_port);
	sockaddr.sin_family = AF_INET;

	m_local.sin_family = AF_INET;
	m_local.sin_addr.s_addr = INADDR_ANY;

	if (!::connect(m_socket, (struct sockaddr *) &sockaddr, sizeof(struct sockaddr)))
		return CONN_CONNECTED;

	return clear();

}

DWord Connection::getLocalIP(){
	return ntohl(m_local.sin_addr.s_addr);
}

Word Connection::getPort(){
	return ntohs(m_local.sin_port);
}

ConnectionError Connection::listen(){

	ConnectionError e;
	while (!m_exit){
		e = receive();
		if (e != CONN_NO_ERROR )
			return e;
		emit dataReceived(); 
	}
	return CONN_NO_ERROR;
}

ConnectionError Connection::receive(){
	fd_set fset;
	struct timeval t;

	FD_ZERO(&fset);
	FD_SET(m_socket, &fset);

	while (!FD_ISSET(m_socket, &fset) && !m_exit){
		t.tv_sec = 1; /* FIXME: provisional */
		t.tv_usec = 0;
		if (select(m_socket + 1, &fset, NULL, NULL, &t) == -1)
			return CONN_INPUT_ERROR;
	}
	
	if (m_exit)
		return CONN_NO_ERROR;

	int ret_val;
	Byte buf[BUFLEN];

	ret_val = recv(m_socket, buf, BUFLEN, 0);
	
	switch (ret_val){
		case -1:
			perror("recv");
			return CONN_INPUT_ERROR;
		case 0:
			return CONN_ERR_USER_REQUEST;
		default:
			m_parser->add(buf, ret_val);
	}
//#ifdef DEBUG
	unsigned int i;
	fprintf (stderr, "RECEIVING: ");
	for (i=0; i < ret_val; i++)
		fprintf(stderr, "%02x ", buf[i]);
	fprintf(stderr, "\n");
//#endif

	return CONN_NO_ERROR;
}

ConnectionError Connection::send(Buffer &b){
	/* Blocks until the data is sent */
	Byte bbuff[b.len()];
	unsigned int size = b.len();
	unsigned int sent_data = 0;
	int ret;

	b.copy(bbuff);
	b.wipe(); /* delete the buffer */

//#ifdef DEBUG
	unsigned int i;
	fprintf (stderr, "SENDING: ");
	for (i=0; i < size; i++)
		fprintf(stderr, "%02x ", bbuff[i]);
	fprintf(stderr, "\n");
//#endif
	while (sent_data < size){
		if ((ret = ::send(m_socket, bbuff + sent_data, size - sent_data, 0)) < 0)
			return CONN_OUTPUT_ERROR;
		sent_data += ret;
	}
	return CONN_NO_ERROR;

}

void Connection::disconnect(){
	m_exit = true;
	clear();
}

Connection::~Connection() { }


}

#include "connection.moc"
