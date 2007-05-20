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

#ifndef _CMDLOGIN_H_
#define _CMDLOGIN_H_
#include "command.h" 
#include "buffer.h"
#include <QStringList>	// for VER protocols

namespace libimmsnp {

class VER : public Command {
//  VER 1 MSNP11 MSNP10 CVR0\r\n
public:
	VER (int idtr);
	virtual ~VER();
	Buffer makeCmd();
	void addProtocolSupported (QString protocol);
private:
	QStringList m_listProtocol;
};

class CVR : public Command {
// CVR 2 0x040c winnt 5.1 i386 MSNMSGR 7.0.0777 msmsgs alice@hotmail.com\r\n
public:
	CVR (int idtr);
	virtual ~CVR();
	Buffer makeCmd();
	void addLocaleId (QString ); //get it from http://www.html.dk/dokumentation/vbscript/HTML/vsmscLCID.htm
	void addOs (QString osType,QString osVer);
	void addArch (QString );
	void addClient (QString clientName, QString clientVer);
	void addPassport (QString );
private:
	QString m_locale;
	QString m_osType;
	QString m_osVer;
	QString m_arch;
	QString m_clientName;
	QString m_clientVer;
	QString m_passport;
};

class USR : public Command {
// USR 3 TWN I alice@passport.com\r\n
// USR 7 TWN S t=53*1hAu8ADuD3TEwdXoOMi08sD*2!cMrntT....\r\n 
public:
	USR (int idtr);
	virtual ~USR();
	void addTwnType (QString twnType);
	void addTicket (QString ticket); 
	void addPassport (QString );
	Buffer makeCmd();

private:
	QString m_twnType;
	QString m_ticket;
	QString m_passport;
};

class GCF : public Command {
// 
public:
	GCF (int idtr);
	virtual ~GCF();
	Buffer  makeCmd();
};

class SYN : public Command {
// 
public:
	SYN (int idtr);
	virtual ~SYN();
	Buffer  makeCmd();
};
class QRY : public Command {
//QRY trid idstring payload_length\r\nmd5digest
public:
	QRY (int idtr);
	virtual ~QRY();
	Buffer makeCmd();
	void addProductId (QString);
	void addMd5(QString);
private:
	QString m_md5;
	QString m_productId;
};

}
#endif //_CMDLOGIN_H_

