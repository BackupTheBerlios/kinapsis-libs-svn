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

#ifndef _MSOCKET_H
#define _MSOCKET_H

#include <sys/types.h>       // For data types
#include <sys/socket.h>      // For socket(), connect(), send(), and recv()
#include <netdb.h>           // For gethostbyname()
#include <arpa/inet.h>       // For inet_addr()
#include <unistd.h>          // For close()
#include <netinet/in.h>      // For sockaddr_in
#include <string>
#include <qobject.h>
#include "buffer.h"


namespace libimmsnp {
class msocket :public QObject{
Q_OBJECT
public:
	//constructors
	msocket (std::string host, int port);
	msocket (std::string hostPort);
	msocket ();
	//Copy constructors
	msocket(const msocket&);

	// set port
	int setPort (int);

	//set host
	int setHost (std::string host);
	
	std::string getHost(){return m_host;};
	int getPort(){return m_port;};
	// Start the connection
	int connect ();

	// send the buffer's content
	int send (QString buf);
	
	// recive into buffer
	int recv (QString&);

	//operator =
	msocket& operator= (const msocket&);

	
	// destructor
	~msocket ();

protected:
	int m_port ;
	std::string m_host ;
	int m_sockFd ;
	struct sockaddr_in m_SockAddr;
	char* m_buf;
	const static int m_bufsize = 20000;
signals:
void disconnected(ConnectionError);

};
}

#endif //_MSOCKET_H
