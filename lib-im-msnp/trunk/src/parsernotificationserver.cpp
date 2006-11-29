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
ParserNS::ParserNS(QString msnPassport, QString msnPass, QString initialStatus, Client* c){
	m_idtr = 2;
	m_msnPassport = msnPassport;
	m_msnPass = msnPass;
	m_connectionSteps = 0;
	m_socket = 0;
	m_groups = 0;
	m_contacts = 0;
	m_initialStatus = initialStatus;
	m_client = c;
	m_hasCommand = false;
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

		QString ip   = s.mid (3, s.find (':') - 3);	
		int     port = s.mid (s.find(':')+1, 4).toInt() ;	

		m_client->makeConnection(ip, port);
		
		// Restart secuence of authentication.
		VER v(m_client->getIdtr());
		v.addProtocolSupported("MSNP12");
		v.addProtocolSupported("MSNP11");
		v.addProtocolSupported("MSNP10");
		m_client->send(v);

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

	qDebug ("START htpps request to " + QString(url));
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
		qDebug ("END https request to " + QString(url));
		return cookie;
	}
	qDebug ("ERROR making request to " + QString(url));
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
			// TODO: if ticket = "" login failed
			//qDebug (m_ticket);

			// start Twenner
			std::string tmpUrl = httpsReq ("https://nexus.passport.com/rdr/pprdr.asp", "");
			int iniUrl = tmpUrl.find("DALogin=")+8;
			int finUrl = tmpUrl.substr(iniUrl, tmpUrl.size() - iniUrl).find (",DAReg=");

			std::string url("https://");
			url += tmpUrl.substr(iniUrl,finUrl);

			std::string auth = std::string("Authorization: Passport1.4 OrgVerb=GET,OrgURL=http%3A%2F%2Fmessenger%2Emsn%2Ecom,sign-in=") + m_msnPassport.replace('@', "%40").replace('.', "%2E").latin1() + ",pwd=" +  m_msnPass.replace('@', "%40").replace('.', "%2E").latin1() + "," + m_ticket.latin1();

      			// Don't urlEncode the m_ticket, only the id and password.
 			m_ticket = QString (httpsReq (url, auth));

			USR u(m_client->getIdtr());
			u.addTwnType ("S");
			u.addTicket (m_ticket);
			m_client->send (u);

			// añado a lista de comprobacion 
		}
		else if (s.contains ("OK ")){
			qDebug ("!! USR OK");
	                m_buf.advance (l);
	                m_buf.removeFromBegin();
		}
		else {
			qDebug ("!! OTHER USR OK:" + s + "#");
	                m_buf.advance (l);
	                m_buf.removeFromBegin();
		}
	}
	else m_hasCommand = false;

//	m_buf.gotoBegin();
//	this->parse ();
}

void ParserNS::parseMsg (){
	
	QString a;
	m_buf.data(a);
	if (a.contains ("Content-Type: text/x-msmsgsprofile")){
		int l = a.find ("\r\n\r\n") + 4;
		m_buf.gotoBegin();
		m_buf.advance (l);
		m_buf.removeFromBegin();

		SYN s (m_client->getIdtr());
		m_client->send (s);
		// añado a lista de comprobacion 
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
			qDebug ("!!GTC OK");
		else if (s.contains ("N"))
			qDebug ("!!GTC Not OK");
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
			qDebug ("!!BLP Chat YES");
		else if (s.contains ("BL"))
			qDebug ("!!BLP Chat NO");
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
			qDebug ("!!PRP My name is");
			// TODO: emit signal sending my name
		else if (s.contains ("PHH") || s.contains ("PHW")  || s.contains ("PHM") || s.contains ("MOB") || s.contains ("MBE") ||  s.contains ("WWE"))
			qDebug ("!!PRP Telefonos");
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
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_buf.advance (l);
		m_buf.removeFromBegin();
		m_contacts--;
		QStringList fields = QStringList::split(" ",s);
		QStringList::iterator point = fields.begin();
		Contact* c = new Contact;
		c->setPassport (fields[0].replace("N=",""));
		c->setNickName (fields[1].replace("F=","")); // TODO: remove replace
		c->setId (fields[2].replace("C=",""));
		c->setList (fields[3]);
		m_prevContact = fields[0];
		emit newContactArrived (c); 
	}
	else m_hasCommand = false;
	if (m_contacts == 0) {
		CHG c (m_client->getIdtr());
		c.addStatusCode (m_initialStatus);
		c.addClientId ("1342558252");
		qDebug ("#################");
		qDebug ("#Roster received#");
		qDebug ("#################");
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
		emit connected();
	}
	else m_hasCommand = false;
}

// This pieze of code  is based on code http://msnpiki.msnfanatic.com/extra/CHL_Cpp.zip
void challenge (const char *szChallenge, char *szOutput) {
	const char *szClientID="PROD0090YUAUV{2B";
	const char *szClientCode="YMM8C_H7KCQ2S_KL";
	int i;
	MD5_CTX mdContext;
	MD5Init(&mdContext);
	MD5Update(&mdContext,(unsigned char *)szChallenge,strlen(szChallenge));
	MD5Update(&mdContext,(unsigned char *)szClientCode,strlen(szClientCode));
	MD5Final(&mdContext);
	unsigned char pMD5Hash[16];
	memcpy(pMD5Hash,mdContext.digest,16);
	int *pMD5Parts=(int *)mdContext.digest;
	
	for (i=0; i<4; i++) {
		pMD5Parts[i]&=0x7FFFFFFF;
	}
	
	int nchlLen=strlen(szChallenge)+strlen(szClientID);
	if (nchlLen%8!=0)
		nchlLen+=8-(nchlLen%8);
	char *chlString=new char[nchlLen];
	memset(chlString,'0',nchlLen);
	memcpy(chlString,szChallenge,strlen(szChallenge));
	memcpy(chlString+strlen(szChallenge),szClientID,strlen(szClientID));
	int *pchlStringParts=(int *)chlString;

	long long nHigh=0;
	long long nLow=0;

	for (i=0; i<(nchlLen/4)-1; i+=2) {
		long long temp=pchlStringParts[i];
		temp=(pMD5Parts[0] * (((0x0E79A9C1 * (long long)pchlStringParts[i]) % 0x7FFFFFFF)+nHigh) + pMD5Parts[1])%0x7FFFFFFF;
		nHigh=(pMD5Parts[2] * (((long long)pchlStringParts[i+1]+temp) % 0x7FFFFFFF) + pMD5Parts[3]) % 0x7FFFFFFF;
		nLow=nLow + nHigh + temp;
	}
	
	nHigh=(nHigh+pMD5Parts[1]) % 0x7FFFFFFF;
	nLow=(nLow+pMD5Parts[3]) % 0x7FFFFFFF;
	delete[] chlString;

	unsigned long *pNewHash=(unsigned long *)pMD5Hash;
	
	pNewHash[0]^=nHigh;
	pNewHash[1]^=nLow;
	pNewHash[2]^=nHigh;
	pNewHash[3]^=nLow;

	char szHexChars[]="0123456789abcdef";
	
	for (i=0; i<16; i++) {
		szOutput[i*2]=szHexChars[(pMD5Hash[i]>>4)&0xF];
		szOutput[(i*2)+1]=szHexChars[pMD5Hash[i]&0xF];
	}
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
		QString displayName = fields[2];
		QString capabilities = fields[3];
		emit statusChanged (passport, status, displayName, capabilities);
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
				qDebug ("## MUSIC --->" + personalSong + "#");
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
		QStringList::iterator point = fields.begin();
		QString passport = fields[0].replace("\r\n","");
		emit statusChanged (passport, "DIS","","");
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

		m_buf += 3; // The command

		m_buf.setPosition(3);
		if (cmd == "VER"){
				qDebug ("Parsing VER");
				// VER 1 MSNP12 MSNP11 MSNP10 CVR0\r\n
				lIdtr = m_buf.getInt (idtr);;
				m_buf.advance (1+lIdtr+1);
				// TODO : quitar de la lista de comprobacion
				parseVer();
}
		else if (cmd == "CVR"){
				qDebug ("Parsing CVR");
				// CVR 2 0x0409 winnt 5.1 i386 MSNMSGR 7.5.0324 msmsgs probando_msnpy@hotmail.com\r\n
				lIdtr = m_buf.getInt (idtr);;
				m_buf.setPosition(3 + lIdtr);
				parseCvr();
				// TODO : quitar de la lista de comprobacion
				//parseCvr();
}
		else if (cmd == "USR"){
				qDebug ("Parsing USR");
				lIdtr = m_buf.getInt (idtr);;
				m_buf.advance (1+lIdtr+1);
				// TODO : quitar de la lista de comprobacion
				parseUsr();
}
		else if (cmd == "XFR"){
				qDebug ("Parsing XFR");
				// XFR 2 NS 207.46.107.27:1863 0 207.46.28.93:1863\r\n
				lIdtr = m_buf.getInt (idtr);;
				m_buf.advance (1+lIdtr+1);
				parseXfr();
				// TODO : quitar de la lista de comprobacion
}
		else if (cmd ==  "ADC"){
				qDebug ("Parsing ADC");
				lIdtr = m_buf.getInt (idtr);;
				m_buf.setPosition(3 + lIdtr);
				// TODO : quitar de la lista de comprobacion
				//parseAdc();
}
		else if (cmd ==  "ADG"){
				qDebug ("Parsing ADG");
				// TODO : quitar de la lista de comprobacion
				//parseAdg();
}
		else if (cmd ==  "BLP"){
				qDebug ("Parsing BLP");
				// TODO : quitar de la lista de comprobacion
				parseBlp();
}
		else if (cmd ==  "BPR"){
				qDebug ("Parsing BPR");
				// TODO : quitar de la lista de comprobacion
				parseBpr();
}
		else if (cmd ==  "CHG"){
				qDebug ("Parsing CHG");
				lIdtr = m_buf.getInt (idtr);
				m_buf.setPosition(3 + lIdtr);
				// TODO : quitar de la lista de comprobacion
				parseChg();
}
		else if (cmd ==  "CHL"){
				qDebug ("Parsing CHL");
				m_buf.advance (1 + 1 + 1); // CHL[ 0 ].....
				parseChl();
}
		else if (cmd ==  "FLN"){
				qDebug ("Parsing FLN");
				// TODO : quitar de la lista de comprobacion
				parseFln();
}
		else if (cmd ==  "GCF"){
				qDebug ("Parsing GCF");
				lIdtr = m_buf.getInt (idtr);
				m_buf.setPosition(3 + lIdtr);
				// TODO : quitar de la lista de comprobacion
				//parseGcf();
}
		else if (cmd ==  "GTC"){
				qDebug ("Parsing GTC");
				// TODO : quitar de la lista de comprobacion
				parseGtc();
}
		else if (cmd ==  "ILN"){
				qDebug ("Parsing ILN");
				lIdtr = m_buf.getInt (idtr);
				m_buf.setPosition(3 + lIdtr + 1 );
				// TODO : quitar de la lista de comprobacion
				parseNln();
}
		else if (cmd ==  "LSG"){
				qDebug ("Parsing LSG");
				m_buf.setPosition(3 + 1);
				// TODO : quitar de la lista de comprobacion
				parseLsg();
}
		else if (cmd ==  "LST"){
				qDebug ("Parsing LST");
				m_buf.setPosition(3 + 1);
				// TODO : quitar de la lista de comprobacion
				parseLst();
}
		else if (cmd ==  "NLN"){
				qDebug ("Parsing NLN");
				m_buf.setPosition(3);
				// TODO : quitar de la lista de comprobacion
				parseNln();
}
		else if (cmd ==  "OUT"){
				qDebug ("Parsing OUT");
				// TODO : quitar de la lista de comprobacion
				//parseOut();
}
		else if (cmd ==  "PNG"){
				qDebug ("Parsing PNG");
				// TODO : quitar de la lista de comprobacion
				//parsePng();
}
		else if (cmd ==  "PRP"){
				qDebug ("Parsing PRP");
				// TODO : quitar de la lista de comprobacion
				parsePrp();
}
		else if (cmd ==  "QRY"){
				qDebug ("Parsing QRY");
				QString s;
				m_buf.data(s);
				qDebug(s);
				lIdtr = m_buf.getInt (idtr);
				m_buf.setPosition(3 + lIdtr + 3); // QRY 11\r\n
				m_buf.removeFromBegin();
				// TODO : quitar de la lista de comprobacion
}
		else if (cmd ==  "SYN"){
				//qDebug ("Parsing SYN");
				lIdtr = m_buf.getInt (idtr);
				m_buf.setPosition(3 + lIdtr);
				// TODO : quitar de la lista de comprobacion
				parseSyn();
}
		else if (cmd ==  "REA"){ // Deprecated ??
				qDebug ("Parsing REA");
				lIdtr = m_buf.getInt (idtr);
				m_buf.setPosition(3 + lIdtr);
				// TODO : quitar de la lista de comprobacion
				//parseRea();
}
		else if (cmd ==  "REG"){
				qDebug ("Parsing REA");
				// TODO : quitar de la lista de comprobacion
				//parseReg();
}
		else if (cmd ==  "REM"){
				qDebug ("Parsing REM");
				lIdtr = m_buf.getInt (idtr);
				m_buf.setPosition(3 + lIdtr);
				// TODO : quitar de la lista de comprobacion
				//parseRem();
}
		else if (cmd ==  "RMG"){
				qDebug ("Parsing RMG");
				// TODO : quitar de la lista de comprobacion
				//parseRmg();
}
		else if (cmd ==  "RNG") {
			qDebug ("Parsing RNG");
			// TODO : quitar de la lista de comprobacion
			//parseRng();
}
		else if (cmd ==  "MSG") {
			qDebug ("Parsing MSG");
			parseMsg();
}
		else if (cmd ==  "SBS") {
			qDebug ("Parsing SBS");
			parseSbs();
}
		else if (cmd ==  "UBX") {
			qDebug ("Parsing UBX");
			parseUbx();
}
		else {	// pueden venir errores 715 1\r\n
			// quitar el primero 
			// reañadir al buffer ppal
			//
			// Errors:
			if (cmd ==  "715") {
				qDebug ("####################");
				qDebug ("### Not expected ###");
				qDebug ("####################");
				exit (-1);
			}
			if (cmd ==  "540") {
				qDebug ("######################");
				qDebug ("### Bad MD5 digest ###");
				qDebug ("######################");
				exit (-1);
			}

			QString error;
			m_buf.data(error);
			qDebug ("UNKNOW command: " + error.replace('\n',"\\n").replace('\r',"\\r"));
			m_buf.gotoBegin();
			QString bad;
			m_buf.getQString (bad,1);	
			printf ("%s",bad.latin1());
			m_buf.advance(1);
			m_buf.removeFromBegin();
			break;			
		}
	QString d;
	int len = m_buf.data(d);
	printf ("#### Data:%s\n", d.replace('\n',"\\n").replace('\r',"\\r").latin1());
	}	
	m_isParsing = false;
}
}
#include "parsernotificationserver.moc"
