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
		m_roster = new Roster ;
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
		else m_chatList[chat]->send(c);
	}

	void Client::connect() {
		m_mainSocket = new msocket();

		if (m_mainSocket->connect("messenger.hotmail.com",1863)){
			m_parser = new ParserNS (m_msnPassport, m_msnPass, m_initialStatus, this, m_mainSocket, "MSNP12");
			QObject::connect(m_parser, SIGNAL(clientIpPort (QString, QString)), this, SLOT(clientIpPort(QString, QString)));
			QObject::connect(m_parser, SIGNAL(connected()), this, SLOT(connected()));
			QObject::connect(m_parser, SIGNAL(disconnected(ConnectionError)), this, SLOT(disconnected(ConnectionError)));
			QObject::connect(m_parser, SIGNAL(newGroupArrived(Group*)), this, SLOT(newGroupArrived(Group*)));
			QObject::connect(m_parser, SIGNAL(newContactArrived(Contact*)), this, SLOT(newContactArrived(Contact*)));
			QObject::connect(m_parser, SIGNAL(statusChanged (QString, State, QString, QString)), this, SLOT(statusChanged(QString, State, QString, QString)));
			QObject::connect(m_parser, SIGNAL(personalMessage (QString, QString)), this, SLOT(personalMessage(QString, QString)));
			QObject::connect(m_parser, SIGNAL(hasBlog (QString)), this, SLOT(hasBlog(QString)));
			QObject::connect(m_parser, SIGNAL(groupDeleted (QString)), this, SLOT(groupDeleted(QString)));
			QObject::connect(m_parser, SIGNAL(chatRequest (QString, int, QString, QString, QString, QString)), this, SLOT(chatRequest(QString, int, QString, QString, QString, QString)));
			//QObject::connect(m_parser, SIGNAL(initChatSB (QString, QString)), this, SLOT(initChatSB(QString, QString)));
			QObject::connect(m_parser, SIGNAL(disconnected(ConnectionError)), this, SLOT(disconnected(ConnectionError)));
			m_parser->init();
		}
		qDebug("###### End of Client");
	}       
	
	void Client::initChat(QString destPassport){
		m_destPassport << destPassport;
		XFR x(nextIdtr());
		send (x);
	}

	void Client::getEmoticon(QString destPassport){
		
	}
		
	void Client::sendIsTyping(int chatId){
		if (m_chatList[chatId]){
			MSG msg = MSG();
			msg.addClientName(m_msnPassport);
			msg.addType(MSG_ISTYPING);
			sendChat (chatId, msg);
		}
	}
                
	void Client::disconnect(){
		OUT out;
		send (out);
	}

	void Client::sendChat(int chatId, Command& msg) {
		if (m_chatList[chatId]){
			msg.addTrid (m_chatList[chatId]->nextIdtr());
			send (msg, chatId);
		}
	}

	void Client::addContact(Contact& contact, Group& group){
		// >>ADC 80 FL N=probando_msnpy2@hotmail.com F=+34123456789^M
		// <<ADC 80 FL N=probando_msnpy2@hotmail.com F=+34123456789 C=26a1f2a9-2560-4941-be6a-c4dc7adead55

		// >> ADC 45 FL C=f39c2bb5-8a18-4f40-ae18-75aa595b747e cc57fad0-f245-42c2-a5f7-e13df408cb95
		// << ADC 45 FL C=f39c2bb5-8a18-4f40-ae18-75aa595b747e cc57fad0-f245-42c2-a5f7-e13df408cb95

		qDebug("MSN::Log::Client ## Adding contact %s to group %s\r\n",contact.getPassport().toUtf8().data(), group.getName().toUtf8().data());
		ADC c (nextIdtr());
		c.addList (QString("FL"));
		c.addPassport (contact.getPassport());
		if (!group.getName().isEmpty()){
			c.addId ( (m_roster->getContact(contact.getPassport()))->getId() );
			c.addGroupId (m_roster->getGroupId(group.getName()));
			qDebug() << "mail:" << (m_roster->getContact(contact.getPassport()))->getId() << "Group:" << m_roster->getGroupId(group.getName());
		}
		send (c);
	}
	void Client::delContact(Contact& contact){
		// >> REM 46 FL f39c2bb5-8a18-4f40-ae18-75aa595b747e
		// << REM 46 FL f39c2bb5-8a18-4f40-ae18-75aa595b747e
		qDebug("MSN::Log::Client ## Deleting contact %s\r\n", contact.getPassport().toUtf8().data());
		REM d (nextIdtr());
		d.addList (QString("FL"));
		d.addId((m_roster->getContact(contact.getPassport()))->getId());
		send (d);
	}
	
	void Client::blockContact(Contact& contact){
		// >> REM 55 AL probando_msnpy@hotmail.com
		// << REM 55 AL probando_msnpy@hotmail.com
		// >> ADC 56 BL N=probando_msnpy@hotmail.com
		// << ADC 56 BL N=probando_msnpy@hotmail.com
		qDebug("MSN::Log::Client ## Blocking contact %s\r\n",contact.getPassport().toUtf8().data());

		REM d (nextIdtr());
		d.addList (QString("AL"));
		d.addId(contact.getPassport());
		send (d);

		ADC c (nextIdtr());
		c.addList (QString("BL"));
		c.addPassport(contact.getPassport());
		send (c);
	}

	void Client::deblockContact(Contact& contact){
		// >>> REM 425 BL probando_msnpy@hotmail.com
		// <<< REM 425 BL probando_msnpy@hotmail.com
		// >>> ADC 426 AL N=probando_msnpy@hotmail.com
		// <<< ADC 426 AL N=probando_msnpy@hotmail.com
		qDebug("MSN::Log::Client ## deblocking contact %s\r\n",contact.getPassport().toUtf8().data());
		REM d (nextIdtr());
		d.addList (QString("BL"));
		d.addId(contact.getPassport());
		send (d);

		ADC c (nextIdtr());
		c.addList (QString("AL"));
		c.addPassport(contact.getPassport());
		send (c);
	}

	void Client::addGroup(Group& group){
		// >> ADG 44 grupo%20de%20prueba
		// << ADG 44 grupo%20de%20prueba cc57fad0-f245-42c2-a5f7-e13df408cb95
		qDebug("MSN::Log::Client ## Adding group %s\r\n", group.getName().toUtf8().data());
		ADG g (nextIdtr());
		// TODO : Url encode
		g.addName(group.getName());
		send (g);
	}

	void Client::delGroup(Group& group){
		//>> RMG 47 cc57fad0-f245-42c2-a5f7-e13df408cb95
		//<< RMG 47 cc57fad0-f245-42c2-a5f7-e13df408cb95
		qDebug("MSN::Log::Client ## Deleting group %s\n", group.getName().toUtf8().data());
		RMG g (nextIdtr());
		g.addId(m_roster->getGroupId(group.getName()));
		send (g);
	}

	void Client::changeStatus (State status){
		// CHG idtr status capabilities\r\n
		printf("MSN::Log::Client ## Status Changed\n");
		CHG cmd (nextIdtr());
		cmd.addStatusCode(status);
		cmd.addCapabilities ("1342558252");
		send (cmd);
	}

	Client::~Client(){
		delete m_mainSocket;
	}


	/**************************
	 * ******* SLOTS **********
	 * ************************/
	void Client::clientIpPort (QString ip, QString port){
		qDebug () << "# CLIENT IP PORT" << ip << port;
		m_ip = ip;
		m_port = port;
	}

	void Client::connected() {
		//qDebug ("MSN::Client::SIGNAL ## notifyConnect");
		emit notifyConnect();
	}

	void Client::disconnected(ConnectionError e) {
		delete m_roster;
		delete m_mainSocket;
		m_roster = 0;
		m_mainSocket = 0;
		//printf ("MSN::Client::SIGNAL ## DISCONNECT\n"); 
		emit notifyDisconnect(e);
	}

	void Client::newGroupArrived (Group* group) {
	//	printf ("MSN::Client::SIGNAL ## newGroup\n"); 
		emit notifyNewGroup(group);

		m_roster->addGroup(group);
	}

	void Client::newContactArrived (Contact* contact) {
		if (contact->getGroupId() != "") {
			QString g = m_roster->getGroupName(contact->getGroupId());
			contact->setGroupName(g);
		}
	//	printf ("MSN::Client::SIGNAL ## NewContact\n"); 
		emit notifyNewContact(contact);
		m_roster->addContact(contact);
	}

	void Client::statusChanged (QString passport, State status, QString displayName, QString capabilities ){
		Contact* c = m_roster->getContact(passport);
		c->setStatus (status);
		c->setDisplayName (displayName);
		c->setCapabilities (capabilities);
		//printf ("MSN::Client::SIGNAL ## Presence form:%s displayName:%s CAP:%s\n", passport.toUtf8().data(), displayName.replace("\r\n","\\r\\n").toUtf8().data(), capabilities.toUtf8().data()); 
		emit notifyPresence (c);

	}

	void Client::personalMessage (QString passport, QString personalMsg){
		if (personalMsg != ""){
			Contact* c = m_roster->getContact(passport);
			c->setPersMsg (personalMsg);
			//printf ("MSN::Client::SIGNAL ## personalMsg from:%s persMSG:%s\n", passport.toUtf8().data(), personalMsg.replace("\r\n","\\r\\n").toUtf8().data()); 
			emit notifyPersonalMessage (c);
		}
	}

	void Client::hasBlog (QString passport) {
		printf ("MSN::Client::SIGNAL ## Blog :%s\n",passport.toUtf8().data());
	}

	void Client::groupDeleted (QString groupId) {
		printf ("MSN::Client::SIGNAL ## Deleted Group:%s\n",groupId.toUtf8().data());
		//m_roster->delGroupById (groupId);
	}

	void Client::chatRequest(QString address, int port, QString contact, QString fName, QString ticket, QString sessid){
		printf ("MSN::Log::CLient ## Calling:%s From: %s\n",contact.toUtf8().data(), address.toUtf8().data());
		ParserSB* chatParser = new ParserSB (address, port, ++m_chatCount, m_msnPassport, ticket, sessid, this);
		m_chatList[m_chatCount] = chatParser;
		emit (notifyNewChat (m_chatCount, contact));

		QObject::connect(chatParser, SIGNAL(chatArrivedMessage(int, QString, MSG)), this, SLOT(chatArrivedMessage(int, QString, MSG)));
		QObject::connect(chatParser, SIGNAL(chatInfo(int, QString, QString)), this, SLOT(chatInfo(int, QString, QString)));
		QObject::connect(chatParser, SIGNAL(chatIsTyping(int, QString)), this, SLOT(chatIsTyping(int, QString)));
		QObject::connect(chatParser, SIGNAL(chatLeavedTheRoom(int, QString)), this, SLOT(chatLeavedTheRoom(int, QString)));
		QObject::connect(chatParser, SIGNAL(incomingFileTransfer(Transfer*, int)), this, SLOT(incomingFileTransfer(Transfer*,int)));
       		QObject::connect(chatParser, SIGNAL(fileTransferProgress(int ,int, int)), this, SLOT(fileTransferProgress(int ,int, int)));	
       		QObject::connect(chatParser, SIGNAL(fileTransferFinished(int)), this, SLOT(fileTransferFinished(int)));
		chatParser->start();
	}

	void Client::chatArrivedMessage (int chatId, QString msnPassport, MSG chatMsg) {
		//printf ("MSN::Client::SIGNAL ## Chat message at:%i from:%s MSG:%s\n", chatId, msnPassport.toUtf8().data(), chatMsg.getMsg().replace("\r\n","\\r\\n").toUtf8().data()); 
		emit notifyChatArrivedMessage (chatId, msnPassport, chatMsg);
	}

	void Client::chatInfo (int chatId, QString chatMsnClient, QString chatIsLogging) {
		//printf ("MSN::Client::SIGNAL ## ChatInfo at:%i from:%s Logging:%s\n", chatId, chatMsnClient.toUtf8().data(), chatIsLogging.toUtf8().data()); 
		emit notifyChatInfo (chatId, chatMsnClient, chatIsLogging);
	}

	void Client::chatIsTyping (int chatId, QString chatMsnPassport){
		//printf ("MSN::Client::SIGNAL ## chat Typing at:%i from:%s\n", chatId, chatMsnPassport.toUtf8().data()); 
		emit notifyChatIsTyping (chatId, chatMsnPassport);
	}

	void Client::chatLeavedTheRoom (int chatId, QString passport) {
		//printf ("MSN::Client::SIGNAL ## chatLeaved by:%s from:%i\n", passport.toUtf8().data(), chatId ); 
		emit notifyChatLeavedTheRoom (chatId, passport);
	}

	void Client::incomingFileTransfer (Transfer* msg, int chatId) {
		
		QByteArray path = QByteArray("./" + msg->getFileName());
		qDebug () << "MSN::Client::SIGNAL ## INCOMING FILE TRANSFER form chat" << chatId << path; 
		if (msg->getFileName() != QByteArray("1.TXT")) m_chatList[chatId]->acceptFileTransfer(msg, path);
	}
       	void Client::fileTransferProgress(int ftId,int received, int total) {
		qDebug ("MSN::Client::SIGNAL ## RECEIVED for %i : %i of %i", ftId, received, total); 
	}
       	void Client::fileTransferFinished(int ftId){
		qDebug ("MSN::Client::SIGNAL ## INCOMING FILE TRANSFER FINISHED : %i", ftId); 
	}


	/******************************
	 * ******* LISTENERS **********
	 * ***************************/

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
	}

	void Client::delRosterListener (RosterListener *rl){
		QObject::disconnect (this, 0, rl, 0);
	}

	void Client::addPresenceListener(PresenceListener *pl) {
		QObject::connect(this, SIGNAL(notifyPresence(Contact*)), pl, SLOT(presenceChangedSlot(Contact*)));
		QObject::connect(this, SIGNAL(notifyPersonalMessage(Contact*)), pl, SLOT(personalMessageSlot(Contact*)));
	}

	void Client::delPresenceListener (PresenceListener *pl){
		QObject::disconnect (this, 0, pl, 0);
	}

	void Client::addChatListener (ChatListener *chl){
		QObject::connect (this, SIGNAL(notifyNewChat (int, QString)), chl, SLOT(newChatSlot (int, QString)));
		QObject::connect (this, SIGNAL(notifyChatLeavedTheRoom(int, QString)), chl, SLOT(chatLeavedTheRoomSlot(int, QString)));
		QObject::connect (this, SIGNAL(notifyChatIsTyping(int, QString)), chl, SLOT(chatIsTypingSlot(int, QString)));
		QObject::connect (this, SIGNAL(notifyChatInfo (int, QString, QString)), chl, SLOT(chatInfoSlot(int, QString, QString)));
		QObject::connect (this, SIGNAL(notifyChatArrivedMessage (int, QString, MSG)),chl, SLOT(chatArrivedMessageSlot(int, QString, MSG)));
	}

	void Client::delChatListener (ChatListener *chl){
		QObject::disconnect (this, 0, chl, 0);
	}

}
#include "client.moc"
