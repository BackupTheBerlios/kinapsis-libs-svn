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
#include "login.h"
#include "contact.h"
#include "roster.h"
#include "command.h"

namespace libimmsnp {


	// msnGroupId = 5faec9a4-b368-4def-8f11-6be610b665d0
	// msnGroupName = group_one
	//
	// msnPassport = aaa@hotmail.com
	// msnFriendlyName = Aaa Carter
	// msnPersMsg = Hello everybody
	// msnContactId = 07ad2594-ef7f-4a1e-96e1-0158611521ed
	// msnContactCapabilities =
	// msnPresenceState = BSY
	//
	// chatMsnClient = Amsn 0.95
	// chatIsLogging = (Y|N)
	//


	Client::Client(QString msnPassport, QString msnPass, QString initialStatus, QString msnHost, int msnPort){
		m_parser = 0;
		m_conn = 0;
		m_mainSocket = 0;
		m_msnPassport = msnPassport;
		m_msnPass = msnPass;
		m_idtr = 1;
		m_threads.setAutoDelete(TRUE);
		m_roster = 0;
		m_initialStatus = initialStatus;
//		m_chats.setAutoDelete(true);
	}

	int Client::getIdtr() {
		return ++m_idtr;
	}

	void Client::send(Command& c) {
	 	m_mainSocket->send (c.makeCmd());
	}

	void Client::makeConnection (QString ip, int port){
		m_mainSocket = new msocket(ip, port);
		m_mainSocket->connect();
//		if (!m_conn->finished()) {
//			m_conn->terminate();
//			m_conn->wait();
//			delete m_conn;
//		}
		m_conn = new Connection (m_mainSocket, m_parser);
		m_threads.append (m_conn);
		m_conn->start();
	}

	int Client::connect() {
		m_parser = new ParserNS (m_msnPassport, m_msnPass, m_initialStatus, this);
		m_roster = new Roster ();

		QObject::connect(m_parser, SIGNAL(mainSocket(msocket*)), this, SLOT(mainSocket(msocket*)));
		QObject::connect(m_parser, SIGNAL(connected()), this, SLOT(connected()));
		QObject::connect(m_parser, SIGNAL(newGroupArrived(Group*)), this, SLOT(newGroupArrived(Group*)));
		QObject::connect(m_parser, SIGNAL(newContactArrived(Contact*)), this, SLOT(newContactArrived(Contact*)));
		QObject::connect(m_parser, SIGNAL(statusChanged (QString, State, QString, QString)), this, SLOT(statusChanged(QString, State, QString, QString)));
		QObject::connect(m_parser, SIGNAL(personalMessage (QString, QString)), this, SLOT(personalMessage(QString, QString)));
		QObject::connect(m_parser, SIGNAL(hasBlog(QString)), this, SLOT(hasBlog(QString)));

		m_mainSocket = new msocket("messenger.hotmail.com",1863);
		m_mainSocket->connect();
		m_conn = new Connection (m_mainSocket, m_parser);
		m_conn->start();

		VER v(1);
		v.addProtocolSupported("MSNP12");
		v.addProtocolSupported("MSNP11");
		v.addProtocolSupported("MSNP10");
		send(v);
		m_threads.append(m_conn);
		m_conn->wait();

//		QObject::connect(m_conn, SIGNAL(disconnected()), this, SLOT(disconnected()));
//		QObject::connect(m_parser, SIGNAL(contactDisconnected (QString)),this, SLOT(contactDisconnected (QString)));
//		QObject::connect(m_parser, SIGNAL(chatRequest (QString, QString, QString, QString)), this, SLOT(chatRequest(QString, QString, QString, QString)));
//		QObject::connect(m_parser, SIGNAL(chatCreated (QString, QString)), this, SLOT(chatCreated(QString, QString)));

		//m_conn->connect(initialStatus);
	}

//	int Client::sendTxtMsg (int chatCount, QString chatMsg){
//		QString tmp;
//		for (uint i = 0; i < m_chats.count() ; ++i){
//			if (m_chats.at(i)){
//				if (m_chats.at(i)->getChatId () == chatCount){
//					return m_chats.at(i)->Send (chatMsg.latin1());
//				}
//			}
//		}
//	}
//
//	void Client::changeStatus (PresenceStatus newStatus){
//		std::ostringstream msgStatus;
//
//		if (newStatus == PresenceNLN) msgStatus << "CHG" << " 1 " << "NLN\r\n";
//		if (newStatus == PresenceBSY) msgStatus << "CHG" << " 1 " << "BSY\r\n";
//		if (newStatus == PresenceIDL) msgStatus << "CHG" << " 1 " << "IDL\r\n";
//		if (newStatus == PresenceBRB) msgStatus << "CHG" << " 1 " << "BRB\r\n";
//		if (newStatus == PresenceAWY) msgStatus << "CHG" << " 1 " << "AWY\r\n";
//		if (newStatus == PresencePHN) msgStatus << "CHG" << " 1 " << "PHN\r\n";
//		if (newStatus == PresenceLUN) msgStatus << "CHG" << " 1 " << "LUN\r\n";
//		if (newStatus == PresenceHDN) msgStatus << "CHG" << " 1 " << "HDN\r\n";
//		m_mainSocket->send (msgStatus.str());
//
//	}
//
//	int Client::initNewChat (std::string chatFriend){
//		std::string buf;
//		m_mainSocket->send(QString("XFR 57 SB\r\n"));
//	}
//
//	void Client::chatCreated(QString hostPort, QString ticket){
//	//	std::string ansBuf;
//		msocket* sock = new msocket (hostPort.latin1() );
//		sock->connect();
//
//		std::ostringstream  tmp;
//		tmp << "USR 58 " << m_msnPassport.latin1() << " " << ticket.latin1() << "\r\n";
//		sock->send(tmp.str());
//		std::string aaa;
//		sock->recv(aaa);
//		std::cout << aaa;
//		std::ostringstream  tmp2;
//		tmp2 << "CAL 59 " << "vaticano666@hotmail.com\r\n";
//		sock->send(tmp2.str());
//		std::string aaaa;
//		sock->recv(aaaa);
//		std::cout << aaaa;
//		sock->recv(aaaa);
//		std::cout << aaaa;
//		sock->send (QString ("MSG 60 U 98\r\nMIME-Version: 1.0\r\nContent-Type: text/x-clientcaps\r\n\r\nClient-Name: aMSN 0.96rc1\r\nChat-Logging: N\r\n"));
//		sock->send (QString ("MSG 61 U 99\r\nMIME-Version: 1.0\r\nContent-Type: text/x-msmsgscontrol\r\nTypingUser: probando_msnpy@hotmail.com\r\n\r\n\r\n"));
//		sock->send (QString ("MSG 62 A 123\r\nMIME-Version: 1.0\r\nContent-Type: text/plain; charset=UTF-8\r\nX-MMS-IM-Format: FN=helvetica; EF=; CO=000000; CS=0; PF=22\r\n\r\na"));
//		std::string a;
//		sock->recv(a);
//		std::cout << a;
//
//
//		
//
//
//	//	Parser* chatParser = new Parser(m_msnPassport, m_msnPass, ++m_chatCount);
//	//	Chat* oneChat = new Chat (chatParser, m_chatCount, sock);
//	//	oneChat->Start();
//	//	m_chats.append (oneChat);
//	//	sleep(1); // if I don't wait it doesn't work
//	//	emit (notifyNewChat (m_chatCount, msnPassport));
//
//	//	QObject::connect(chatParser, SIGNAL(chatArrivedMessage(int, QString, QString)), this, SLOT(chatArrivedMessage(int, QString, QString)));
//	//	QObject::connect(chatParser, SIGNAL(chatInfo(int, QString, QString)), this, SLOT(chatInfo(int, QString, QString)));
//	//	QObject::connect(chatParser, SIGNAL(chatIsTyping(int, QString)), this, SLOT(chatIsTyping(int, QString)));
//	//	QObject::connect(chatParser, SIGNAL(chatLeavedTheRoom(int, QString)), this, SLOT(chatLeavedTheRoom(int, QString)));
//	//	
//	//	//// me conecto a 207.46.26.114:1863
//	//	//   send("USR 58 vaticano666@hotmail.com 1481854017.25321957.215204203")
//	//	//   recv ("USR 58 OK vaticano666@hotmail.com pedro")
//	//	//   send ("CAL 59 wisar27@hotmail.com");
//	//	//   recv ("CAL 59 RINGING 1481854017");
//	//	//   recv ("JOI wisar27@hotmail.com Lui..ao 1342558268");
//	//	//   send ("MSG 60 U 98\r\nMIME-Version: 1.0\nContent-Type: text/x-clientcaps\n\nClient-Name: aMSN 0.96rc1\nChat-Logging: N");
//		std::cout << "Init new chat";
//	}

	Client::~Client(){
		delete m_parser;
//		delete m_conn;
		delete m_mainSocket;
	}

	void Client::addConnectionListener (ConnectionListener* cl){
		QObject::connect (this, SIGNAL(notifyConnect()), cl, SLOT(connectSlot()));
		QObject::connect (this, SIGNAL(notifyDisconnect()), cl, SLOT(disconnectSlot()));
	}

	void Client::delConnectionListener (ConnectionListener* cl){
		QObject::disconnect (this, 0, cl, 0);
	}

	void Client::addRosterListener (RosterListener *rl){
		QObject::connect (this, SIGNAL(notifyNewContact(Contact*)), rl, SLOT(onNewContactSlot(Contact*)));
		QObject::connect (this, SIGNAL(notifyHasBlog(QString)), rl, SLOT(hasBlogSlot(QString)));
	}

	void Client::delRosterListener (RosterListener *rl){
		QObject::disconnect (this, 0, rl, 0);
	}
//
//	void Client::addChatListener (ChatListener *chl){
//		QObject::connect (this, SIGNAL(notifyNewChat (int, QString)), chl, SLOT(newChatSlot (int, QString)));
//		QObject::connect (this, SIGNAL(notifyChatLeavedTheRoom(int, QString)), chl, SLOT(chatLeavedTheRoomSlot(int, QString)));
//		QObject::connect (this, SIGNAL(notifyChatIsTyping(int, QString)), chl, SLOT(chatIsTypingSlot(int, QString)));
//		QObject::connect (this, SIGNAL(notifyChatInfo (int, QString, QString)), chl, SLOT(chatInfoSlot(int, QString, QString)));
//		QObject::connect (this, SIGNAL(notifyChatArrivedMessage (int, QString, QString)),chl, SLOT(chatArrivedMessageSlot(int, QString, QString)));
//	}
//
//	void Client::delChatListener (ChatListener *chl){
//		QObject::disconnect (this, 0, chl, 0);
//	}
//
	void Client::addPresenceListener(PresenceListener *pl) {
		QObject::connect(this, SIGNAL(notifyPresence(QString, State, QString, QString)), pl, SLOT(presenceChangedSlot(QString, State, QString, QString)));
		QObject::connect(this, SIGNAL(notifyPersonalMessage(QString, QString)), pl, SLOT(personalMessageSlot(QString, QString)));
//		QObject::connect(this, SIGNAL(notifyContactDisconnected (QString)), pl, SLOT(contactDisconnectedSlot(QString)));
	}

	void Client::delPresenceListener (PresenceListener *pl){
		QObject::disconnect (this, 0, pl, 0);
	}


	void Client::mainSocket(msocket* mainSocket){
		m_mainSocket = mainSocket;
		m_conn = new Connection (m_mainSocket, m_parser);
		m_conn->start();
		m_conn->wait();
	}

	void Client::connected() {
		emit notifyConnect();
	}

	void Client::disconnected() {
		emit notifyDisconnect();
	}

	void Client::newGroupArrived (Group* g) {
		qDebug("# GROUP: " + g->getName() + " " + g->getId());
		m_roster->addGroup (g);
	}
//
	void Client::newContactArrived (Contact* contact) {
		QPtrList<Group> lsg = m_roster->getGroups();
		for (uint i = 0; i < lsg.count() ; ++i)
			if (lsg.at(i))
				if (lsg.at(i)->getId () == contact->getId())
					contact->setGroupName(lsg.at(i)->getName());

		m_roster->addContact(contact);
		emit notifyNewContact(contact);
	}
//
	void Client::statusChanged (QString passport, State status, QString displayName, QString capabilities ){
		 emit notifyPresence (passport, status, displayName, capabilities);
	}

	void Client::personalMessage (QString passport, QString personalMsg){
//		QPtrList<Contact> lsc = m_roster->getContacts();
//		for (uint i = 0; i < lsc.count() ; ++i)
//			if (lsc.at(i))
//				if (lsc.at(i)->getMsnName() == msnPassport)
//					lsc.at(i)->setPersMsg (persMsg);
//
		if (personalMsg != "")
			emit notifyPersonalMessage (passport, personalMsg);
	}

	void Client::hasBlog(QString passport) {
		emit notifyHasBlog(passport);
	}
//
//	void Client::chatRequest(QString hostPort, QString msnPassport, QString ticket, QString sessid){
//
//		Commands ANScmd;
//		std::string ansBuf;
//		msocket* sock = new msocket (hostPort.latin1() );
//		sock->connect();
//		sock->send(ANScmd.ANS(0, m_msnPassport, ticket.latin1(), sessid.latin1()));
//
//		Parser* chatParser = new Parser(m_msnPassport, m_msnPass, ++m_chatCount);
//		Chat* oneChat = new Chat (chatParser, m_chatCount, sock);
//		oneChat->Start();
//		m_chats.append (oneChat);
//		sleep(1); // if I don't wait it doesn't work
//		emit (notifyNewChat (m_chatCount, msnPassport));
//
//		QObject::connect(chatParser, SIGNAL(chatArrivedMessage(int, QString, QString)), this, SLOT(chatArrivedMessage(int, QString, QString)));
//		QObject::connect(chatParser, SIGNAL(chatInfo(int, QString, QString)), this, SLOT(chatInfo(int, QString, QString)));
//		QObject::connect(chatParser, SIGNAL(chatIsTyping(int, QString)), this, SLOT(chatIsTyping(int, QString)));
//		QObject::connect(chatParser, SIGNAL(chatLeavedTheRoom(int, QString)), this, SLOT(chatLeavedTheRoom(int, QString)));
//
//
//	}
//
//	void Client::chatArrivedMessage (int chatId, QString msnPassport, QString chatMsg) {
//		emit notifyChatArrivedMessage (chatId, msnPassport, chatMsg);
//	}
//
//	void Client::chatInfo (int chatId, QString chatMsnClient, QString chatIsLogging) {
//		emit notifyChatInfo (chatId, chatMsnClient, chatIsLogging);
//	}
//
//	void Client::chatIsTyping (int chatId, QString chatMsnPassport){
//		emit notifyChatIsTyping (chatId, chatMsnPassport);
//	}
//
//	void Client::chatLeavedTheRoom (int chatId, QString chatMsnPassport){
//		emit notifyChatLeavedTheRoom (chatId, chatMsnPassport);
//	}
//
//	void Client::contactDisconnected (QString msnPassport){
//		emit notifyContactDisconnected (msnPassport);
//	}
}
#include "client.moc"
