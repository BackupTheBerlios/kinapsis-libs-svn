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
#include "parsernotificationserver.h"
#include "cmdlogin.h"
#include "cmdnotificationserver.h"
#include "challenge.h"
#include "soap.h"
#include "parserswichboardserver.h"

namespace libimmsnp {
ParserNS::ParserNS(QString msnPassport, QString msnPass, State initialStatus, Client* c, msocket* s, QString protocol){
	m_idtr = 1;
	m_msnPassport = msnPassport;
	m_msnPass = msnPass;
	m_socket = s;
	m_initialStatus = initialStatus;
	m_client = c;
	m_hasCommand = false;
	m_isParsing = false;
	m_protocol = protocol;
}
void ParserNS::run(){

	VER v(m_client->nextIdtr());
	v.addProtocolSupported(m_protocol);
	m_client->send(v);
	Buffer data;
	qDebug("MSN::ParserNS:: Connection START");
	while ((m_socket->recv(data)) != -1){
		feed (data);
		data.clear();
		if (!m_isParsing){parse();}
	}
	qDebug("MSN::ParserNS:: END Connecton with %s",m_socket->getHost().toUtf8().data());

}
void ParserNS::feed (Buffer b){
	m_hasCommand = true;
	m_buf << b;
}

void ParserNS::parseVer () {
	// VER 1 MSNP12 MSNP11 MSNP10 CVR0\r\n	
	QRegExp rx;
	rx.setPattern("(^VER \\d+ " + m_protocol + " CVR0\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		QString s = rx.cap(1);
		m_buf.remove(0,s.size());
		CVR c(m_client->nextIdtr());
		c.addLocaleId ("0x0409");
        	c.addOs ("winnt", "5.1");
        	c.addArch ("i386");
        	c.addClient ("MSNMSGR", "8.1.0178");
        	c.addPassport (m_msnPassport);

		m_client->send(c);
	}
	m_hasCommand = false;
}

void ParserNS::parseCvr () {
	// CVR 2 8.1.0178 8.1.0178 8.0.0787 http://msgr.dlservice.microsoft.com/download/1/A/4/1A4FEB1A-18E0-423A-B898-F697402E4F7F/Install_Messenger.exe http://get.live.com\r\n
	QRegExp rx;
	rx.setPattern("(^CVR \\d+ [\\d|\\D]*\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		QString s = rx.cap(1);
		m_buf.remove(0,s.size());

		USR u(m_client->nextIdtr());
		u.addTwnType ("I");
		u.addPassport (m_msnPassport);
		m_client->send (u);
	}
	m_hasCommand = false;
}

void ParserNS::parseXfr () {
	//  XFR 3 NS 207.46.108.73:1863 U D\r\n
	//  XFR 3 SB 207.46.27.170:1863 CKI 1568090226.21815116.102267\r\n        for chatting
	QRegExp rx;
	rx.setPattern("(^XFR \\d+ ([N]?S[B]?) (\\S+):(\\d{4}) (\\S+) (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		QString type = rx.cap(2);
		QString address = rx.cap(3);
		int port = rx.cap(4).toInt();
		QString ticket = rx.cap(6);

		m_buf.remove(0,rx.cap(1).size());

		if (type == "NS"){
			m_socket->connect(address, port);
			VER v(m_client->nextIdtr());
			v.addProtocolSupported(m_protocol);
			m_client->send(v);
		}

		if (type == "SB"){
			qDebug("Connecting at: %s:%i ticket:%s",address.toUtf8().data(), port, ticket.toUtf8().data());
			ParserSB* chatParser = new ParserSB (address, port, m_client->nextChatCount(), m_msnPassport, ticket, "", m_client);
			chatParser->start();
			// //emit newChat ...
		}
	}
	m_hasCommand = false;

}
void ParserNS::parseGcf () {
	//GCF 0 5187\r\n<Policies><Policy... USR 6 TWN S lc=1033,id=507,tw=40,ru=http%3A%2F%2Fmessenger%2Emsn%2Ecom,ct=1179166797,kpp=1,kv=9,ver=2.1.6000.1,rn=fJ0uZaXS,tpf=a0bf38b5ffd1253d4c57930ef5eb343f\r\n 
	QString payloadLen = 0;
	QRegExp rx;
	rx.setPattern("(^GCF [\\d|\\D]*USR \\d+ TWN S ([\\d|\\D]*)\r\n)"); 
//	rx.setPattern("(^GCF [\\d|\\D]*USR \\d+ SSO S MBI_KEY_OLD ([\\d|\\D]*)\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		m_ticket = rx.cap(2);
		QString s = rx.cap(1);
		m_buf.remove(0,s.size());
		Challenge challenge(m_msnPassport, m_msnPass, m_ticket);
		m_ticket = challenge.getTicket();

		if (m_ticket == ""){
			//emit disconnected (ConnAuthenticationFailed);
		}

		USR u(m_client->nextIdtr());
		u.addTwnType ("S");
		u.addTicket (m_ticket);
		m_client->send (u);
	}
	m_hasCommand = false;
}
void ParserNS::parseUsr () {
	// USR 7 OK probando_msnpy@hotmail.com 1 0 
	QRegExp rx;
	rx.setPattern("(^USR \\d+ TWN S ([\\d|\\D]*)\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		m_ticket = rx.cap(2);
		QString s = rx.cap(1);
		m_buf.remove(0,s.size());
		Challenge challenge(m_msnPassport, m_msnPass, m_ticket);
		m_ticket = challenge.getTicket();
		USR u(m_client->nextIdtr());
		u.addTwnType ("S");
		u.addTicket (m_ticket);
		m_client->send (u);
	}
	else {
		rx.setPattern("(^USR \\d+ OK [\\d|\\D]* 1 0\r\nSBS 0 null\r\n)"); 
		if (rx.indexIn(m_buf.data()) != -1){
			QString s = rx.cap(1);
			m_buf.remove(0,s.size());
		}
	}

	m_hasCommand = false;
}

void ParserNS::parseMsg () {
	// MSG Hotmail Hotmail 544\r\nMIME-Version: 1.0\r\nCo ... \r\n\r\n
	QRegExp rx;
	rx.setPattern("(^MSG Hotmail Hotmail (\\d+)\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		QString payload = rx.cap(2).toUtf8().data();
		rx.setPattern("(^MSG Hotmail Hotmail \\d+\r\n([\\d|\\D]{" + payload + "}))");
		if (rx.indexIn(m_buf.data()) != -1){
			// TODO: check diferent types of MSG
			QString msg = rx.cap(2);
			m_buf.remove(0,rx.cap(1).size());
			rx.setPattern("^MIME-Version: 1.0\r\nContent-Type: text/x-msmsgsprofile");
			if (rx.indexIn(msg) != -1){
				SYN s(m_client->nextIdtr());
				m_client->send (s);
			}
			// <MD><E><I>16</I><IU>6</IU><O>0</O><OU>0</OU></E>
			rx.setPattern("^MIME-Version: 1.0\r\nContent-Type: text/x-msmsgsinitialmdatanotification; charset=UTF-8\r\n\r\nMail-Data: <MD><E><I>(\\d+)</I><IU>(\\d+)</IU><O>(\\d+)</O><OU>(\\d+)</OU></E><Q><QTM>409600</QTM><QNM>204800</QNM></Q></MD>\r\nInbox-URL: /cgi-bin/HoTMaiL\r\nFolders-URL: /cgi-bin/folders\r\nPost-URL: http://www.hotmail.com\r\n");
			if (rx.indexIn(msg) != -1){
				int inbox = rx.cap(1).toInt();
				int inboxNotRead = rx.cap(2).toInt();
				int spam = rx.cap(3).toInt();
				int spamNotRead = rx.cap(4).toInt();
				qDebug("MSN::Log::ParserNS ## New Mail inbox not Readed :%i of %i; spam not Readed :%i of %i", inboxNotRead, inbox, spamNotRead, spam);
			}
		}
		else m_hasCommand = false;

	}
	else m_hasCommand = false;
}

void ParserNS::parseSyn () {
	// SYN 8 2006-12-09T11:15:07.163-08:00 2006-06-10T20:12:54.183-07:00 1 6\r\n
	QRegExp rx;
	rx.setPattern("(^SYN \\d+ [\\d|\\D]* (\\d+) (\\d+)\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		m_contacts = rx.cap(2).toInt();
		m_groups = rx.cap(3).toInt();
		m_buf.remove(0,rx.cap(1).size());
	}
	else m_hasCommand = false;
}

void ParserNS::parseGtc () {
	// GTC A\r\nBLP AL\r\n 
	QRegExp rx;
	rx.setPattern("(^GTC (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		m_buf.remove(0,rx.cap(1).size());
	}
	else m_hasCommand = false;
}

void ParserNS::parseBlp () {
	// BLP AL\r\n 
	QRegExp rx;
	rx.setPattern("(^BLP (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		m_buf.remove(0,rx.cap(1).size());
	}
	else m_hasCommand = false;
}

void ParserNS::parseBpr () {
	// BPR HSB 1\r\n  
	// BPR PHM tel:34%20610703044 0\r\n
	QRegExp rx;
	//rx.setPattern("(^BPR ([\\S]{3}))"); 
	rx.setPattern("(^BPR HSB 1\r\n)");
	if (rx.indexIn(m_buf.data()) != -1){
		m_buf.remove(0,rx.cap(1).size());
		////emit hasBlog (m_prevContact);
		return;
	}
	else {
		       //BPR PHM tel:34%20610703044 0
		rx.setPattern("(^BPR PHM (\\S+) 0\r\n)");
		if (rx.indexIn(m_buf.data()) != -1){
			m_buf.remove(0,rx.cap(1).size());
			return;
		}
		else {
			m_hasCommand = false;
			return;
		}
	}

}

void ParserNS::parsePrp () {
	// 
	QRegExp rx;
	rx.setPattern("(^PRP (\\S+) (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		if (rx.cap(2) == "MFN") {
			// TODO: //emit signal sending my name
			qDebug ("MSN::Log::ParserNS ## MFN: %s", rx.cap(3).toUtf8().data());
		}
		m_buf.remove(0,rx.cap(1).size());
	}
	else m_hasCommand = false;
}

void ParserNS::parseLsg () {
	// 
	QRegExp rx;
	rx.setPattern("(^LSG (\\S+) (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		QString nombre = rx.cap(2);
		QString id = rx.cap(3);
		//qDebug ("MSN::Log::ParserNS ## New Group=%s id=%s",nombre.toUtf8().data(),id.toUtf8().data());
		Group* g = new Group(nombre, id);
		//emit newGroupArrived(g);
		m_buf.remove(0,rx.cap(1).size());
	}
}

void ParserNS::parseLst () {
	// LST N=aaaaaaaaa@hotmail.com F=nick:%20aaÃ³ C=07ad2594-ef7f-4a1e-96e1-0158611521ed 11 1 5faec9a4-b368-4def-8f11-6be610b665d0\r\n
	// LST N=aaaaaaaaa@hotmail.com F=nick:%20aaÃ³ C=07ad2594-ef7f-4a1e-96e1-0158611521ed 11 1\r\n
	// LST N=aaaaaaaaa@hotmail.com 11 1\r\n
	
	QRegExp rx;
	rx.setPattern("(^LST N=(\\S+)[ F=]?([\\S]*)[ C=]?([\\S]*) (\\d+) (\\d)([ ]?[\\S]*)+\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		QString mail = rx.cap(2);
		QString nick = rx.cap(3);
		QString id   = rx.cap(4);
		QString num1 = rx.cap(5); 
		QString num2 = rx.cap(6); 
		QString groupId = rx.cap(7).split(" ")[0];
		//qDebug ("MSN::Log::ParserNS ## New Contact=%s nick=%s", mail.toUtf8().data(), nick.toUtf8().data());
		m_buf.remove(0,rx.cap(1).size());
		Contact* c = new Contact(mail,"" , nick, id, groupId);
		//emit newContactArrived (c); 
		if (--m_contacts == 0) {
			CHG c (m_client->nextIdtr());
			c.addStatusCode (m_initialStatus);
			c.addCapabilities ("1342558252");
			//qDebug ("MSN::Log::ParserNS ## Roster received#");
			m_client->send (c);
			emit connected();
			//qDebug ("MSN::Log::ParserNS ## CONNECTED");
		}
		return;
	}

	else m_hasCommand = false;
}

void ParserNS::parseChg () {
	// CHG 9 NLN 1342558252\r\n
	QRegExp rx;
	rx.setPattern("(^CHG \\d+ (\\S+) (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		QString estado = rx.cap(2);
		QString capabilities = rx.cap(3);
		//qDebug ("MSN::Log::ParserNS ## State Changed=%s capabilities=%s",estado.toUtf8().data(), capabilities.toUtf8().data());
		m_buf.remove(0,rx.cap(1).size());
	}
	else m_hasCommand = false;
}

void ParserNS::parseChl (){
	// <<< CHL 0 22940274951814684551\r\n
	// >>> QRY 11 PROD0090YUAUV{2B 32\r\nffeb4c3cf93db6a4b708b246baade0d9(no newline)
	QRegExp rx;
	rx.setPattern("(^CHL 0 (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		char strChallengeOutTmp[32];
		Challenge c("","","");
		c.makeHash (rx.cap(2).toUtf8().data(), strChallengeOutTmp);
		QString strChallengeOut(strChallengeOutTmp);
		m_buf.remove(0,rx.cap(1).size());

		QRY q (m_client->nextIdtr());
		q.addProductId("PROD0090YUAUV{2B");
		q.addMd5(strChallengeOut);
                m_client->send (q);
	}
	else m_hasCommand = false;
}

void ParserNS::parseQry () {
	// 
	QRegExp rx;
	rx.setPattern("(^QRY \\d+\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		m_buf.remove(0,rx.cap(1).size());
		//TODO:remove this line
		//m_client->initChat("probando_msnpy@hotmail.com");
	}
	else m_hasCommand = false;
}

void ParserNS::parseNln () {
	// ILN 9 BSY XXXXXXXX@hotmail.com Diecoballa,%20Watashi%20wa%20supeinjin%20desu,%20watashi%20no%20namae%20wa%20Akimoto%20Makoto%20desu 1616760868 %3Cmsnobj%20Creator%3D%22darkerviti%40hotmail.com%22%20Size%3D%2229089%22%20Type%3D%223%22%20Location%3D%22TFR11A.dat%22%20Friendly%3D%22AAA%3D%22%20SHA1D%3D%22Ra4vcWsSOszskaurp71jzB0RSUk%3D%22%20SHA1C%3D%22zMQH9sLLaWshs9B8k27FglzFJ%2Fk%3D%22%2F%3E\r\n
	// ILN 9 NLN XXXXXXXXX@hotmail.com pedro 1342558252\r\n
	// NLN NLN XXXXXXXXXXX@hotmail.com pedro 1342558252 0\r\n
	QRegExp rx;
	rx.setPattern("([I|N]LN[ ]?[\\d+]? ([\\S+]{3}) (\\S+) (\\S+) (\\S+)[ ]?([\\d]+)[ ]?([\\S]*)\r\n)");

	if (rx.indexIn(m_buf.data()) != -1){
		QString state = rx.cap(2);
		QString passport = rx.cap(3);
		QString nick = rx.cap(4);
		QString capabilities = rx.cap(5);
		//qRegisterMetaType<State>("State");
		//if (state == "NLN") //emit statusChanged (passport, STATUS_NLN, nick, capabilities);
		//else if (state == "BSY") //emit statusChanged (passport, STATUS_BSY, nick, capabilities);
		//else if (state == "IDL") //emit statusChanged (passport, STATUS_IDL, nick, capabilities);
		//else if (state == "BRB") //emit statusChanged (passport, STATUS_BRB, nick, capabilities);
		//else if (state == "AWY") //emit statusChanged (passport, STATUS_AWY, nick, capabilities);
		//else if (state == "PHN") //emit statusChanged (passport, STATUS_PHN, nick, capabilities);
		//else if (state == "LUN") //emit statusChanged (passport, STATUS_LUN, nick, capabilities);
		//qDebug ("MSN::Log::ParserNS ## Status Changed:%s mail:%s nick:%s capabi:%s number:%s obj:%s#",state.toUtf8().data(), passport.toUtf8().data(), nick.toUtf8().data(), capabilities.toUtf8().data());
		m_buf.remove(0,rx.cap(1).size());
	}
	else m_hasCommand = false;
}

void ParserNS::parseFln () {
	// FLN XXXXXXXXXX@hotmail.com\r\n 
	QRegExp rx;
	rx.setPattern("(^FLN (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		QString passport = rx.cap(2);
		//qDebug ("MSN::Log::ParserNS ## Contact %s Disconnected",rx.cap(2).toUtf8().data());
		//emit statusChanged (passport,STATUS_OFF ,"","");
		m_buf.remove(0,rx.cap(1).size());
	}
	else m_hasCommand = false;
}

void ParserNS::parseUbx () {
	// UBX passport@hotmail.com xxx\r\n<Data><PSM>My Personal Message</PSM><CurrentMedia></CurrentMedia></Data>
	QRegExp rx;
	rx.setPattern("(^UBX \\S+ (\\d+)\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		QString payload = rx.cap(2).toUtf8().data();

		rx.setPattern("(^UBX (\\S+) \\d+\r\n([\\d|\\D]{" + payload + "}))");
		if (rx.indexIn(m_buf.data()) != -1){
			// TODO: check diferent types of UBX
			QString passport = rx.cap(2);
			QString personalMsg = rx.cap(3);
			m_buf.remove(0,rx.cap(1).size());
			//qDebug("MSN::Log::ParserNS ## Personal MSG mail:%s  msg:%s", passport.toUtf8().data(), personalMsg.toUtf8().data());
			//emit personalMessage(passport, personalMsg);
		}
	}
	else m_hasCommand = false;
}

void ParserNS::parseRng () {
	// RNG 1165534299 207.46.26.23:1863 CKI 22351196.235248171 XXXXXXXX@hotmail.com XXXXXX\r\n
	QRegExp rx;
	rx.setPattern("(^RNG (\\S+) ([\\d|\\D]*):(\\d+) CKI (\\S+) (\\S+) (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		QString sessid = rx.cap(2);
		QString address = rx.cap(3);
		QString port = rx.cap(4);
		QString ticket = rx.cap(5);
		QString contact = rx.cap(6);
		QString fName = rx.cap(7);
		qDebug ("MSN::Log::ParserNS::Calling ## sessid:%s address:%s port:%s ticket:%s contact:%s FriendName:%s",sessid.toUtf8().data(),address.toUtf8().data(),port.toUtf8().data(),ticket.toUtf8().data(),contact.toUtf8().data(),fName.toUtf8().data());
		m_buf.remove(0,rx.cap(1).size());
		ParserSB* chatParser = new ParserSB (address, port.toInt(), m_client->nextChatCount(), m_msnPassport, ticket, sessid, m_client);
		chatParser->start();
		////emit chatRequest (address, port, contact, fName ,ticket, sessid);
	}
	else m_hasCommand = false;
}

void ParserNS::parseOut () {
	// OUT OTH\r\n
	QRegExp rx;
	rx.setPattern("(^OUT (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		QString reason = rx.cap(2);
		m_buf.remove(0,rx.cap(1).size());
		if (reason == "OTH"){
			//emit disconnected (ConnAtOtherPlace);
		}
	}
	else m_hasCommand = false;
}

void ParserNS::parseError () {
	// 215 1\r\n
	QRegExp rx;
	rx.setPattern("(^(\\d{3}) \\d+\r\n)"); 
	if (rx.indexIn(m_buf.data()) != -1){
		QString reason = rx.cap(2);
		m_buf.remove(0,rx.cap(1).size());
		if (reason == "715") {
			qDebug("MSN::ERROR::ParserNS ## Not expected");
			//emit disconnected (ConnNotExpected);;
		}
		if (reason == "540") {
			qDebug("MSN::ERROR::ParserNS ## Bad MD5 digest");
			//emit disconnected (ConnBadMd5Digest);
		}
		if (reason == "800") {
			qDebug("MSN::ERROR::ParserNS ## Too many switchboard sessions more than 8 in 60 seconds");
			////emit ;
		}
	}
	else m_hasCommand = false;
}

void ParserNS::parse (){
	m_isParsing = true;
	QString cmd;
	while (m_buf.size() && m_hasCommand){
		//qDebug("MSN::ParserNS::Log::BUFFER <%s>",m_buf.dataDebug());
		cmd = m_buf.getCmd();
		if (cmd == "VER"){
			qDebug ("MSN::Log::ParserNS ## Parsing VER");
			parseVer();
		}

		else if (cmd == "CVR"){
			qDebug ("MSN::Log::ParserNS ## Parsing CVR");
			parseCvr();
		}
		else if (cmd == "XFR"){
			qDebug ("MSN::Log::ParserNS ## Parsing XFR");
			parseXfr();
		}
		else if (cmd == "GCF"){
			qDebug ("MSN::Log::ParserNS ## Parsing GCF");
			parseGcf();
		}
		else if (cmd == "USR"){
			qDebug ("MSN::Log::ParserNS ## Parsing USR");
			parseUsr();
		}
		else if (cmd == "MSG"){
			qDebug ("MSN::Log::ParserNS ## Parsing MSG");
			parseMsg();
		}
		else if (cmd == "SYN"){
			qDebug ("MSN::Log::ParserNS ## Parsing SYN");
			parseSyn();
		}
		else if (cmd == "GTC"){
			qDebug ("MSN::Log::ParserNS ## Parsing GTC");
			parseGtc();
		}
		else if (cmd == "BLP"){
			qDebug ("MSN::Log::ParserNS ## Parsing BLP");
			parseBlp();
		}
		else if (cmd == "BPR"){
			qDebug ("MSN::Log::ParserNS ## Parsing BPR");
			parseBpr();
		}
		else if (cmd == "PRP"){
			qDebug ("MSN::Log::ParserNS ## Parsing PRP");
			parsePrp();
		}
		else if (cmd == "LSG"){
			qDebug ("MSN::Log::ParserNS ## Parsing LSG");
			parseLsg();
		}
		else if (cmd == "LST"){
			qDebug ("MSN::Log::ParserNS ## Parsing LST");
			parseLst();
		}
		else if (cmd == "CHG"){
			qDebug ("MSN::Log::ParserNS ## Parsing LST");
			parseChg();
		}
		else if (cmd == "CHL"){
			qDebug ("MSN::Log::ParserNS ## Parsing CHL");
			parseChl();
		}
		else if (cmd == "QRY"){
			qDebug ("MSN::Log::ParserNS ## Parsing QRY");
			parseQry();
		}
		/// Connected
		else if (cmd == "NLN"){
			qDebug ("MSN::Log::ParserNS ## Parsing NLN");
			parseNln();
		}
		else if (cmd == "ILN"){
			qDebug ("MSN::Log::ParserNS ## Parsing ILN");
			parseNln();
		}

		else if (cmd == "UBX"){
			qDebug ("MSN::Log::ParserNS ## Parsing UBX");
			parseUbx();
		}
		else if (cmd == "FLN"){
			qDebug ("MSN::Log::ParserNS ## Parsing FLN");
			parseFln();
		}
		// Calling
		else if (cmd == "RNG"){
			qDebug ("MSN::Log::ParserNS ## Parsing RNG");
			parseRng();
		}

		else if (cmd == "OUT"){
			qDebug ("MSN::Log::ParserNS ## Parsing OUT");
			parseOut();
		}

		else if (cmd.contains(QRegExp::QRegExp("^\\d{3}"))){
			qDebug ("###########################ERROOOOOOOR");
			parseError();
		}

		else break;			
		
	}	
	m_isParsing = false;
}
}
#include "parsernotificationserver.moc"
