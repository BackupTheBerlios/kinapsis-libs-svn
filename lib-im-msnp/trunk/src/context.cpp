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
	m_cLength 	= decoded.mid(0,4);
	m_version 	= decoded.mid(4,4);
	m_size 		= decoded.mid(8,8);
	m_type 		= decoded.mid(16,4);
	QTextCodec *codec = QTextCodec::codecForName("UTF-16");
	m_name	 	= QByteArray(QString(codec->toUnicode(decoded.mid(20,520)).toUtf8()).toUtf8());
	qDebug() << m_size.toHex() << getSize() <<m_name.toHex() << m_name;
	m_preview 	= decoded.mid(574);	

	qDebug() << m_preview.toBase64() << m_name << m_size;

	QFile * fd =  new QFile("preview-" + m_name + ".png");
   if (fd->open(QIODevice::WriteOnly)){
         fd->write(decoded.mid(574));
         fd->close();
   }
}	

QByteArray int2bbyte (int num, int len){
	QByteArray tmp2 = QByteArray::number(num,16);
	while (tmp2.size() < len) {
		tmp2.insert(0,"0");
	}
	QByteArray tmp3;
	QByteArray a;
	if (len == 8) {
		a = QByteArray::fromHex(tmp2);
		tmp3 = a.mid(3,1).toHex().data();
		tmp3.append(a.mid(2,1).toHex().data());
		tmp3.append(a.mid(1,1).toHex().data());
		tmp3.append(a.mid(0,1).toHex().data());
	}
	if (len == 16){
		a = QByteArray::fromHex(tmp2);
		tmp3 = a.mid(7,1).toHex().data();
		tmp3.append(a.mid(6,1).toHex().data());
		tmp3.append(a.mid(5,1).toHex().data());
		tmp3.append(a.mid(4,1).toHex().data());
		tmp3.append(a.mid(3,1).toHex().data());
		tmp3.append(a.mid(2,1).toHex().data());
		tmp3.append(a.mid(1,1).toHex().data());
		tmp3.append(a.mid(0,1).toHex().data());
	}

	return QByteArray::fromHex(tmp3.data());
}
QByteArray Context::getContextString(QString file){
	m_cLength 	= QByteArray::fromHex("027e 0000");
	m_version 	= QByteArray::fromHex("0003 0000");

	QFile * fd 	= new QFile(m_name);
   	if (fd->open(QIODevice::ReadOnly)){
		m_size = int2bbyte (fd->size(),16);	
	}
	fd->close();

	// m_type = 0 -> file transfer with preview data
	// 	  = 1 -> file transfer without preview data
	// 	  = 4 -> background sharing 
	m_type			= QByteArray::fromHex("0001 0000");
	QTextCodec *codec 	= QTextCodec::codecForName("UTF-16");
	m_name			= QByteArray(codec->toUnicode(file.toUtf8()).toUtf8()).leftJustified(520, '\0');
	m_dataUnknown 		= QByteArray::fromHex("0001 0000").leftJustified(30, '\0');
	m_ffffUnknown       	= QByteArray::fromHex("ffff ffff");

	return  QByteArray(m_cLength + m_version + m_size + m_type + m_name + m_dataUnknown + m_ffffUnknown).toBase64();
}
}
