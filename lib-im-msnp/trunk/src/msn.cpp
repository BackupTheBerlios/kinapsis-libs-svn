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
	QString id ("probando_msnpy@hotmail.com");
	QString pass ("gargolas");
	m_client = new Client (id, pass, STATUS_NLN);
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

int main(void){
	using namespace libimmsnp;
	MsnTest cliente;
	cliente.start();
	cliente.wait();
}

