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

namespace libimmsnp {

Buffer::Buffer(){
	m_lenCmd = 0;
}

Buffer& Buffer::operator += (int l){
	m_lenCmd += l;
	return *this;
}

Buffer& Buffer::operator << (QChar q){
	m_data << q;
	return *this;
}

Buffer& Buffer::operator << (QString q){
	
	for (uint i=0; i < q.length(); i ++)
		m_data << q.at(i);
	return *this;
}

Buffer& Buffer::operator << (Buffer b){
	
	for (uint i=0; i < b.len(); i ++)
		m_data << b.getCharInPosition(i);
	return *this;
}

QChar Buffer::getChar () {
	QChar c;

	c = *m_it;
	m_it++;

	return c;
}

Buffer& Buffer::operator>>(QChar &c){
	c = getChar();
	return *this;
}
int Buffer::getTilChar (QString &s, QChar c){
	// c included

	int l = 0;
	BIterator it = m_it;
	QCString tmp;

	while (*(it) != c){
		if (it == m_data.end())
			return -1;
		tmp += *(it);
		l++;
		it++;
	}
	tmp += *(it);
	l ++;

	s = QString::fromUtf8(tmp);
	return l;
}

int Buffer::getNChar (QString &s, uint len){
	// c included

	uint l = 0;
	BIterator it = m_it;
	QCString tmp;

	while (len-- !=0){
		if (it == m_data.end())
			return -1;
		tmp += *(it);
		l++;
		it++;
	}

	s = QString::fromUtf8(tmp);
	return l;
}


int Buffer::getQString (QString &c, uint len) {
	QCString tmp;

	if (len > this->len()){
		for (uint i=0; i < this->len(); i ++)
			tmp += *(m_data.at(i));
	}
	else {
		for (uint i=0; i < len; i ++)
			tmp += *(m_data.at(i));
	}

	c = QString::fromUtf8(tmp);
	return c.length();
}

int Buffer::getInt (int& idtr) {
	// devuelve la longitud del buffer que ha leido hasta sacar el entero.
	uint l = 0;
	BIterator it = m_it;
	QChar c = *(it);
	QCString s;

	while (c.digitValue() == -1){
		if (it == m_data.end())
			break;
			
		it++;
		c = *(it);
	}
	while (c.digitValue() != -1){
		if (it == m_data.end())
			break;
		s += c;
		l++;
		it++;
		c = *(it);
	}

	idtr = s.toInt();
	return l;
}

unsigned int Buffer::data(QString& d) {
	QCString tmp;
	for (uint i=0; i < this->len(); i ++)
		tmp += *(m_data.at(i));

	d = QString::fromUtf8(tmp);
	return d.length();
}

unsigned int Buffer::len(){
	return m_data.count();
}
void Buffer::gotoBegin(){
	m_it = m_data.begin();
}

void Buffer::gotoEnd(){
	m_it = m_data.fromLast();
}

void Buffer::setPosition(unsigned int pos){
	m_it = m_data.at(pos);
}

QChar Buffer::getCharInPosition(unsigned int pos){
	return  *(m_data.at(pos));
}

void Buffer::advance(int pos){
	m_it += pos;
}

void Buffer::prepend(QString s){
	for (uint i=0; i < s.length(); i ++)
		m_data.push_front(s.at(i));
}

void Buffer::remove(unsigned int num){
	unsigned int i=0;

	if (num > len())
		num = len();
	for (i=0; i < num; i++)
		m_it = m_data.remove(m_it);
}

void Buffer::removeFromBegin(){
	m_it = m_data.erase(m_data.begin(), m_it);
}



Buffer::~Buffer() { }


}
