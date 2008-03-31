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

#include <QString>

namespace libimmsnp {
ParserNS::ParserNS(QString msnPassport, QString msnPass, State initialStatus, Client* c, msocket* s, QString protocol){
	m_idtr = 1;
	m_msnPassport = msnPassport;
	m_msnPass = msnPass;
	m_socket = s;
	m_initialStatus = initialStatus;
	m_client = c;
	//m_hasCommand = false;
	m_isParsing = false;
	m_protocol = protocol;
}
void ParserNS::init(){

	VER v(m_client->nextIdtr());
	v.addProtocolSupported(m_protocol);
	m_client->send(v);
	QByteArray data;
	qDebug("MSN::ParserNS:: Connection START");
	while ((m_socket->recv(data)) != -1){
		feed (data);
		data.clear();
		if (!m_isParsing){parse();}
	}
	qDebug("MSN::ParserNS:: END Connecton with %s",m_socket->getHost().toUtf8().data());

}
void ParserNS::feed (QByteArray b){
	m_hasCommand = true;
	m_buf.append(b);
}

void ParserNS::parseVer () {
	// VER 1 MSNP12 MSNP11 MSNP10 CVR0\r\n	
	QRegExp rx;
	rx.setPattern("(^VER \\d+ " + m_protocol + " CVR0\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
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
	else m_hasCommand = false;
}

void ParserNS::parseCvr () {
	// CVR 2 8.1.0178 8.1.0178 8.0.0787 http://msgr.dlservice.microsoft.com/download/1/A/4/1A4FEB1A-18E0-423A-B898-F697402E4F7F/Install_Messenger.exe http://get.live.com\r\n
	QRegExp rx;
	rx.setPattern("(^CVR \\d+ [\\d|\\D]*\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		QString s = rx.cap(1);
		m_buf.remove(0,s.size());

		USR u(m_client->nextIdtr());
		u.addTwnType ("I");
		u.addPassport (m_msnPassport);
		m_client->send (u);
	}
	////m_hasCommand = false;
}

void ParserNS::parseXfr () {
	//  XFR 3 NS 207.46.108.73:1863 U D\r\n
	//  XFR 3 SB 207.46.27.170:1863 CKI 1568090226.21815116.102267\r\n        for chatting
	QRegExp rx;
	rx.setPattern("(^XFR \\d+ ([N]?S[B]?) (\\S+):(\\d{4}) (\\S+) (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
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
			//qDebug("Connecting at: %s:%i ticket:%s",address.toUtf8().data(), port, ticket.toUtf8().data());
			emit chatRequest (address, port, "", "" ,ticket, "");
			//ParserSB* chatParser = new ParserSB (address, port, m_client->nextChatCount(), m_msnPassport, ticket, "", m_client);
			//chatParser->start();
			// //emit newChat ...
		}
	}
	////m_hasCommand = false;

}
void ParserNS::parseGcf () {
	//GCF 0 5187\r\n<Policies><Policy... USR 6 TWN S lc=1033,id=507,tw=40,ru=http%3A%2F%2Fmessenger%2Emsn%2Ecom,ct=1179166797,kpp=1,kv=9,ver=2.1.6000.1,rn=fJ0uZaXS,tpf=a0bf38b5ffd1253d4c57930ef5eb343f\r\n 
	QString payloadLen = 0;
	QRegExp rx;
	rx.setPattern("(^GCF [\\d|\\D]*USR \\d+ TWN S ([\\d|\\D]*)\r\n)"); 
//	rx.setPattern("(^GCF [\\d|\\D]*USR \\d+ SSO S MBI_KEY_OLD ([\\d|\\D]*)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		m_ticket = rx.cap(2);
		QString s = rx.cap(1);
		m_buf.remove(0,s.size());
		Challenge challenge(m_msnPassport, m_msnPass, m_ticket);
		m_ticket = challenge.getTicket();

		if (m_ticket == ""){
			emit disconnected (ConnAuthenticationFailed);
		}
		else {
			USR u(m_client->nextIdtr());
			u.addTwnType ("S");
			u.addTicket (m_ticket);
			m_client->send (u);
		}
	}
	//m_hasCommand = false;
}
void ParserNS::parseUsr () {
	// USR 7 OK probando_msnpy@hotmail.com 1 0 
	QRegExp rx;
	rx.setPattern("(^USR \\d+ TWN S ([\\d|\\D]*)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
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
		if (rx.indexIn(m_buf) != -1){
			QString s = rx.cap(1);
			m_buf.remove(0,s.size());
		}
		else {
			m_hasCommand = false;
		}
	}

}

void ParserNS::parseMsg () {
	// MSG Hotmail Hotmail 544\r\nMIME-Version: 1.0\r\nCo ... \r\n\r\n
	QRegExp rx;
	rx.setPattern("(^MSG Hotmail Hotmail (\\d+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		QString payload = rx.cap(2).toUtf8().data();
		rx.setPattern("(^MSG Hotmail Hotmail \\d+\r\n([\\d|\\D]{" + payload + "}))");
		if (rx.indexIn(m_buf) != -1){
			// TODO: check diferent types of MSG
			QString msg = rx.cap(2);
			m_buf.remove(0,rx.cap(1).size());
			rx.setPattern("^MIME-Version: 1.0\r\nContent-Type: text/x-msmsgsprofile");
			if (rx.indexIn(msg) != -1){
				SYN s(m_client->nextIdtr());
				m_client->send (s);
			}
			rx.setPattern("ClientIP: (\\d+\\.\\d+\\.\\d+\\.\\d+)\r\nClientPort: (\\d+)");
                        if (rx.indexIn(msg) != -1){
				emit clientIpPort(rx.cap(1), rx.cap(2));
			}

			// <MD><E><I>16</I><IU>6</IU><O>0</O><OU>0</OU></E>
			rx.setPattern("^MIME-Version: 1.0\r\nContent-Type: text/x-msmsgsinitialmdatanotification; charset=UTF-8\r\n\r\nMail-Data: <MD><E><I>(\\d+)</I><IU>(\\d+)</IU><O>(\\d+)</O><OU>(\\d+)</OU></E><Q><QTM>409600</QTM><QNM>204800</QNM></Q></MD>\r\nInbox-URL: /cgi-bin/HoTMaiL\r\nFolders-URL: /cgi-bin/folders\r\nPost-URL: http://www.hotmail.com\r\n");
			if (rx.indexIn(msg) != -1){
				int inbox = rx.cap(1).toInt();
				int inboxNotRead = rx.cap(2).toInt();
				int spam = rx.cap(3).toInt();
				int spamNotRead = rx.cap(4).toInt();
				qDebug("MSN::Log::ParserNS : New Mail inbox not Readed :%i of %i; spam not Readed :%i of %i", inboxNotRead, inbox, spamNotRead, spam);
			}
		}
	//	//else m_hasCommand = false;

	}
	////else m_hasCommand = false;
}

void ParserNS::parseSyn () {
	// SYN 8 2006-12-09T11:15:07.163-08:00 2006-06-10T20:12:54.183-07:00 1 6\r\n
	// SYN 8 2008-02-15T03:50:53.817-08:00 2008-02-15T03:43:37.837-08:00 2 0\r\nGTC A\r\nBLP BL\r\nPRP MFN probando_msnpy3@hotmail.com\r\nPRP MBE N\r\nPRP WWE 0\r\nLST N=probando_msnpy3@hotmail.com F=probando_msnpy3@hotmail.com C=4e395d4f-46cd-474a-a2d0-a3c53837652c 17 1\r\nLST N=probando_msnpy@hotmail.com F=Nick%20deprueba 16 1\r\n
	QRegExp rx;
	rx.setPattern("(^SYN \\d+ (\\S+) (\\S+) (\\d+) (\\d+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		m_contacts = rx.cap(4).toInt();
		m_groups = rx.cap(5).toInt();
		m_buf.remove(0,rx.cap(1).size());
	}
	else m_hasCommand = false;
}

void ParserNS::parseGtc () {
	// GTC A\r\nBLP AL\r\n 
	QRegExp rx;
	rx.setPattern("(^GTC (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		m_buf.remove(0,rx.cap(1).size());
	}
	////else m_hasCommand = false;
}

void ParserNS::parseBlp () {
	// BLP AL\r\n 
	QRegExp rx;
	rx.setPattern("(^BLP (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		m_buf.remove(0,rx.cap(1).size());
	}
	////else m_hasCommand = false;
}

void ParserNS::parseBpr () {
	// BPR HSB 1\r\n  
	QRegExp rx;
	//rx.setPattern("(^BPR ([\\S]{3}))"); 
	rx.setPattern("(^BPR HSB 1\r\n)");
	if (rx.indexIn(m_buf) != -1){
		m_buf.remove(0,rx.cap(1).size());
		qDebug("MSN::Log::ParserNS :HasBlog:%s" ,m_prevContact.toUtf8().data());
		emit hasBlog (m_prevContact);
		return;
	}
	else {
		rx.setPattern("(^BPR MOB Y\r\n)");
		if (rx.indexIn(m_buf) != -1){
			m_buf.remove(0,rx.cap(1).size());
			return;
		}
			else {
				//BPR PHH tel:34%20925825866 0\r\nBPR PHW tel:34%20625914471 0\r\nBPR PHM tel:34%20625914471 0\r\nBPR HSB 1\r\n
				//rx.setPattern("(^BPR PHM (\\S+) 0\r\n)");
				rx.setPattern("(^BPR PH. \\S+ \\d+\r\n)");
				if (rx.indexIn(m_buf) != -1){
					m_buf.remove(0,rx.cap(1).size());
				}
				else m_hasCommand = false;
			}
	}

}

void ParserNS::parsePrp () {
	// 
	QRegExp rx;
	rx.setPattern("(^PRP (\\S+) (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		if (rx.cap(2) == "MFN") {
			// TODO: //emit signal sending my name
			//qDebug ("MSN::Log::ParserNS : MFN: %s", rx.cap(3).toUtf8().data());
		}
		m_buf.remove(0,rx.cap(1).size());
	}
	////else m_hasCommand = false;
}

void ParserNS::parseLsg () {
	// ADG 10 Minewgroup 59ed80fa-a585-4488-9d36-a976cdf7488f\r\n 
	// LSG Grupo%20Msn d45b8f16-037f-48f6-b972-815e6f2d71fc\r\n
	QRegExp rx;
	rx.setPattern("(^LSG (\\S+) (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		QString nombre = rx.cap(2);
		QString id = rx.cap(3);
		Group* g = new Group(nombre, id);
		emit newGroupArrived(g);
		m_buf.remove(0,rx.cap(1).size());
	}
	rx.setPattern("(^ADG \\d+ (\\S+) (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		QString nombre = rx.cap(2);
		QString id = rx.cap(3);
		//qDebug ("MSN::Log::ParserNS : New Group=%s id=%s",nombre.toUtf8().data(),id.toUtf8().data());
		Group* g = new Group(nombre, id);
		emit newGroupArrived(g);
		m_buf.remove(0,rx.cap(1).size());
	}
}

void ParserNS::parseLst () {
	QRegExp vx;

	// LST N=aaaaaaaaa@hotmail.com F=nick:%20aaÃ³ C=07ad2594-ef7f-4a1e-96e1-0158611521ed 11 1 5faec9a4-b368-4def-8f11-6be610b665d0\r\n
	// LST N=aaaaaaaaa@hotmail.com F=nick:%20aaÃ³ C=07ad2594-ef7f-4a1e-96e1-0158611521ed 11 1\r\n
	// LST N=probando_msnpy@hotmail.com F=Nick%20deprueba 16 1\r\n
	
	vx.setPattern("(^LST N=(\\S+) (F=(.*) C=(\\S+) |F=(.*) )?(\\d+) \\d+( \\S+)?\r\n)"); 
        vx.setMinimal(TRUE);
	if (vx.indexIn(m_buf) != -1){
		m_buf.remove(0, vx.cap(1).size());
		QString mail	= vx.cap(2);
		QString nick	= vx.cap(4);
		QString id	= vx.cap(5);
		QString list	= vx.cap(7);
		QString groupId	= vx.cap(8);
		Contact *c = new Contact(mail, nick, id);
		c->setGroupId(groupId);
		list = vx.cap(7);
		c->setList(list);
		qDebug ("##5 de %i# %i,1 %s,mail %s,3 %s,nick %s,id %s,6 %s,List %s, Grupos %s,9 %s \n",m_contacts,vx.capturedTexts().size(), vx.cap(1).toUtf8().replace("\r\n","\\r\\n").data(), vx.cap(2).toUtf8().data(), vx.cap(3).toUtf8().data(), vx.cap(4).toUtf8().data(), vx.cap(5).toUtf8().data(), vx.cap(6).toUtf8().data(), list.toUtf8().data(), vx.cap(8).toUtf8().data(), vx.cap(9).toUtf8().data());
		emit newContactArrived (c); 
		m_prevContact = mail;
		if (--m_contacts == 0) {
			qDebug ("MSN::Log::ParserNS : Roster Received");
			CHG c (m_client->nextIdtr());
			c.addStatusCode (m_initialStatus);
			//c.addCapabilities ("1342558252");
			c.addCapabilities ("1879343140");
			m_client->send (c);
			emit connected();
		}
	}
	else {
		m_hasCommand = false;
	}
}

void ParserNS::parseChg () {
	// CHG 9 NLN 1342558252\r\n
	QRegExp rx;
	rx.setPattern("(^CHG \\d+ (\\S+) (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		QString estado = rx.cap(2);
		QString capabilities = rx.cap(3);
		//qDebug ("MSN::Log::ParserNS : State Changed=%s capabilities=%s",estado.toUtf8().data(), capabilities.toUtf8().data());
		m_buf.remove(0,rx.cap(1).size());
	}
	////else m_hasCommand = false;
}

void ParserNS::parseChl (){
	// <<< CHL 0 22940274951814684551\r\n
	// >>> QRY 11 PROD0090YUAUV{2B 32\r\nffeb4c3cf93db6a4b708b246baade0d9(no newline)
	QRegExp rx;
	rx.setPattern("(^CHL 0 (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
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
	////else m_hasCommand = false;
}

void ParserNS::parseQry () {
	// 
	QRegExp rx;
	rx.setPattern("(^QRY \\d+\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		m_buf.remove(0,rx.cap(1).size());
		//TODO:remove this line
		//m_client->initChat("probando_msnpy@hotmail.com");
	}
	//else m_hasCommand = false;
}

void ParserNS::parseNln () {
	// ILN 9 BSY XXXXXXXX@hotmail.com Diecoballa,%20Watashi%20wa%20supeinjin%20desu,%20watashi%20no%20namae%20wa%20Akimoto%20Makoto%20desu 1616760868 %3Cmsnobj%20Creator%3D%22darkerviti%40hotmail.com%22%20Size%3D%2229089%22%20Type%3D%223%22%20Location%3D%22TFR11A.dat%22%20Friendly%3D%22AAA%3D%22%20SHA1D%3D%22Ra4vcWsSOszskaurp71jzB0RSUk%3D%22%20SHA1C%3D%22zMQH9sLLaWshs9B8k27FglzFJ%2Fk%3D%22%2F%3E\r\n
	// ILN 9 NLN XXXXXXXXX@hotmail.com pedro 1342558252\r\n
	// NLN NLN XXXXXXXXXXX@hotmail.com pedro 1342558252 0\r\n
	QString state;
	QString passport;
	QString nick;
	QString capabilities;

	if (m_buf.indexOf("\r\n") >= 0) {
		QList<QByteArray> p2 = m_buf.split(' ');
		if (p2.size() == 6 )  {
			bool ok;
		  	int dec = p2[1].toInt(&ok, 10);
			if (ok) {
				// ILN 9 NLN XXXXXXXXX@hotmail.com pedro 1342558252\r\n
				state = p2[2].data();
				passport = p2[3].data();
				nick   = p2[4].data();
				capabilities = p2[5].data();
			}
			else {
				// NLN NLN XXXXXXXXXXX@hotmail.com pedro 1342558252 0\r\n
				state = p2[1].data();
				passport = p2[2].data();
				nick   = p2[3].data();
				capabilities = p2[4].data();
			}

		}
		if (p2.size() >= 7 )  {
			state = p2[2].data();
			passport = p2[3].data();
			nick   = p2[4].data();
			capabilities = p2[5].data();
		}

		qRegisterMetaType<State>("State");
		if (state == "NLN") emit statusChanged (passport, STATUS_NLN, nick, capabilities);
		else if (state == "BSY") emit statusChanged (passport, STATUS_BSY, nick, capabilities);
		else if (state == "IDL") emit statusChanged (passport, STATUS_IDL, nick, capabilities);
		else if (state == "BRB") emit statusChanged (passport, STATUS_BRB, nick, capabilities);
		else if (state == "AWY") emit statusChanged (passport, STATUS_AWY, nick, capabilities);
		else if (state == "PHN") emit statusChanged (passport, STATUS_PHN, nick, capabilities);
		else if (state == "LUN") emit statusChanged (passport, STATUS_LUN, nick, capabilities);
		//qDebug ("MSN::Log::ParserNS : Status Changed:%s mail:%s nick:%s capabi:%s#",state.toUtf8().data(), passport.toUtf8().data(), nick.toUtf8().data(), capabilities.toUtf8().data());
		m_buf.remove(0,m_buf.indexOf("\r\n")+2);
	}
	else m_hasCommand = false;
}

void ParserNS::parseFln () {
	// FLN XXXXXXXXXX@hotmail.com\r\n 
	QRegExp rx;
	rx.setPattern("(^FLN (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		QString passport = rx.cap(2);
		//qDebug ("MSN::Log::ParserNS : Contact %s Disconnected",rx.cap(2).toUtf8().data());
		emit statusChanged (passport,STATUS_OFF ,"","");
		m_buf.remove(0,rx.cap(1).size());
	}
	//else m_hasCommand = false;
}

void ParserNS::parseUbx () {
	// UBX passport@hotmail.com xxx\r\n<Data><PSM>My Personal Message</PSM><CurrentMedia></CurrentMedia></Data>
	QRegExp rx;
	rx.setPattern("(^UBX \\S+ (\\d+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		QString payload = rx.cap(2).toUtf8().data();

		rx.setPattern("(^UBX (\\S+) \\d+\r\n([\\d|\\D]{" + payload + "}))");
		if (rx.indexIn(m_buf) != -1){
			// TODO: check diferent types of UBX
			QString passport = rx.cap(2);
			QString personalMsg = rx.cap(3);
			m_buf.remove(0,rx.cap(1).size());
			rx.setPattern("<PSM>([\\d|\\D]*)</PSM>");
			if (rx.indexIn(personalMsg) != -1){
				personalMsg = rx.cap(1);
			}
			//qDebug("MSN::Log::ParserNS : Personal MSG mail:%s  msg:%s", passport.toUtf8().data(), personalMsg.toUtf8().data());
			emit personalMessage(passport, personalMsg);
		}
		else m_hasCommand = false;
	}
	else m_hasCommand = false;
}

void ParserNS::parseRng () {
	// RNG 1165534299 207.46.26.23:1863 CKI 22351196.235248171 XXXXXXXX@hotmail.com XXXXXX\r\n
	QRegExp rx;
	rx.setPattern("(^RNG (\\S+) ([\\d|\\D]*):(\\d+) CKI (\\S+) (\\S+) (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		QString sessid = rx.cap(2);
		QString address = rx.cap(3);
		int port = rx.cap(4).toInt();
		QString ticket = rx.cap(5);
		QString contact = rx.cap(6);
		QString fName = rx.cap(7);
		//qDebug ("MSN::Log::ParserNS::Calling : sessid:%s address:%s port:%i ticket:%s contact:%s FriendName:%s",sessid.toUtf8().data(),address.toUtf8().data(),port ,ticket.toUtf8().data(),contact.toUtf8().data(),fName.toUtf8().data());
		m_buf.remove(0,rx.cap(1).size());
		emit chatRequest (address, port, contact, fName ,ticket, sessid);
	}
	//else m_hasCommand = false;
}

void ParserNS::parseNot () {
	// NOT 1119\r\n<NOTIFICATION id="0" siteid="45705" siteurl="http://storage.msn.com"><TO pid="0x0:0xAA816ADF" name="#########@hotmail.com" ><VIA agent="messenger"/></TO><MSG id="0"><SUBSCR url="s.htm"/><ACTION url="a.htm"/><BODY>&lt;NotificationData xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema"&gt;  &lt;SpaceHandle&gt;    &lt;ResourceID&gt;1paJbsiSBiLG6HFnu7qoU6mw!101&lt;/ResourceID&gt;  &lt;/SpaceHandle&gt;  &lt;ComponentHandle&gt;    &lt;ResourceID&gt;2EC83A1E19F85738!101&lt;/ResourceID&gt;  &lt;/ComponentHandle&gt;  &lt;OwnerCID&gt;3371008222045951800&lt;/OwnerCID&gt;  &lt;LastModifiedDate&gt;2007-09-11T15:21:40-07:00&lt;/LastModifiedDate&gt;  &lt;HasNewItem&gt;true&lt;/HasNewItem&gt;  &lt;ComponentSummary&gt;    &lt;Component xsi:type="Folder"&gt;      &lt;ResourceID&gt;2EC83A1E19F85738!118&lt;/ResourceID&gt;    &lt;/Component&gt;    &lt;Items&gt;      &lt;Item xsi:type="Photo"&gt;        &lt;ResourceID&gt;2EC83A1E19F85738!0&lt;/ResourceID&gt;      &lt;/Item&gt;    &lt;/Items&gt;  &lt;/ComponentSummary&gt;&lt;/NotificationData&gt;</BODY></MSG></NOTIFICATION>\r\n
	QRegExp rx;
	rx.setPattern("(^NOT (\\d+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		QString payload = rx.cap(1).toUtf8().data();

		rx.setPattern("(^NOT \\d+\r\n<NOTIFICATION .*</NOTIFICATION>)"); 
		rx.setMinimal(TRUE);
		if (rx.indexIn(m_buf) != -1){
			m_buf.remove(0,rx.cap(1).size());
		}
		else m_hasCommand = false;
	}
	else m_hasCommand = false;
}

void ParserNS::parseOut () {
	// OUT OTH\r\n
	QRegExp rx;
	rx.setPattern("(^OUT (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		QString reason = rx.cap(2);
		m_buf.remove(0,rx.cap(1).size());
		if (reason == "OTH"){
			emit disconnected (ConnAtOtherPlace);
		}
	}
	//else m_hasCommand = false;
}

void ParserNS::parseRem () {
	// REM 0 RL xxxxxxxx@hotmail.com\r\n
	QRegExp rx;
	rx.setPattern("(^REM \\d+ (\\S+) (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		QString list = rx.cap(2);
		QString passport = rx.cap(3);
		m_buf.remove(0,rx.cap(1).size());
		qDebug ("MSN::Log::ParserNS::%s has deleted you from:%s list",passport.toUtf8().data(), list.toUtf8().data());
	}
	//else m_hasCommand = false;
}

void ParserNS::parseAdc () {
	// ADC 13 BL N=probando_msnpy2@hotmail.com\r\n
	QRegExp rx;
	QString list;
	QString passport;
	QString nick;
	QString id;
	QString groupId;
	rx.setPattern("(^ADC \\d+ (\\S+) N=(\\S+)( F=(.*) \\d+| F=(.*) C=(\\S+))?\r\n)");
	if (rx.indexIn(m_buf) != -1){
		list = rx.cap(2);
		passport = rx.cap(3);
		nick = rx.cap(5);
		id = rx.cap(6);
		m_buf.remove(0,rx.cap(1).size());
		qDebug ("MSN::Log::ParserNS:######:1 %s,List %s,mail %s,4 %s,nick %s,6 %s",rx.cap(1).toUtf8().replace("\r\n","\\r\\n").data(), list.toUtf8().data(), passport.toUtf8().data(), rx.cap(4).toUtf8().data(), nick.toUtf8().data(), rx.cap(6).toUtf8().data());
		Contact* c = new Contact(passport, nick, id);
		c->setList(list);
		emit newContactArrived(c);
	}
	else {
		rx.setPattern("(^ADC \\d+ (\\S+) C=(\\S+) (\\S+)\r\n)");
		if (rx.indexIn(m_buf) != -1){
			list = rx.cap(2);
			id = rx.cap(3);
			groupId = rx.cap(4);
			m_buf.remove(0,rx.cap(1).size());
			qDebug ("MSN::Log::ParserNS:######:1 %s,lista %s,msnId %s,group %s,5 %s,6 %s",rx.cap(1).toUtf8().replace("\r\n","\\r\\n").data(), list.toUtf8().data(), id.toUtf8().data(), groupId.toUtf8().data(), rx.cap(5).toUtf8().data(), rx.cap(6).toUtf8().data());
		}
		else m_hasCommand = false;
		
	}
}

void ParserNS::parseRmg () {
	// RMG 12 59ed80fa-a585-4488-9d36-a976cdf7488f\r\n
	QRegExp rx;
	rx.setPattern("(^RMG \\d+ (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		QString groupId = rx.cap(2);
		m_buf.remove(0,rx.cap(1).size());
		qDebug ("MSN::Log::ParserNS::Group Deleted:%s list",groupId.toUtf8().data());
		emit groupDeleted(groupId);
	}
	//else m_hasCommand = false;
}
void ParserNS::parseError () {
	// 215 1\r\n
	QRegExp rx;
	rx.setPattern("(^(\\d{3}) \\d+\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		QString reason = rx.cap(2);
		m_buf.remove(0,rx.cap(1).size());
		if (reason == "715") {
			qDebug("MSN::ERROR::ParserNS : Not expected");
			emit disconnected (ConnNotExpected);;
		}
		if (reason == "540") {
			qDebug("MSN::ERROR::ParserNS : Bad MD5 digest");
			emit disconnected (ConnBadMd5Digest);
		}
		if (reason == "800") {
			qDebug("MSN::ERROR::ParserNS : Too many switchboard sessions more than 8 in 60 seconds");
			////emit ;
		}
	}
	//else m_hasCommand = false;
}

void ParserNS::parse (){
	m_isParsing = true;
	QString cmd;
	while (m_buf.size() && m_hasCommand){
		qDebug("MSN::ParserNS::Log::BUFFER <%s>", QByteArray(m_buf).replace("\\r\\n", "\\\\r\\\\n").replace("\r\n", "\\r\\n").data());

		cmd = m_buf.mid(0,3).data();
		if (cmd == "VER"){
			//qDebug ("MSN::Log::ParserNS : Parsing VER");
			parseVer();
		}

		else if (cmd == "CVR"){
			qDebug ("MSN::Log::ParserNS : Parsing CVR");
			parseCvr();
		}
		else if (cmd == "XFR"){
			qDebug ("MSN::Log::ParserNS : Parsing XFR");
			parseXfr();
		}
		else if (cmd == "GCF"){
			qDebug ("MSN::Log::ParserNS : Parsing GCF");
			parseGcf();
		}
		else if (cmd == "USR"){
			qDebug ("MSN::Log::ParserNS : Parsing USR");
			parseUsr();
		}
		else if (cmd == "MSG"){
			qDebug ("MSN::Log::ParserNS : Parsing MSG");
			parseMsg();
		}
		else if (cmd == "SYN"){
			qDebug ("MSN::Log::ParserNS : Parsing SYN");
			parseSyn();
		}
		else if (cmd == "GTC"){
			qDebug ("MSN::Log::ParserNS : Parsing GTC");
			parseGtc();
		}
		else if (cmd == "BLP"){
			qDebug ("MSN::Log::ParserNS : Parsing BLP");
			parseBlp();
		}
		else if (cmd == "BPR"){
			qDebug ("MSN::Log::ParserNS : Parsing BPR");
			parseBpr();
		}
		else if (cmd == "PRP"){
			qDebug ("MSN::Log::ParserNS : Parsing PRP");
			parsePrp();
		}
		else if (cmd == "LSG" || cmd == "ADG"){
			qDebug ("MSN::Log::ParserNS : Parsing LSG");
			parseLsg();
		}
		else if (cmd == "LST"){
			qDebug ("MSN::Log::ParserNS : Parsing LST");
			parseLst();
		}
		else if (cmd == "CHG"){
			qDebug ("MSN::Log::ParserNS : Parsing CHG");
			parseChg();
		}
		else if (cmd == "CHL"){
			qDebug ("MSN::Log::ParserNS : Parsing CHL");
			parseChl();
		}
		else if (cmd == "QRY"){
			qDebug ("MSN::Log::ParserNS : Parsing QRY");
			parseQry();
		}
		/// Connected
		else if (cmd == "NLN"){
			qDebug ("MSN::Log::ParserNS : Parsing NLN");
			parseNln();
		}
		else if (cmd == "ILN"){
			qDebug ("MSN::Log::ParserNS : Parsing ILN");
			parseNln();
		}

		else if (cmd == "UBX"){
			qDebug ("MSN::Log::ParserNS : Parsing UBX");
			parseUbx();
		}
		else if (cmd == "FLN"){
			qDebug ("MSN::Log::ParserNS : Parsing FLN");
			parseFln();
		}
		// Calling
		else if (cmd == "RNG"){
			qDebug ("MSN::Log::ParserNS : Parsing RNG");
			parseRng();
		}

		else if (cmd == "OUT"){
			qDebug ("MSN::Log::ParserNS : Parsing OUT");
			parseOut();
		}

		else if (cmd == "NOT"){
			qDebug ("MSN::Log::ParserNS : Parsing NOT");
			parseNot();
		}

		else if (cmd == "REM"){
			qDebug ("MSN::Log::ParserNS : Parsing REM");
			parseRem();
		}

		else if (cmd == "ADC"){
			qDebug ("MSN::Log::ParserNS : Parsing ADC");
			parseAdc();
		}

		else if (cmd == "RMG"){
			qDebug ("MSN::Log::ParserNS : Parsing RMG");
			parseRmg();
		}

		else if (cmd.contains(QRegExp::QRegExp("^\\d{3}"))){
			qDebug ("###########################ERROOOOOOOR");
			parseError();
		}

		else {
			break;			
			qDebug("MSN::ParserNS::Log::SALIMOOOOSSS");
		}
	}	
	//fprintf(stderr,"MSN::ParserNS::Log::NO COMMAND BUFFER size %i, hasCommand %i <", m_buf.size(), m_hasCommand);
	//m_buf.toChars();
	//fprintf(stderr,">\n");
	m_isParsing = false;
}
}
#include "parsernotificationserver.moc"
