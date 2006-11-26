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

#include "connection.h"
#include "commands.h"
#include "md5.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>
#include <curl/curl.h>

namespace libimmsnp {
	
	Connection::Connection (ParserNS* parser, QString server, int port ,QString msnPassport, QString msnPass, msocket* socket)
		: m_parser( parser ), m_state ( StateDisconnected ), m_disconnect ( ConnNoError ),
		  m_buf(), m_server( server ),m_port( port ), m_msnPassport ( msnPassport ), m_msnPass ( msnPass ),
		  m_socket(socket), m_bufsize( 20000 ),m_cancel( true ),m_idtr(1) {
	}

 	Connection::~Connection() {
		delete m_socket;
		std::cout << "Connection closed with : " << m_server << ":" << m_port << "\n";
	}

	int  Connection::setTicket (std::string ticket){
		m_ticket = ticket;
		return 0;
	}

	std::string Connection::itos (int i){
		std::ostringstream sstr;
		sstr << i;
		std::string str (sstr.str());
		return str;
	}
		
	ConnectionError Connection::VER (){
		Commands cmd;
		QString msg ("VER 1 MSNP11 MSNP10 CVR0\r\n");
		 
	 	if (m_socket->send (msg) == 0) {
			return ConnErrorVER;
		}

		QString data;
	 	if (m_socket->recv (data) == 0) {
	 		return ConnErrorVER;
	 	}
		m_parser->feed (data);
		m_parser->parse();
		return ConnNoError;
		
	}
	
	ConnectionError Connection::CVR (){
		Commands cmd;
		std::string msg;
		msg = cmd.CVR(m_idtr,m_msnPassport);
		 
		m_idtr += 1;
		
	 	if (m_socket->send (msg) == 0) {
			return ConnErrorCVR;
		}
		
	 	if (m_socket->recv (m_buf) == 0) {
	 		return ConnErrorCVR;
	 	}
		return ConnNoError;
		
	}
	
	ConnectionError Connection::USRIni (){
		Commands cmd;
		std::string msg;
		msg = cmd.USRIni(m_idtr, m_msnPassport);
		 
		m_idtr += 1;
		
	 	if (m_socket->send (msg) == 0) {
			return ConnErrorUSR;
		}

	 	if (m_socket->recv (m_buf) == 0) {
	 		return ConnErrorUSR;
	 	}
		return ConnNoError;
		
	}
	
	ConnectionError Connection::USRFin (){
		Commands cmd;
		std::string msg;
		std::string tmpBuf;
		msg = cmd.USRFin(m_idtr, m_ticket);
		 
		m_idtr += 1;
	 	if (m_socket->send (msg) == 0) {
			return ConnErrorUSR;
		}

	 	if (m_socket->recv (tmpBuf) == 0) {
	 		return ConnErrorUSR;
	 	}
//		m_buf = tmpBuf;
		
		//if (m_buf.size() >= 4) {
		//	while (m_buf.substr( m_buf.size()-4,4) != "\r\n\r\n"){
	 	//		if (m_socket->recv (tmpBuf) == 0) {
	 	//			return ConnErrorUSR;
	 	//		}
		//		m_buf += tmpBuf;
		//	}
		//}
		return ConnNoError;
		
	}
	
	ConnectionError Connection::SYN (){
		Commands cmd;
		std::string msg;
		msg = cmd.SYN(m_idtr,"0");
		 
		m_idtr += 1;
		
	 	if (m_socket->send (msg) == 0) {
			return ConnErrorUSR;
		}
	 	if (m_socket->recv (m_buf) == 0) {
	 		return ConnErrorUSR;
	 	}

		return ConnNoError;
		
	}
	
	ConnectionError Connection::CHG (std::string initialStatus){
		Commands cmd;
		std::string msg;
		// "1342210084"
		// oficial 7.5 = 1342558252
		msg = cmd.CHG(m_idtr, "1342558252", initialStatus);
		 
		m_idtr += 1;
		
	 	if (m_socket->send (msg) == 0) {
			return ConnErrorUSR;
		}

	 	if (m_socket->recv (m_buf) == 0) {
	 		return ConnErrorUSR;
	 	}
		return ConnNoError;
		
	}


	// This pieze of code  is based on code http://msnpiki.msnfanatic.com/extra/CHL_Cpp.zip
	void Connection::challenge (const char *szChallenge, char *szOutput) {
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
	
	ConnectionError Connection::QRY (std::string challenge){
		Commands cmd;
		std::string msg;	
		msg = cmd.QRY(m_idtr, challenge);
		 
		m_idtr += 1;
		
	 	if (m_socket->send (msg) == 0) {
			return ConnErrorUSR;
		}

	 	if (m_socket->recv (m_buf) == 0) {
	 		return ConnErrorUSR;
	 	}
		return ConnNoError;
		
	}
	
	std::string Connection::replace (std::string& linea, std::string oldChr, std::string newChr){
		while (1) {
			const unsigned  int pos = linea.find(oldChr);
			if (pos == std::string::npos) break;
			linea.replace(pos,1,newChr);
		}
		return linea;
		
	}

	std::string Connection::urlEncode (std::string& linea){
		Connection::replace (linea, "@", "%40");
		Connection::replace (linea, ".", "%2E");
		return linea;
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
		 
		
	ConnectionError Connection::Tweener() {
		
		std::string tmpUrl = httpsReq ("https://nexus.passport.com/rdr/pprdr.asp", "");
		int iniUrl = tmpUrl.find("DALogin=")+8;
		int finUrl = tmpUrl.substr(iniUrl, tmpUrl.size() - iniUrl).find (",DAReg=");

		std::string url("https://");
		url += tmpUrl.substr(iniUrl,finUrl);

//		std::string auth = std::string("Authorization: Passport1.4 OrgVerb=GET,OrgURL=http%3A%2F%2Fmessenger%2Emsn%2Ecom,sign-in=") + Connection::urlEncode(m_msnPassport) + ",pwd=" +  Connection::urlEncode(m_msnPass) + "," + m_ticket;
		// Don't urlEncode the m_ticket, only the id and password.
// 		if ((m_ticket = httpsReq (url, auth)) != ""){
//			return ConnNoError;
//		}
//		else return ConnErrorTwr; 
	}

	std::string Connection::httpsReq (std::string url, std::string headers) {
		CURL *curl;
		CURLcode ret;
		curl_slist *slist = NULL;

		std::string cookie; 

		std::cout << "START htpps request to " << url << "\n";
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
			std::cout << "END https request to " << url << "\n";
			return cookie;
		}
		std::cout <<  "ERROR making request to " << url << "\n";
		return "";
	}

	void Connection::connect(std::string initialStatus) {

		
		// Start secuence of authentication.
		VER();
//
//		QString msg ("VER 1 MSNP12 MSNP11 MSNP10 CVR0\r\n");
//		 
//	 	m_socket->send (msg);
//		QString data;
//	 	m_socket->recv (data); 
//		QString msg1 ("CVR 2 0x0409 winnt 5.1 i386 MSNMSGR 7.5.0324 msmsgs probando_msnpy@hotmail.com\r\n");
//	 	m_socket->send (msg1);
//		QString data1;
//	 	m_socket->recv (data1); 
//		qDebug ("Paso 1");
//		CVR();
//		USRIni();
//		
//		// New parser host:port
//		int posXfr;
//		int posIniIP;
//		int tamIP;
//		int posIniPort;
//		int tamPort;
//		
//		if ((posXfr = m_buf.find ("XFR")) != 0){
//			//return ConnErrorXFR;
//		}
//		else {
//			posIniIP = m_buf.find ("S")+2;
//			tamIP = m_buf.find (":") - posIniIP;
//			posIniPort = m_buf.find (":")+1;
//			tamPort = 4; 
//
//			m_server = m_buf.substr(posIniIP,tamIP);
//			m_port = std::atoi ( m_buf.substr(posIniPort,tamPort).c_str());
//		}
//		// End new parser host:port
//
//		// Connect with new host
//		delete m_socket;
//		m_socket = new msocket(m_server,m_port);
//		m_socket->connect();
//		emit mainSocket(m_socket);
//		
//		// Restart secuence of authentication.
//		VER();
//		CVR();
//		USRIni();
//
//		
//		// Start ticket parser
//		m_ticket = m_buf.substr (m_buf.find ("lc"),m_buf.size()-m_buf.find ("lc")-2);
//		
//		// End ticket parser
//
//		Tweener();
//
//		USRFin();
//	
//		SYN(); // ask for roaster
//
//		if (m_parser->makeRoaster (m_socket, m_buf) == PARSER_OK){
//
//			CHG (initialStatus); // set initial state
//
//			emit connected();
//
//			int lenMBuf;
//			while (1){
//
//				// Socket error -> Disconnect
//				if ((lenMBuf = (m_socket->recv (m_buf))) == -1) {
//					delete m_parser;
//					m_parser = 0;
//					delete m_socket;
//					m_socket = 0;
//					emit disconnected();
//				}
//
//				// Last step to be connected
//				if (m_buf.substr(0,3) == "CHL"){
//					char strChallengeOutTmp[32]; 
//					challenge (m_buf.substr(6,20).c_str(),strChallengeOutTmp);
//					std::string strChallengeOut (strChallengeOutTmp);
//					QRY (strChallengeOut.substr(0,32));
//				}
//				else {
//				m_parser->feed(m_buf);
//				}
//			}
//		}
	}
}

#include "connection.moc"
