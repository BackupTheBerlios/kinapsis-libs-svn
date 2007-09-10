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

#include "msn.h"

namespace libimmsnp {


MsnTest::MsnTest () {
//	QString id ("probando_msnpy@hotmail.com");
//	QString pass ("gargolas");
	m_client = new Client (id, pass, STATUS_HDN);
        m_client->addConnectionListener(this);

}

void MsnTest::onConnect()  {
        m_connected = 1;
        printf("# CONNECTED\n\n\n");
}

void MsnTest::onDisconnect(ConnectionError) {
        printf("# DISCONNECTED\n\n\n");
}

void MsnTest::run (){
	m_client->connect();
	qDebug("END OF CONNECT");
}

void MsnTest::initChat (QString chatFriend){
	m_client->initChat (chatFriend);
}


MsnTest::~MsnTest() {
}

}

int main(int argc, char *argv[]){
	using namespace libimmsnp;
	QCoreApplication a(argc, argv); 
	MsnTest cliente;
	cliente.setApp(&a); 
	cliente.start();
	a.exec(); 
	cliente.wait();
}

