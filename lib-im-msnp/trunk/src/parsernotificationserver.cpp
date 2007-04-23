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

#include "parsernotificationserver.h"
#include "client.h"
#include "login.h"
#include "notificationServer.h"
#include "md5.h"
#include <qstringlist.h>

namespace libimmsnp {
ParserNS::ParserNS(QString msnPassport, QString msnPass, State initialStatus, Client* c){
	m_idtr = 1;
	m_msnPassport = msnPassport;
	m_msnPass = msnPass;
	m_socket = 0;
	m_groups = 0;
	m_contacts = 0;
	m_initialStatus = initialStatus;
	m_client = c;
	m_hasCommand = false;
	m_isParsing = false;
	m_connectionSteps = 0;
}
void ParserNS::feed (Buffer b){
	m_buf << b;
}

void ParserNS::feed (QString s){
	m_hasCommand = true;
	m_buf << s;
}

//ParserOperation ParserNS::makeRoaster (msocket* s, std::string bufSyn){
//	//SYN trid synchversion numberbuddies numbergroups
//	
//}

//void ParserNS:

void ParserNS::parseVer () {
	QString s;
	int l;
	QString tmp;
	m_buf.data(tmp);
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		if (s.contains( "MSNP12" ) || s.contains("MSNP11") || s.contains("MSNP10")){
			m_buf.advance (l);
			m_buf.removeFromBegin();
			QString a;
			m_buf.data(a);
			CVR c(m_client->getIdtr());
			c.addLocaleId ("0x0409");
                        c.addOs ("winnt", "5.1");
                        c.addArch ("i386");
                        c.addClient ("MSNMSGR", "7.5.0324");
                        c.addPassport (m_msnPassport);

			m_client->send(c);
			// añado a lista de comprobacion 
		}
		else {
			// return bad state
			m_buf.gotoBegin();
			return;
		}
	}
	else m_hasCommand = false;
	m_buf.gotoBegin();
//	this->parse ();
}
void ParserNS::parseCvr () {
	// << CVR 1 7.5.0324 7.5.0324 7.5.0299 http://msgr.dlservice.microsoft.com/download/5/a/8/5a892c0f-5b87-4767-8927-6fe5d8cfc582/Install_MSN_Messenger.exe http://messenger.msn.com\r\n
	QString s;
	int l;
	QString tmp;
	m_buf.data(tmp);
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_buf.advance (l);
		m_buf.removeFromBegin();

		USR u(m_client->getIdtr());
		u.addTwnType ("I");
		u.addPassport (m_msnPassport);
		m_client->send (u);
		// añado a lista de comprobacion 
	}
	else m_hasCommand = false;
	m_buf.gotoBegin();
//	this->parse ();
}

void ParserNS::parseXfr () {
	// XFR 3 NS 207.46.107.61:1863 0 207.46.28.93:1863\r\n
	QString s;
	int l;
	QString tmp;
	m_buf.data(tmp);
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_buf.advance (l);
		m_buf.removeFromBegin();
		if (s.contains ("NS")){
			QString ip   = s.mid (3, s.find (':') - 3);	
			int     port = s.mid (s.find(':')+1, 4).toInt() ;	
			m_client->makeConnection(ip, port);
			
			// Restart secuence of authentication.
			VER v(m_client->getIdtr());
			v.addProtocolSupported("MSNP12");
			v.addProtocolSupported("MSNP11");
			v.addProtocolSupported("MSNP10");
			m_client->send(v);
			m_client->startConnection();
		}
		else if (s.contains ("SB")){
		// XFR 15 SB 207.46.108.37:1863 CKI 17262740.1050826919.32308\r\n
			QStringList fields = QStringList::split(" ",s);
			QString ipPort = fields[1];
			QString authType = fields[2];
			QString ticket = fields[3].replace ("\r\n","");
			emit initChatSB (ipPort, ticket);
		}

	}
	else m_hasCommand = false;
}

static size_t msnCurlWriteFn(void *ptr, size_t size, size_t nmemb, void *stream){
	size_t realsize = size * nmemb;
	return realsize;
}

static size_t msnCurlWriteHeadersFn(void *ptr, size_t size, size_t nmemb, void *stream){
	
	size_t realsize = size * nmemb;
	std::string *_cookie = reinterpret_cast<std::string *> (stream);
	std::string _headers = std::string((char *)ptr, size * nmemb);
	
	if (_headers.size() > 350){

		int posIni = _headers.find("'")+1;
		int posFin = _headers.substr(posIni ,_headers.size() - posIni ).find ("'");
		std::string tmpTicket = _headers.substr(posIni,posFin);
		*_cookie = tmpTicket;
	}
	
	return realsize;
}

std::string ParserNS::httpsReq (std::string url, std::string headers) {
	CURL *curl;
	CURLcode ret;
	curl_slist *slist = NULL;

	std::string cookie; 

	printf ("MSN::Log::ParserNS ## Start htpps request to %s\n",QString(url).latin1());
	curl = curl_easy_init();
	if(curl) {
		slist = curl_slist_append(slist, headers.c_str());
								
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);	
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &msnCurlWriteFn);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, &msnCurlWriteHeadersFn);
		curl_easy_setopt(curl, CURLOPT_WRITEHEADER, &cookie);
		
		ret = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		curl_slist_free_all(slist);
		printf ("MSN::Log::ParserNS ## End htpps request to %s\n",QString(url).latin1());
		return cookie;
	}
	printf ("MSN::ERROR::ParserNS ## Making request to %s\n",QString(url).latin1());
	return "";
}

void ParserNS::parseUsr () {
	// USR 6 TWN S lc=1033,id=507,tw=40,ru=httpNAN0.000000-0.000000messenger-7.880785E-44msn-7.880713E-44com,ct=1164389091,kpp=1,kv=9,ver=2.1.6000.1,rn=b8sHoiRj,tpf=7a344a23f6f3d65183709edca5c7b97a\r\n
	QString s;
	int l;
	QString tmp;
	m_buf.data(tmp);
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		if (s.contains ("TWN S")){
			m_buf.advance (l);
			m_buf.removeFromBegin();
			m_ticket = s.mid (s.find ("lc"),s.length()-s.find ("lc")-2);

			// start Twenner
			std::string tmpUrl = httpsReq ("https://nexus.passport.com/rdr/pprdr.asp", "");
			int iniUrl = tmpUrl.find("DALogin=")+8;
			int finUrl = tmpUrl.substr(iniUrl, tmpUrl.size() - iniUrl).find (",DAReg=");

			std::string url("https://");
			url += tmpUrl.substr(iniUrl,finUrl);

			std::string auth = std::string("Authorization: Passport1.4 OrgVerb=GET,OrgURL=http%3A%2F%2Fmessenger%2Emsn%2Ecom,sign-in=") + m_msnPassport.replace('@', "%40").replace('.', "%2E").latin1() + ",pwd=" +  m_msnPass.replace('@', "%40").replace('.', "%2E").latin1() + "," + m_ticket.latin1();

      			// Don't urlEncode the m_ticket, only the id and password.
 			m_ticket = QString (httpsReq (url, auth));
			if (m_ticket == ""){
				emit disconnected (ConnAuthenticationFailed);
			}
			printf ("MSN::Log::ParserNS ## Ticket %s#\n",m_ticket.latin1());
			USR u(m_client->getIdtr());
			u.addTwnType ("S");
			u.addTicket (m_ticket);
			m_client->send (u);

			// añado a lista de comprobacion 
		}
		else if (s.contains ("OK ")){
			printf ("MSN::Log::ParserNS ## USR OK\n");
	                m_buf.advance (l);
	                m_buf.removeFromBegin();
		}
		else {
			printf ("MSN::Log::ParserNS ## Other USR OK:%s#\n",s.latin1());
	                m_buf.advance (l);
	                m_buf.removeFromBegin();
		}
	}
	else m_hasCommand = false;

//	m_buf.gotoBegin();
//	this->parse ();
}

void ParserNS::parseMsg (){
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_buf.advance (l);
		QStringList fields = QStringList::split(" ",s);
		QString passport = fields[0];
		QString callerNick = fields[1];
		int payload = fields[2].replace("\r\n","").toInt();
		QString dataPayload;
		if ((m_buf.getNChar (dataPayload,payload)) == payload){
			m_buf.advance (payload);
			m_buf.removeFromBegin();
			SYN s (m_client->getIdtr());
			m_client->send (s);
			// añado a lista de comprobacion 
		}
		else m_hasCommand = false;
	}
	else m_hasCommand = false;
}

void ParserNS::parseSyn () {
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_buf.advance (l);
		m_buf.removeFromBegin();
		QString g = s.mid (s.findRev(" "), s.length () - s.findRev(" ") - 2 );
		QString ctmp = s.mid (0, s.find (g + "\r\n"));
		QString c = s.mid(ctmp.findRev(" "), ctmp.length () - ctmp.findRev(" "));
		m_contacts = c.toInt();
		m_groups = g.toInt();
	}
	else m_hasCommand = false;
}

void ParserNS::parseGtc () {
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		if (s.contains ("A"))
			printf ("MSN::Log::ParserNS ## GTC OK\n");
		else if (s.contains ("N"))
			printf ("MSN::Log::ParserNS ## GTC Not OK\n");
		m_buf.advance (l);
		m_buf.removeFromBegin();
	}
	else m_hasCommand = false;
}
void ParserNS::parseBlp () {
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		if (s.contains ("AL"))
			printf ("MSN::Log::ParserNS ## BLP Chat YES\n");
		else if (s.contains ("BL"))
			printf ("MSN::Log::ParserNS ## BLP Chat NO\n");
		m_buf.advance (l);
		m_buf.removeFromBegin();
	}
	else m_hasCommand = false;
}

void ParserNS::parsePrp () {
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_buf.advance (l);
		m_buf.removeFromBegin();
		if (s.contains ("MFN"))
			printf ("MSN::Log::ParserNS ## !!PRP My name is\n");
			// TODO: emit signal sending my name
		else if (s.contains ("PHH") || s.contains ("PHW")  || s.contains ("PHM") || s.contains ("MOB") || s.contains ("MBE") ||  s.contains ("WWE"))
			printf ("MSN::Log::ParserNS ## !!PRP Telefonos\n");
	}
	else m_hasCommand = false;
}

void ParserNS::parseLsg (){
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_buf.advance (l);
		m_buf.removeFromBegin();
		m_groups--;
		QStringList fields = QStringList::split(" ",s);
		QStringList::iterator point = fields.begin();
		Group* g = new Group(fields[0],fields[1].replace("\r\n",""));
		emit newGroupArrived(g);
	}
	else m_hasCommand = false;

}
void ParserNS::parseLst (){
	// LST N=alice@hotmail.com F=alice C=167266c7-15bf-440e-9940-1e3223e4f1a4 11 1 409a87ab-ab83-4dc1-a577-de97e948bc57\r\n
	// LST N=encarta@conversagent.com 4 1\r\n
	// FL = your contacts list
	// RL = contacts that hae you in their contact list
	//
	// BL = contacts you have blocked
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_buf.advance (l);
		m_buf.removeFromBegin();
		m_contacts--;
		QStringList fields = QStringList::split(" ",s);
		Contact* c = new Contact;
		c->setPassport (fields[0].replace("N=",""));
		if (fields.count() > 3){
			c->setNickName (fields[1].replace("F=","").replace("%20"," ")); // TODO: remove replace
			c->setId (fields[2].replace("C=",""));
			c->setList (fields[3]);
			if (fields[5]) {
				QStringList groups = QStringList::split(",",fields[5].replace("\r\n",""));
				QString oneGroup = groups[0];
				c->setGroupId(oneGroup);
			}
		}
		else{
			c->setList (fields[2]);
		}
		m_prevContact = fields[0];
		emit newContactArrived (c); 
	}
	else m_hasCommand = false;
	if (m_contacts == 0) {
		CHG c (m_client->getIdtr());
		c.addStatusCode (m_initialStatus);
		c.addCapabilities ("1342558252");
		printf ("MSN::Log::ParserNS ## Roster received#\n");
		m_client->send (c);
                // añado a lista de comprobacion 
		m_hasCommand = false;
	}
}

void ParserNS::parseSbs (){
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_buf.advance (l);
		m_buf.removeFromBegin();
	}
	else m_hasCommand = false;

}

void ParserNS::parseChg (){
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_buf.advance (l);
		m_buf.removeFromBegin();
		if (m_connectionSteps == 0){
			emit connected();
			m_connectionSteps = 1;
		}
	}
	else m_hasCommand = false;
}

// This pieze of code  is based on code http://tmsnc.sourceforge.net/chl.c 
/* MSNP11 Challenge Handler v2
 * Based on the C++ implementation. 
 * This implementation works on big endian systems as well.
 * Compile: 'gcc chl.c -o chl -lcrypto -Wall'
 *
 * Licensed for distribution under the GPL version 2
 * Copyright 2005 Sanoi <sanoix@gmail.com>
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/md5.h>

#define BUFSIZE 256

int isBigEndian(void)
{
  short int word = 0x0100;
  char *byte = (char *)&word;
  return(byte[0]);
}

unsigned int swapInt(unsigned int dw)
{
  unsigned int tmp;
  tmp =  (dw & 0x000000FF);
  tmp = ((dw & 0x0000FF00) >> 0x08) | (tmp << 0x08);
  tmp = ((dw & 0x00FF0000) >> 0x10) | (tmp << 0x08);
  tmp = ((dw & 0xFF000000) >> 0x18) | (tmp << 0x08);
  return(tmp);
}

void challenge(const char *input, char *output) {
  char *productKey = "YMM8C_H7KCQ2S_KL",
       *productID  = "PROD0090YUAUV{2B",
       *hexChars   = "0123456789abcdef",
       buf[BUFSIZE];
  unsigned char md5Hash[16], *newHash;
  unsigned int *md5Parts, *chlStringParts, newHashParts[5];
  
  long long nHigh=0, nLow=0;
  
  int i, bigEndian;

  /* Determine our endianess */
  bigEndian = isBigEndian();

  /* Create the MD5 hash */
  snprintf(buf, BUFSIZE-1, "%s%s", input, productKey);
  MD5((unsigned char *)buf, strlen(buf), md5Hash);

  /* Split it into four integers */
  md5Parts = (unsigned int *)md5Hash;
  for(i=0; i<4; i++) {  
    /* check for endianess */
    if(bigEndian)
      md5Parts[i] = swapInt(md5Parts[i]);
    
    /* & each integer with 0x7FFFFFFF          */
    /* and save one unmodified array for later */
    newHashParts[i] = md5Parts[i];
    md5Parts[i] &= 0x7FFFFFFF;
  }
  
  /* make a new string and pad with '0' */
  snprintf(buf, BUFSIZE-5, "%s%s", input, productID);
  i = strlen(buf);
  memset(&buf[i], '0', 8 - (i % 8));
  buf[i + (8 - (i % 8))]='\0';
  
  /* split into integers */
  chlStringParts = (unsigned int *)buf;
  
  /* this is magic */
  for (i=0; i<(strlen(buf)/4)-1; i+=2) {
    long long temp;

    if(bigEndian) {
      chlStringParts[i]   = swapInt(chlStringParts[i]);
      chlStringParts[i+1] = swapInt(chlStringParts[i+1]);
    }

    temp=(md5Parts[0] * (((0x0E79A9C1 * (long long)chlStringParts[i]) % 0x7FFFFFFF)+nHigh) + md5Parts[1])%0x7FFFFFFF;
    nHigh=(md5Parts[2] * (((long long)chlStringParts[i+1]+temp) % 0x7FFFFFFF) + md5Parts[3]) % 0x7FFFFFFF;
    nLow=nLow + nHigh + temp;
  }
  nHigh=(nHigh+md5Parts[1]) % 0x7FFFFFFF;
  nLow=(nLow+md5Parts[3]) % 0x7FFFFFFF;
  
  newHashParts[0]^=nHigh;
  newHashParts[1]^=nLow;
  newHashParts[2]^=nHigh;
  newHashParts[3]^=nLow;
  
  /* swap more bytes if big endian */
  for(i=0; i<4 && bigEndian; i++)
    newHashParts[i] = swapInt(newHashParts[i]); 
  
  /* make a string of the parts */
  newHash = (unsigned char *)newHashParts;
  
  /* convert to hexadecimal */
  for (i=0; i<16; i++) {
    output[i*2]=hexChars[(newHash[i]>>4)&0xF];
    output[(i*2)+1]=hexChars[newHash[i]&0xF];
  }
  
  output[32]='\0';
}

void ParserNS::parseChl (){
	// <<< CHL 0 22940274951814684551\r\n
	// >>> QRY 11 PROD0090YUAUV{2B 32\r\nffeb4c3cf93db6a4b708b246baade0d9(no newline)
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_buf.advance (l);
		m_buf.removeFromBegin();

		char strChallengeOutTmp[32];
		challenge (s.mid (0,20).latin1(),strChallengeOutTmp);
		QString strChallengeOut (strChallengeOutTmp);

		QRY q (m_client->getIdtr());
		q.addProductId("PROD0090YUAUV{2B");
		q.addMd5(strChallengeOut);
                m_client->send (q);
                // añado a lista de comprobacion 
	}
	else m_hasCommand = false;
}

void ParserNS::parseNln (){
	// NLN trid statuscode account_name display_name clientid
	// ILN 9 NLN alice@hotmail.com Me%20and%20you 1342558244 %3C....\r\n
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
                // añado a lista de comprobacion 
		m_buf.advance (l);
		m_buf.removeFromBegin();
		QStringList fields = QStringList::split(" ",s);
		QStringList::iterator point = fields.begin();
		QString status = fields[0];
		QString passport = fields[1];
		QString displayName = fields[2].replace("%20"," ");
		QString capabilities = fields[3].replace ("\r\n","");
		if (status == "NLN") emit statusChanged (passport, STATUS_NLN, displayName, capabilities);
		else if (status == "BSY") emit statusChanged (passport, STATUS_BSY, displayName, capabilities);
		else if (status == "IDL") emit statusChanged (passport, STATUS_IDL, displayName, capabilities);
		else if (status == "BRB") emit statusChanged (passport, STATUS_BRB, displayName, capabilities);
		else if (status == "AWY") emit statusChanged (passport, STATUS_AWY, displayName, capabilities);
		else if (status == "PHN") emit statusChanged (passport, STATUS_PHN, displayName, capabilities);
		else if (status == "LUN") emit statusChanged (passport, STATUS_LUN, displayName, capabilities);
	}
	else m_hasCommand = false;
}

void ParserNS::parseUbx (){
	// 
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_buf.advance (l);
		QStringList fields = QStringList::split(" ",s);
		QStringList::iterator point = fields.begin();
		QString passport =  fields[0];
		int payload = fields[1].toInt();

		QString pay;
		int payl;
		if ((payl = m_buf.getNChar (pay,payload)) == payload){
			m_buf.advance (payl);
			m_buf.removeFromBegin();
			QString personalMsg = pay.mid(pay.find("<PSM>") + 5, pay.find("</PSM>") - pay.find("<PSM>") - 5);
//			<CurrentMedia>\0Music\01\0{0} - {1}\0Bark at the Moon\0Ozzy Osbourne\0Bark at the Moon\0{BDF1E3EF-E88C-4FC1-9A08-EB6BBABAE75C}\0</CurrentMedia>
			QString personalSong = pay.mid(pay.find("<CurrentMedia>") + 14, pay.find("</CurrentMedia>") - pay.find("<CurrentMedia>") - 14);
			if (personalSong != "") 
				printf ("MSN::Log::ParserNS ## Music::%s\n",personalSong.latin1());
			emit personalMessage(passport, personalMsg);
		}
		else m_hasCommand = false;
		
	}
	else m_hasCommand = false;
}

void ParserNS::parseBpr (){
	// 
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_buf.advance (l);
		m_buf.removeFromBegin();
		if (s.contains("HSB 1"))
			emit hasBlog (m_prevContact);

	}
	else m_hasCommand = false;
}
void ParserNS::parseFln (){
	// 
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_buf.advance (l);
		m_buf.removeFromBegin();
		QStringList fields = QStringList::split(" ",s);
		QString passport = fields[0].replace("\r\n","");
		emit statusChanged (passport,STATUS_OFF ,"","");
	}
	else m_hasCommand = false;
}

void ParserNS::parseOut (){
	// OUT OTH\r\n
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_buf.advance (l);
		m_buf.removeFromBegin();
		if (s.contains("OTH")){
			emit disconnected (ConnAtOtherPlace);
		}
	}
	else m_hasCommand = false;
}
void ParserNS::parseRng (){
	// RNG 1815263688 207.46.27.86:1863 CKI 7133130.10838165 ranita@hotmail.com me%20And%20All
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		s = s.replace("\r\n","");
		m_buf.advance (l);
		m_buf.removeFromBegin();
		QStringList fields = QStringList::split(" ",s);
		QString sessionId = fields[0];
		QString ipPort = fields[1];
		QString authType = fields[2];
		QString ticket = fields[3];
		QString passport = fields[4];
		QString personalMsg = fields[4];
		emit chatRequest (ipPort, passport, ticket, sessionId);
	}
	else m_hasCommand = false;
}

void ParserNS::parseAdg (){
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_buf.advance (l);
		m_buf.removeFromBegin();
		QStringList fields = QStringList::split(" ",s);
		QStringList::iterator point = fields.begin();
		Group* g = new Group(fields[0],fields[1].replace("\r\n",""));
		emit newGroupArrived(g);
	}
	else m_hasCommand = false;

}

void ParserNS::parse (){
	m_isParsing = true;
	QString cmd;
	int idtr;
	int lIdtr;

	QString d1;
	m_buf.data(d1);
	while (m_buf.len() && m_hasCommand){
		/* We have more data in the buffer */
		m_buf.gotoBegin();

		cmd = "";
		m_buf.getQString(cmd, 3);
		m_buf.setPosition(3);
		if (cmd == "VER"){
			printf ("MSN::Log::ParserNS ## Parsing VER\n");
			// VER 1 MSNP12 MSNP11 MSNP10 CVR0\r\n
			lIdtr = m_buf.getInt (idtr);
			m_buf.advance (1+lIdtr+1);
			// TODO : quitar de la lista de comprobacion
			parseVer();
}
		else if (cmd == "CVR"){
			printf ("MSN::Log::ParserNS ## Parsing CVR\n");
			// CVR 2 0x0409 winnt 5.1 i386 MSNMSGR 7.5.0324 msmsgs probando_msnpy@hotmail.com\r\n
			lIdtr = m_buf.getInt (idtr);
			m_buf.setPosition(3 + lIdtr);
			parseCvr();
			// TODO : quitar de la lista de comprobacion
			//parseCvr();
}
		else if (cmd == "USR"){
			printf ("MSN::Log::ParserNS ## Parsing USR\n");
			lIdtr = m_buf.getInt (idtr);
			m_buf.advance (1+lIdtr+1);
			// TODO : quitar de la lista de comprobacion
			parseUsr();
}
		else if (cmd == "XFR"){
			printf ("MSN::Log::ParserNS ## Parsing XFR\n");
			// XFR 2 NS 207.46.107.27:1863 0 207.46.28.93:1863\r\n
			lIdtr = m_buf.getInt (idtr);
			m_buf.advance (1+lIdtr+1);
			parseXfr();
			// TODO : quitar de la lista de comprobacion
}
		else if (cmd ==  "ADC"){
			printf ("MSN::Log::ParserNS ## Parsing ADC\n");
			lIdtr = m_buf.getInt (idtr);
			m_buf.setPosition(3 + lIdtr);
			// TODO : quitar de la lista de comprobacion
			//parseAdc();
}
		else if (cmd ==  "ADG"){
			printf ("MSN::Log::ParserNS ## Parsing ADG\n");
			// TODO : quitar de la lista de comprobacion
			parseAdg();
}
		else if (cmd ==  "BLP"){
			printf ("MSN::Log::ParserNS ## Parsing BLP\n");
			// TODO : quitar de la lista de comprobacion
			parseBlp();
}
		else if (cmd ==  "BPR"){
			printf ("MSN::Log::ParserNS ## Parsing BPR\n");
			// TODO : quitar de la lista de comprobacion
			parseBpr();
}
		else if (cmd ==  "CHG"){
			printf ("MSN::Log::ParserNS ## Parsing CHG\n");
			lIdtr = m_buf.getInt (idtr);
			m_buf.setPosition(3 + lIdtr);
			// TODO : quitar de la lista de comprobacion
			parseChg();
}
		else if (cmd ==  "CHL"){
			printf ("MSN::Log::ParserNS ## Parsing CHL\n");
			m_buf.advance (1 + 1 + 1); // CHL[ 0 ].....
			parseChl();
}
		else if (cmd ==  "FLN"){
			printf ("MSN::Log::ParserNS ## Parsing FLN\n");
			// TODO : quitar de la lista de comprobacion
			parseFln();
}
		else if (cmd ==  "GCF"){
			printf ("MSN::Log::ParserNS ## Parsing GCF\n");
			lIdtr = m_buf.getInt (idtr);
			m_buf.setPosition(3 + lIdtr);
			// TODO : quitar de la lista de comprobacion
			//parseGcf();
}
		else if (cmd ==  "GTC"){
			printf ("MSN::Log::ParserNS ## Parsing GTC\n");
			// TODO : quitar de la lista de comprobacion
			parseGtc();
}
		else if (cmd ==  "ILN"){
			printf ("MSN::Log::ParserNS ## Parsing ILN\n");
			lIdtr = m_buf.getInt (idtr);
			m_buf.setPosition(3 + lIdtr + 1 );
			// TODO : quitar de la lista de comprobacion
			parseNln();
}
		else if (cmd ==  "LSG"){
			printf ("MSN::Log::ParserNS ## Parsing LSG\n");
			m_buf.setPosition(3 + 1);
			// TODO : quitar de la lista de comprobacion
			parseLsg();
}
		else if (cmd ==  "LST"){
			printf ("MSN::Log::ParserNS ## Parsing LST\n");
			m_buf.setPosition(3 + 1);
			// TODO : quitar de la lista de comprobacion
			parseLst();
}
		else if (cmd ==  "NLN"){
			printf ("MSN::Log::ParserNS ## Parsing NLN\n");
			m_buf.setPosition(3);
			// TODO : quitar de la lista de comprobacion
			parseNln();
}
		else if (cmd ==  "OUT"){
			printf ("MSN::Log::ParserNS ## Parsing OUT\n");
			// TODO : quitar de la lista de comprobacion
			parseOut();
}
		else if (cmd ==  "PNG"){
			printf ("MSN::Log::ParserNS ## Parsing PNG\n");
			// TODO : quitar de la lista de comprobacion
			//parsePng();
}
		else if (cmd ==  "PRP"){
			printf ("MSN::Log::ParserNS ## Parsing PRP\n");
			// TODO : quitar de la lista de comprobacion
			parsePrp();
}
		else if (cmd ==  "QRY"){
			printf ("MSN::Log::ParserNS ## Parsing QRY\n");
			QString s;
			m_buf.data(s);
			lIdtr = m_buf.getInt (idtr);
			m_buf.setPosition(3 + lIdtr + 3); // QRY 11\r\n
			m_buf.removeFromBegin();
			// TODO : quitar de la lista de comprobacion
}
		else if (cmd ==  "SYN"){
			printf ("MSN::Log::ParserNS ## Parsing SYN\n");
			lIdtr = m_buf.getInt (idtr);
			m_buf.setPosition(3 + lIdtr);
			// TODO : quitar de la lista de comprobacion
			parseSyn();
}
		else if (cmd ==  "REA"){ // Deprecated ??
			printf ("MSN::Log::ParserNS ## Parsing REA\n");
			lIdtr = m_buf.getInt (idtr);
			m_buf.setPosition(3 + lIdtr);
			// TODO : quitar de la lista de comprobacion
			//parseRea();
}
		else if (cmd ==  "REG"){
			printf ("MSN::Log::ParserNS ## Parsing REA\n");
			// TODO : quitar de la lista de comprobacion
			//parseReg();
}
		else if (cmd ==  "REM"){
			printf ("MSN::Log::ParserNS ## Parsing REM\n");
			lIdtr = m_buf.getInt (idtr);
			m_buf.setPosition(3 + lIdtr);
			// TODO : quitar de la lista de comprobacion
			//parseRem();
}
		else if (cmd ==  "RMG"){
			printf ("MSN::Log::ParserNS ## Parsing RMG\n");
				// TODO : quitar de la lista de comprobacion
				//parseRmg();
}
		else if (cmd ==  "RNG") {
			printf ("MSN::Log::ParserNS ## Parsing RNG\n");
			// TODO : quitar de la lista de comprobacion
			parseRng();
}
		else if (cmd ==  "MSG") {
			m_buf.advance (1);
			printf ("MSN::Log::ParserNS ## Parsing MSG\n");
			parseMsg();
}
		else if (cmd ==  "SBS") {
			printf ("MSN::Log::ParserNS ## Parsing SBS\n");
			parseSbs();
}
		else if (cmd ==  "UBX") {
			printf ("MSN::Log::ParserNS ## Parsing UBX\n");
			parseUbx();
}
		else if (cmd ==  "OUT") {
			printf ("MSN::Log::ParserNS ## Disconnecting\n");
			exit(-1);
}
		else {
			// Errors:
			if (cmd ==  "715") {
				printf ("MSN::ERROR::ParserNS ## Not expected\n");
				emit disconnected (ConnNotExpected);
			}
			if (cmd ==  "540") {
				printf ("MSN::ERROR::ParserNS ## Bad MD5 digest\n");
				emit disconnected (ConnBadMd5Digest);
			}

			if (cmd ==  "800") {
				// TODO: add ->  Last switchboard fail
				QString s;
				int l;
				if ((l = m_buf.getTilChar (s,'\n')) != -1){
					m_buf.advance (l);
					m_buf.removeFromBegin();
					printf ("MSN::ERROR::ParserNS ## Too many switchboard sessions >8/60sec\n");
				}
				else m_hasCommand = false;
			}

			QString error;
			m_buf.data(error);
			printf ("MSN::ERROR::ParserNS ## Unknow Command: %s\n",error.replace('\n',"\\n").replace('\r',"\\r").latin1());
			break;			
		}
	QString d;
	m_buf.data(d);
	printf ("MSN::Log::ParserNS ## Buffer Data<%s>\n", d.replace('\n',"\\n").replace('\r',"\\r").latin1());
	}	
	m_isParsing = false;
}
}
#include "parsernotificationserver.moc"
