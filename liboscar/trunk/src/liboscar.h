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


#ifndef _LIBOSCAR_H_
#define _LIBOSCAR_H_

#define ICQ_LOGIN_SERVER "login.icq.com"
#define ICQ_LOGIN_PORT 5190

#define AIM_LOGIN_SERVER "login.oscar.aol.com"
#define AIM_LOGIN_PORT 5190

namespace liboscar {

	typedef unsigned char Byte;
	typedef unsigned short int Word;
	typedef unsigned int DWord;

	enum ProtocolType {
		AIM,
		ICQ
	};

	enum ConnectionStatus {
		CONN_DISCONNECTED,
		CONN_CONNECTED,
		CONN_CONNECTING
	};

	enum ConnectionError {
		CONN_ERR_LOGIN_CONN_FAILED,
		CONN_ERR_CONN_FAILED,
		CONN_INPUT_ERROR,
		CONN_OUTPUT_ERROR,
		CONN_ERR_USER_REQUEST,
		CONN_ERR_UNEXPECTED,
		CONN_NO_ERROR,
		CONN_RECONNECT
	};

	enum ClientState {
		CLI_NO_STATE,
		CLI_AUTHING,
		CLI_CONNECTING,
		CLI_CONNECTED,
		CLI_REQUESTING_UIN
	};

	enum SNACError {
		ERROR_HEADER,
		ERROR_RATE_SERVER,
		ERROR_RATE_CLIENT,
		ERROR_OFFLINE,
		ERROR_SERVICE_NA,
		ERROR_SERVICE_UNDEF,
		ERROR_OBSOLETE,
		ERROR_UNSUP_SERVER,
		ERROR_UNSUP_CLIENT,
		ERROR_REFUSED_CLIENT,
		ERROR_REPLY_BIG,
		ERROR_RESPONSE_LOST,
		ERROR_REQ_DENIED,
		ERROR_SNAC,
		ERROR_RIGHTS,
		ERROR_BLOCKED,
		ERROR_EVIL_SENDER,
		ERROR_EVIL_RECIP,
		ERROR_RECIP_UNAVAILABLE,
		ERROR_NO_MATCH,
		ERROR_LIST_OVER,
		ERROR_REQ_AMBIGUOUS,
		ERROR_QUEUE_FULL,
		ERROR_IS_AOL
	};

	enum DisconnectReason {
		NO_ERROR,
		MULTIPLE_LOGINS,
		BAD_PASSWORD,
		NON_EXISTANT_UIN,
		TOO_MANY_CLIENTS,
		RATE_EXCEEDED,
		OLD_VERSION,
		RECONNECTING_TOO_FAST,
		CANT_REGISTER
	};

	enum PresenceStatus {
		STATUS_OFFLINE,
		STATUS_INVISIBLE,
		STATUS_DND,
		STATUS_OCUPPIED,
		STATUS_NA,
		STATUS_AWAY,
		STATUS_FFC,
		STATUS_ONLINE
	};

	enum FirewallConfiguration {
		FIREWALL,
		SOCKS_PROXY,
		NORMAL
	};

	enum InfoRequestType {
		GENERAL_INFO,
		SHORT_USER_INFO,
		AWAY_MESSAGE,
		CAPABILITIES
	};

	enum ContactError {
		EMPTY_UIN_LIST,
		UIN_NOT_IN_LIST,
		CONTACT_LIST_FULL
	};

	enum ICBMError {
		USER_OFFLINE,
		CLIENT_NOT_SUPPORT_TYPE_2,
		MALFORMED_PACKET
	};

	enum MessageEncoding {
		ASCII,
		UCS2BE,
		LOCAL
	};

	enum MessageType {
		TYPE_PLAIN,
		TYPE_CHAT,
		TYPE_FILEREQ,
		TYPE_URL,
		TYPE_AUTHREQ,
		TYPE_AUTHDENY,
		TYPE_AUTHOK,
		TYPE_SERVER,
		TYPE_ADDED,
		TYPE_WWP,
		TYPE_EEXPRESS,
		TYPE_CONTACTS,
		TYPE_PLUGIN,
		TYPE_AUTOAWAY,
		TYPE_AUTOBUSY,
		TYPE_AUTONA,
		TYPE_AUTODND,
		TYPE_AUTOFFC
	};

	enum MessageFlags {
		FLAG_NORMAL,
		FLAG_AUTO,
		FLAG_MULTI
	};

	enum MessageRequest {
		REQUEST,
		CANCEL,
		ACCEPT
	};

	enum LostReason {
		MESSAGE_INVALID,
		MESSAGE_TOO_LARGE,
		MESSAGE_RATE_EXCEEDED,
		SENDER_TOO_EVIL,
		CLIENT_TOO_EVIL
	};

	enum UserClass {
		CLASS_UNCONFIRMED,
		CLASS_ADMINISTRATOR,
		CLASS_AOL,
		CLASS_COMMERCIAL,
		CLASS_FREE,
		CLASS_AWAY,
		CLASS_ICQ,
		CLASS_WIRELESS,
		CLASS_UNKNOWN100,
		CLASS_UNKNOWN200,
		CLASS_UNKNOWN400,
		CLASS_UNKNOWN800
	};
}

#endif // _LIBOSCAR_H_
