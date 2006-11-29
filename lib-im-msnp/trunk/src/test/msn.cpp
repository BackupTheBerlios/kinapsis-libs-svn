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

Client test;
MsnTest::MsnTest () {
	test.addConnectionListener(this);
//	test.addRosterListener(this);
//	test.addPresenceListener(this);
//	test.addChatListener(this);
	m_initialStatus = "HDN";
	m_connected = 0;
	m_chatid = 0;
}

void MsnTest::run (){
        QString id ("user@hotmail.com");
        QString pass ("pass");
	QString host ("messenger.hotmail.com");
	test.connect(id,pass,m_initialStatus);
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

//void MsnTest::onNewContact(Contact* c){
//	qDebug("# CONTACT: " + c->getMsnName()+ " " + c->getFriendlyName() + " " + c->getMsnId() +  " " + c->getGroupName () + " " + c->getGroupId());
//}
//
//void MsnTest::presenceChanged (QString c, PresenceStatus status, QString msnPersMsg, QString msnCapabilities){
//	if (status == PresenceNLN) qDebug("# STATE CHANGE : " + c + " -> NLN " + msnPersMsg + " " + msnCapabilities); 
//	if (status == PresenceBSY) qDebug("# STATE CHANGE : " + c + " -> BSY " + msnPersMsg + " " + msnCapabilities);
//	if (status == PresenceIDL) qDebug("# STATE CHANGE : " + c + " -> IDL " + msnPersMsg + " " + msnCapabilities);    	
//	if (status == PresenceBRB) qDebug("# STATE CHANGE : " + c + " -> BRB " + msnPersMsg + " " + msnCapabilities);    	
//	if (status == PresenceAWY) qDebug("# STATE CHANGE : " + c + " -> AWY " + msnPersMsg + " " + msnCapabilities);    	
//	if (status == PresencePHN) qDebug("# STATE CHANGE : " + c + " -> PHN " + msnPersMsg + " " + msnCapabilities);    	
//	if (status == PresenceLUN) qDebug("# STATE CHANGE : " + c + " -> LUN " + msnPersMsg + " " + msnCapabilities);    	
//	if (status == PresenceHDN) qDebug("# STATE CHANGE : " + c + " -> HDN " + msnPersMsg + " " + msnCapabilities);    	
//	if (status == PresenceDIS) qDebug("# STATE CHANGE : " + c + " -> DIS " + msnPersMsg + " " + msnCapabilities);    	
//} 
//
//void MsnTest::personalMessage(QString c, QString msg) {
//	qDebug("##INITIAL MSG:" + c + " " + msg);
//}
//
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
	
//	
////	while (not cliente.getConnected()) {std::cout << "";};
////	cliente.changeStatus(PresenceIDL);
////	cliente.initNewChat ("vaticano666@hotmail.com");
//
////	while (cliente.getChatId()) {std::cout << "";};
////	cliente.sendTxtMsg (cliente.getChatId(), "Hola feo");
//
//
	cliente.wait();
//	msocket *s = new msocket("messenger.hotmail.com",1863);
//	s->connect();
//	Buffer b;
//	using namespace std; 
//	b << "VER 13 MSNP12 MSNP11 MSNP10 CVR0\r\n$!£$$!£$!·$";
//	b.gotoBegin();
//	ParserNS p("user","pass",s) ;
//	p.feed (b);
//
//	QString a;
//	b.data(a);
//	qDebug(a);
//
//	p.parse();

}
