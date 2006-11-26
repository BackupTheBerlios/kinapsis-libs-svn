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
#include <qvaluelist.h>
#include <qstring.h>

namespace libimmsnp {

class Buffer {

public:
	Buffer();

	Buffer& operator+=(int);

	Buffer& operator<<(QChar);
	Buffer& operator<<(QString);
	Buffer& operator<<(Buffer);
	Buffer& operator>>(QChar &);
	int getTilChar (QString&, QChar);
	int getQString (QString&, int);
	unsigned int data(QString&);

	int getInt (int&);

	QChar getChar ();

	unsigned int len();

	void gotoBegin();
	void gotoEnd();
	void setPosition(unsigned int);
	QChar getCharInPosition(unsigned int);
	void advance(int);
	
	void prepend (QString);
	void remove(unsigned int num = 1);
        void removeFromBegin();
	
	virtual ~Buffer();

private:
	typedef QValueList<QChar>::iterator BIterator;

	QValueList<QChar> m_data;
	BIterator m_it;
	int m_lenCmd;

};

}

#endif // _BUFFER_H_
