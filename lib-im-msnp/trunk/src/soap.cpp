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

#include "soap.h"
#include "msocket.h"
#include "buffer.h"
#include <QByteArray>
#include <QUrl>
namespace libimmsnp {

	Soap::Soap(QString msnPassport, QString msnPass, QString mbiKey){
		m_msnPassport = msnPassport;
		m_msnPass = msnPass;
		m_mbiKey = mbiKey;
	}

	QString Soap::getChallengeText(){
		QString auth = "GET /login2.srf HTTP/1.1\r\n";
		auth += QString("Authorization: Passport1.4 OrgVerb=GET,OrgURL=http%3A%2F%2Fmessenger%2Emsn%2Ecom,sign-in=") + m_msnPassport.replace('@', "%40").replace('.', "%2E") + QString(",pwd=") +  m_msnPass.replace('@', "%40").replace('.', "%2E") + QString(",") + m_mbiKey;
		auth += "User-Agent: MSMSGS\r\n";
		auth += "Host: login.passport.com\r\n";
		auth += "Connection: Keep-Alive\r\n";
		auth += "Cache-Control: no-cache\r\n\r\n";
		return auth;

	}

	QString Soap::ticket(){
		QString xmlPost;
		xmlPost += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		xmlPost += "<Envelope xmlns=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:wsse=\"http://schemas.xmlsoap.org/ws/2003/06/secext\" xmlns:saml=\"urn:oasis:names:tc:SAML:1.0:assertion\" xmlns:wsp=\"http://schemas.xmlsoap.org/ws/2002/12/policy\" xmlns:wsu=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\" xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/03/addressing\" xmlns:wssc=\"http://schemas.xmlsoap.org/ws/2004/04/sc\" xmlns:wst=\"http://schemas.xmlsoap.org/ws/2004/04/trust\">\n";
		xmlPost += "  <Header>\n";
		xmlPost += "    <ps:AuthInfo xmlns:ps=\"http://schemas.microsoft.com/Passport/SoapServices/PPCRL\" Id=\"PPAuthInfo\">\n";
		xmlPost += "      <ps:HostingApp>{7108E71A-9926-4FCB-BCC9-9A9D3F32E423}</ps:HostingApp>\n";
		xmlPost += "      <ps:BinaryVersion>4</ps:BinaryVersion>\n";
		xmlPost += "      <ps:UIVersion>1</ps:UIVersion>\n";
		xmlPost += "      <ps:Cookies></ps:Cookies>\n";
		xmlPost += "      <ps:RequestParams>AQAAAAIAAABsYwQAAAAzMDg0</ps:RequestParams>\n";
		xmlPost += "    </ps:AuthInfo>\n";
		xmlPost += "    <wsse:Security>\n";
		xmlPost += "       <wsse:UsernameToken Id=\"user\">\n";
		xmlPost += "         <wsse:Username>" + QString(QUrl::toPercentEncoding(m_msnPassport).data()) + "</wsse:Username> \n";
		xmlPost += "         <wsse:Password>" + QString(QUrl::toPercentEncoding(m_msnPass).data()) + "</wsse:Password>\n";
		xmlPost += "       </wsse:UsernameToken>\n";
		xmlPost += "    </wsse:Security>\n";
		xmlPost += "  </Header>\n";
		xmlPost += "  <Body>\n";
		xmlPost += "    <ps:RequestMultipleSecurityTokens xmlns:ps=\"http://schemas.microsoft.com/Passport/SoapServices/PPCRL\" Id=\"RSTS\">\n";
		xmlPost += "      <wst:RequestSecurityToken Id=\"RST0\">\n";
		xmlPost += "        <wst:RequestType>http://schemas.xmlsoap.org/ws/2004/04/security/trust/Issue</wst:RequestType>\n";
		xmlPost += "        <wsp:AppliesTo>\n";
		xmlPost += "          <wsa:EndpointReference>				\n";
		xmlPost += "            <wsa:Address>http://Passport.NET/tb</wsa:Address>\n";
		xmlPost += "          </wsa:EndpointReference>\n";
		xmlPost += "        </wsp:AppliesTo>\n";
		xmlPost += "      </wst:RequestSecurityToken>\n";
		xmlPost += "      <wst:RequestSecurityToken Id=\"RST1\">\n";
		xmlPost += "       <wst:RequestType>http://schemas.xmlsoap.org/ws/2004/04/security/trust/Issue</wst:RequestType>\n";
		xmlPost += "        <wsp:AppliesTo>\n";
		xmlPost += "          <wsa:EndpointReference>\n";
		xmlPost += "            <wsa:Address>messenger.msn.com</wsa:Address>\n";
		xmlPost += "          </wsa:EndpointReference>\n";
		xmlPost += "        </wsp:AppliesTo>\n";
		QUrl tic = QUrl::fromEncoded(QByteArray(m_mbiKey.toUtf8().data()));
		xmlPost += "        <wsse:PolicyReference URI=\"?" + QString(QUrl::toPercentEncoding((tic.toString().replace(",","&"))).data()) + "\"></wsse:PolicyReference>\n";
		xmlPost += "      </wst:RequestSecurityToken>\n";
		xmlPost += "    </ps:RequestMultipleSecurityTokens>\n";
		xmlPost += "  </Body>\n";
		xmlPost += "</Envelope>\n";
		QString headers;
		headers += "POST /RST.srf HTTP/1.1\r\n";
		headers += "Accept: text/*\r\n";
		headers += "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)\r\n";
		headers += "Host: loginnet.passport.com\r\n";
		headers += "Content-Length: " + QString::number(xmlPost.size ()) + "\r\n";
		headers += "Connection: Keep-Alive\r\n";
		headers += "Cache-Control: no-cache\r\n";
		headers += "\r\n";
		headers += xmlPost;
		return headers;
	}

	Soap::~Soap(){
	}
}
#include "soap.moc"













//		xmlPost += "<Envelope xmlns=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:wsse=\"http://schemas.xmlsoap.org/ws/2003/06/secext\" xmlns:saml=\"urn:oasis:names:tc:SAML:1.0:assertion\" xmlns:wsp=\"http://schemas.xmlsoap.org/ws/2002/12/policy\" xmlns:wsu=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\" xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/03/addressing\" xmlns:wssc=\"http://schemas.xmlsoap.org/ws/2004/04/sc\" xmlns:wst=\"http://schemas.xmlsoap.org/ws/2004/04/trust\">\n";
//		xmlPost += "<Header>\n";
//		xmlPost += "<ps:AuthInfo xmlns:ps=\"http://schemas.microsoft.com/Passport/SoapServices/PPCRL\" Id=\"PPAuthInfo\">\n";
//		xmlPost += "<ps:HostingApp>\n";
//		xmlPost += "{7108E71A-9926-4FCB-BCC9-9A9D3F32E423}";
//		xmlPost += "</ps:HostingApp>\n";
//		xmlPost += "<ps:BinaryVersion>\n";
//		xmlPost += "4";
//		xmlPost += "</ps:BinaryVersion>\n";
//		xmlPost += "<ps:UIVersion>\n";
//		xmlPost += "1";
//		xmlPost += "</ps:UIVersion>\n";
//		xmlPost += "<ps:Cookies>\n";
//		xmlPost += "</ps:Cookies>\n";
//		xmlPost += "<ps:RequestParams>\n";
//		xmlPost += "AQAAAAIAAABsYwQAAAAxMDQ0";
//		xmlPost += "</ps:RequestParams>\n";
//		xmlPost += "</ps:AuthInfo>\n";
//		xmlPost += "<wsse:Security>\n";
//		xmlPost += "<wsse:UsernameToken Id=\"user\">\n";
//		xmlPost += "<wsse:Username>\n";
//		xmlPost += msnPassport;
//		xmlPost += "</wsse:Username>\n";
//		xmlPost += "<wsse:Password>\n";
//		xmlPost += msnPass;
//		xmlPost += "</wsse:Password>\n";
//		xmlPost += "</wsse:UsernameToken>\n";
//		xmlPost += "</wsse:Security>\n";
//		xmlPost += "</Header>\n";
//		xmlPost += "<Body>\n";
//		xmlPost += "<ps:RequestMultipleSecurityTokens xmlns:ps=\"http://schemas.microsoft.com/Passport/SoapServices/PPCRL\" Id=\"RSTS\">\n";
//		xmlPost += "<wst:RequestSecurityToken Id=\"RST0\">\n";
//		xmlPost += "<wst:RequestType>\n";
//		xmlPost += "http://schemas.xmlsoap.org/ws/2004/04/security/trust/Issue";
//		xmlPost += "</wst:RequestType>\n";
//		xmlPost += "<wsp:AppliesTo>\n";
//		xmlPost += "<wsa:EndpointReference>\n";
//		xmlPost += "<wsa:Address>\n";
//		xmlPost += "http://Passport.NET/tb";
//		xmlPost += "</wsa:Address>\n";
//		xmlPost += "</wsa:EndpointReference>\n";
//		xmlPost += "</wsp:AppliesTo>\n";
//		xmlPost += "</wst:RequestSecurityToken>\n";
//		xmlPost += "<wst:RequestSecurityToken Id=\"RST1\">\n";
//		xmlPost += "<wst:RequestType>\n";
//		xmlPost += "http://schemas.xmlsoap.org/ws/2004/04/security/trust/Issue";
//		xmlPost += "</wst:RequestType>\n";
//		xmlPost += "<wsp:AppliesTo>\n";
//		xmlPost += "<wsa:EndpointReference>\n";
//		xmlPost += "<wsa:Address>\n";
//		//xmlPost += "messengerclear.live.com";
//		//xmlPost += "</wsa:Address>\n";
//		//xmlPost += "</wsa:EndpointReference>\n";
//		//xmlPost += "</wsp:AppliesTo>\n";
//		//xmlPost += "<wsse:PolicyReference URI=\"MBI_KEY_OLD\">\n";
//		//xmlPost += "</wsse:PolicyReference>\n";
//		//xmlPost += "</wst:RequestSecurityToken>\n";
//		//xmlPost += "<wst:RequestSecurityToken Id=\"RST2\">\n";
//		//xmlPost += "<wst:RequestType>\n";
//		//xmlPost += "http://schemas.xmlsoap.org/ws/2004/04/security/trust/Issue";
//		//xmlPost += "</wst:RequestType>\n";
//		//xmlPost += "<wsp:AppliesTo>\n";
//		//xmlPost += "<wsa:EndpointReference>\n";
//		//xmlPost += "<wsa:Address>\n";
//		xmlPost += "messenger.msn.com";
//		xmlPost += "</wsa:Address>\n";
//		xmlPost += "</wsa:EndpointReference>\n";
//		xmlPost += "</wsp:AppliesTo>\n";
//		xmlPost += "<wsse:PolicyReference URI=\"?id=507\">\n";
//		xmlPost += "</wsse:PolicyReference>\n";
//		xmlPost += "</wst:RequestSecurityToken>\n";
//		//xmlPost += "<wst:RequestSecurityToken Id=\"RST3\">\n";
//		//xmlPost += "<wst:RequestType>\n";
//		//xmlPost += "http://schemas.xmlsoap.org/ws/2004/04/security/trust/Issue";
//		//xmlPost += "</wst:RequestType>\n";
//		//xmlPost += "<wsp:AppliesTo>\n";
//		//xmlPost += "<wsa:EndpointReference>\n";
//		//xmlPost += "<wsa:Address>\n";
//		//xmlPost += "contacts.msn.com";
//		//xmlPost += "</wsa:Address>\n";
//		//xmlPost += "</wsa:EndpointReference>\n";
//		//xmlPost += "</wsp:AppliesTo>\n";
//		//xmlPost += "<wsse:PolicyReference URI=\"MBI\">\n";
//		//xmlPost += "</wsse:PolicyReference>\n";
//		//xmlPost += "</wst:RequestSecurityToken>\n";
//		//xmlPost += "<wst:RequestSecurityToken Id=\"RST4\">\n";
//		//xmlPost += "<wst:RequestType>\n";
//		//xmlPost += "http://schemas.xmlsoap.org/ws/2004/04/security/trust/Issue";
//		//xmlPost += "</wst:RequestType>\n";
//		//xmlPost += "<wsp:AppliesTo>\n";
//		//xmlPost += "<wsa:EndpointReference>\n";
//		//xmlPost += "<wsa:Address>\n";
//		//xmlPost += "messengersecure.live.com";
//		//xmlPost += "</wsa:Address>\n";
//		//xmlPost += "</wsa:EndpointReference>\n";
//		//xmlPost += "</wsp:AppliesTo>\n";
//		//xmlPost += "<wsse:PolicyReference URI=\"MBI_SSL\">\n";
//		//xmlPost += "</wsse:PolicyReference>\n";
//		//xmlPost += "</wst:RequestSecurityToken>\n";
//		//xmlPost += "<wst:RequestSecurityToken Id=\"RST5\">\n";
//		//xmlPost += "<wst:RequestType>\n";
//		//xmlPost += "http://schemas.xmlsoap.org/ws/2004/04/security/trust/Issue";
//		//xmlPost += "</wst:RequestType>\n";
//		//xmlPost += "<wsp:AppliesTo>\n";
//		//xmlPost += "<wsa:EndpointReference>\n";
//		//xmlPost += "<wsa:Address>\n";
//		//xmlPost += "spaces.live.com";
//		//xmlPost += "</wsa:Address>\n";
//		//xmlPost += "</wsa:EndpointReference>\n";
//		//xmlPost += "</wsp:AppliesTo>\n";
//		//xmlPost += "<wsse:PolicyReference URI=\"MBI\">\n";
//		//xmlPost += "</wsse:PolicyReference>\n";
//		//xmlPost += "</wst:RequestSecurityToken>\n";
//		//xmlPost += "<wst:RequestSecurityToken Id=\"RST6\">\n";
//		//xmlPost += "<wst:RequestType>\n";
//		//xmlPost += "http://schemas.xmlsoap.org/ws/2004/04/security/trust/Issue";
//		//xmlPost += "</wst:RequestType>\n";
//		//xmlPost += "<wsp:AppliesTo>\n";
//		//xmlPost += "<wsa:EndpointReference>\n";
//		//xmlPost += "<wsa:Address>\n";
//		//xmlPost += "storage.msn.com";
//		//xmlPost += "</wsa:Address>\n";
//		//xmlPost += "</wsa:EndpointReference>\n";
//		//xmlPost += "</wsp:AppliesTo>\n";
//		//xmlPost += "<wsse:PolicyReference URI=\"MBI\">\n";
//		//xmlPost += "</wsse:PolicyReference>\n";
//		//xmlPost += "</wst:RequestSecurityToken>\n";
//		xmlPost += "</ps:RequestMultipleSecurityTokens>\n";
//		xmlPost += "</Body>\n";
//		xmlPost += "</Envelope>\n";
//
//		QString headers;
//		headers += "POST /RST.srf HTTP/1.1\r\n";
//		headers += "Accept: text/*\r\n";
//		headers += "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; IDCRL 4.100.313.1; IDCRL-cfg 4.0.5633.0; App msnmsgr.exe, 8.1.178.0, {7108E71A-9926-4FCB-BCC9-9A9D3F32E423})\r\n";
//		headers += "Host: login.live.com\r\n";
//		headers += "Content-Length: " + QString::number(xmlPost.size ()) + "\r\n";
//		headers += "Connection: Keep-Alive\r\n";
//		headers += "Cache-Control: no-cache\r\n\r\n";
//		headers += xmlPost;
//		
//		int size = (xmlPost.size ());
//		qDebug("%i",size);
//		//xmlPost.replace ("XXXXXXXXXX", QString::number(size + 4));
