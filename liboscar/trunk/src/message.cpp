/***************************************************************************
 *   Copyright (C) 2006-2008 by Luis Cidoncha                              *
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
	m_format = 0x0001;
	m_msg = "";
	m_uin = UIN(0);
	
	m_encoding = ASCII;
	m_type = TYPE_PLAIN;
	m_flags = FLAG_NORMAL;

	m_cookie = 0; 

	m_ch2cookie = 0x0000;
	m_ch2req = REQUEST;

	m_time.setDate(QDate(1970,1,1));
	m_time.setTime(QTime(0,0));
}

Message::~Message() { }
	
void Message::fromMessage(Message &m){
	m_format = m.m_format;
	m_msg = m.m_msg;
	m_uin = m.m_uin;
	
	m_encoding = m.m_encoding;
	m_type = m.m_type;
	m_flags = m.m_flags;

	m_cookie = m.m_cookie; 

	m_ch2cookie = m.m_ch2cookie;
	m_ch2req = m.m_ch2req;

	m_time = m.m_time;
}

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

MessageFlags Message::getFlags(){
	return m_flags;
}

void Message::setFlags(MessageFlags flags) {
	m_flags = flags;
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

QWord Message::getCookie(){
	return m_cookie;
}

void Message::setCookie(QWord c){
	m_cookie = c;
}
	
Word Message::getCh2Cookie(){
	return m_ch2cookie;
}

void Message::setCh2Cookie(Word cookie){
	m_ch2cookie = cookie;
}

FTData Message::getFTData(){
	return m_ftd;
}

Byte Message::typeToByte(MessageType t) {
	switch (t) {
		default:
		case TYPE_PLAIN:
			return 0x01;
		case TYPE_CHAT:
			return 0x02;
		case TYPE_FILEREQ:
			return 0x03;
		case TYPE_URL:
			return 0x04;
		case TYPE_AUTHREQ:
			return 0x06;
		case TYPE_AUTHDENY:
			return 0x07;
		case TYPE_AUTHOK:
			return 0x08;
		case TYPE_SERVER:
			return 0x09;
		case TYPE_ADDED:
			return 0x0c;
		case TYPE_WWP:
			return 0x0d;
		case TYPE_EEXPRESS:
			return 0x0e;
		case TYPE_CONTACTS:
			return 0x13;
		case TYPE_PLUGIN:
			return 0x1a;
		case TYPE_AUTOAWAY:
			return 0xe8;
		case TYPE_AUTOBUSY:
			return 0xe9;
		case TYPE_AUTONA:
			return 0xea;
		case TYPE_AUTODND:
			return 0xeb;
		case TYPE_AUTOFFC:
			return 0xec;
	}
}

Byte Message::flagsToByte(MessageFlags f) {
	switch (f) {
		default:
		case FLAG_NORMAL:
			return 0x01;
		case FLAG_AUTO:
			return 0x03;
		case FLAG_MULTI:
			return 0x80;
	}
}

Buffer& Message::pack() {

	UnformattedTLV* tlvm;
	UnformattedTLV* tlv;

	m_data << (QWord) m_cookie;

	m_data << m_format;
	m_uin.appendUin(m_data);

	switch (m_format) {
		default:
		case 0x0001:
			// Channel-1 message
			tlvm = new UnformattedTLV(TLV_TYPE_PASSWORD);
		
			tlv = new UnformattedTLV(TLV_TYPE_CAPABILITIES2);
			tlv->data() << (Word) 0x0106; // FIXME: ¿?
		
			tlvm->data() << tlv->pack();
			delete tlv;
		
			tlv = new UnformattedTLV(TLV_TYPE_MESSAGE);
			tlv->data() << (Word) m_encoding;
			tlv->data() << (Word) 0x0000; // Unknown
			if (m_encoding == UCS2BE && m_msg.length()){
				Word *ucs = (Word *) m_msg.utf16();
				while (*ucs){
					tlv->data() << (Word) *ucs;
					ucs++;
				}
			}
			else
				tlv->data() << m_msg;
		
			tlvm->data() << tlv->pack();
			delete tlv;
		
			m_data << tlvm->pack();
			delete tlvm;
			break;
		case 0x0002:
			// Channel-2 message
			packCh2();
			break;
		case 0x0004:
			// Channel-4 message
			tlvm = new UnformattedTLV(TLV_TYPE_SERVER);
			tlvm->data().setLittleEndian();
			tlvm->data() << (DWord) m_uin.getId().toUInt(); // XXX: port
			tlvm->data() << typeToByte(m_type);
			tlvm->data() << flagsToByte(m_flags);
			tlvm->data() << (Word) m_msg.length();
			tlvm->data() << m_msg;

			m_data << tlvm->pack();
			delete tlvm;
			break;
	}

	if (m_type != TYPE_FILEREQ){
		// Request offline storage TODO: we don't want to store it ever
		tlv = new UnformattedTLV(TLV_TYPE_COOKIE);
		m_data << tlv->pack(); 
		delete tlv;
	}

	return m_data;
}

void Message::packCh2(){

	// This code sucks
	// TODO: FT packing ????
	
	UnformattedTLV* tlvm;
	UnformattedTLV* tlv;

	tlvm = new UnformattedTLV(0x0005); // This TLV contains all the stuff

	tlvm->data() << (Word) m_ch2req;
	tlvm->data() << (QWord) m_cookie;

	// FIXME: FT cap. add it to the capabilities
	// XXX: what happens if it's not a FT message?
	tlvm->data() << (DWord) 0x09461343;
	tlvm->data() << (DWord) 0x4c7f11d1;
	tlvm->data() << (DWord) 0x82224445;
	tlvm->data() << (DWord) 0x53540000;

	if (m_ch2req == REQUEST) { // Requests have a looot of info
		// FIXME: add message string (TLV 0x000c) ??
		tlv = new UnformattedTLV(0x000a);
		tlv->data() << m_ftd.getReqNumber();
		tlvm->data() << tlv->pack();
		delete tlv;
	
		tlv = new UnformattedTLV(0x000f);
		tlvm->data() << tlv->pack();
		delete tlv;
	
		tlv = new UnformattedTLV(0x0003);
		tlv->data() << m_ftd.getClientIP(); // internal IP
		tlvm->data() << tlv->pack();
		delete tlv;
	
		tlv = new UnformattedTLV(0x0005);
		tlv->data() << (Word) m_ftd.getListeningPort(); // listen port
		tlvm->data() << tlv->pack();
		delete tlv;
	
		tlv = new UnformattedTLV(0x0017);
		tlv->data() << (~m_ftd.getListeningPort()); // port check
		tlvm->data() << tlv->pack();
		delete tlv;
	
		if (m_ftd.getProxyUsed()) {
			tlv = new UnformattedTLV(0x0010); // proxy flag
			tlvm->data() << tlv->pack();
			delete tlv;
	
			tlv = new UnformattedTLV(0x0002);
			tlv->data() << m_ftd.getProxyIP(); // proxy ip
			tlvm->data() << tlv->pack();
			delete tlv;
	
			tlv = new UnformattedTLV(0x0016);
			tlv->data() << (DWord) ~(m_ftd.getProxyIP().toLong()); // proxy ip check
			tlvm->data() << tlv->pack();
			delete tlv;
	
		}
		tlv = new UnformattedTLV(0x000b);
		tlv->data() << (Word) 0x0000;
		tlvm->data() << tlv->pack();
		delete tlv;
	
		// Message stuff
		tlv = new UnformattedTLV(0x2711);
		if (m_type == TYPE_FILEREQ) {
			tlv->data() << (Word) (m_ftd.getFileCount() > 1 ? 0x0002 : 0x0001);
			tlv->data() << m_ftd.getFileCount();
			tlv->data() << m_ftd.getFileSize();
			// FIXME: check encoding ...
			tlv->data() << m_ftd.getFileName();
			tlv->data() << (Byte) 0x00; // NULL-terminated strings
	
			tlvm->data() << tlv->pack();
			delete tlv;
	
			tlv = new UnformattedTLV(0x2712); // file encoding stuff FIXME: as above
			tlv->data() << "us-ascii";
			tlvm->data() << tlv->pack();
			delete tlv;
		}
		else {
			tlv->data().setLittleEndian();
			tlv->data() << (Word) 0x001b; // 27
			tlv->data() << (Word) 0x0008;
		
			for (unsigned int i = 0; i < 16; i++)
				tlv->data() << (Byte) 0x00; //plugin
		
			tlv->data() << (Word) 0x0000;
			tlv->data() << (DWord) 0x00000003; //capabilities: as seen in doc
			tlv->data() << (Byte) 0x00;
		
			tlv->data() << m_ch2cookie;
		
			tlv->data() << (Word) 0x000e; //len: 14
			tlv->data() << m_ch2cookie; // again :-?
			for (unsigned int i = 0; i < 12; i++)
				tlv->data() << (Byte) 0x00; // I hate the OSCAR guys
		
			// Let's go for the fucking real message
			
			tlv->data() << typeToByte(m_type);
			tlv->data() << flagsToByte(m_flags);
		
			tlv->data() << (Word) 0x0000; // status
			tlv->data() << (Word) 0x0000; // prio
		
			tlv->data() << (Word) (m_msg.length() + 1);
			tlv->data() << m_msg;
			tlv->data() << (Byte) 0x00; // the string must be null terminated
		
			tlvm->data() << tlv->pack();
			delete tlv;
		}
	}
	
	m_data << tlvm->pack();
	delete tlvm;
}

void Message::parse(Buffer &b) {

	Word w;

	m_time = QDateTime::currentDateTime(); // No date info for online messages

	b >> m_cookie;

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
	Word type=0, len=0, w;
	Byte by;

	m_type = TYPE_PLAIN;
	m_flags = FLAG_NORMAL;

	// XXX: well it seems like ICQ5 sends a unknown 0x13 TLV here.
	// Ignore TLVs other than 0x0004 and 0x0002
	// TODO: make this a TLVchain
	
	while (type != 0x0004 && type != 0x0002){
		b.advance(len);
		b >> type; b >> len;
	}

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

		m_msg = QString::fromUtf16(strucs2);
	}
	else {
		while (len--){
			b >> by;
			m_msg.append(by);
		}
	}

	b.removeFromBegin();
	// TODO: are we getting a empty 0x06 TLV in the end? what does that mean?
}

void Message::parseCh2(Buffer &b) {
	Word type=0, len=0, reqType=0, w;
	DWord dw;
	DWord c1,c2,c3,c4;
	UnformattedTLV tlv(TLV_TYPE_GENERIC);
	QString s;

	// XXX: well it seems like ICQ5 sends a unknown 0x13 TLV here.
	// Ignore TLVs other than 0x0005
	
	while (type != 0x0005){
		b.advance(len);
		b >> type;
		b >> len;
	}

	b >> reqType; m_ch2req = (MessageRequest) reqType;

	b.advance(8); // Cookie

	b >> c1; // 16 bytes of capability
	b >> c2; //
	b >> c3; //
	b >> c4; //

	if (c1 == 0x09461343 && c2 == 0x4c7f11d1
			&& c3 == 0x82224445 && c4 == 0x53540000)
		m_type = TYPE_FILEREQ;

	len -= 26; // Type, cookie and capability

	while (len > 0) {
		tlv.parse(b);
		switch (tlv.getType()){
			// XXX: ignoring lots of stuff here
			case 0x0002: // Proxy IP
				tlv.data() >> dw;
				m_ftd.setProxyIP(dw);
				break;
			case 0x0003: // Client IP address
				tlv.data() >> dw;
				m_ftd.setClientIP(dw);
				break;
			case 0x0004: // Server's client IP (or external client IP)
				tlv.data() >> dw;
				m_ftd.setExternalIP(dw);
				break;
			case 0x0005: // External port
				tlv.data().readString(s);
				m_ftd.setListeningPort(s);
				break;
			case 0x000b:
				break;
			case 0x000e: // Locale (seems only to be in FT reqs)
				tlv.data().readString(s);
				m_ftd.setLocale(s);
				break;
			case 0x000d: // Encoding
				tlv.data().readString(s);
				m_ftd.setEncoding(s);
				break;
			case 0x000f:
				break;
			case 0x000a: // Request number for file transfer
				tlv.data() >> w;
				m_ftd.setReqNumber(w);
				break;
			case 0x000c: // FT User Message
				tlv.data().readString(s);
				m_ftd.setMessage(s);
				break;
			case 0x0010: // Proxy flag
				m_ftd.setProxyUsed(true);
				break;
			case 0x0016: // Proxy IP Check XXX:ignoring ATM
				break;
			case 0x0017: // Port Check XXX:ignoring ATM
				break;
			case 0x2711:
				if (m_type == TYPE_FILEREQ){
					parse2711file(tlv.data());
				}
				else{
					// real message :-)
					parse2711(tlv.data());
				}
				break;
			case 0x2712: // File name encoding
				tlv.data().readString(s);
				m_ftd.setFileEncoding(s);
				break;
			default:
				qDebug("Unknown TLV in channel 2 message: %d", tlv.getType());
				break;
		}
		len -= (tlv.len() + 4);
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

void Message::parse2711file(Buffer& b){
	
	Word w;
	DWord dw;
	QString s;

	m_ftd.setCookie(m_cookie); // copy the cookie, we'll need it

	b >> w;

	if (w == 0x0001)
		m_ftd.setMultiple(false);
	else
		m_ftd.setMultiple(true);

	b >> w;
	m_ftd.setFileCount(w);

	b >> dw;
	m_ftd.setFileSize(dw);

	b.readString(s);
	m_ftd.setFileName(s);

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
