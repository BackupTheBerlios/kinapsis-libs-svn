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
#include <qthread.h>
namespace libimmsnp {

class ParserNS;

class Connection : public QThread {

public:
	Connection (msocket*, ParserNS*);
	~Connection ();

	void run ();
private:
	msocket* m_socket;
	ParserNS* m_parser;
};
}
#endif // _CONNECTION_H_
