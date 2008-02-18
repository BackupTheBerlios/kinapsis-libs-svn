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

void Buffer::toChars(){
	int i; 
	for (i=0; i < this->size(); i++){
		if ((const char) this->at(i) == '\n')
			fprintf(stderr, "\\n");
		else
			if ((const char) this->at(i) == '\r')
				fprintf(stderr, "\\r");
			else
	        		fprintf(stderr, "%c",(const char) this->at(i));
	}
	//fprintf(stderr, "\n");
}

void Buffer::toHex(){
	int i; 
	for (i=0; i < this->size(); i++){
	        fprintf(stderr, "%02x ",(unsigned char) this->at(i));
	}
	fprintf(stderr, "\n");
	
}
char* Buffer::dataDebug(){
	QString tmp = this->data();
	return tmp.replace("\r\n","\\r\\n").toUtf8().data();
}

Buffer::~Buffer() { }

}
