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

#include "client.h"
#include "challenge.h"
#include "cmdswichboardserver.h"
#include "cmdnotificationserver.h"

namespace libimmsnp {

	Client::Client(QString msnPassport, QString msnPass, State initialStatus){
		m_parser = 0;
		m_mainSocket = 0;
		m_msnPassport = msnPassport;
		m_msnPass = msnPass;
		m_idtr = 1;
		m_chatCount = 0;
		m_initialStatus = initialStatus;
		m_roster = 0;
	}

	int 	Client::nextIdtr() {return m_idtr++;}

	int 	Client::nextChatCount() { return m_chatCount++;}

	QString Client::nextChatPassport () {
		QString mail = m_destPassport[0];
		m_destPassport.removeAt(0);
		return mail;
	}

	void Client::send(Command& c, int chat) {
		if (chat == -1)  m_mainSocket->send (c.makeCmd());
		else m_chatList[chat]->send(c.makeCmd());
	}

	void Client::connect() {
		//#include <QRegExp>
		//QString a = "";
		//QRegExp rx;
		//rx.setPattern("(");
		//if (rx.indexIn(a) != -1){
		//	qDebug("type:%s ip:%s port:%s dato1:%s dato2:%s", rx.cap(2).toUtf8().data(), rx.cap(3).toUtf8().data() , rx.cap(4).toUtf8().data() , rx.cap(5).toUtf8().data() , rx.cap(6).toUtf8().data());
		//}
			
		m_mainSocket = new msocket();
		if (m_mainSocket->connect("messenger.hotmail.com",1863)){
			m_parser = new ParserNS (m_msnPassport, m_msnPass, m_initialStatus, this, m_mainSocket, "MSNP12");
			QObject::connect(m_parser, SIGNAL(connected()), this, SLOT(connected()));
			//QObject::connect(m_parser, SIGNAL(disconnected(ConnectionError)), this, SLOT(disconnected(ConnectionError)));
			//QObject::connect(m_parser, SIGNAL(newGroupArrived(Group*)), this, SLOT(newGroupArrived(Group*)));
			//QObject::connect(m_parser, SIGNAL(newContactArrived(Contact*)), this, SLOT(newContactArrived(Contact*)));
			//QObject::connect(m_parser, SIGNAL(statusChanged (QString, State, QString, QString)), this, SLOT(statusChanged(QString, State, QString, QString)));
			//QObject::connect(m_parser, SIGNAL(personalMessage (QString, QString)), this, SLOT(personalMessage(QString, QString)));
			//QObject::connect(m_parser, SIGNAL(chatRequest (QString, QString, QString, QString)), this, SLOT(chatRequest(QString, QString, QString, QString)));
			//QObject::connect(m_parser, SIGNAL(initChatSB (QString, QString)), this, SLOT(initChatSB(QString, QString)));
			//QObject::connect(m_parser, SIGNAL(disconnected(ConnectionError)), this, SLOT(disconnected(ConnectionError)));
			m_parser->init();
		}
	}       
	
	void Client::initChat(QString destPassport){
		m_destPassport << destPassport;
		XFR x(nextIdtr());
		send (x);
	}
                
	void Client::disconnect(){
		OUT out;
		send (out);
	}

	Client::~Client(){
		delete m_mainSocket;
	}


	/// SLOTS
	//
	void Client::connected() {
		qDebug ("MSN::Client::SIGNAL ## notifyConnect");
		//emit notifyConnect();
	}

	//void Client::disconnected(ConnectionError e) {
	//	delete m_roster;
	//	delete m_mainSocket;
	//	m_roster = 0;
	//	m_mainSocket = 0;
	//	printf ("MSN::Client::SIGNAL ## DISCONNECT\n"); 
	//	//emit notifyDisconnect(e);
	//}

	//void Client::newGroupArrived (Group* group) {
	//	printf ("MSN::Client::SIGNAL ## newGroup\n"); 
	//	//emit notifyNewGroup(group);

	//	m_roster->addGroup(group);
	//}

	//void Client::newContactArrived (Contact* contact) {
	//	if (contact->getGroupId() != "") {
	//		QString g = m_roster->getGroupName(contact->getGroupId());
	//		contact->setGroupName(g);
	//	}
	//	printf ("MSN::Client::SIGNAL ## NewContact\n"); 
	//	//emit notifyNewContact(contact);
	//	m_roster->addContact(contact);
	//}

	//void Client::statusChanged (QString passport, State status, QString displayName, QString capabilities ){
	//	Contact* c = m_roster->getContact(passport);
	//	c->setStatus (status);
	//	c->setDisplayName (displayName);
	//	c->setCapabilities (capabilities);
	//	printf ("MSN::Client::SIGNAL ## Presence form:%s displayName:%s CAP:%s\n", passport.toUtf8().data(), displayName.replace("\r\n","\\r\\n").toUtf8().data(), capabilities.toUtf8().data()); 
	//	//emit notifyPresence (c);

	//}

	//void Client::personalMessage (QString passport, QString personalMsg){
	//	if (personalMsg != ""){
	//		Contact* c = m_roster->getContact(passport);
	//		c->setPersMsg (personalMsg);
	//		printf ("MSN::Client::SIGNAL ## personalMsg from:%s persMSG:%s\n", passport.toUtf8().data(), personalMsg.replace("\r\n","\\r\\n").toUtf8().data()); 
	//		//emit notifyPersonalMessage (c);
	//	}
	//}

//	void Client::chatRequest(QString ipPort, QString passport, QString ticket, QString sessionId){
//		printf ("MSN::Log::ParserNS ## Calling:%s From: %s\n",passport.toUtf8().data(),ipPort.toUtf8().data());
//		msocket* sock = new msocket (ipPort.toUtf8().data());
//		sock->connect();
//
//		ANS ans(1);
//		ans.addPassport (m_msnPassport);
//		ans.addTicket (ticket);
//		ans.addSessId (sessionId);
//		sock->send(ans.makeCmd());
//		QString m;
//		sock->recv(m);
//		ParserSB* chatParser = new ParserSB (sock, this, ++m_chatCount);
//		Chat* oneChat = new Chat (chatParser, m_chatCount, sock);
//		oneChat->Start();
//		m_chatList[m_chatCount] = oneChat;
//		//emit (notifyNewChat (m_chatCount, passport));
//		printf ("MSN::Client::SIGNAL ## newChat at:%i with:%s\n", m_chatCount, passport.toUtf8().data()); 
//
//		QObject::connect(chatParser, SIGNAL(chatArrivedMessage(int, QString, QString)), this, SLOT(chatArrivedMessage(int, QString, QString)));
//		QObject::connect(chatParser, SIGNAL(chatInfo(int, QString, QString)), this, SLOT(chatInfo(int, QString, QString)));
//		QObject::connect(chatParser, SIGNAL(chatIsTyping(int, QString)), this, SLOT(chatIsTyping(int, QString)));
//		QObject::connect(chatParser, SIGNAL(chatLeavedTheRoom(int, QString)), this, SLOT(chatLeavedTheRoom(int, QString)));
//	}
//
//	void Client::initChatSB(QString ipPort, QString ticket) {
//		msocket* sock = new msocket (ipPort.toUtf8().data());
//		sock->connect();
//
//		USRchat usr (1);
//		usr.addPassport (m_msnPassport);
//		usr.addTicket (ticket);
//		sock->send(usr.makeCmd());
//		QString m;
//		sock->recv(m);
//
//		CAL cal (1);
//		cal.addPassport (m_destPassport);
//		sock->send(cal.makeCmd());
//		sock->recv(m);
//
//		m_chatCount ++;
//		ParserSB* chatParser = new ParserSB (sock, this, m_chatCount);
//		Chat* oneChat = new Chat (chatParser, m_chatCount, sock);
//		oneChat->Start();
//		m_chatList[m_chatCount] = oneChat;
//
//		QObject::connect(chatParser, SIGNAL(chatArrivedMessage(int, QString, QString)), this, SLOT(chatArrivedMessage(int, QString, QString)));
//		QObject::connect(chatParser, SIGNAL(chatInfo(int, QString, QString)), this, SLOT(chatInfo(int, QString, QString)));
//		QObject::connect(chatParser, SIGNAL(chatIsTyping(int, QString)), this, SLOT(chatIsTyping(int, QString)));
//		QObject::connect(chatParser, SIGNAL(chatLeavedTheRoom(int, QString)), this, SLOT(chatLeavedTheRoom(int, QString)));
//		QObject::connect(chatParser, SIGNAL(newChat(int, QString)), this, SLOT(newChat(int, QString)));
//		//printf ("Contact:%s has left the room\n", passport.toUtf8().data());
//		
//	}
}
#include "client.moc"
