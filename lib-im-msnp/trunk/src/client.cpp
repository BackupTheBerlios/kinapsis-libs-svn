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
#include "notificationServer.h"

namespace libimmsnp {

	Client::Client(QString msnPassport, QString msnPass, State initialStatus){
		m_parser = 0;
		m_conn = 0;
		m_mainSocket = 0;
		m_msnPassport = msnPassport;
		m_msnPass = msnPass;
		m_idtr = 1;
		m_roster = 0;
		m_initialStatus = initialStatus;
	}

	int Client::getIdtr() {
		return ++m_idtr;
	}

	void Client::send(Command& c) {
	 	m_mainSocket->send (c.makeCmd());
	}

	void Client::makeConnection (QString ip, int port){
		qDebug ("Terminando conexion");
		m_mainSocket = new msocket(ip, port);
		m_mainSocket->connect();
	}

	void Client::startConnection(){
		delete m_conn;
		m_parser = new ParserNS (m_msnPassport, m_msnPass, m_initialStatus, this);
		QObject::connect(m_parser, SIGNAL(mainSocket(msocket*)), this, SLOT(mainSocket(msocket*)));
		QObject::connect(m_parser, SIGNAL(connected()), this, SLOT(connected()));
		QObject::connect(m_parser, SIGNAL(newGroupArrived(Group*)), this, SLOT(newGroupArrived(Group*)));
		QObject::connect(m_parser, SIGNAL(newContactArrived(Contact*)), this, SLOT(newContactArrived(Contact*)));
		QObject::connect(m_parser, SIGNAL(statusChanged (QString, State, QString, QString)), this, SLOT(statusChanged(QString, State, QString, QString)));
		QObject::connect(m_parser, SIGNAL(personalMessage (QString, QString)), this, SLOT(personalMessage(QString, QString)));
		QObject::connect(m_parser, SIGNAL(hasBlog(QString)), this, SLOT(hasBlog(QString)));
		m_conn = new Connection (m_mainSocket, m_parser);
		m_conn->run();
	}


	void Client::connect() {
		m_parser = new ParserNS (m_msnPassport, m_msnPass, m_initialStatus, this);
		m_roster = new Roster ();
		m_mainSocket = new msocket("messenger.hotmail.com",1863);
		m_mainSocket->connect();
		m_conn = new Connection (m_mainSocket, m_parser,3);

		VER v(1);
		v.addProtocolSupported("MSNP12");
		v.addProtocolSupported("MSNP11");
		v.addProtocolSupported("MSNP10");
		send(v);
		m_conn->run();

	}

	void Client::changeStatus (State status){
		printf(":::::::::::::::::::::Status Changed\n");
		std::ostringstream msgStatus;
		CHG cmd (getIdtr());
		cmd.addStatusCode(status);
		cmd.addClientId ("1342558252");
		send (cmd);

	}
	void Client::changeNick(QString nick) {
		// PRP idtr nick\r\n
		PRP prp (getIdtr());
		prp.addNick(nick);
		send (prp);
	}

	Client::~Client(){
		delete m_parser;
		delete m_mainSocket;
//		if (!m_conn->finished()) {
//			m_conn->terminate();
//			m_conn->wait();
//	        }

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
		//m_conn->start();
		//m_conn->wait();
	}

	void Client::connected() {
		emit notifyConnect();
	}

	void Client::disconnected() {
		emit notifyDisconnect();
	}

	void Client::newGroupArrived (Group* g) {
		printf("# GROUP: %s %s\n",g->getName().latin1(),g->getId().latin1());
		m_roster->addGroup (g);
	}

	void Client::newContactArrived (Contact* contact) {
		QPtrList<Group> lsg = m_roster->getGroups();
		for (uint i = 0; i < lsg.count() ; ++i)
			if (lsg.at(i))
				if (lsg.at(i)->getId () == contact->getId())
					contact->setGroupName(lsg.at(i)->getName());

		m_roster->addContact(contact);
		emit notifyNewContact(contact);
	}

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
}
#include "client.moc"
