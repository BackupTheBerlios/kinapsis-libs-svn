/*
  Copyright (c) 2007 by Pedro Arnedo <vaticano666@gmail.com>
  This file is part of the lib-im-msnp library.
  https://developer.berlios.de/projects/kinapsis-libs/

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/

#ifndef _CMDNOTIFICATIONSERVER_H_
#define _CMDNOTIFICATIONSERVER_H_
#include "command.h" 
#include "buffer.h"
#include "libimmsnp.h"

namespace libimmsnp {
class CHG : public Command {
// CHG 9 NLN 1073791084\r\n 
public:
	CHG (int idtr);
	virtual ~CHG();
	Buffer makeCmd();
	void addStatusCode (State statusCode);
	void addCapabilities (QString capabilities);
	void addMsnObj (QString msnObj);
private:
	QString m_statusCode;
	QString m_capabilities;
	QString m_msnObj;
};

//class PRP : public Command {
//// PRP idtr nick\r\n 
//public:
//	PRP (int idtr);
//	virtual ~PRP();
//	Buffer makeCmd();
//	void addNick (QString nick);
//private:
//	QString m_nick;
//};
//
class XFR : public Command {
// XFR 15 SB\r\n 
public:
	XFR (int idtr);
	virtual ~XFR();
	Buffer makeCmd();
};

class OUT : public Command {
// OUT\r\n 
public:
	OUT ();
	virtual ~OUT();
	Buffer makeCmd();
};

class ADC : public Command {
// ADC 58 FL C=2c2d3fa5-9360-42c2-a166-7f4529f02fa4 22a6a1c3-f93b-423c-9c0e-413f4884603b\r\n
public:
	ADC (int idtr);
	virtual ~ADC();
	Buffer makeCmd();
	void addList (QString);

	void addId (QString);
	void addGroupId (QString);

	void addPassport (QString passport);
	void addDisplayName (QString dispName);
private:
	QString m_list;

	QString m_id;
	QString m_groupId;

	QString m_passport;
	QString m_dispName;
};

class REM : public Command {
// 
public:
	REM (int idtr);
	virtual ~REM();
	Buffer makeCmd();

	void addList (QString);
	void addId (QString);
private:
	QString m_list;
	QString m_id;
};

class ADG : public Command {
public:
	ADG (int idtr);
	virtual ~ADG();
	Buffer makeCmd();
	void addName (QString);
private:
	QString m_name;
};

class RMG : public Command {
public:
	RMG (int idtr);
	virtual ~RMG();
	Buffer makeCmd();
	void addId (QString);
private:
	QString m_name;
};
}
#endif //_CMDNOTIFICATIONSERVER_H_

