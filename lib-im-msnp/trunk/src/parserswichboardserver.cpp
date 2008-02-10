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
#include <QRegExp>
#include "parserswichboardserver.h"
#include "cmdswichboardserver.h"

namespace libimmsnp {
ParserSB::ParserSB (QString address, int port, int chatId, QString msnPassport, QString ticket, QString sessid, Client* c){
	m_idtr = 1;
	m_address = address;
	m_port = port;
	m_socket = 0;
	m_client = c;
	m_chatId = chatId;
	m_msnPassport = msnPassport;
	m_ticket = ticket;
	m_sessid = sessid;
	m_hasCommand = false;
	m_isParsing = false;
	m_endChat = false;
}
void ParserSB::run(){
	m_socket = new msocket ();
	m_socket->connect(m_address, m_port);

	if (m_sessid != ""){ // they start the chat
		ANS ans(m_idtr++);
		ans.addPassport (m_msnPassport);
		ans.addTicket (m_ticket);
		ans.addSessId (m_sessid);
		m_socket->send(ans.makeCmd());
	}

	else {	// I start the chat
		USRchat usr (m_idtr++);
		usr.addPassport (m_msnPassport);
		usr.addTicket (m_ticket);
		m_socket->send(usr.makeCmd());
	}

	Buffer data;
	//qDebug("MSN::ParserSB:: Chat %i START",m_chatId);
	while (!m_endChat && ((m_socket->recv(data)) != -1)){
		feed (data);
		data.clear();
		if (!m_isParsing){parse();}
	}
	//qDebug("MSN::ParserSB:: END Connecton with buddy: %s at:%s",m_buddy.toUtf8().data(), m_socket->getHost().toUtf8().data());
	delete m_socket;
	m_socket = 0;
}

void ParserSB::send (Command& c){
	m_socket->send(c.makeCmd());
}

void ParserSB::feed (Buffer b){
	m_hasCommand = true;
	m_buf << b;
}

void ParserSB::parseIro(){
	// IRO 1 1 1 XXXXXXX@gmail.com XXXXX 1073741860\r\n 
	// IRO trid rooster roostercount passport friendlyname
	QRegExp rx;
	rx.setPattern("(^IRO \\d+ (\\d+) (\\d+) (\\S+) (\\S+) (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		int rooster = rx.cap(2).toInt();
		int rosterCount = rx.cap(3).toInt();
		m_buddy = rx.cap(4);
		QString fName = rx.cap(5);

		m_buf.remove(0,rx.cap(1).size());
	}
	else m_hasCommand = false;
}
void ParserSB::parseAns(){
	//ANS trid "OK"\r\n
	QRegExp rx;
	rx.setPattern("(^ANS (\\d+) OK\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		// TODO: 
		m_buf.remove(0,rx.cap(1).size());
	}
	else m_hasCommand = false;
}

void ParserSB::parseMsg () {
	// MSG XXXXXXXX@gmail.com XXXXXX 590\r\nMIME-Version: 1.0\r\nContent-Type: application/x-msnmsgrp2p\r\nP2P-Dest: XXXXXXXX@hotmail.com\r\n\r\n......
	// MSG XXXXXXXX@hotmail.com XXXX 128\r\nMIME-Version: 1.0\r\nContent-Type: text/plain; charset=UTF-8\r\nX-MMS-IM-Format: FN=Helvetica; EF=; CO=000000; CS=0; PF=22\r\n\r\nholaaa
	QRegExp rx;
	rx.setPattern("(^MSG (\\S+) (\\S+) (\\d+)\r\n)MIME-Version: 1.0\r\nContent-Type: (\\S+)[\r\n|;]"); 
	if (rx.indexIn(m_buf.data()) != -1){
		QString senderPassport = rx.cap(2);
		QString msgType = rx.cap(5);
		QString payload = rx.cap(4);
		Buffer data;
		data << m_buf.mid(rx.cap(1).size(), payload.toInt());
		m_buf.remove(0, rx.cap(1).size() + payload.toInt());

		if (msgType == QString("text/x-clientcaps")){
			rx.setPattern("\r\n\r\nClient-Name: ([\\S|\\s]*)\r\nChat-Logging: (\\S)[\r\n]+$");
			if (rx.indexIn(data.data()) != -1){
				//qDebug("MSG chat info client:%s logging:%s",rx.cap(1).toUtf8().data(), rx.cap(2).toUtf8().data());
				emit chatInfo (m_chatId, rx.cap(1), rx.cap(2));
				return;
			}
		}

		if (msgType == QString("text/x-msmsgscontrol")){
			rx.setPattern("\r\nTypingUser: (\\S+)[\r\n]+$");
			if (rx.indexIn(data.data()) != -1){
				//qDebug("MSG Typing user:%s ",rx.cap(1).toUtf8().data());
				emit chatIsTyping ( m_chatId,senderPassport);
				return;
			}
		}


		if (msgType == QString("text/plain")){
			QString font; 
			QString effect;
			QString color;
			QString charSet;
			QString fontType; 
			QString codif;
			MSG msg ;
			qRegisterMetaType<MSG>("MSG");

			rx.setPattern("charset=(\\S+)");
			if (rx.indexIn(data.data()) != -1){
				codif = rx.cap(1);
			}

			// X-MMS-IM-Format: FN=Arial; EF=BI; CO=0
			rx.setPattern("\r\nX-MMS-IM-Format: FN=(\\S+); EF=(\\S*); CO=(\\S*); CS=(\\S*); PF=(\\d*)");
			if (rx.indexIn(data.data()) != -1){
				font = rx.cap(1);
				effect = rx.cap(2);
				color = rx.cap(3);
				charSet = rx.cap(4);
				fontType = rx.cap(5);
			}
			rx.setPattern("\r\nX-MMS-IM-Format: FN=(\\S+); EF=(\\S*); CO=(\\S*)");
			if (rx.indexIn(data.data()) != -1){
				font = rx.cap(1);
				effect = rx.cap(2);
				color = rx.cap(3);
			}
			msg.addColor(color);
			msg.addFont(font);
			if (effect.contains('B')) msg.addEffect(EFFECT_BOLD);
			if (effect.contains('I')) msg.addEffect(EFFECT_ITALIC);
			if (effect.contains('U')) msg.addEffect(EFFECT_UNDERLINE);

			msg.addMsg( data.mid (data.indexOf("\r\n\r\n")+4));

			//qDebug("MSG Codificacion:%s Fuente:%s Efectos:%s Color:%s Character sets:%s Font Type:%s msg:%s",codif.toUtf8().data(), font.toUtf8().data(), effect.toUtf8().data(), color.toUtf8().data(), charSet.toUtf8().data(), fontType.toUtf8().data() ,msg.dataDebug());
			emit chatArrivedMessage(m_chatId, senderPassport, msg);
			return;

		}

		if (msgType == QString("text/x-msnmsgr-datacast")){
			rx.setPattern("\r\n\r\nID: 1[\r\n]+$");
			if (rx.indexIn(data.data()) != -1){
				qDebug("MSN::ParserSB::Log #%i: MSG Zumbido", m_chatId);
			}
		}

		if (msgType == QString("application/x-msnmsgrp2p")){
			rx.setPattern("\r\nP2P-Dest: (\\S+)\r\n\r\n");
			if (rx.indexIn(data.data()) != -1){
				qDebug("MSN::ParserSB::Log #%i: MSG archivo datos:%s", m_chatId, data.dataDebug());
			}
		}

	}
	else m_hasCommand = false;
}

void ParserSB::parseUsr(){
	// << USR 827 OK XXXXXXXXX@hotmail.com XXXX\r\n
	// >> CAL 828 XXXXXXX@gmail.com\r\n
	QRegExp rx;
	rx.setPattern("(^USR \\d+ OK \\S+ \\S+\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		m_buf.remove(0,rx.cap(1).size());
		CAL cal(m_idtr++);
		cal.addPassport (m_client->nextChatPassport());
		m_socket->send(cal.makeCmd());
	}
	else m_hasCommand = false;
}

void ParserSB::parseCal(){
	// << CAL 828 RINGING 482462925\r\n
	// >> CAL 32 XXXXXXXX@gmail.com\r\n
	QRegExp rx;
	rx.setPattern("(^CAL \\d+ RINGING (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		m_buf.remove(0,rx.cap(1).size());
	}
	else m_hasCommand = false;
}

void ParserSB::parseJoi(){
	// << JOI XXXXXXXXXXXX@gmail.com XXXXXXXX 1073741860.73741860.912301
	QRegExp rx;
	rx.setPattern("(^JOI (\\S+) (\\S+) (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		QString email = rx.cap(2);
		QString fName = rx.cap(3);
		QString capabilities = rx.cap(4);
		m_buf.remove(0,rx.cap(1).size());
		//qDebug("MSN::ParserSB::Log #%i::NEW CHAT with %s fname:%s capab:%s", m_chatId,email.toUtf8().data(), fName.toUtf8().data(), capabilities.toUtf8().data());
		// TODO: remove three lines below
	}
	else m_hasCommand = false;
}

void ParserSB::parseAck(){
	// ACK 3\r\n
	QRegExp rx;
	rx.setPattern("(^ACK (\\d+)\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		m_buf.remove(0,rx.cap(1).size());
	}
	else m_hasCommand = false;
}


void ParserSB::parseBye () {
	// BYE XXXXXXXXX@hotmail.com 1\r\n 
	QRegExp rx;
	rx.setPattern("(^BYE (\\S+)[ ]?(\\d*)\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		m_buf.remove(0, rx.cap(1).size());
		m_endChat = true;
		emit chatLeavedTheRoom (m_chatId,rx.cap(2));
		m_socket->close();
	}
	else m_hasCommand = false;
}

void ParserSB::parseError () {
	// 215 1\r\n
	QRegExp rx;
	rx.setPattern("(^(\\d{3}) \\d+\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		qDebug ("MSN::ParserSB::Log #%i ## ERROR : %s", m_chatId, rx.cap(2).toUtf8().data());
		if (rx.cap(2) == "215") {
			m_socket->close();	
			m_endChat = true;
		}
		m_buf.remove(0,rx.cap(1).size());
	}
	else m_hasCommand = false;
}

void ParserSB::parse (){
	m_isParsing = true;
	QString cmd;
	while (m_buf.size() && m_hasCommand){
		qDebug("MSN::ParserSB::Log #%i::BUFFER <%s>", m_chatId,m_buf.dataDebug());
		cmd = m_buf.getCmd();
		if (cmd == "IRO"){
			qDebug ("MSN::ParserSB::Log #%i : Parsing IRO", m_chatId);
			parseIro();
		}

		else if (cmd == "ANS"){
			qDebug ("MSN::ParserSB::Log #%i : Parsing ANS", m_chatId);
			parseAns();
		}

		else if (cmd == "MSG"){
			qDebug ("MSN::ParserSB::Log #%i : Parsing MSG", m_chatId);
			parseMsg();

		}

		else if (cmd == "ACK"){
			qDebug ("MSN::ParserSB::Log #%i : Parsing ACK", m_chatId);
			parseAck();

		}

		else if (cmd == "USR"){
			qDebug ("MSN::ParserSB::Log #%i : Parsing USR", m_chatId);
			parseUsr();

		}

		else if (cmd == "CAL"){
			qDebug ("MSN::ParserSB::Log #%i : Parsing CAL", m_chatId);
			parseCal();

		}

		else if (cmd == "JOI"){
			qDebug ("MSN::ParserSB::Log #%i : Parsing JOI", m_chatId);
			parseJoi();

		}
		
		else if (cmd == "BYE"){
			qDebug ("MSN::ParserSB::Log #%i : Parsing bye", m_chatId);
			parseBye();
		}

		else if (cmd.contains(QRegExp::QRegExp("^\\d{3}"))){
			parseError();
		}
		else break;			
	}	
	m_isParsing = false;
}
}
#include "parserswichboardserver.moc"
