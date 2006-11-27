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
#include "md5.h"

namespace libimmsnp {
ParserNS::ParserNS(QString msnPassport, QString msnPass){
	m_idtr = 2;
	m_msnPassport = msnPassport;
	m_msnPass = msnPass;
	m_connectionSteps = 0;
	m_socket = 0;
	m_groups = 0;
	m_contacts = 0;
	m_initialStatus = "BSY";
}
ParserNS::ParserNS(QString msnPassport, QString msnPass, msocket* s){
	m_idtr = 2;
	m_msnPassport = msnPassport;
	m_msnPass = msnPass;
	m_connectionSteps = 0;
	m_socket = s;
	m_groups = 0;
	m_contacts = 0;
	m_initialStatus = "BSY";
}

void ParserNS::feed (Buffer b){
	m_buf << b;
}

void ParserNS::feed (QString s){
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
		if (s.contains( "MSNP11" ) || s.contains("MSNP10")){
			QString cvr ("CVR " + QString ("%1").arg(m_idtr++) + " 0x0409 winnt 5.1 i386 MSNMSGR 7.5.0324 msmsgs " + m_msnPassport + "\r\n" );
			m_socket->send (cvr);
			// añado a lista de comprobacion 
			m_buf.advance (l);
			m_buf.removeFromBegin();
			QString data;
			m_socket->recv(data);
			this->feed(data);

		}
		else {
			// return bad state
			m_buf.gotoBegin();
			return;
		}
	}
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
		QString usr ("USR " + QString ("%1").arg(m_idtr++) + " TWN I " + m_msnPassport + "\r\n");
		m_socket->send (usr);
		// añado a lista de comprobacion 
		m_buf.advance (l);
		m_buf.removeFromBegin();
		QString data;
		m_socket->recv(data);
		this->feed(data);
	}
	m_buf.gotoBegin();
//	this->parse ();
}

void ParserNS::parseXfr () {
	// XFR 2 NS 207.46.107.27:1863 0 207.46.28.93:1863\r\n
	QString s;
	int l;
	QString tmp;
	m_buf.data(tmp);
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		QString ip = s.mid(3, s.find(':')-3) ;	
		int port = s.mid(s.find(':')+1,4).toInt() ;	
		
		//qDebug("IP:" + ip + "#" + QString("%1").arg(port) + "#");
		delete m_socket;
		m_socket = new msocket(ip,port);
		m_socket->connect();
		// TODO: add signal to connection.cpp and client.cpp
		emit mainSocket(m_socket);

		// Restart secuence of authentication.
		QString msg ("VER " + QString ("%1").arg(m_idtr++) + " MSNP11 MSNP10 CVR0\r\n");
		 
		if (m_socket->send (msg) == 0) {
			return ;
		}

		QString data;
		if (m_socket->recv (data) == 0) {
			return ;
		}

		m_buf.advance (l);
		m_buf.removeFromBegin();

		this->feed (data);
	}
	m_buf.gotoBegin();
	QString ss; 
	m_buf.data(ss);
//	this->parse ();
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
			m_ticket = s.mid (s.find ("lc"),s.length()-s.find ("lc")-2);
			// TODO: if ticket = "" login failed
			//qDebug (m_ticket);

			// start Twenner
			std::string tmpUrl = httpsReq ("https://nexus.passport.com/rdr/pprdr.asp", "");
			int iniUrl = tmpUrl.find("DALogin=")+8;
			int finUrl = tmpUrl.substr(iniUrl, tmpUrl.size() - iniUrl).find (",DAReg=");

			std::string url("https://");
			url += tmpUrl.substr(iniUrl,finUrl);
			//qDebug (QString(url));

			std::string auth = std::string("Authorization: Passport1.4 OrgVerb=GET,OrgURL=http%3A%2F%2Fmessenger%2Emsn%2Ecom,sign-in=") + m_msnPassport.replace('@', "%40").replace('.', "%2E").latin1() + ",pwd=" +  m_msnPass.replace('@', "%40").replace('.', "%2E").latin1() + "," + m_ticket.latin1();

      			// Don't urlEncode the m_ticket, only the id and password.
 			m_ticket = QString (httpsReq (url, auth));
			QString usr ("USR " + QString ("%1").arg(m_idtr++) + " TWN S " + m_ticket + "\r\n");
			//qDebug (usr);
			m_socket->send (usr);
			// añado a lista de comprobacion 
			m_buf.advance (l);
			m_buf.removeFromBegin();
			QString data;
			m_socket->recv(data);
			this->feed(data);
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

	m_buf.gotoBegin();
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
		QString syn ("SYN " + QString ("%1").arg(m_idtr++) + " 2005-04-23T18:57:44.8130000-07:00 2005-04-23T18:57:54.2070000-07:00\r\n");
		m_socket->send (syn);
		// añado a lista de comprobacion 
		QString data;
		m_socket->recv(data);
		this->feed(data);
	}
}

void ParserNS::parseSyn () {
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		QString g = s.mid (s.findRev(" "), s.length () - s.findRev(" ") - 2 );
		QString ctmp = s.mid (0, s.find (g + "\r\n"));
		QString c = s.mid(ctmp.findRev(" "), ctmp.length () - ctmp.findRev(" "));
		m_contacts = c.toInt();
		m_groups = g.toInt();

		m_buf.advance (l);
		m_buf.removeFromBegin();
	}
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
}

void ParserNS::parsePrp () {
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		if (s.contains ("MFN"))
			qDebug ("!!PRP My name is");
			// TODO: emit signal sending my name
		else if (s.contains ("PHH") || s.contains ("PHW")  || s.contains ("PHM") || s.contains ("MOB") || s.contains ("MBE") ||  s.contains ("WWE"))
			qDebug ("!!PRP Telefonos");
		m_buf.advance (l);
		m_buf.removeFromBegin();
	}
}

void ParserNS::parseLsg (){
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_groups--;
		qDebug ("==== m_groups :" + QString("%1").arg(m_groups));
		m_buf.advance (l);
		m_buf.removeFromBegin();
	}

}
void ParserNS::parseLst (){
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_contacts--;
		qDebug ("==== m_contacts :" + QString("%1").arg(m_contacts));
		m_buf.advance (l);
		m_buf.removeFromBegin();
	}
	if (m_contacts == 0) {
		QString chg ("CHG " + QString ("%1").arg(m_idtr++) + " " +  m_initialStatus + " 1342558252" + "\r\n");
		m_socket->send (chg);
                // añado a lista de comprobacion 
                QString data;
                m_socket->recv(data);
                this->feed(data);
	}
}

	

void ParserNS::parseSbs (){
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_buf.advance (l);
		m_buf.removeFromBegin();
	}

}

void ParserNS::parseChg (){
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
		m_buf.advance (l);
		m_buf.removeFromBegin();
		emit connected();
	}
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
		challenge (s.mid (6,20).latin1(),strChallengeOutTmp);
		QString strChallengeOut (strChallengeOutTmp);
		QString qry ("QRY " + QString ("%1").arg(m_idtr++) + " PROD0090YUAUV{2B 32/r/n" + strChallengeOut);
                m_socket->send (qry);
                // añado a lista de comprobacion 
                QString data;
                m_socket->recv(data);
                this->feed(data);
	}
}

void ParserNS::parseIln (){
	// ILN 9 AWY vaticano666@hotmail.com pedro 268435488
	QString s;
	int l;
	if ((l = m_buf.getTilChar (s,'\n')) != -1){
                // añado a lista de comprobacion 
		m_buf.advance (l);
		m_buf.removeFromBegin();
	}
}

void ParserNS::parse (){
	QString cmd;
	int idtr;
	int lIdtr;

	QString d1;
	m_buf.data(d1);
//	qDebug ("Buff vacioIni:" + d1.replace('\n',"\\n").replace('\r',"\\r") + "#");
	while (m_buf.len()){
		/* We have more data in the buffer */
		m_buf.gotoBegin();

		cmd = "";
		m_buf.getQString(cmd, 3);

		m_buf += 3; // The command

		m_buf.setPosition(3);
		//qDebug ("## cmd:" + cmd);
		if (cmd == "VER"){
				qDebug ("Parsing VER");
				// VER 1 MSNP12 MSNP11 MSNP10 CVR0\r\n
				lIdtr = m_buf.getInt (idtr);;
				m_buf.advance (1+lIdtr+1);
				m_buf +=(1+lIdtr+1); // space int space

				// TODO : quitar de la lista de comprobacion
				parseVer();
				//qDebug (cmd + ::QString("%1").arg(idtr));
				//qDebug (cmd);
				//break;			
}
		else if (cmd == "CVR"){
				qDebug ("Parsing CVR");
				// CVR 2 0x0409 winnt 5.1 i386 MSNMSGR 7.5.0324 msmsgs probando_msnpy@hotmail.com\r\n
				lIdtr = m_buf.getInt (idtr);;
				m_buf.setPosition(3 + lIdtr);
				parseCvr();
				// TODO : quitar de la lista de comprobacion
				//parseCvr();
				//break;			
}
		else if (cmd == "USR"){
				qDebug ("Parsing USR");
				lIdtr = m_buf.getInt (idtr);;
				m_buf.advance (1+lIdtr+1);
				// TODO : quitar de la lista de comprobacion
				parseUsr();
				//break;			
}
		else if (cmd == "XFR"){
				qDebug ("Parsing XFR");
				// XFR 2 NS 207.46.107.27:1863 0 207.46.28.93:1863\r\n
				lIdtr = m_buf.getInt (idtr);;
				m_buf.advance (1+lIdtr+1);
				parseXfr();
				// TODO : quitar de la lista de comprobacion
				//parseUsr();
				//break;			
}
		else if (cmd ==  "ADC"){
				qDebug ("Parsing ADC");
				lIdtr = m_buf.getInt (idtr);;
				m_buf.setPosition(3 + lIdtr);
				// TODO : quitar de la lista de comprobacion
				//parseAdc();
				//break;			
}
		else if (cmd ==  "ADG"){
				qDebug ("Parsing ADG");
				// TODO : quitar de la lista de comprobacion
				//parseAdg();
				//break;			
}
		else if (cmd ==  "BLP"){
				qDebug ("Parsing BLP");
				// TODO : quitar de la lista de comprobacion
				parseBlp();
				//break;			
}
		else if (cmd ==  "BPR"){
				qDebug ("Parsing BPR");
				// TODO : quitar de la lista de comprobacion
				//parseBpr();
				//break;			
}
		else if (cmd ==  "CHG"){
				qDebug ("Parsing CHG");
				lIdtr = m_buf.getInt (idtr);
				m_buf.setPosition(3 + lIdtr);
				// TODO : quitar de la lista de comprobacion
				parseChg();
				//break;			
}
		else if (cmd ==  "CHL"){
				qDebug ("Parsing CHL");
				lIdtr = m_buf.getInt (idtr);
				m_buf.setPosition(3 + lIdtr);
				// TODO : quitar de la lista de comprobacion
				parseChl();
				//break;			
}
		else if (cmd ==  "FLN"){
				qDebug ("Parsing FLN");
				// TODO : quitar de la lista de comprobacion
				//parseFln();
				//break;			
}
		else if (cmd ==  "GCF"){
				qDebug ("Parsing GCF");
				lIdtr = m_buf.getInt (idtr);
				m_buf.setPosition(3 + lIdtr);
				// TODO : quitar de la lista de comprobacion
				//parseChl();
				//break;			
}
		else if (cmd ==  "GTC"){
				qDebug ("Parsing GTC");
				// TODO : quitar de la lista de comprobacion
				parseGtc();
				//break;			
}
		else if (cmd ==  "ILN"){
				qDebug ("Parsing ILN");
				lIdtr = m_buf.getInt (idtr);
				m_buf.setPosition(3 + lIdtr);
				// TODO : quitar de la lista de comprobacion
				parseIln();
				//break;			
}
		else if (cmd ==  "LSG"){
				qDebug ("Parsing LSG");
				// TODO : quitar de la lista de comprobacion
				parseLsg();
				//break;			
}
		else if (cmd ==  "LST"){
				qDebug ("Parsing LST");
				// TODO : quitar de la lista de comprobacion
				parseLst();
				//break;			
}
		else if (cmd ==  "NLN"){
				qDebug ("Parsing NLN");
				// TODO : quitar de la lista de comprobacion
				//parseNln();
				//break;			
}
		else if (cmd ==  "OUT"){
				qDebug ("Parsing OUT");
				// TODO : quitar de la lista de comprobacion
				//parseOut();
				//break;			
}
		else if (cmd ==  "PNG"){
				qDebug ("Parsing PNG");
				// TODO : quitar de la lista de comprobacion
				//parsePng();
				//break;			
}
		else if (cmd ==  "PRP"){
				qDebug ("Parsing PRP");
				// TODO : quitar de la lista de comprobacion
				parsePrp();
				//break;			
}
		else if (cmd ==  "QNG"){
				qDebug ("Parsing QNG");
				// TODO : quitar de la lista de comprobacion
				//parseQng();
				//break;			
}
		else if (cmd ==  "QRY"){
				qDebug ("Parsing QRY");
				lIdtr = m_buf.getInt (idtr);
				m_buf.setPosition(3 + lIdtr + 3); // QRY 11\r\n
				m_buf.removeFromBegin();
				// TODO : quitar de la lista de comprobacion
				//break;			
}
		else if (cmd ==  "SYN"){
				//qDebug ("Parsing SYN");
				lIdtr = m_buf.getInt (idtr);
				m_buf.setPosition(3 + lIdtr);
				// TODO : quitar de la lista de comprobacion
				parseSyn();
				//break;			
}
		else if (cmd ==  "REA"){ // Deprecated ??
				qDebug ("Parsing REA");
				lIdtr = m_buf.getInt (idtr);
				m_buf.setPosition(3 + lIdtr);
				// TODO : quitar de la lista de comprobacion
				//parseRea();
				//break;			
}
		else if (cmd ==  "REG"){
				qDebug ("Parsing REA");
				// TODO : quitar de la lista de comprobacion
				//parseReg();
				//break;			
}
		else if (cmd ==  "REM"){
				qDebug ("Parsing REM");
				lIdtr = m_buf.getInt (idtr);
				m_buf.setPosition(3 + lIdtr);
				// TODO : quitar de la lista de comprobacion
				//parseRem();
				//break;			
}
		else if (cmd ==  "RMG"){
				qDebug ("Parsing RMG");
				// TODO : quitar de la lista de comprobacion
				//parseRmg();
				//break;			
}
		else if (cmd ==  "RNG") {
			qDebug ("Parsing RNG");
			// TODO : quitar de la lista de comprobacion
			//parseRng();
			//break;			
}
		else if (cmd ==  "MSG") {
			qDebug ("Parsing MSG");
			parseMsg();
			//break;			
}
		else if (cmd ==  "SBS") {
			qDebug ("Parsing SBS");
			parseSbs();
			//break;			
}
		else {	// pueden venir errores 715 1\r\n
			// quitar el primero 
			// reañadir al buffer ppal
			QString error;
			m_buf.data(error);
			qDebug ("UNKNOW command: " + error.replace('\n',"\\n").replace('\r',"\\r"));
			break;			
		}
	QString d;
	int len = m_buf.data(d);
	qDebug ("####" + d + "#### len:" + QString ("%1").arg(len) );
	}	


	// emit buffer empty
	QString d;
	m_buf.data(d);
//	qDebug ("Buff vacio:" + d.replace('\n',"\\n").replace('\r',"\\r") + "#");
	emit bufferEmpty();
}
}
//#include "parsernotificationserver.moc"
