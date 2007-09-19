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

#include "cmdswichboardserver.h"

namespace libimmsnp {

ANS::ANS(int idtr) : Command ("ANS", idtr) {}
ANS::~ANS(){}
Buffer ANS::makeCmd(){
	//ANS trid your_passport ticket sessid\r\n
	Buffer res;
	res += beginCmd();
	res += " " + m_passport  + " " +  m_ticket + " " + m_sessId;
	res += endCmd();
	return res;
}
void ANS::addPassport (QString passport) {m_passport = passport;}
void ANS::addTicket (QString ticket) {m_ticket = ticket;}
void ANS::addSessId (QString sessId) {m_sessId = sessId;}

MSG::MSG() : Command ("MSG", 0, "") {
	m_color = "000000";
	m_font = "Helvetica";
}
MSG::~MSG(){}
Buffer MSG::makeCmd(){
	// MSG xxxxxxxxxx@hotmail.com xxxxxx 126\r\nMIME-Version: 1.0\r\nContent-Type: text/plain; charset=UTF-8\r\nX-MMS-IM-Format: FN=Helvetica; EF=; CO=000000; CS=0; PF=22\r\n\r\nhola
	Buffer res;
	QString msg = "MIME-Version: 1.0\r\nContent-Type: text/plain; charset=UTF-8\r\nX-MMS-IM-Format: FN=" + m_font + "; EF=" + m_bold + m_italic + m_underline + "; CO=" + m_color + "; CS=0; PF=22\r\n\r\n" + m_msg;
	res << beginCmd() + " A " + QString::number(msg.size()) + "\r\n" + msg + endCmd();
	return res;
}
void MSG::addColor (QString color) {m_color = color;}
void MSG::addFont (QString font) {m_font = font.replace(" ", "%20");}
void MSG::addEffect (Effect effect) {
	switch(effect){
		case EFFECT_ITALIC:
			m_italic = "I";
			break;
		case EFFECT_BOLD:
			m_bold = "B";
			break;
		case EFFECT_UNDERLINE:
			m_underline = "U";
			break;
		default:
			break;
		}
}
	
void MSG::addMsg (QString msg) {m_msg = msg;}

USRchat::USRchat(int idtr) : Command ("USR", idtr) {}
USRchat::~USRchat(){}
Buffer USRchat::makeCmd(){
	//USR 1 example@passport.com 17262740.1050826919.32308\r\n
	Buffer res;
	res << beginCmd() + " " + m_passport + " " + m_ticket + endCmd();
	return res;
}
void USRchat::addPassport (QString passport) {m_passport = passport;}
void USRchat::addTicket (QString ticket) {m_ticket = ticket;}


CAL::CAL(int idtr) : Command ("CAL", idtr) {}
CAL::~CAL(){}
Buffer CAL::makeCmd(){
	//CAL 1 example@passport.com\r\n
	Buffer res;
	res << beginCmd() + " " + m_passport + endCmd();
	return res;
}
void CAL::addPassport (QString passport) {m_passport = passport;}

BYE::BYE() : Command ("BYE") {}
BYE::~BYE(){}
Buffer BYE::makeCmd(){
	//BYE xxxx@hotmail.com\r\n
	Buffer res;
	res << beginCmd() + " " + m_passport + endCmd();
	return res;
}
void BYE::addPassport (QString passport) {m_passport = passport;}
}
