/***************************************************************************
 *   Copyright (C) 2005-2008 by Luis Cidoncha                              *
 *   luis.cidoncha@gmail.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "uin.h"
#include "oscarparser.h"
#include "service.h"
#include "connection.h"
#include "oscarconnectionresult.h"
#include "liboscar.h"
#include "message.h"
#include "capabilities.h"
#include "roster.h"
#include "rosterlistener.h"
#include "rosterprocess.h"
#include "loginst2process.h"
#include "servicesetupprocess.h"
#include "offlinemessagesprocess.h"
#include "presenceprocess.h"
#include "loginservice.h"
#include "connectionlistener.h"
#include "messagelistener.h"
#include "presencelistener.h"
#include "istypinglistener.h"
#include <qobject.h>
#include <qbytearray.h>

namespace liboscar {

	class UIN;
	class Connection;
//	class Parser;

class Client : public Service {
Q_OBJECT

	friend class RosterProcess;
	friend class ServiceSetupProcess;
	friend class OfflineMessagesProcess;
	friend class PresenceProcess;

public:
	Client(const ProtocolType type = ICQ);
	Client(const UIN& uin, const QString& password, const ProtocolType type = ICQ);

	QString getPassword();
	void setPassword(const QString& password);
	
	UIN getUIN();
	void setUIN(const UIN& uin);
	
	void setType(const ProtocolType type);
	ProtocolType getType();

	void setAwayMessage(QString message);
	QString getAwayMessage();

	void connect();

	// Actions
	void disconnect();
	void sendMessage(UIN uin, QString message);
	void sendMessage(Message message);
	void setPresence(PresenceStatus status);
	void sendTypingNotice(UIN uin, IsTypingType type);

	// Process actions
	
		// RosterProcess
	bool addContact(UIN uin);
	bool delContact(UIN uin);
	bool authorize(UIN uin, QString message, bool ack);


	// Process resources
	Roster* getRoster();
	Capabilities& getCapabilities();

	// Listener's connections
	void addConnectionListener(ConnectionListener *cl);
	void delConnectionListener(ConnectionListener *cl);
	void addRosterListener(RosterListener *rl);
	void delRosterListener(RosterListener *rl);
	void addMessageListener(MessageListener *ml);
	void delMessageListener(MessageListener *ml);
	void addPresenceListener(PresenceListener *pl);
	void delPresenceListener(PresenceListener *pl);
	void addIsTypingListener(IsTypingListener *tl);
	void delIsTypingListener(IsTypingListener *tl);

	virtual ~Client();

signals:
	// ConnectionListener
	void notifyConnect();
	void notifyDisconnect(OscarConnectionResult);

	// MessageListener
	void notifyMessage(Message message);

	//istyping
	void notifyTypingEvent(UIN uin, IsTypingType type);

public slots:

	// From m_conn
	void BOSConnected();
	void BOSDisconnected();
	void BOSError(SocketError);

	// From loginservice
	void loginServiceEnded(ConnectionResult);

	// From rosterprocess
	void finishedConnection();

	void getBOSInfo(QString server, QString port, QByteArray cookie);
	void unexpectedDisconnect(QString reason, DisconnectReason error);
	void messageArrived(Message message);
	void newTypingEvent(UIN uin, IsTypingType type);

protected:
	FirewallConfiguration getFirewall();

	void run(); // We use here our own run()

//private slots:
//	void disconnectSlot(); // When we finish, we'll get this to emit notifyDisconnect

private:
	void initvalues();
	void createProcess();
	void createSupport();
	void registerMeta(); // from Service

	void connBOS(QString bos, int port);
	void create();

	UIN m_uin;
	QString m_password;
	ProtocolType m_type;
	QString m_awaymsg;

	ConnectionResult m_cr;
	
	FirewallConfiguration m_firewall;

	Roster* m_roster;

	Capabilities m_cap;

	bool m_connected;
	bool m_logged;

	//
	// Process
	//
	
	RosterProcess* m_rp;
	LoginSt2Process* m_l2p;
	ServiceSetupProcess* m_ssp;
	OfflineMessagesProcess* m_omp;
	PresenceProcess* m_pp;

	LoginService* m_ls;
};

}

#endif // _CLIENT_H_
