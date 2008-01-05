/***************************************************************************
 *   Copyright (C) 2005-2008 by Luis Cidoncha                              *
 *   luis.cidoncha@gmail.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "buffer.h"

namespace liboscar {

Buffer::Buffer(){
	m_lendian = false;
}

/*Buffer::Buffer(const Buffer& other){
	m_lendian = false;
	unsigned int i = 0;
	Byte b;

	other.gotoBegin();
	for (i=0; i < other.len(); i++){
		other >> b;
		m_data << b;
	}
}*/

Buffer& Buffer::operator<<(Byte b){
	m_data << b;

	return *this;
}

Buffer& Buffer::operator<<(Word w){
	if (m_lendian){
		m_data << (w & 0xFF);
		m_data << ((w >> 8) & 0xFF);
	}
	else {
		m_data << ((w >> 8) & 0xFF);
		m_data << (w & 0xFF);
	}

	return *this;
}

Buffer& Buffer::operator<<(DWord dw) {
	if (m_lendian){
		m_data << (dw & 0xFF);
		m_data << ((dw >> 8) & 0xFF);
		m_data << ((dw >> 16) & 0xFF);
		m_data << ((dw >> 24) & 0xFF);
	}
	else{
		m_data << ((dw >> 24) & 0xFF);
		m_data << ((dw >> 16) & 0xFF);
		m_data << ((dw >> 8) & 0xFF);
		m_data << (dw & 0xFF);
	}

	return *this;
}

Buffer& Buffer::operator<<(QWord qw) {
	if (m_lendian){
		m_data << (qw & 0xFF);
		m_data << ((qw >> 8) & 0xFF);
		m_data << ((qw >> 16) & 0xFF);
		m_data << ((qw >> 24) & 0xFF);
		m_data << ((qw >> 32) & 0xFF);
		m_data << ((qw >> 40) & 0xFF);
		m_data << ((qw >> 48) & 0xFF);
		m_data << ((qw >> 56) & 0xFF);
	}
	else{
		m_data << ((qw >> 56) & 0xFF);
		m_data << ((qw >> 48) & 0xFF);
		m_data << ((qw >> 40) & 0xFF);
		m_data << ((qw >> 32) & 0xFF);
		m_data << ((qw >> 24) & 0xFF);
		m_data << ((qw >> 16) & 0xFF);
		m_data << ((qw >> 8) & 0xFF);
		m_data << (qw & 0xFF);
	}

	return *this;
}


Buffer& Buffer::operator<<(const QString& s){
	unsigned int i;
	QByteArray c;

	c = s.toAscii();

	for (i=0;i < s.length(); i++)
		m_data << (Byte) c[i];


	return *this;
}

Buffer& Buffer::operator<<(Buffer &other){

	unsigned int i = 0;
	Byte b;

	other.gotoBegin();
	for (i=0; i < other.len(); i++){
		other >> b;
		m_data << b;
	}
	return *this;
}

Buffer& Buffer::operator<<(QByteArray &a){
	unsigned int i = 0;
	Byte b;

	for (i=0; i < a.size(); i++)
		m_data << a[i];

	return *this;
}

Byte Buffer::getByte(){
	Byte b;

	b = *m_it;
	m_it++;

	return b;
}

Buffer& Buffer::operator>>(Byte &b){
	b = getByte();
	return *this;
}

Word Buffer::getWord(){
	Byte b1,b2;
	Word w;

	if (m_lendian){
		b2 = getByte();
		b1 = getByte();
	}
	else{
		b1 = getByte();
		b2 = getByte();
	}
	
	w = b1;
	w <<= 8;
	w |= (0xFFFF & b2);

	return w;
}

Buffer& Buffer::operator>>(Word &w){

	if (m_data.size() < 2)
		w = (Word) -1;
	else 
		w = getWord();
	
	return *this;
}

void Buffer::readString(QString &s){
	Word len;

	len = getWord();

	if (!len)
		return;

	if (m_data.size() >= len){
		while (len--)
			s.append(getByte());
	}
}

void Buffer::readString(QString &s, Word len){
	if (m_data.size() >= len){
		while (len--)
			s.append(getByte());
	}
}

DWord Buffer::getDWord(){
	DWord dw;
	Word w1; /* low word */
	Word w2; /* high word */

	if  (m_lendian){
		w2 = getWord();
		w1 = getWord();
	}
	else {
		w1 = getWord();
		w2 = getWord();
	}

	dw = w1;
	dw <<= 16;
	dw |= (0xFFFFFFFF & w2);
	return dw;
}

Buffer& Buffer::operator>>(DWord &dw){

	if (m_data.size() < 4)
		dw = (DWord) -1;
	else 
		dw = getDWord();

	return *this;
}

Buffer& Buffer::operator>>(QWord &qw){
	DWord d1; /* low dword */
	DWord d2; /* high dword */

	if (m_data.size() < 8)
		qw = (QWord) -1;
	else {
		if  (m_lendian){
			d2 = getDWord();
			d1 = getDWord();
		}
		else {
			d1 = getDWord();
			d2 = getDWord();
		}

		QWord mask = -1; // 0xffffffffffffffff
		qw = d1;
		qw <<= 32;
		qw |= (mask & d2);
	}

	return *this;
}


void Buffer::prepend(Byte b){
	m_data.push_front(b);
}

void Buffer::prepend(Word w){
	if (m_lendian){
		m_data.push_front((w >> 8) & 0xFF);
		m_data.push_front(w & 0xFF);
	}
	else {
		m_data.push_front(w & 0xFF);
		m_data.push_front((w >> 8) & 0xFF);
	}
}

void Buffer::prepend(DWord dw){
	if (m_lendian) {
		m_data.push_front((dw >> 24) & 0xFF);
		m_data.push_front((dw >> 16) & 0xFF);
		m_data.push_front((dw >> 8) & 0xFF);
		m_data.push_front(dw & 0xFF);
	}
	else {
		m_data.push_front(dw & 0xFF);
		m_data.push_front((dw >> 8) & 0xFF);
		m_data.push_front((dw >> 16) & 0xFF);
		m_data.push_front((dw >> 24) & 0xFF);
	}
}

void Buffer::remove(unsigned int num){
	unsigned int i=0;

	if (num > len())
		num = len();
	for (i=0; i < num; i++)
		m_it = m_data.erase(m_it);
}

void Buffer::removeFromBegin(){
	m_it = m_data.erase(m_data.begin(), m_it);
}

void Buffer::wipe(){
	/* Removes all the data */
	m_data.clear();
}

void Buffer::copy(Byte *bb){
	BIterator it = m_data.begin();
	unsigned int i;

	for (i = 0; i < m_data.size(); i++){
		bb[i] = *it;
		it++;
	}
}

QByteArray Buffer::toByteArray(){
	QByteArray arr;

	BIterator it = m_data.begin();
	unsigned int i;

	for (i = 0; i < m_data.size(); i++){
		arr[i] = *it;
		it++;
	}
	return arr;
}

void Buffer::setLittleEndian(){
	m_lendian = true;
}

void Buffer::setBigEndian(){
	m_lendian = false;
}

void Buffer::gotoBegin(){
	m_it = m_data.begin();
}
void Buffer::gotoEnd(){
	m_it = m_data.end();
}
void Buffer::setPosition(unsigned int pos){
	m_it = m_data.begin();
	// FIXME: check data lenght
	m_it += pos;
}

void Buffer::advance(int pos){
	m_it += pos;
}

void Buffer::setLength(unsigned int length){
	unsigned int i;
	unsigned int len = m_data.size();

	if (length > len)
		return ;

	for (i = length; i < len; i++)
		m_data.pop_back();
}

unsigned int Buffer::len(){
	return m_data.size();
}

Buffer::~Buffer() { }

}

