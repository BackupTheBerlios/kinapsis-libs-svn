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

#ifndef _SWICHBOARDSERVER_H_
#define _SWICHBOARDSERVER_H_
#include "command.h" 
#include "libimmsnp.h"

namespace libimmsnp {
class ANS : public Command {
// ANS trid your_passport ticket sessid\r\n 
public:
	ANS (int idtr);
	virtual ~ANS();
	QString makeCmd();
	void addPassport (QString passport);
	void addTicket (QString ticket);
	void addSessId (QString sessId);
private:
	QString m_passport;
	QString m_ticket;
	QString m_sessId;
};

class MSG : public Command {
// ANS trid your_passport ticket sessid\r\n 
public:
	MSG (int idtr);
	virtual ~MSG();
	QString makeCmd();
	void addMsg (QString msg);
private:
	QString m_msg;
};

class USRchat : public Command {
// ANS trid your_passport ticket sessid\r\n 
public:
	USRchat (int idtr);
	virtual ~USRchat();
	QString makeCmd();
	void addPassport (QString passport);
	void addTicket (QString passport);
private:
	QString m_passport;
	QString m_ticket;
};

class CAL : public Command {
// CAL 1 carlitos@hotmail.com\r\n
public:
	CAL (int idtr);
	virtual ~CAL();
	QString makeCmd();
	void addPassport (QString passport);
private:
	QString m_passport;
};

class BYE : public Command {
// BYEr\n
public:
	BYE ();
	virtual ~BYE();
	QString makeCmd();
	void addPassport (QString passport);
private:
	QString m_passport;
};
}
#endif //_SWICHBOARDSERVER_H_

