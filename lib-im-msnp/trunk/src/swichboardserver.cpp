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

#include "swichboardserver.h"

namespace libimmsnp {

ANS::ANS(int idtr) : Command ("ANS", idtr) {}
ANS::~ANS(){}
QString ANS::makeCmd(){
	//ANS trid your_passport ticket sessid\r\n
	QString res;
	res += beginCmd();
	res += " " + m_passport  + " " +  m_ticket + " " + m_sessId;
	res += endCmd();
	return res;
}
void ANS::addPassport (QString passport) {m_passport = passport;}
void ANS::addTicket (QString ticket) {m_ticket = ticket;}
void ANS::addSessId (QString sessId) {m_sessId = sessId;}
}
