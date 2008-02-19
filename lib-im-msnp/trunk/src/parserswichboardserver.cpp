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

	QByteArray data;
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

void ParserSB::feed (QByteArray b){
	m_hasCommand = true;
	m_buf.append(b);
}

void ParserSB::parseIro(){
	// IRO 1 1 1 XXXXXXX@gmail.com XXXXX 1073741860\r\n 
	// IRO trid rooster roostercount passport friendlyname
	QRegExp rx;
	rx.setPattern("(^IRO \\d+ (\\d+) (\\d+) (\\S+) (\\S+) (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		QString rooster = rx.cap(2);
		QString rosterCount = rx.cap(3);
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
	if (rx.indexIn(m_buf) != -1){
		// TODO: 
		m_buf.remove(0,rx.cap(1).size());
		MSG m;
		m.addType(MSG_IDENTIFICATION);
		m.addClientName(m_client->getClientName());
		m.addClientVersion(m_client->getClientVer());
		m.addClientIsLogging(m_client->getClientIsLogging());
		m_socket->send(m.makeCmd());
	}
	else m_hasCommand = false;
}

void ParserSB::parseMsg () {
	// MSG XXXXXXXX@gmail.com XXXXXX 590\r\nMIME-Version: 1.0\r\nContent-Type: application/x-msnmsgrp2p\r\nP2P-Dest: XXXXXXXX@hotmail.com\r\n\r\n......
	// MSG XXXXXXXX@hotmail.com XXXX 128\r\nMIME-Version: 1.0\r\nContent-Type: text/plain; charset=UTF-8\r\nX-MMS-IM-Format: FN=Helvetica; EF=; CO=000000; CS=0; PF=22\r\n\r\nholaaa
	QRegExp rx;
	QByteArray data;
	rx.setPattern("(^MSG (\\S+) (\\S+) (\\d+)\r\n)MIME-Version: 1.0\r\nContent-Type: (\\S+)[\r\n|;]"); 
	if (rx.indexIn(m_buf) != -1){
		QString header = rx.cap(1);
		QString senderPassport = rx.cap(2);
		QString msgType = rx.cap(5);
		QString payload = rx.cap(4);
		data.append(m_buf.mid(rx.cap(1).size(), payload.toInt()));
		if (data.size() == payload.toInt()) {
			m_buf.remove(0, header.size() + payload.toInt());

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
				// Zumbidos
				rx.setPattern("\r\n\r\nID: 1[\r\n]+$");
				if (rx.indexIn(data.data()) != -1){
					qDebug("MSN::ParserSB::Log #%i: MSG Zumbido", m_chatId);
				}
				// Guiños
				// Content-Type: text/x-msnmsgr-datacast\r\nMessage-ID: {879ED7A5-DF1C-BEAA-6D8B-65F6137734C5}\r\nChunks: 3\r\n\r\nID: 2\r\nData: <msnobj Creator="vaticano666@hotmail.com" Size="95742" Type="8" Location="TFR1B4.tmp" Friendly="QgByAGUAYQBrACAARABhAG4AYwBlAHIA" SHA1D="i++iSwaoqftUJZ3j+9d2jccqtmo=" SHA1C="oCshAefPuHtWCODyB/eX+103nOY=" stamp="MIIIngYJ.....
				// MSG vaticano666@hotmail.com Pedro 1266\r\nMessage-ID: {879ED7A5-DF1C-BEAA-6D8B-65F6137734C5}\r\nChunk: 1\r\n\r\n6CAcoATQBpAGMAcgBvAHMAbwBmA...
				// MSG vaticano666@hotmail.com Pedro 837\r\nMessage-ID: {879ED7A5-DF1C-BEAA-6D8B-65F6137734C5}\r\nChunk: 2\r\n\r\nzwj7QM7Rs/4p...
				rx.setPattern("^MIME-Version: 1.0\r\nContent-Type: text/x-msnmsgr-datacast\r\nMessage-ID: \\{(\\S+)\\}\r\nChunks: (\\d+)\r\n\r\nID: (\\d+)\r\nData: <msnobj Creator=\"(\\S+)\" Size=\"(\\d+)\" Type=\"(\\d+)\" Location=\"(\\S+)\" Friendly=\"(\\S+)\" SHA1D=\"(\\S+)\" SHA1C=\"(\\S+)\" stamp=\"(\\S+)$");
				if (rx.indexIn(data.data()) != -1){
					QString messageID	= rx.cap(1);
					QString chunks		= rx.cap(2);  
					QString id		= rx.cap(3); 
					QString creator		= rx.cap(4); 
					QString size		= rx.cap(5); 
					QString type		= rx.cap(6); 
					QString location	= rx.cap(7);    
					QString friendly	= rx.cap(8);    
					QString sha1D		= rx.cap(9);  
					QString sha1C		= rx.cap(10); 
					QString dataWink	= rx.cap(11); 	
					//QStringList list = rx.capturedTexts();
					//QStringList::iterator it = list.begin();
					//printf ("MSN::ParserSB::Log #%i: MSG Guiño > ", m_chatId);
 					//while (++it != list.end()) {
 					//    printf("%s,",(*it).toUtf8().replace("\r\n","\\r\\n").data());
 					//}
					printf ("MSN::ParserSB::Log #%i: MSG Guiño %s,%s\n",m_chatId, messageID.toUtf8().data(), dataWink.toUtf8().data() );
				}
			}

			if (msgType == QString("application/x-msnmsgrp2p")){
				rx.setPattern("\r\nP2P-Dest: (\\S+)\r\n\r\n");
				if (rx.indexIn(data.data()) != -1){
					printf("MSN::ParserSB::Log #%i: MSG archivo datos:", m_chatId);
					data.toHex();
					printf("\n");
					QByteArray binaryHeader = data.mid(data.indexOf("\r\n\r\n")+4, 48);

					//http://msnpiki.msnfanatic.com/index.php/MSNC:Binary_Headers
					// Little-endian abcd = dbca in memory
					// \r\n\r\n
					// SessionID	Identifier	Data offset 		Total data size		Message length	Flag          	identifier	unique ID      	data size
					// 00 00 00 00 	51 67 07 00	00 00 00 00 00 00 00 00 2a 02 00 00 00 00 00 00 2a 02 00 00 	00 00 00 00   	dd 30 48 00  	00 00 00 00    	00 00 00 00 00 00 00 00
					QByteArray sessionID	 = binaryHeader.mid(0,4);	
					QByteArray identifier	 = binaryHeader.mid(4,4);	
					QByteArray dataOffset	 = binaryHeader.mid(8,8);	
					QByteArray totalDataSize = binaryHeader.mid(16,8);	
					QByteArray flag		 = binaryHeader.mid(20,4);	
					QByteArray messageLength = binaryHeader.mid(24,4);	
					QByteArray ackUniqueID	 = binaryHeader.mid(28,4);	
					QByteArray ackIdentifier = binaryHeader.mid(32,4);	
					QByteArray ackDataSize	 = binaryHeader.mid(36,8);	
					//qDebug() << "####### BINARY" << binaryHeader.toHex();
					//qDebug () << "######## sid"<< sessionID.toHex() << "Id" << identifier.toHex() << "dataOffset" << dataOffset.toHex() << "totalDataSize" << totalDataSize.toHex() << "messageLength" << messageLength.toHex() << "flag" << flag.toHex() << "ackIdentifier" << ackIdentifier.toHex() << "ackUniqueID" << ackUniqueID.toHex() << "ackDataSize" << ackDataSize.toHex();

					QByteArray invitation = data.mid(data.indexOf("\r\n\r\n")+52);
					//INVITE MSNMSGR:probando_msnpy@hotmail.com MSNSLP/1.0\r\nTo: <msnmsgr:probando_msnpy@hotmail.com>\r\nFrom: <msnmsgr:vaticano666@hotmail.com>\r\nVia: MSNSLP/1.0/TLP ;branch={39844FA1-2352-F681-92ED-5180CBD34F21}\r\nCSeq: 0\r\nCall-ID: {D4F03DF4-E61A-5A2E-2588-ACEE1BA9706A}\r\nMax-Forwards: 0\r\nContent-Type: application/x-msnmsgr-sessionreqbody\r\nContent-Length: 199\r\n\r\nEUF-GUID: {4BD96FC0-AB17-4425-A14A-439185962DC8}\r\nSessionID: 94449907\r\nAppID: 4\r\nContext: ewBCADgAQgBFADcAMABEAEUALQBFADIAQwBBAC0ANAA0ADAAMAAtAEEARQAwADMALQA4ADgARgBGADgANQBCADkARgA0AEUAOAB9AA==\r\n\r\n 00 00 00 00 00
					QRegExp fx;
					QByteArray a;
					fx.setPattern("(^(\\S+) MSNMSGR:\\S+ MSNSLP/1.0\r\nTo: <msnmsgr:(\\S+)>\r\nFrom: <msnmsgr:(\\S+)>\r\nVia: MSNSLP/1.0/TLP ;branch=\\{(\\S+)\\}\r\nCSeq: (\\d+)\r\nCall-ID: \\{(\\S+)\\}\r\nMax-Forwards: (\\d+)\r\nContent-Type: (\\S+)\r\nContent-Length: (\\d+)\r\n\r\nEUF-GUID: \\{(\\S+)\\}\r\nSessionID: (\\d+)\r\nAppID: (\\d+)\r\nContext: (\\S+))");
					if (fx.indexIn(invitation) != -1){
						QString EUF_GUID = fx.cap(11);
						QString p2pSessionId = fx.cap(12);
						
						qDebug() << "$$$$ " << EUF_GUID << " " << p2pSessionId;

						//res = SessionID + random + Data offset+ Total data size +  00 00 00 00  + 02 00 00 00 + Identifier + identifier + Total data size
						QByteArray response = sessionID + QByteArray::fromHex("3B 99 4F 02") + dataOffset + totalDataSize + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("02 00 00 00") + identifier + ackIdentifier + totalDataSize;
						MSG p2p;
						p2p.addType(MSG_P2P);
						p2p.addDestPassport(fx.cap(4));
						p2p.addP2PData(response);
						//p2p.makeCmd().toChars();
						//p2p.makeCmd().toHex();
						m_socket->send(p2p.makeCmd());
						// ahora envio esto:
						//MSG 241 D 152
						//MIME-Version: 1.0
						//Content-Type: application/x-msnmsgrp2p
						//P2P-Dest: darth_leviathan@hotmail.com
						//................=................x6b.I.c=...........
						//
					}
				}
			}
		}

	}
	else {
		rx.setPattern("(^MSG (\\S+) (\\S+) (\\d+)\r\n)"); 
		if (rx.indexIn(m_buf) != -1){
			QString header = rx.cap(1);
			QString senderPassport = rx.cap(2);
			QString payload = rx.cap(4);
			qDebug ("%s, %s, %s",header.toUtf8().data(),senderPassport.toUtf8().data(),payload.toUtf8().data());
			data.clear();
			data.append(m_buf.mid(header.size(), payload.toInt()));
			// MSG vaticano666@hotmail.com Pedro 837\r\nMessage-ID: {879ED7A5-DF1C-BEAA-6D8B-65F6137734C5}\r\nChunk: 2\r\n\r\nzwj7QM7Rs/4...
			if (data.size() == payload.toInt()) {
				m_buf.remove(0, header.size() + payload.toInt());
				rx.setPattern("^Message-ID: \\{(\\S+)\\}\r\nChunk: (\\d+)\r\n\r\n(\\S+)");
				if (rx.indexIn(data.data()) != -1){
					QString messageID2	= rx.cap(1);
					QString chunks2		= rx.cap(2);  
					QString dataWink2	= rx.cap(3); 
					printf ("MSN::ParserSB::Log #%i: MSG Guiño %s,%s\n",m_chatId, messageID2.toUtf8().data(), dataWink2.toUtf8().data() );
				}
			}
		}
		else
			m_hasCommand = false;
	}
}

void ParserSB::parseUsr(){
	// << USR 827 OK XXXXXXXXX@hotmail.com XXXX\r\n
	// >> CAL 828 XXXXXXX@gmail.com\r\n
	QRegExp rx;
	rx.setPattern("(^USR \\d+ OK \\S+ \\S+\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
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
	if (rx.indexIn(m_buf) != -1){
		m_buf.remove(0,rx.cap(1).size());
	}
	else m_hasCommand = false;
}

void ParserSB::parseJoi(){
	// << JOI XXXXXXXXXXXX@gmail.com XXXXXXXX 1073741860.73741860.912301
	QRegExp rx;
	rx.setPattern("(^JOI (\\S+) (\\S+) (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		QString email = rx.cap(2);
		QString fName = rx.cap(3);
		QString capabilities = rx.cap(4);
		m_buf.remove(0,rx.cap(1).size());
		MSG m;
		m.addType(MSG_IDENTIFICATION);
		m.addClientName(m_client->getClientName());
		m.addClientVersion(m_client->getClientVer());
		m.addClientIsLogging(m_client->getClientIsLogging());
		m_socket->send(m.makeCmd());
		//qDebug("MSN::ParserSB::Log #%i::NEW CHAT with %s fname:%s capab:%s", m_chatId,email.toUtf8().data(), fName.toUtf8().data(), capabilities.toUtf8().data());
		// TODO: remove three lines below
	}
	else m_hasCommand = false;
}

void ParserSB::parseAck(){
	// ACK 3\r\n
	QRegExp rx;
	rx.setPattern("(^ACK (\\d+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		m_buf.remove(0,rx.cap(1).size());
	}
	else m_hasCommand = false;
}


void ParserSB::parseBye () {
	// BYE XXXXXXXXX@hotmail.com 1\r\n 
	QRegExp rx;
	rx.setPattern("(^BYE (\\S+)[ ]?(\\d*)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
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
	if (rx.indexIn(m_buf) != -1){
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
		qDebug("MSN::ParserSB::Log #%i::BUFFER <%s>", m_chatId,QByteArray(m_buf).replace("\\r\\n", "\\\\r\\\\n").replace("\r\n", "\\r\\n").data());
		cmd = m_buf.mid(0,3).data();
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
