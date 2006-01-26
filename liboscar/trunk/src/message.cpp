/***************************************************************************
 *   Copyright (C) 2006 by Luis Cidoncha                                   *
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


#include "message.h"
#include "tlv.h"

namespace liboscar {

Message::Message() {
	m_msg = "";
	m_uin = UIN(0);
}

Message::~Message() { }
	
QString Message::getText(){
	return m_msg;
}

void Message::setText(QString message) {
	m_msg = message;
}

UIN Message::getUin() {
	return m_uin;
}

void Message::setUin(UIN uin) {
	m_uin = uin;
}

Word Message::getFormat(){
	return m_format;
}

void Message::setFormat(Word format){
	m_format = format;
}

UserInfo Message::getInfo(){
	return m_info;
}

MessageEncoding Message::getEncoding(){
	return m_encoding;
}

void Message::setEncoding(MessageEncoding encoding) {
	m_encoding = encoding;
}

MessageType Message::getType(){
	return m_type;
}

void Message::setType(MessageType type) {
	m_type = type;
}

MessageRequest Message::getRequest(){
	return m_ch2req;
}

void Message::setRequest(MessageRequest req) {
	m_ch2req = req;
}

QDateTime Message::getTime(){
	return m_time;
}

void Message::setTime(QDateTime time){
	m_time = time;
}

Buffer& Message::pack() {

	UnformattedTLV* tlvm;
	UnformattedTLV* tlv;

	m_data << (DWord) m_cookiehigh;
	m_data << (DWord) m_cookielow;

	m_data << m_format;
	m_uin.appendUin(m_data);

	switch (m_format) {
		default:
		case 0x0001:
			tlvm = new UnformattedTLV(TLV_TYPE_PASSWORD);
		
			tlv = new UnformattedTLV(TLV_TYPE_CAPABILITIES2);
			tlv->data() << (Word) 0x0106; // FIXME: ¿?
		
			tlvm->data() << tlv->pack();
			delete tlv;
		
			tlv = new UnformattedTLV(TLV_TYPE_MESSAGE);
			tlv->data() << (Word) m_encoding;
			tlv->data() << (Word) 0x0000; // Unknown
			tlv->data() << m_msg;
		
			tlvm->data() << tlv->pack();
			delete tlv;
		
			m_data << tlvm->pack();
			delete tlvm;
			tlv = new UnformattedTLV(TLV_TYPE_COOKIE);
			m_data << tlv->pack(); 
			delete tlv;
			break;
		case 0x0002:
			// TODO
			break;
		case 0x0004:
			// TODO
			break;
	}

	return m_data;
}

void Message::parse(Buffer &b) {

	Word w;

	m_time = QDateTime::currentDateTime(); // No date info for online messages

	b >> m_cookiehigh;
	b >> m_cookielow;

	b >> w;
	m_format = w;

	m_info.parse(b);
	m_uin = m_info.getUin();

	// Fixed part parsed. Let's go to format specifics
	
	switch (m_format) {
		case 0x0001:
			parseCh1(b);
			break;
		case 0x0002:
			parseCh2(b);
			break;
		case 0x0004:
			parseCh4(b);
			break;
		default:
			qDebug("Unknown message format.");
			break;
	}
}

MessageType Message::byteToType(Byte b) {
	switch (b) {
		default:
		case 0x01:
			return TYPE_PLAIN;
		case 0x02:
			return TYPE_CHAT;
		case 0x03:
			return TYPE_FILEREQ;
		case 0x04:
			return TYPE_URL;
		case 0x06:
			return TYPE_AUTHREQ;
		case 0x07:
			return TYPE_AUTHDENY;
		case 0x08:
			return TYPE_AUTHOK;
		case 0x09:
			return TYPE_SERVER;
		case 0x0c:
			return TYPE_ADDED;
		case 0x0d:
			return TYPE_WWP;
		case 0x0e:
			return TYPE_EEXPRESS;
		case 0x13:
			return TYPE_CONTACTS;
		case 0x1a:
			return TYPE_PLUGIN;
		case 0xe8:
			return TYPE_AUTOAWAY;
		case 0xe9:
			return TYPE_AUTOBUSY;
		case 0xea:
			return TYPE_AUTONA;
		case 0xeb:
			return TYPE_AUTODND;
		case 0xec:
			return TYPE_AUTOFFC;
	}
}

MessageFlags Message::byteToFlags(Byte b) {
	switch (b) {
		default:
		case 0x01:
			return FLAG_NORMAL;
		case 0x03:
			return FLAG_AUTO;
		case 0x80:
			return FLAG_MULTI;
	}
}

void Message::parseCh1(Buffer &b) {
	UnformattedTLV tlv(TLV_TYPE_GENERIC);
	Word type, len, w;
	Byte by;

	m_type = TYPE_PLAIN;
	m_flags = FLAG_NORMAL;

	b >> type; b >> len;

	if (type == 0x0004){
		b >> type; b >> len; // ¿only AUTOAWAY? TODO automated messages
		m_type = TYPE_AUTOAWAY;
	}

	if (type != 0x0002){
		qDebug("No message TLV in channel 1 message");
		return;
	}

	tlv.parse(b); // Capabilities

	b >> type;
	b >> len;

	len -= 4; // Two words

	b >> w;
	switch (w){
		case 0x0000:
			m_encoding = ASCII;
			break;
		case 0x0002:
			m_encoding = UCS2BE;
			break;
		case 0x0003:
			m_encoding = LOCAL;
			break;
	}
	b >> w;

	if (m_encoding == UCS2BE){
		len /=2; // Reading Words

		Word strucs2[len];
		int i = 0;

		while (len--)
			b >> strucs2[i++];
		strucs2[i++] = 0;

		m_msg = QString::fromUcs2(strucs2);
	}
	else {
		while (len--){
			b >> by;
			m_msg.append(by);
		}
	}
	qDebug("Message: " + m_msg);
	b.removeFromBegin();
	// TODO: are we getting a empty 0x06 TLV in the end? what does that mean?
}

void Message::parseCh2(Buffer &b) {
	Word type, len, reqType;
	DWord dw;
	UnformattedTLV tlv(TLV_TYPE_GENERIC);

	b >> type;
	b >> len;

	b >> reqType; m_ch2req = (MessageRequest) reqType;

	b.advance(8); // Cookie
	b.advance(16); // Capability

	len -= 26; // Type, cookie and capability

	while (len > 0) {
		tlv.parse(b);
		switch (tlv.getType()){
			// XXX: ignoring lots of stuff here
			case 0x0004:
			case 0x0005:
			case 0x000a:
			case 0x000b:
			case 0x000f:
				break;
			case 0x2711:
				// real message :-)
				parse2711(tlv.data());
				break;
			default:
				qDebug("Unknown TLV in channel 2 message");
				break;
		}
		len -= tlv.len();
	}
}

void Message::parseCh4(Buffer &b) {
	// TODO: handle user class and encodings

	Word type, len;
	DWord dw;
	Byte by;

	m_encoding = ASCII;

	b >> type;
	b >> len;
	b >> dw; // UIN in hex

	b >> by; m_type = byteToType(by);
	b >> by; m_flags = byteToFlags(by);

	b.setLittleEndian();
	b >> len;
	b.setBigEndian();

	while (len--){
		b >> by;
		m_msg.append(by);
	}
	b.removeFromBegin();
}

void Message::parse2711(Buffer& b){
	
	Word w, len;
	
	b.setLittleEndian();

	b >> w; b >> w; // len and protocol 

	b.advance(23); // plugin info && unknown && client cap && unknown

	b >> m_ch2cookie; // We'll need to send this in reply
	
	b >> len;
	b >> w; //same cookie
	b.advance(len - 2); // extra data

	// Here comes the message itself
	Byte by;

	b >> by; m_type = byteToType(by);
	b >> by; m_flags = byteToFlags(by);

	b >> w; b >> w; // status and priority stuff: ignoring

	b.readString(m_msg);
	m_encoding = ASCII;

	if (m_type == TYPE_PLAIN){
		// we can receive color info for plain messages
		DWord dw;
		b >> dw; b >> dw;
		if (b.len() > 4){
			QString cap;
			// Capability info
			b.readString(cap);
			if (cap == "{0946134E-4C7F-11D1-8222-444553540000}")
				m_encoding = UCS2BE;
		}
	}
	b.removeFromBegin();
	b.setBigEndian();
}


}
