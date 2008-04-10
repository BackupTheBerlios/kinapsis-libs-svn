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

#ifndef _CONTEXT_H_
#define _CONTEXT_H_
#include <QByteArray>
namespace libimmsnp {
class Context{

public:
	Context();
	virtual ~Context();
	void parse(QByteArray);	
	QByteArray getName() {return m_name;}
	int getSize()	;
	QByteArray getPreview() {return m_preview;}
	
private:
	QByteArray m_cLength;
	QByteArray m_version;
	QByteArray m_size;
	QByteArray m_type;
	QByteArray m_name; 	
	QByteArray m_preview;
};

}
#endif //_CONTEXT_H_

