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

#include "cmdlogin.h"
namespace libimmsnp {

// VER

VER::VER(int idtr) : Command ("VER", idtr) {}
VER::~VER(){}
Buffer VER::makeCmd(){
	Buffer res;
	res << beginCmd() + " " + m_listProtocol.join(" ") + " CVR0" + endCmd();
	return res;
}

void VER::addProtocolSupported (QString protocol){
	m_listProtocol << protocol;
}


// CVR trid locale_id os_type os_ver arch clientname clientver MSMSGS passport\r\n
// CVR 2    0x040c    winnt   5.1    i386 MSNMSGR    7.0.0777  msmsgs alice@hotmail.com\r\n

CVR::CVR(int idtr) : Command ("CVR", idtr) {}

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

Buffer CVR::makeCmd(){
	Buffer res;
	res << beginCmd();
	res << " " + m_locale + " " + m_osType + " " + m_osVer + " " + m_arch + " " + m_clientName + " " + m_clientVer;
	res << " msmsgs " + m_passport;
	res << endCmd();
	return res;
}

USR::USR(int idtr) : Command ("USR", idtr) {}

USR::~USR(){}

void USR::addTwnType (QString twnType) {m_twnType = twnType;}

void USR::addTicket (QString ticket) {m_ticket = ticket;}

void USR::addPassport (QString passport) {m_passport = passport;}

Buffer USR::makeCmd(){
	// USR 3 SSO I alice@passport.com\r\n
	//Buffer res;
	//if (m_twnType == "I") 	res << beginCmd() + " TWN " + m_twnType + " " + m_passport + endCmd();
	//else 			res << beginCmd() + " TWN " + m_twnType + " " + m_ticket + endCmd();
	//return res;
	Buffer res;
	if (m_twnType == "I") 	res << beginCmd() + " TWN " + m_twnType + " " + m_passport + endCmd();
	else 			res << beginCmd() + " TWN " + m_twnType + " " + m_ticket + endCmd();
	return res;
}

GCF::GCF(int idtr) : Command ("GCF", idtr) {}
GCF::~GCF(){}
Buffer  GCF::makeCmd(){
	Buffer res;
	res << beginCmd() + " Shields.xml" + endCmd() ;
	return res;
}

SYN::SYN(int idtr) : Command ("SYN", idtr) {}
SYN::~SYN(){}
Buffer  SYN::makeCmd(){
	Buffer res;
	res << beginCmd() + " 0 0" + endCmd() ;
	return res;
}

QRY::QRY (int idtr):Command ("QRY", idtr, "")  {}
QRY::~QRY() {}
Buffer QRY::makeCmd() {
	Buffer res;
	res += beginCmd() + " " + m_productId + " 32\r\n" + m_md5.mid(0,32) + endCmd();
	return res;
}
void QRY::addProductId (QString productId) {
	m_productId = productId;
}
void QRY::addMd5(QString md5){
	m_md5 = md5; 
}


}
