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

#include "login.h"
namespace libimmsnp {
VER::VER(int idtr) : Command ("VER", idtr) {}
VER::~VER(){}
QString VER::makeCmd(){
	QString res;
	QValueList <QString> :: iterator i;
	res += beginCmd();
	for (i = m_listProtocol.begin(); i != m_listProtocol.end(); i++ )
		res += " " + *(i);
	res += " CVR0" + endCmd();
	return res;
}


void VER::addProtocolSupported (QString protocol){
	m_listProtocol.append (protocol);
}

CVR::CVR(int idtr) : Command ("CVR", idtr) {}
// CVR trid locale_id os_type os_ver arch clientname clientver MSMSGS passport\r\n
// CVR 2    0x040c    winnt   5.1    i386 MSNMSGR    7.0.0777  msmsgs alice@hotmail.com\r\n
CVR::~CVR(){}
void CVR::addLocaleId (QString locale) {m_locale = locale;}
void CVR::addOs (QString osType, QString osVer) {
	m_osType = osType;
	m_osVer = osVer;
}
void CVR::addArch (QString arch) {m_arch = arch;}
void CVR::addClient (QString clientName, QString clientVer) {
	m_clientName = clientName;
	m_clientVer = clientVer;
}
void CVR::addPassport (QString passport) {m_passport = passport;}
QString CVR::makeCmd(){
	QString res;
	res += beginCmd();
	res += " " + m_locale + " " + m_osType + " " + m_osVer + " " + m_arch + " " + m_clientName + " " + m_clientVer;
	res += " msmsgs " + m_passport;
	res += endCmd();
	return res;
}

USR::USR(int idtr) : Command ("USR", idtr) {}
USR::~USR(){}
void USR::addTwnType (QString twnType) {m_twnType = twnType;}
void USR::addTicket (QString ticket) {m_ticket = ticket;}
void USR::addPassport (QString passport) {m_passport = passport;}
QString USR::makeCmd(){
	// USR trid TWN I passport\r\n 
	// USR 3    TWN I alice@passport.com\r\n
	QString res;
	res += beginCmd();
	if (m_twnType == "I") 
		res += " TWN " + m_twnType + " " + m_passport;
	else res += " TWN " + m_twnType + " " + m_ticket;
	res += endCmd();
	return res;
}

SYN::SYN(int idtr) : Command ("SYN", idtr) {}
SYN::~SYN(){}
QString SYN::makeCmd(){
	// USR trid TWN I passport\r\n 
	// USR 3    TWN I alice@passport.com\r\n
	QString res;
	res += beginCmd();
	res += " 2005-04-23T18:57:44.8130000-07:00 2005-04-23T18:57:54.2070000-07:00";
	res += endCmd();
	return res;
}
}
