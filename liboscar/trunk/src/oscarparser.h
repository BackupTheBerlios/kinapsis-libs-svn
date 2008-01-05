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


#ifndef _OSCARPARSER_H_
#define _OSCARPARSER_H_

#include "buffer.h"
#include "client.h"
#include "service.h"
#include "families.h"
#include "message.h"
#include "roster.h"
#include "userinfo.h"
#include "parser.h"
#include "snac_roster.h"
#include "snac_bos.h"
#include "snac_contact.h"
#include "snac_icbm.h"
#include "snac_service.h"
#include "snac_location.h"
#include "snac_icq.h"
#include <qstring.h>
#include <qbytearray.h>

namespace liboscar {

class OscarParser : public Parser {
Q_OBJECT

public:
	OscarParser();

	virtual ~OscarParser();

signals:
	void serverDisconnected(QString reason, DisconnectReason error);
	void newMessage(Message msg);
	void typingEventArrived(UIN uin, IsTypingType type);

	//
	// To LoginSt1Process && LoginSt2Process signals
	//
	void recvHello();

	//
	// To LoginSt1Process signals
	//
	void receivedBOS(QString server, QString port, QByteArray cookie);

	//
	// To RosterProcess signals
	//
	void rosterArrived(Buffer buf);
	void rosterServerAck(RosterModifyAck a);
	void authReq(UIN uin, QString reason);
	void addedYou(UIN uin);
	
	//
	// To LoginSt2Process signals
	//
	void serverFamilies(SrvFamiliesSNAC sf);
	void serverServicesVersion(SrvVersionsSNAC svs);
	void serverRateLimits(SrvRatesSNAC sra);

	//
	// To ServiceSetupProcess signals
	//
	void locationLimits(SrvReplyLocationSNAC srl);
	void BLMLimits(SrvReplyBuddySNAC srb);
	void ICBMLimits(SrvReplyICBMSNAC sri);
	void PRMLimits(SrvReplyBOSSNAC srb);
	void SSILimits(SrvReplyListsSNAC srr);
	
	//
	// To OfflineMessagesProcess signals
	//
	void serverMetaReply(SrvMetaReplySNAC mrs);

	//
	// To PresenceProcess signals
	// 
	void statusChanged(UIN uin, PresenceStatus status);
	void awayMessageArrived(UIN uin, QString awaymsg);
	void serverUserInfo(SrvUserInfoSNAC suis);

	//
	// To UinRegistrationProcess signals
	//
	void newUin(UIN uin);

public slots:
	void parse();

private:
	void parseCh1(Buffer& buf);
	void parseCh2(Buffer& buf);
	void parseCh4(Buffer& buf);
	void parseCh5(Buffer& buf);

	void parseCh2Service(Buffer& buf);
	void parseCh2Location(Buffer& buf);
	void parseCh2Contact(Buffer& buf);
	void parseCh2ICBM(Buffer& buf);
	void parseCh2BOS(Buffer& buf);
	void parseCh2Interval(Buffer& buf);
	void parseCh2Roster(Buffer& buf);
	void parseCh2ICQ(Buffer& buf);
	void parseCh2NewUser(Buffer& buf);

	void sendKeepAlive();

	Families m_fam;

};

}

#endif // _OSCARPARSER_H_
