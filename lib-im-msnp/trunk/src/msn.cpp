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
#include "contact.h"

namespace libimmsnp {


MsnTest::MsnTest () {
	QString id ("probando_msnpy@hotmail.com");
	QString pass ("gargolas");

	// TODO:  segfault if bad password
	m_client = new Client (id, pass, STATUS_NLN);
        m_client->addConnectionListener(this);
	m_client->addRosterListener(this);
	m_client->addPresenceListener(this);
	m_client->addChatListener(this);

}

void MsnTest::onConnect()  {
        m_connected = 1;
        printf("# CONNECTED\n\n\n");
}

void MsnTest::onDisconnect(ConnectionError) {
        printf("# DISCONNECTED\n\n\n");
}

void MsnTest::onNewContact(Contact* c){
	printf ("# CONTACT:%s\n",c->getPassport().toUtf8().data());
}

void MsnTest::onNewGroup(Group* g){
	printf ("# GROUP:%s\n",g->getName().toUtf8().data());
}

void MsnTest::presenceChanged (Contact* c){
	switch (c->getStatus()){
		case STATUS_NLN:
			printf("# State Changed. User:%s State:%s Capabilies:%s Personal MSG:%s\n",c->getPassport().toUtf8().data(),"Online", c->getCapabilities().toUtf8().data(), c->getDisplayName().toUtf8().data());
			m_client->initChat(c->getPassport());
			break;
		case STATUS_BSY:
			printf("# State Changed. User:%s State:%s Capabilies:%s Personal MSG:%s\n",c->getPassport().toUtf8().data(),"Do Not Disturb", c->getCapabilities().toUtf8().data(), c->getDisplayName().toUtf8().data()); 
			break;
		case STATUS_IDL:
			printf("# State Changed. User:%s State:%s Capabilies:%s Personal MSG:%s\n",c->getPassport().toUtf8().data(),"Not Available", c->getCapabilities().toUtf8().data(), c->getDisplayName().toUtf8().data()); 
			break;
		case STATUS_BRB:
			printf("# State Changed. User:%s State:%s Capabilies:%s Personal MSG:%s\n",c->getPassport().toUtf8().data(),"Back early", c->getCapabilities().toUtf8().data(), c->getDisplayName().toUtf8().data()); 
			break;
		case STATUS_AWY:
			printf("# State Changed. User:%s State:%s Capabilies:%s Personal MSG:%s\n",c->getPassport().toUtf8().data(),"Away", c->getCapabilities().toUtf8().data(), c->getDisplayName().toUtf8().data()); 
			break;
		case STATUS_PHN:
			printf("# State Changed. User:%s State:%s Capabilies:%s Personal MSG:%s\n",c->getPassport().toUtf8().data(),"At phone", c->getCapabilities().toUtf8().data(), c->getDisplayName().toUtf8().data()); 
			break;
		case STATUS_LUN:
			printf("# State Changed. User:%s State:%s Capabilies:%s Personal MSG:%s\n",c->getPassport().toUtf8().data(),"Eating", c->getCapabilities().toUtf8().data(), c->getDisplayName().toUtf8().data()); 
			break;
		default:
			printf("# State Changed. User Disconnected:%s\n",c->getPassport().toUtf8().data()); 
			break;
	}
}

void MsnTest::personalMessage(Contact* c) {
	printf ("#Mensaje personal de %s #--> %s\n",c->getPassport().toUtf8().data(), c->getPersMsg().toUtf8().data() );
}

void MsnTest::newChat (int chatId, QString msnPassport){
        qDebug ("## NEW CHAT, begin with: %s in chat number: %i", msnPassport.toUtf8().data(), chatId);
}

void MsnTest::chatLeavedTheRoom(int chatId, QString chatMsnPassport){
	qDebug ("## %s has left the room: %i", chatMsnPassport.toUtf8().data(), chatId);
}

void MsnTest::chatIsTyping(int chatId, QString chatMsnPassport){
	qDebug ("## Typing user: %s in chat number: %i ", chatMsnPassport.toUtf8().data(), chatId);
}

void MsnTest::chatInfo(int chatId, QString chatMsnClient, QString chatIsLogging){
	qDebug ("## CLientName: %s Is Logging: %s in chat number: %i", chatMsnClient.toUtf8().data(), chatIsLogging.toUtf8().data(), chatId);
}

void MsnTest::chatArrivedMessage(int chatId, QString chatMsnPassport, MSG chatMsg){
	qDebug ("## MESSAGE: `%s' from chat number: %i written by: %s", chatMsg.getMsg().toUtf8().data(), chatId, chatMsnPassport.toUtf8().data());
	if (chatMsg.getMsg() == "eco") {
		MSG m; 
		m.addMsg("Eco Eco");
		m.addEffect(EFFECT_ITALIC);
		m.addEffect(EFFECT_BOLD);
		m.addEffect(EFFECT_UNDERLINE);
		m_client->sendChat(chatId, m);
	}
	if (chatMsg.getMsg() == "exit") m_client->disconnect();
	if (chatMsg.getMsg() == "state") m_client->changeStatus(libimmsnp::STATUS_BSY);

	if (chatMsg.getMsg() == "delPassport") {
		Contact c;
		c.setPassport(chatMsnPassport);
		m_client->delContact(c);
	}
	if (chatMsg.getMsg() == "blkPassport") {
		Contact c;
		c.setPassport(chatMsnPassport);
		m_client->blockContact(c);
	}
	if (chatMsg.getMsg() == "deBlkPassport") {
		Contact c;
		c.setPassport(chatMsnPassport);
		m_client->deblockContact(c);
	}
	if (chatMsg.getMsg() == "addGroup") {
		Group g;
		g.setName(QString ("Minewgroup"));
		m_client->addGroup(g);
	}

	if (chatMsg.getMsg() == "delGroup") {
		Group g;
		g.setName(QString ("Minewgroup"));
		m_client->delGroup(g);
	}
	
}

void MsnTest::run (){
	m_client->connect();
	qDebug("END OF CONNECT");
	exit();
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
//int main(int argc, char *argv[]){
//	using namespace libimmsnp;
//	QCoreApplication a(argc, argv); 
//	QRegExp rx;
//	Buffer buf;
//	buf << QString ("ADC 0 RL N=xxxxxxxxxxx@hotmail.com F=xxxxxx 1\r\n");
//	rx.setPattern("(^ADC \\d+ (\\S+) N=(\\S+) F=(\\S+) \\d+\r\n)"); 
//	if (rx.indexIn(buf.data()) != -1){
//		QString list = rx.cap(2);
//		QString passport = rx.cap(3);
//		QString friendName = rx.cap(4);
//		qDebug ("MSN::Log::ParserNS::%s with fName: %s has added you to:%s list",passport.toUtf8().data(), friendName.toUtf8().data(), list.toUtf8().data());
//	}
//	a.exec(); 
//}

