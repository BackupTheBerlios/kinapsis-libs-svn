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


#include "client.h"

namespace liboscar {

Client::Client(){
	initvalues();
}

Client::Client(const UIN& uin, const QString& password){
	m_uin = uin;
	m_password = password;
	initvalues();
}

void Client::initvalues(){
	m_bos = "";
	m_bosport = -1;
	m_conn = m_logconn = 0;
	m_parser = 0;
	m_inlogin = false;
}

QString Client::getPassword(){
	return m_password;
}

void Client::setPassword(const QString& password){
	m_password = password;
}

UIN Client::getUIN(){
	return m_uin;
}

void Client::setUIN(const UIN& uin){
/*	TODO: if (m_conn){
		if (m_conn->state() == CONN_CONNECTED)
			;
	} OJO */
	m_uin = uin;
}

void Client::disconnect(){

	if (m_inlogin && m_logconn)
		m_logconn->disconnect();
	else if (!m_inlogin && m_conn)
		m_conn->disconnect();

}

ConnectionResult Client::connect(){
	/* FIXME: comprobar estado en que se queda despues de error */
	ConnectionStatus s;
	ConnectionError e;

	if (!m_parser)
		m_parser = new Parser();

	if (!m_logconn)
		m_logconn = new Connection(ICQ_LOGIN_SERVER, ICQ_LOGIN_PORT, m_parser);

	m_inlogin = true;

	s = m_logconn->connect();
	if (s != CONN_CONNECTED){
		ConnectionResult res(false, CONN_ERR_LOGIN_CONN_FAILED);
		return res;
	}
	e = m_logconn->listen();

	if (e != CONN_NO_ERROR){
		ConnectionResult res(false, e);
		return res;
	}

	delete m_logconn;
	m_logconn = 0;
	m_inlogin = false;

	/* login finished; connect to BOS */
	if (!m_conn)
		m_conn = new Connection(m_bos, m_bosport, m_parser);

	s = m_conn->connect();
	if (s != CONN_CONNECTED){
		ConnectionResult res(false, CONN_ERR_CONN_FAILED);
		return res;
	}

	e = m_conn->listen();
	if (e != CONN_NO_ERROR){
		ConnectionResult res(false, e);
		return res;
	}

	delete m_conn;
	m_conn = 0;

	ConnectionResult res(true, CONN_NO_ERROR);
	return res;
}

Client::~Client() { }


}
