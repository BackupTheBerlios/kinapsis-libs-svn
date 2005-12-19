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


#include "parser.h"
#include "flap.h"
#include "errortlv.h"
#include "passwordtlv.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define POSITIVE_MASK 0x7fff

namespace liboscar {

Parser::Parser(Client *c){ 
	srand(time(NULL));
	m_seq = (Word) POSITIVE_MASK * (rand()/RAND_MAX); /* Keep it positive */
	m_client = c;
}

void Parser::add(Byte *data, int len){
	int i;

	for (i=0; i < len; i++)
		m_buf << data[i];
}

void Parser::parse(){
	Byte b = 0, ch = 0;
	Word w = 0;
	Buffer buf;

	/* We have more data in the buffer */
	m_buf.gotoBegin();

	if (m_buf.len() < 6) /* Yet haven't enough data */
		return;

	m_buf >> b;

	if (b != 0x2a){
		qDebug(QString("Invalid FLAP header. Discarding data"));
		m_buf.wipe();
		return; 
	}
	
	m_buf >> ch;
	m_buf >> w; /* Sequence number */
	m_buf >> w; /* Data lenght */

	if (m_buf.len() < (w + 6))
		return;

	/* Copy the FLAP to a local buffer */
	buf << m_buf;
	buf.setLength(w + 6);
	buf.setPosition(6);
	buf.removeFromBegin();
	buf.gotoBegin();

	/* remove the FLAP from the parser buffer */
	m_buf.gotoBegin();
	m_buf.remove(w + 6);

	switch (ch){
		case 0x01:
			parseCh1(buf);
			break;
		case 0x02:
			parseCh2(buf);
			break;
		case 0x04:
			parseCh4(buf);
			break;
		case 0x05:
			parseCh5(buf);
			break;
		default:
			qDebug(QString("FLAP on unknown channel: %1").arg(ch));
			break;
	}

}

void Parser::parseCh1(Buffer& buf){
	FLAP f(0x01, getNextSeqNumber(), 0); // Unknown length
	DWord dw = 0;
	UnformattedTLV *tlv;
	PasswordTLV* pt;

	buf >> dw;

	if (dw != 0x00000001){
		qDebug("Unknown header on channel 1");
		return ;
	}

	f.data() << (DWord) 0x00000001; // Hello header

	if (m_client->state() == CLI_CONNECTED){ // Send cookie
		tlv = new UnformattedTLV(TLV_TYPE_COOKIE);
		m_cookie.gotoBegin();
		tlv->data() << m_cookie;
		f.addTLV(tlv);
	}

	if ((m_client->state() == CLI_AUTHING) || (m_client->state() == CLI_CONNECTED)){

		// UIN
		tlv = new UnformattedTLV(TLV_TYPE_UIN);
		tlv->data() << m_client->getUIN().getUin();
		f.addTLV(tlv);

		// Password
		pt = new PasswordTLV();
		pt->setPassword(m_client->m_password);
		f.addTLV(pt);
		
		// Version
		tlv = new UnformattedTLV(TLV_TYPE_VERSION);
		tlv->data() << TLV_VERSION_ICQ2003B;
		f.addTLV(tlv); 

		// ClientId
		tlv = new UnformattedTLV(TLV_TYPE_CLIENTID);
		tlv->data() << (Word) 0x010a;
		f.addTLV(tlv); 

		// Versionmajor
		tlv = new UnformattedTLV(TLV_TYPE_VERMAJOR);
		tlv->data() << TLV_VERMAJOR;
		f.addTLV(tlv); 

		// Versionminor
		tlv = new UnformattedTLV(TLV_TYPE_VERMINOR);
		tlv->data() << TLV_VERMINOR_ICQ2003B;
		f.addTLV(tlv); 

		// Lesser
		tlv = new UnformattedTLV(TLV_TYPE_LESSER);
		tlv->data() << TLV_LESSER;
		f.addTLV(tlv); 

		// Build
		tlv = new UnformattedTLV(TLV_TYPE_BUILD);
		tlv->data() << TLV_BUILD_ICQ2003B;
		f.addTLV(tlv); 

		// Distrib
		tlv = new UnformattedTLV(TLV_TYPE_DISTRIBUTION);
		tlv->data() << TLV_DISTRIBUTION;
		f.addTLV(tlv); 

		// Language
		tlv = new UnformattedTLV(TLV_TYPE_LANGUAGE);
		tlv->data() << (Word) 0x656e; // en
		f.addTLV(tlv); 

		// Country
		tlv = new UnformattedTLV(TLV_TYPE_COUNTRY);
		tlv->data() << (Word) 0x7573; // us
		f.addTLV(tlv); 

	}

	m_client->send(f.pack());
}

void Parser::parseCh2(Buffer& buf){
}

void Parser::parseCh4(Buffer& buf){

	Word id, l, err;
	Byte b;
	unsigned int i;
	QString reason;
	QString server;
	QString port;

	ErrorTLV errt;

	while (buf.len()){
		buf >> id;
		buf >> l;
		switch (id){
			case 0x0001:
				// Ignore UIN
				buf.advance(l);
				buf.removeFromBegin();
				break;
			case 0x0004:
				for (i=0; i < l; i++){
					buf >> b;
					reason.append(b);
				}
				buf.removeFromBegin();
				break;
			case 0x0005:
				buf >> b;
				while (b != 0x3a){
					server.append(b);
					buf >> b;
				}
				for (i=0; i < 4; i++){
					buf >> b;
					port.append(b);
				}
				buf.removeFromBegin();
				break;
			case 0x0006:
				for (i=0; i < l; i++){
					buf >> b;
					m_cookie << b;
				}
				buf.removeFromBegin();
				break;
			case 0x0008:
				errt.parse(buf);
				buf.removeFromBegin();
				break;
			default:
				qDebug("Unknown TLV on channel 4");
				buf.wipe();
				break;
		}
	}

	if (server.isEmpty()){ // Got an unexpected disconnection
		emit serverDisconnected(reason, errt.getError());
	}
	else // We got the BOS && cookie info :-)
		emit receivedBOS(server, port);
}

void Parser::parseCh5(Buffer& buf){
	if (buf.len() != 0)
		qDebug("Unknown extra data on channel 5");
	sendKeepAlive();
}

void Parser::sendKeepAlive(){
	FLAP f(0x05, getNextSeqNumber(), 0);
	m_client->send(f.pack());
}

Word Parser::getNextSeqNumber(){
	m_seq = ++m_seq & POSITIVE_MASK;
	return m_seq;
}

Parser::~Parser() { }


}

#include "parser.moc"
