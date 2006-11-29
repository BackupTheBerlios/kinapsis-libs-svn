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

#include "notificationServer.h"

namespace libimmsnp {

CHG::CHG(int idtr) : Command ("CHG", idtr) {}
CHG::~CHG(){}
QString CHG::makeCmd(){
	// CHG 9 NLN 1073791084\r\n
	// CHG trid statuscode clientid msnobj
	QString res;
	res += beginCmd();
	res += " " + m_statusCode + " " + m_clientId;
	res += endCmd();
	return res;
}
void CHG::addStatusCode (QString statusCode) {m_statusCode = statusCode;}
void CHG::addClientId (QString clientId) {m_clientId = clientId;}
void CHG::addMsnObj (QString msnObj) {m_msnObj = msnObj;}

}
