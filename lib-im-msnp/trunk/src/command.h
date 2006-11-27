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

#ifndef _COMMAND_H_
#define _COMMAND_H_
#include <qstring.h>

namespace libimmsnp {
class Command {
public:
	Command(QString cmdName, int transaction=0, QString endCmd = "\r\n"): m_trid (transaction), m_endCmd (endCmd), m_cmdName (cmdName) {};
	virtual ~Command() {};
	virtual QString makeCmd() = 0;
	QString beginCmd() {return m_cmdName + QString((hasTransaction()) ? QString(" %1").arg(m_trid):"");};
	bool hasTransaction () {return (m_trid)?true:false;};
	QString endCmd () {return m_endCmd;};

private:
	int m_trid;
	QString m_endCmd;
	QString m_cmdName;
};
}
#endif //_COMMAND_H_

