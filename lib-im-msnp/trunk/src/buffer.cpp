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

#include "buffer.h"
#include <QList>

namespace libimmsnp {

Buffer::Buffer(){
}

Buffer& Buffer::operator << (QString q){
	this->append(q);
	return *this;
}

Buffer& Buffer::operator << (QByteArray q){
	this->append(q);
	return *this;
}

Buffer& Buffer::operator << (Buffer q){
	this->append(q.data());
	return *this;
}

char* Buffer::dataDebug(){
	QString tmp = this->data();
	return tmp.replace("\r\n","\\r\\n").toUtf8().data();
}

Buffer Buffer::getLine(){
	Buffer tmp;
	tmp << *this;

	Buffer res;
	QByteArray line = tmp.split('\n')[0];
	res << line;
	return res;
}

QString Buffer::getCmd(){
	QByteArray line = this->mid(0,3);
	return QString (line.data());
}

Buffer::~Buffer() { }

}
