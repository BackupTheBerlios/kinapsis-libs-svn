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

#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "libimmsnp.h"
#include "msocket.h"
#include <qobject.h>
namespace libimmsnp {

class ParserNS;

class Connection :public QObject {

Q_OBJECT

public:
	Connection (msocket*, ParserNS*, int iter = -1);
	~Connection ();
	void disconnect () {m_disconnect = true;};

	void run ();
signals:
	void disconnected (ConnectionError);
private:
	msocket* m_socket;
	ParserNS* m_parser;
	int m_iterations;
	bool m_disconnect;
};
}
#endif // _CONNECTION_H_
