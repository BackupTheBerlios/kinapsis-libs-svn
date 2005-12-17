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

namespace liboscar {

	typedef unsigned char Byte;
	typedef unsigned short int Word;
	typedef unsigned int DWord;

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
		CONN_NO_ERROR
	};

	enum ClientState {
		CLI_NO_STATE,
		CLI_AUTHING,
		CLI_CONNECTING,
		CLI_CONNECTED,
		CLI_REQUESTING_UIN
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
}

#endif // _LIBOSCAR_H_
