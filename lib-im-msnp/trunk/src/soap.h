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

#ifndef _SOAP_H_
#define _SOAP_H_

#include "libimmsnp.h"
#include <qobject.h>
#include <QBuffer>
#include <QHttp>

namespace libimmsnp {

class Soap : public  QObject { 

Q_OBJECT
	
public:
	Soap (QString msnPassport, QString msnPass, QString mbiKey);
	QString getChallengeText();
	QString ticket();
	virtual ~Soap();
private:
	QString m_msnPassport;	
	QString m_msnPass;
	QString m_mbiKey;
};
}
#endif // _SOAP_H__
