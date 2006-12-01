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

QString id ("user@hotmail.com");
QString pass ("pass");
Client test(id,pass,QString("HDN"));

MsnTest::MsnTest () {
	test.addConnectionListener(this);
	test.addRosterListener(this);
	test.addPresenceListener(this);
//	test.addChatListener(this);
	m_connected = 0;
//	m_chatid = 0;
}

void MsnTest::run (){
	test.connect();
}

MsnTest::~MsnTest() {
}

void MsnTest::onConnect()  {
	m_connected = 1;
	qDebug("# CONNECTED\n\n\n");
}

void MsnTest::onDisconnect() {
	qDebug("# DISCONNECTED\n\n\n");
}

void MsnTest::onNewContact(Contact* c){
	qDebug ("# CONTACT:" + c->getPassport());
}

void MsnTest::presenceChanged (QString passport, State status, QString displayName, QString capabilities){
	switch (status){
		case online:
			printf("# State Changed. User:%s State:%s Capabilies:%s Personal MSG:%s\n",passport.latin1(),"online", capabilities.latin1(), displayName.latin1());
			break;
		case away:
			printf("# State Changed. User:%s State:%s Capabilies:%s Personal MSG:%s\n",passport.latin1(),"away", capabilities.latin1(), displayName.latin1()); 
			break;
		case dnd:
			printf("# State Changed. User:%s State:%s Capabilies:%s Personal MSG:%s\n",passport.latin1(),"Do Not Disturb", capabilities.latin1(), displayName.latin1()); 
			break;
		case na:
			printf("# State Changed. User:%s State:%s Capabilies:%s Personal MSG:%s\n",passport.latin1(),"Not Available", capabilities.latin1(), displayName.latin1()); 
			break;
		case offline:
			printf("# State Changed. User Disconnected:%s\n",passport.latin1()); 
			break;
		default:
			printf("# State Not Changed");
	}
}

void MsnTest::personalMessage(QString passport, QString personalMsg) {
	qDebug ("#Mensaje personal de " + passport + " #-->" + personalMsg );
}

void MsnTest::hasBlog(QString passport){
	qDebug ("# " + passport + " has a Blog at " );
}

//void MsnTest::contactDisconnected(QString msnPassport){
//	qDebug ("## " + msnPassport + " HAS DISCONNECTED");
//}
//
//void MsnTest::chatLeavedTheRoom(int chatId, QString chatMsnPassport){
//	QString tmp;
//	qDebug ("## " + chatMsnPassport + " has left the room: " + tmp.setNum(chatId) );
//}
//
//void MsnTest::chatIsTyping(int chatId, QString chatMsnPassport){
//	QString tmp;
//	qDebug ("## Typing user: "+ chatMsnPassport + " in chat number: " + tmp.setNum(chatId));
//}
//
//void MsnTest::chatInfo(int chatId, QString chatMsnClient, QString chatIsLogging){
//	QString tmp;
//	qDebug ("## CLientName: " + chatMsnClient + " " + "Is Logging: " + chatIsLogging + " in chat number: " + tmp.setNum(chatId));
//}
//
//void MsnTest::chatArrivedMessage(int chatId, QString chatMsnPassport, QString chatMsg){
//	QString tmp;
//	qDebug ("## MESSAGE: `" + chatMsg + "' from chat number: " + tmp.setNum(chatId) + " written by: " + chatMsnPassport);
//}
//
//void MsnTest::newChat (int chatId, QString msnPassport){
//	QString tmp;
//        qDebug ("## NEW CHAT, begin with: " + msnPassport +  " in chat number: " + tmp.setNum(chatId));
//	m_chatid = chatId;
//}
//void MsnTest::sendTxtMsg (int chatCount, QString chatMsg) {
//	test.sendTxtMsg(chatCount, chatMsg);
//}
//
//void MsnTest::changeStatus (PresenceStatus newStatus){
//	test.changeStatus (newStatus);
//}
//
//void MsnTest::initNewChat (std::string chatFriend){
//	test.initNewChat (chatFriend);
//}

}

int main(void){
	using namespace libimmsnp;
	MsnTest cliente;
	cliente.start();
	while (true) {}
//	cliente.wait();
}
