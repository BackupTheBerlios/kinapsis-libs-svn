/***************************************************************************
 *   Copyright (C) 2008 by Luis Cidoncha                                   *
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

#include "oftheader.h"

namespace liboscar {

OFTHeader::OFTHeader() { }

OFTHeader::~OFTHeader() { }

void OFTHeader::setType(Word w){
	m_type = (OFTType) w;
}

void OFTHeader::setType(OFTType t){
	m_type = t;
}

void OFTHeader::setCookie(QWord qw){
	m_cookie = qw;
}

void OFTHeader::setTotalFiles(Word w){
	m_totfiles = w;
}

void OFTHeader::setFilesLeft(Word w){
	m_leftfiles = w;
}

void OFTHeader::setTotalParts(Word w){
	m_totparts = w;
}

void OFTHeader::setPartsLeft(Word w){
	m_leftparts = w;
}

void OFTHeader::setTotalSize(DWord dw){
	m_totsize = dw;
}

void OFTHeader::setSizeLeft(DWord dw){
	m_leftsize = dw;
}

void OFTHeader::setChecksum(DWord dw){
	m_chk = dw;
}

void OFTHeader::setBytesReceived(DWord dw){
	m_bytesrecv = dw;
}

void OFTHeader::setReceivedChk(DWord dw){
	m_recvchk = dw;
}

void OFTHeader::setModTime(DWord dw){
	m_modtime = dw;
}

void OFTHeader::setFlags(Byte b){
	m_flags = (OFTFlags) b;
}

void OFTHeader::setEncoding(Word w){
	m_enc = (MessageEncoding) w;
}

void OFTHeader::setFileName(QString s){
	m_fname = s;
}


OFTType OFTHeader::getType(){
	return m_type;
}

QWord OFTHeader::getCookie(){
	return m_cookie;
}

Word OFTHeader::getTotalFiles(){
	return m_totfiles;
}

Word OFTHeader::getFilesLeft(){
	return m_leftfiles;
}

Word OFTHeader::getTotalParts(){
	return m_totparts;
}

Word OFTHeader::getPartsLeft(){
	return m_leftparts;
}

DWord OFTHeader::getTotalSize(){
	return m_totsize;
}

DWord OFTHeader::getSizeLeft(){
	return m_leftsize;
}

DWord OFTHeader::getChecksum(){
	return m_chk;
}

DWord OFTHeader::getBytesReceived(){
	return m_bytesrecv;
}

DWord OFTHeader::getReceivedChk(){
	return m_recvchk;
}

OFTFlags OFTHeader::getFlags(){
	return m_flags;
}

MessageEncoding OFTHeader::getEncoding(){
	return m_enc;
}

QString OFTHeader::getFileName(){
	return m_fname;
}

Buffer& OFTHeader::pack(){

	unsigned int fl = m_fname.length() + ((m_enc == UCS2BE) ? 2 : 1); // always null terminated

	m_buf.removeFromBegin();
	m_buf.gotoBegin();

	m_buf << "OFT2"; // the version
	m_buf << (Word) ((fl > 64) ? fl + 192 : 0x0100); // (? 192 before fname + fname) (: 256)
	m_buf << (Word) m_type;
	m_buf << m_cookie;
	m_buf << (Byte) 0x00; // Enc and comp. 
	m_buf << (Byte) 0x00; // Enc and comp.
	m_buf << m_totfiles;
	m_buf << m_leftfiles;
	m_buf << m_totparts;
	m_buf << m_leftparts;
	m_buf << m_totsize;
	m_buf << m_leftsize;
	m_buf << m_modtime;
	m_buf << m_chk;
	m_buf << (DWord) 0xFFFF0000; // resource fork
	m_buf << (DWord) 0x00000000; // resource fork
	m_buf << (DWord) 0x00000000; // resource fork
	m_buf << (DWord) 0xFFFF0000; // resource fork
	m_buf << m_bytesrecv;
	m_buf << m_recvchk;
	m_buf << "Cool FileXfer"; // yeah yeah, very cool
	QByteArray arr;
	arr.fill(0x00, 19); // 19 + 13 (len of string) == 32
	m_buf << arr;
	m_buf << (Byte) m_flags;
	m_buf << (Byte) 0x1c;
	m_buf << (Byte) 0x11;
	arr.fill(0x00, 85);
	m_buf << arr; // Dummy block and macFileInfo
	m_buf << (Word) m_enc;
	m_buf << (Word) 0x0000;

	m_buf << m_fname;
	if (m_enc == UCS2BE)
		m_buf << (Word) 0x0000;
	else
		m_buf << (Byte) 0x00;
	if (fl < 64){
		arr.fill(0x00, 64-fl);
		m_buf << arr;
	}

	return m_buf;
}

}
