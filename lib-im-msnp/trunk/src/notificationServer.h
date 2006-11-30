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

#ifndef _NOTIFICATIONSERVER_H_
#define _NOTIFICATIONSERVER_H_
#include "command.h" 

namespace libimmsnp {
class CHG : public Command {
// CHG 9 NLN 1073791084\r\n 
public:
	CHG (int idtr);
	virtual ~CHG();
	QString makeCmd();
	void addStatusCode (QString statusCode);
	void addClientId (QString clientId);
	void addMsnObj (QString msnObj);
private:
	QString m_statusCode;
	QString m_clientId;
	QString m_msnObj;
};
}
#endif //_NOTIFICATIONSERVER_H_
