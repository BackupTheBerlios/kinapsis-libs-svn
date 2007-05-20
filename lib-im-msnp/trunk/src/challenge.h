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

#ifndef _SINGLESIGNON_H_
#define _SINGLESIGNON_H_

#include "libimmsnp.h"
#include "msocket.h"
#include <qobject.h>

namespace libimmsnp {

class Challenge { 
	
public:
	Challenge (QString msnPassport, QString msnPass, QString mbiKey);
	void makeHash (const char *input, char *output);
	QString httpsGet (QString url, QString headers);
	QString httpsPost (QString url, QString headers, QString postData); 
	QString getTicket();

	virtual ~Challenge();
	
private:
	QString m_msnPassport;
	QString m_msnPass;
	QString m_mbiKey;
};
}
#endif // _SINGLESIGNON_H_
