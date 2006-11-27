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
CVR::~CVR(){}
QString CVR::makeCmd(){return QString ("a");}

USR::USR(int idtr) : Command ("USR", idtr) {}
USR::~USR(){}
QString USR::makeCmd(){return QString ("a");}

}
