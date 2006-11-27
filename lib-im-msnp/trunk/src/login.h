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

#ifndef _LOGIN_H_
#define _LOGIN_H_
#include "command.h" 
#include <qstringlist.h>

namespace libimmsnp {
class VER : public Command {
//  VER 1 MSNP11 MSNP10 CVR0\r\n
public:
	VER (int idtr);
	virtual ~VER();
	QString makeCmd();
	void addProtocolSupported (QString protocol);
private:
	QValueList<QString> m_listProtocol;
};

class CVR : public Command {
// CVR 2 0x040c winnt 5.1 i386 MSNMSGR 7.0.0777 msmsgs alice@hotmail.com\r\n
public:
	CVR (int idtr);
	virtual ~CVR();
	QString makeCmd();
};

class USR : public Command {
// USR 3 TWN I alice@passport.com\r\n
// USR 7 TWN S t=53*1hAu8ADuD3TEwdXoOMi08sD*2!cMrntT....\r\n 
public:
	USR (int idtr);
	virtual ~USR();
	QString makeCmd();
};
}
#endif //_LOGIN_H_

