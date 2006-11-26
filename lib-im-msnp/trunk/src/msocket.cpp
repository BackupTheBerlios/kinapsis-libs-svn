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
#include <iostream> 	     // For cout cin

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
	std::cout << "Connected with host: " << m_host << " in port: " << m_port << "\n";
	return 0;
}

int msocket::send (std::string buf){
	int size;
	size = ::send (m_sockFd, buf.c_str(), buf.length(), 0);
	std::string tmp = buf;
	replace (tmp,"\r","\\r");
	//std::cout << ">>> host: " << m_host << " len: "<< size << " data: " << replace (tmp,"\n","\\n") << "\n";
	return size;
}

int msocket::send (QString buf){
	int size;
	size = ::send (m_sockFd, buf.utf8().data(), buf.length(), 0);
	QString tmpData = buf;
	qDebug(">> len: " + QString("%1").arg(size) + " data: " + QString(tmpData.utf8().data()).replace('\n',"\\n").replace('\r',"\\r"));
	return size;
}

int msocket::send (Buffer buf){
	int size;
	QString data;
	buf.data(data);
	size = ::send (m_sockFd, data.utf8().data(), buf.len(), 0);
	QString tmpData = data;
	qDebug(">> data: " + tmpData.replace('\n',"\\n").replace('\r',"\\r"));
	return size;
}

int msocket::recv (std::string& buf){
	const int MAX_MSG = 2000;
	int size = 0;
	if ((size = ::recv (m_sockFd, m_buf, MAX_MSG, 0)) == -1){
		std::cout << "<< host: " << m_host << " << len: "<< size << " Error reciving  from socket\n";
	}
	buf.assign( m_buf,  size );
	std::string tmp = buf;
	replace (tmp,"\r","\\r");
	//std::cout << "<<< host: " << m_host << " len: "<< size << " data: " << replace (tmp,"\n","\\n") << "\n";
	return size;
}

int msocket::recv (QString& buf){
	const int MAX_MSG = 2000;
	int size = 0;
	char data[MAX_MSG] ;

	if ((size = ::recv (m_sockFd, data, MAX_MSG, 0)) == -1){
		std::cout << "<< host: " << m_host << " << len: "<< size << " Error reciving  from socket\n";
		return -1;
	}

	buf += QString(data).mid(0,size);
	QString tmpData = buf;
	if (size >0)
	qDebug ("<< len: " + QString("%1").arg(size) + " data: " + tmpData.replace('\n',"\\n").replace('\r',"\\r")); 

	return size;
}
 
int msocket::recv (Buffer& buf){
	const int MAX_MSG = 2000;
	int size = 0;
	char data[MAX_MSG] ;
	if ((size = ::recv (m_sockFd, data, MAX_MSG, 0)) == -1){
		//std::cout << "<< host: " << m_host << " << len: "<< size << " Error reciving  from socket\n";
	}
	QString b;
	buf << QString (data);
	buf.data(b);
	QString tmpData = b;
	qDebug ("<< data: " + tmpData.replace('\n',"\\n").replace('\r',"\\r")); 

	return size;
}

msocket::~msocket () {
	close(m_sockFd);
	//std::cout << "Socket Closed with: " << m_host << ":" << m_port << "\n";
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
