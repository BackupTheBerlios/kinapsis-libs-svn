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
#include "libimmsnp.h"
#include "notificationServer.h"
#include "swichboardserver.h"
#include "parserswichboardserver.h"

namespace libimmsnp {

	Client::Client(QString msnPassport, QString msnPass, State initialStatus){
		m_parser = 0;
		m_conn = 0;
		m_mainSocket = 0;
		m_msnPassport = msnPassport;
		m_msnPass = msnPass;
		m_idtr = 1;
		m_chatCount = 0;
		m_roster = 0;
		m_initialStatus = initialStatus;
	}

	int Client::getIdtr() {
		return m_idtr++;
	}

	void Client::send(Command& c, int chat) {
		if (!chat)
		 	m_mainSocket->send (c.makeCmd());
		else
			(m_chatList[chat]->getSocket())->send(c.makeCmd());
	}

	void Client::disconnect(){
		delete m_conn;
		delete m_parser;
		delete m_mainSocket;
		delete m_roster;
		emit notifyDisconnect(ConnUserDisconnected);
	}

	void Client::makeConnection (QString ip, int port){
		m_mainSocket = new msocket(ip, port);
		m_mainSocket->connect();
	}

	void Client::startConnection(){
		delete m_conn;
		m_parser = new ParserNS (m_msnPassport, m_msnPass, m_initialStatus, this);
		QObject::connect(m_parser, SIGNAL(connected()), this, SLOT(connected()));
		QObject::connect(m_parser, SIGNAL(disconnected(ConnectionError)), this, SLOT(disconnected(ConnectionError)));
		QObject::connect(m_parser, SIGNAL(newGroupArrived(Group*)), this, SLOT(newGroupArrived(Group*)));
		QObject::connect(m_parser, SIGNAL(newContactArrived(Contact*)), this, SLOT(newContactArrived(Contact*)));
		QObject::connect(m_parser, SIGNAL(statusChanged (QString, State, QString, QString)), this, SLOT(statusChanged(QString, State, QString, QString)));
		QObject::connect(m_parser, SIGNAL(personalMessage (QString, QString)), this, SLOT(personalMessage(QString, QString)));
		QObject::connect(m_parser, SIGNAL(hasBlog(QString)), this, SLOT(hasBlog(QString)));
		QObject::connect(m_parser, SIGNAL(chatRequest (QString, QString, QString, QString)), this, SLOT(chatRequest(QString, QString, QString, QString)));
		QObject::connect(m_parser, SIGNAL(initChatSB (QString, QString)), this, SLOT(initChatSB(QString, QString)));
		m_conn = new Connection (m_mainSocket, m_parser);
		m_conn->run();
	}


	void Client::connect() {
		m_parser = new ParserNS (m_msnPassport, m_msnPass, m_initialStatus, this);
		m_roster = new Roster ();
		m_mainSocket = new msocket("messenger.hotmail.com",1863);
		m_mainSocket->connect();
		QObject::connect(m_mainSocket, SIGNAL(disconnected(ConnectionError)), this, SLOT(disconnected(ConnectionError)));
		m_conn = new Connection (m_mainSocket, m_parser,3);

		VER v(getIdtr());
		v.addProtocolSupported("MSNP12");
		v.addProtocolSupported("MSNP11");
		v.addProtocolSupported("MSNP10");
		send(v);
		m_conn->run();
	}

	void Client::changeStatus (State status){
		// CHG idtr status capabilities\r\n
		printf("MSN::Log::Client ## Status Changed\n");
		std::ostringstream msgStatus;
		CHG cmd (getIdtr());
		cmd.addStatusCode(status);
		cmd.addCapabilities ("1342558252");
		send (cmd);
	}

	void Client::changeNick(QString nick) {
		// PRP idtr nick\r\n
		printf("MSN::Log::Client ## Nick Changed\n");
		PRP prp (getIdtr());
		prp.addNick(nick);
		send (prp);
	}

	Client::~Client(){
		delete m_parser;
		delete m_mainSocket;
	}

	void Client::addConnectionListener (ConnectionListener* cl){
		QObject::connect (this, SIGNAL(notifyConnect()), cl, SLOT(connectSlot()));
		QObject::connect (this, SIGNAL(notifyDisconnect(ConnectionError)), cl, SLOT(disconnectSlot(ConnectionError)));
	}

	void Client::delConnectionListener (ConnectionListener* cl){
		QObject::disconnect (this, 0, cl, 0);
	}

	void Client::addRosterListener (RosterListener *rl){
		QObject::connect (this, SIGNAL(notifyNewContact(Contact*)), rl, SLOT(onNewContactSlot(Contact*)));
		QObject::connect (this, SIGNAL(notifyNewGroup(Group*)), rl, SLOT(onNewGroupSlot(Group*)));
		QObject::connect (this, SIGNAL(notifyHasBlog(QString)), rl, SLOT(hasBlogSlot(QString)));
	}

	void Client::delRosterListener (RosterListener *rl){
		QObject::disconnect (this, 0, rl, 0);
	}

	void Client::addPresenceListener(PresenceListener *pl) {
		QObject::connect(this, SIGNAL(notifyPresence(QString, State, QString, QString)), pl, SLOT(presenceChangedSlot(QString, State, QString, QString)));
		QObject::connect(this, SIGNAL(notifyPersonalMessage(QString, QString)), pl, SLOT(personalMessageSlot(QString, QString)));
//		QObject::connect(this, SIGNAL(notifyContactDisconnected (QString)), pl, SLOT(contactDisconnectedSlot(QString)));
	}

	void Client::delPresenceListener (PresenceListener *pl){
		QObject::disconnect (this, 0, pl, 0);
	}

	void Client::addChatListener (ChatListener *chl){
		QObject::connect (this, SIGNAL(notifyNewChat (int, QString)), chl, SLOT(newChatSlot (int, QString)));
	//	QObject::connect (this, SIGNAL(notifyChatLeavedTheRoom(int, QString)), chl, SLOT(chatLeavedTheRoomSlot(int, QString)));
		QObject::connect (this, SIGNAL(notifyChatIsTyping(int, QString)), chl, SLOT(chatIsTypingSlot(int, QString)));
		QObject::connect (this, SIGNAL(notifyChatInfo (int, QString, QString)), chl, SLOT(chatInfoSlot(int, QString, QString)));
		QObject::connect (this, SIGNAL(notifyChatArrivedMessage (int, QString, QString)),chl, SLOT(chatArrivedMessageSlot(int, QString, QString)));
	}

	void Client::delChatListener (ChatListener *chl){
		QObject::disconnect (this, 0, chl, 0);
	}

	void Client::connected() {
		emit notifyConnect();
	}

	void Client::disconnected(ConnectionError e) {
		emit notifyDisconnect(e);
	}

	void Client::newGroupArrived (Group* group) {
		emit notifyNewGroup(group);
		m_roster->addGroup(group);
	}

	void Client::newContactArrived (Contact* contact) {
		if (contact->getGroupId() != "") {
			QString g = m_roster->getGroupName(contact->getGroupId());
			contact->setGroupName(g);
			printf ("MSN::Log::Client ## Contact: %s Group:%s\n", contact->getPassport().latin1(), contact->getGroupName().latin1());
		}
		emit notifyNewContact(contact);
		m_roster->addContact(contact);
	}

	void Client::statusChanged (QString passport, State status, QString displayName, QString capabilities ){
		 emit notifyPresence (passport, status, displayName, capabilities);

	}

	void Client::personalMessage (QString passport, QString personalMsg){
		if (personalMsg != "")
			emit notifyPersonalMessage (passport, personalMsg);
	}

	void Client::hasBlog(QString passport) {
		emit notifyHasBlog(passport);
	}

	void Client::chatArrivedMessage (int chatId, QString msnPassport, QString chatMsg) {
		emit notifyChatArrivedMessage (chatId, msnPassport, chatMsg);
	}

	void Client::chatInfo (int chatId, QString chatMsnClient, QString chatIsLogging) {
		emit notifyChatInfo (chatId, chatMsnClient, chatIsLogging);
	}

	void Client::chatIsTyping (int chatId, QString chatMsnPassport){
		emit notifyChatIsTyping (chatId, chatMsnPassport);
	}

	void Client::chatRequest(QString ipPort, QString passport, QString ticket, QString sessionId){
		printf ("MSN::Log::ParserNS ## Calling:%s From: %s\n",passport.latin1(),ipPort.latin1());
		msocket* sock = new msocket (ipPort.latin1());
		sock->connect();

		ANS ans(1);
		ans.addPassport (m_msnPassport);
		ans.addTicket (ticket);
		ans.addSessId (sessionId);
		sock->send(ans.makeCmd());
		QString m;
		sock->recv(m);
		ParserSB* chatParser = new ParserSB (sock, this, ++m_chatCount);
		Chat* oneChat = new Chat (chatParser, m_chatCount, sock);
		oneChat->Start();
		m_chatList[m_chatCount] = oneChat;
		emit (notifyNewChat (m_chatCount, passport));

		QObject::connect(chatParser, SIGNAL(chatArrivedMessage(int, QString, QString)), this, SLOT(chatArrivedMessage(int, QString, QString)));
		QObject::connect(chatParser, SIGNAL(chatInfo(int, QString, QString)), this, SLOT(chatInfo(int, QString, QString)));
		QObject::connect(chatParser, SIGNAL(chatIsTyping(int, QString)), this, SLOT(chatIsTyping(int, QString)));
//		QObject::connect(chatParser, SIGNAL(chatLeavedTheRoom(int, QString)), this, SLOT(chatLeavedTheRoom(int, QString)));
	}
	void Client::sendChat(int chatId, QString msg) {
		MSG m (m_chatList[chatId]->getIdtr());
		m.addMsg (msg);
		send (m, chatId);
	}

	int Client::initChat(QString destPassport){
		//TODO: Improve this 
		m_destPassport = destPassport;
		XFR x(getIdtr());
		send (x);
		return ++m_chatCount;
	}
	void Client::initChatSB(QString ipPort, QString ticket) {
		msocket* sock = new msocket (ipPort.latin1());
		sock->connect();

		USRchat usr (1);
		usr.addPassport (m_msnPassport);
		usr.addTicket (ticket);
		sock->send(usr.makeCmd());
		QString m;
		sock->recv(m);

		CAL cal (1);
		cal.addPassport (m_destPassport);
		sock->send(cal.makeCmd());
		sock->recv(m);

		ParserSB* chatParser = new ParserSB (sock, this, m_chatCount);
		Chat* oneChat = new Chat (chatParser, m_chatCount, sock);
		oneChat->Start();
		m_chatList[m_chatCount] = oneChat;
//		emit (notifyNewChat (m_chatCount, passport));

		QObject::connect(chatParser, SIGNAL(chatArrivedMessage(int, QString, QString)), this, SLOT(chatArrivedMessage(int, QString, QString)));
		QObject::connect(chatParser, SIGNAL(chatInfo(int, QString, QString)), this, SLOT(chatInfo(int, QString, QString)));
		QObject::connect(chatParser, SIGNAL(chatIsTyping(int, QString)), this, SLOT(chatIsTyping(int, QString)));
//		QObject::connect(chatParser, SIGNAL(chatLeavedTheRoom(int, QString)), this, SLOT(chatLeavedTheRoom(int, QString)));
		
	}

}
#include "client.moc"
