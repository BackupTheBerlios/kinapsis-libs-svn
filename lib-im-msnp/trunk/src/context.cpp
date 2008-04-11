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

#include "context.h"
#include <QDebug>
#include <QTextCodec>
#include <QFile>
namespace libimmsnp {
Context::Context () {
}
Context::~Context() {}
int Context::getSize(){
	bool ok;
	QByteArray tmp = m_size.mid(7,1).toHex().data();
	tmp.append(m_size.mid(6,1).toHex().data());
	tmp.append(m_size.mid(5,1).toHex().data());
	tmp.append(m_size.mid(4,1).toHex().data());
	tmp.append(m_size.mid(3,1).toHex().data());
	tmp.append(m_size.mid(2,1).toHex().data());
	tmp.append(m_size.mid(1,1).toHex().data());
	tmp.append(m_size.mid(0,1).toHex().data());
	return tmp.toInt(&ok,16);
}

void Context::parse(QByteArray data){
	QByteArray decoded = QByteArray::fromBase64(data);
	m_cLength = decoded.mid(0,4);
	m_version = decoded.mid(4,4);
	m_size = decoded.mid(8,8);
	m_type = decoded.mid(16,4);
	QTextCodec *codec = QTextCodec::codecForName("UTF-16");
	m_name = QByteArray(QString(codec->toUnicode(decoded.mid(20,520)).toUtf8()).toUtf8());
	qDebug() << m_size.toHex() << getSize() <<m_name.toHex() << m_name;
	m_preview = decoded.mid(574);	

	QFile * fd =  new QFile("preview.png");
   if (fd->open(QIODevice::WriteOnly)){
         fd->write(decoded.mid(574));
         fd->close();
   }
}	
}
