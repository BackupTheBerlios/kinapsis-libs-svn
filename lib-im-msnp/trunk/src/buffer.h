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

#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "libimmsnp.h"
#include <QByteArray>
#include <qstring.h>

namespace libimmsnp {


class Buffer : public QByteArray{


public:
	Buffer();

	void toChars();
	void toHex();
	char* dataDebug();

	virtual ~Buffer();
};

}

#endif // _BUFFER_H_
