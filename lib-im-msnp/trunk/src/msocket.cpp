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

#include <sys/types.h>       // For data types
#include <sys/socket.h>      // For socket(), connect(), send(), and recv()
#include <netdb.h>           // For gethostbyname()
#include <arpa/inet.h>       // For inet_addr()
#include <unistd.h>          // For close(), dup()
#include <netinet/in.h>      // For sockaddr_in

#include "msocket.h"
namespace libimmsnp {
	
std::string replace (std::string& linea, std::string oldChr, std::string newChr){
	while (1) {
		const unsigned int pos = linea.find(oldChr);
		if (pos == std::string::npos) break;
		linea.replace(pos,1,newChr);
	}
	return linea;
	
}
// Constructor
msocket::msocket (std::string host, int port) : m_port (port),m_host (host), m_sockFd (socket(AF_INET, SOCK_STREAM, 0)){
	 m_buf = (char*)calloc( m_bufsize + 1, sizeof( char ) );
}

msocket::msocket (std::string hostPort) {
	
        int tamIP = hostPort.find(':');
	int posIniPort = hostPort.find(':')+1;
	
	m_host = hostPort.substr(0, tamIP) ;
	m_port = std::atoi ( hostPort.substr(posIniPort, 4).c_str());;
	
	m_sockFd = socket(AF_INET, SOCK_STREAM, 0);
	m_buf = (char*)calloc( m_bufsize + 1, sizeof( char ));
}

msocket::msocket () : m_port (0),m_host (""), m_sockFd (socket(AF_INET, SOCK_STREAM, 0)){
	 m_buf = (char*)calloc( m_bufsize + 1, sizeof( char ) );
}

//Copy constructor
msocket::msocket(const msocket& s) {
	m_port = s.m_port;
	m_host = s.m_host;
	m_sockFd  = dup(s.m_sockFd);
	m_SockAddr = s.m_SockAddr;

	m_buf = new char [m_bufsize];
	strcpy (m_buf , s.m_buf);
}

int msocket::setPort (int port){
	m_port = port;
	return 0;
}

int msocket::setHost (std::string host){
	m_host = host;
	return 0;
}


// connection
int msocket::connect () {
	
	struct hostent *pHostEnt;
	
	// m_host is an ip number
	if(m_host[0] >= '0' && m_host[0] <= '9' )
		m_SockAddr.sin_addr.s_addr = inet_addr( m_host.c_str() );
	// m_host is a name
	else {
		pHostEnt = gethostbyname( m_host.c_str() );
		memcpy( &m_SockAddr.sin_addr, pHostEnt->h_addr, pHostEnt->h_length );
	}
	
	// Socket type
	m_SockAddr.sin_family = AF_INET;
	
	// Port
	m_SockAddr.sin_port = htons( m_port );
	::connect ( m_sockFd , (struct sockaddr *) &m_SockAddr , sizeof(struct sockaddr));
	printf ("MSN::Log::Socket ## Connected with host: %s in port:%i\n",m_host.c_str(),m_port);
	return 0;
}

int msocket::send (QString buf){
	int size;
	size = ::send (m_sockFd, buf.utf8().data(), buf.length(), 0);
	QString tmpData = buf;
	printf("MSN::Log::Socket ##  >>> Host:%s len:%i Data:%s\n",m_host.c_str(), size, QString(tmpData.utf8().data()).replace('\n',"\\n").replace('\r',"\\r").latin1());
	return size;
}

int msocket::recv (QString& buf){
	const int MAX_MSG = 2000;
	int size = 0;
	char data[MAX_MSG] ;

	if ((size = ::recv (m_sockFd, data, MAX_MSG, 0)) == -1){
		printf ("MSN::Log::Socket ##  <<< host: %s len: %i Error reciving  from socket\n",m_host.c_str(), size);
		emit disconnected (ConnSocketError);
		return -1;
	}

	buf += QString(data).mid(0,size);
	QString tmpData = buf;
	if (size >0)
	printf ("MSN::Log::Socket ##  <<< Host:%s len:%i Data:%s\n",m_host.c_str(), size,QString(data).mid(0,size).replace('\n',"\\n").replace('\r',"\\r").latin1());

	return size;
}

msocket::~msocket () {
	close(m_sockFd);
	printf ("MSN::Log::Socket ## Socket closed with host: %s:%i\n",m_host.c_str(),m_port);
}

msocket& msocket::operator= (const msocket &s){
		m_port = s.m_port;
		m_host = s.m_host;
		m_sockFd  = dup(s.m_sockFd);
		m_SockAddr = s.m_SockAddr;

		m_buf = new char [m_bufsize];
		strcpy (m_buf , s.m_buf);
	return (*this);
}
}
#include "msocket.moc"
