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

#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "libimmsnp.h"
#include "parsernotificationserver.h"
#include "msocket.h"
#include "buffer.h"
#include <string>
#include <qobject.h>

namespace libimmsnp {
class Connection : public QObject {
Q_OBJECT
public:
	// Constructor
	Connection(ParserNS*, QString server, int port, QString msnPassport, QString msnPass, msocket*);


	// Destructor
	virtual ~Connection();	

	int  setTicket (std::string ticket);
	// Starting connection
	void connect(std::string initialStatus);
	
	ConnectionError VER ();
	ConnectionError CVR ();
	ConnectionError USRIni ();
	ConnectionError USRFin ();
	ConnectionError Tweener();
	ConnectionError SYN ();
	ConnectionError CHG (std::string initialStatus);
	void challenge (const char *szChallenge, char *szOutput);
	ConnectionError QRY (std::string challenge);
	std::string httpsReq (std::string url, std::string headers);
	
	// convert int to string
	std::string itos(int i);
	
	std::string replace (std::string& linea, std::string oldChr, std::string newChr);

	std::string urlEncode (std::string& linea);

//	// Desconecto
//	void disconnect();
//
//	// Return connection state
//	ConnectionState state() const { return m_state; };

signals:
	void mainSocket(msocket* mainSocket);
	void connected();
	void disconnected();

private:
	ParserNS* m_parser;
	ConnectionState m_state;
	ConnectionError m_disconnect;
	
	Buffer m_buf;	
	
	QString m_server;
	int m_port;
	
	QString m_msnPassport;	
	QString m_msnPass;
	
	msocket* m_socket;
	const int m_bufsize;
	bool m_cancel;
	int m_idtr;

	std::string m_ticket;

}; 
}

#endif // _CONNECTION_H_


