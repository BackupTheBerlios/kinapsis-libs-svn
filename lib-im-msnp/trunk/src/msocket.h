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

#include <QTcpSocket>
#include <QByteArray>
#include <QSslSocket>
#include "libimmsnp.h"


namespace libimmsnp {

class msocket :public QTcpSocket{

Q_OBJECT

public:
	//constructors
	msocket (bool ssl = false);

	// set port
//	int setPort (int);

	//set host
//	int setHost (QString host);
	
	QString getHost(){return m_host;};
	quint16 getPort(){return m_port;};

	// Start the connection
	int connect (QString host, quint16 port);

	// send the buffer's content
	int send (QByteArray buf);
	int send (QString str);
	
	// recive into buffer
	int recv (QByteArray&);

	// destructor
	~msocket ();

protected:
	quint16 m_port ;
	QString m_host ;
	QSslSocket *m_sslSocket;
	QTcpSocket *m_socket;
	QByteArray m_buffer;
	bool m_ssl;
//signals:
//
//void disconnected(ConnectionError);

};
}

#endif //_MSOCKET_H
