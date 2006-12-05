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

QString id ("probando_msnpy@hotmail.com");
QString pass ("gargolas");
Client test(id,pass,STATUS_NLN);

MsnTest::MsnTest () {
	test.addConnectionListener(this);
	test.addRosterListener(this);
	test.addPresenceListener(this);
	test.addChatListener(this);
	m_connected = 0;
	m_chatid = 0;
}

void MsnTest::run (){
	test.connect();
}

MsnTest::~MsnTest() {
}

void MsnTest::onConnect()  {
	m_connected = 1;
	printf("# CONNECTED\n\n\n");
}

void MsnTest::onDisconnect(ConnectionError) {
	printf("# DISCONNECTED\n\n\n");
}

void MsnTest::onNewContact(Contact* c){
	printf ("# CONTACT:%s\n",c->getPassport().latin1());
}

void MsnTest::onNewGroup(Group* g){
	printf ("# GROUP:%s\n",g->getName().latin1());
}

void MsnTest::presenceChanged (QString passport, State status, QString displayName, QString capabilities){
	switch (status){
		case STATUS_NLN:
			printf("# State Changed. User:%s State:%s Capabilies:%s Personal MSG:%s\n",passport.latin1(),"Online", capabilities.latin1(), displayName.latin1());
			break;
		case STATUS_BSY:
			printf("# State Changed. User:%s State:%s Capabilies:%s Personal MSG:%s\n",passport.latin1(),"Do Not Disturb", capabilities.latin1(), displayName.latin1()); 
			break;
		case STATUS_IDL:
			printf("# State Changed. User:%s State:%s Capabilies:%s Personal MSG:%s\n",passport.latin1(),"Not Available", capabilities.latin1(), displayName.latin1()); 
			break;
		case STATUS_BRB:
			printf("# State Changed. User:%s State:%s Capabilies:%s Personal MSG:%s\n",passport.latin1(),"Back early", capabilities.latin1(), displayName.latin1()); 
			break;
		case STATUS_AWY:
			printf("# State Changed. User:%s State:%s Capabilies:%s Personal MSG:%s\n",passport.latin1(),"Away", capabilities.latin1(), displayName.latin1()); 
			break;
		case STATUS_PHN:
			printf("# State Changed. User:%s State:%s Capabilies:%s Personal MSG:%s\n",passport.latin1(),"At phone", capabilities.latin1(), displayName.latin1()); 
			break;
		case STATUS_LUN:
			printf("# State Changed. User:%s State:%s Capabilies:%s Personal MSG:%s\n",passport.latin1(),"Eating", capabilities.latin1(), displayName.latin1()); 
			break;
		default:
			printf("# State Changed. User Disconnected:%s\n",passport.latin1()); 
			break;
	}
}

void MsnTest::personalMessage(QString passport, QString personalMsg) {
	printf ("#Mensaje personal de %s #--> %s\n",passport.latin1(), personalMsg.latin1() );
}

void MsnTest::hasBlog(QString passport){
	QString page (passport.mid(0,passport.length() - passport.find("@")));
	qDebug ("# " + passport + " has a Blog at " + page + ".spaces.msn.com");
}

//void MsnTest::chatLeavedTheRoom(int chatId, QString chatMsnPassport){
//	QString tmp;
//	qDebug ("## " + chatMsnPassport + " has left the room: " + tmp.setNum(chatId) );
//}
//
void MsnTest::chatIsTyping(int chatId, QString chatMsnPassport){
	QString tmp;
	qDebug ("## Typing user: "+ chatMsnPassport + " in chat number: " + tmp.setNum(chatId));
}

void MsnTest::chatInfo(int chatId, QString chatMsnClient, QString chatIsLogging){
	QString tmp;
	qDebug ("## CLientName: " + chatMsnClient + " " + "Is Logging: " + chatIsLogging + " in chat number: " + tmp.setNum(chatId));
}

void MsnTest::chatArrivedMessage(int chatId, QString chatMsnPassport, QString chatMsg){
	QString tmp;
	qDebug ("## MESSAGE: `" + chatMsg + "' from chat number: " + tmp.setNum(chatId) + " written by: " + chatMsnPassport);
	if (chatMsg == "eco") test.sendChat(chatId, "Eco Eco");
}

void MsnTest::newChat (int chatId, QString msnPassport){
	QString tmp;
        qDebug ("## NEW CHAT, begin with: " + msnPassport +  " in chat number: " + tmp.setNum(chatId));
	m_chatid = chatId;
}
//void MsnTest::sendTxtMsg (int chatCount, QString chatMsg) {
//	test.sendTxtMsg(chatCount, chatMsg);
//}
//
void MsnTest::changeStatus (State newStatus){
	test.changeStatus (newStatus);
}

void MsnTest::changeNick (QString nick){
	test.changeNick (nick);
}
//
//void MsnTest::initNewChat (std::string chatFriend){
//	test.initNewChat (chatFriend);
//}

}

int main(void){
	using namespace libimmsnp;
	MsnTest cliente;
	cliente.start();
	while (!cliente.getConnected()) {}
	//cliente.changeStatus(dnd);
	cliente.changeNick(QString("Cambiando_nick"));
	while (1) {}

//	cliente.wait();
}
