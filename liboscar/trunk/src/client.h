/***************************************************************************
 *   Copyright (C) 2005 by Luis Cidoncha                                   *
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
#include "connection.h"
#include "connectionresult.h"
#include "liboscar.h"
#include "parser.h"
#include "message.h"
#include "capabilities.h"
#include "roster.h"
#include "rosterlistener.h"
#include "connectionlistener.h"
#include "messagelistener.h"
#include "presencelistener.h"
#include "uinregistrationlistener.h"
#include <qobject.h>

namespace liboscar {

	class UIN;
	class Connection;
	class ListenerManager;

class Client : public QObject {
Q_OBJECT

	friend class Parser;

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

	ClientState state();

	ConnectionResult connect();
	void disconnect(ConnectionError err=CONN_NO_ERROR);

	// Actions
	void sendMessage(UIN uin, QString message);
	void sendMessage(Message message);
	void setPresence(PresenceStatus status);
	void registerNewUin(QString password);
	void authorize(UIN uin, QString message, bool ack);
	void addContact(UIN uin, bool reqAuth);

	// Listener's connections
	void addConnectionListener(ConnectionListener *cl);
	void delConnectionListener(ConnectionListener *cl);
	void addRosterListener(RosterListener *rl);
	void delRosterListener(RosterListener *rl);
	void addMessageListener(MessageListener *ml);
	void delMessageListener(MessageListener *ml);
	void addPresenceListener(PresenceListener *pl);
	void delPresenceListener(PresenceListener *pl);
	void addUINRegistrationListener(UINRegistrationListener *ul);
	void delUINRegistrationListener(UINRegistrationListener *ul);

	virtual ~Client();

signals:
	// Connection
	void notifyConnect();
	void notifyDisconnect();

	// Roster
	void notifyNewContact(Contact *c);
	void notifyAuthRequest(UIN uin, QString reason);

	// Message
	void notifyMessage(Message message);

	// presence
	void notifyPresence(UIN uin, PresenceStatus status);
	void notifyAwayMessage(UIN uin, QString awaymsg);

	// uinregistration
	void notifyNewUin(UIN uin);

public slots:

	void getBOSInfo(QString server, QString port);
	void unexpectedDisconnect(QString reason, DisconnectReason error);
	void finishedConnection();
	void rosterArrived(Roster roster);
	void messageArrived(Message message);
	void statusChanged(UIN uin, PresenceStatus status);
	void newUin(UIN uin);
	void authReq(UIN uin, QString reason);
	void newAwayMessage(UIN uin, QString awaymsg);

protected:
	DWord getLocalIP();
	Word getPort();
	FirewallConfiguration getFirewall();
	Roster& getRoster();
	Capabilities& getCapabilities();

private:
	void send(Buffer &b);
	void initvalues();

	ConnectionError connAuth();
	ConnectionError connBOS();

	void rosterEditStart();
	void rosterEditEnd();

	QString m_bos;
	int m_bosport;

	UIN m_uin;
	QString m_password;
	ProtocolType m_type;
	QString m_awaymsg;

	ClientState m_state;
	bool m_middledisconnect;
	bool m_exit;

	FirewallConfiguration m_firewall;

	Connection* m_conn;
	Connection* m_logconn;
	Parser* m_parser;

	Roster m_roster;

	Capabilities m_cap;
};

}

#endif // _CLIENT_H_
