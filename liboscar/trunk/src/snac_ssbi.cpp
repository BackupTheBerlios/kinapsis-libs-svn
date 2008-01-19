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


#include "snac_ssbi.h"
#include <qfile.h>

namespace liboscar {

	// SNAC_SSBI main class
SNAC_SSBI::SNAC_SSBI(Word command, bool raw) 
	: SNAC(SNAC_FAM_SSBI, command, raw) { }

SNAC_SSBI::~SNAC_SSBI(){ }
	
	// SrvSSBIErrSNAC
SrvSSBIErrSNAC::SrvSSBIErrSNAC()
	: SNAC_SSBI(SSBI_SRV_SSBI_ERR, true) { }
SrvSSBIErrSNAC::~SrvSSBIErrSNAC() { }

Word SrvSSBIErrSNAC::getError() {
	return m_error;
}

void SrvSSBIErrSNAC::parse(Buffer& b) {
	Word w;

	b >> m_error;

	// TODO: parse this TLV
	b >> w;
	b >> w;
	b.advance(w);

	b.removeFromBegin();
}

	// SrvUploadAckSNAC 
SrvUploadAckSNAC::SrvUploadAckSNAC()
	: SNAC_SSBI(SSBI_SRV_UPLOADACK, true) { }

SrvUploadAckSNAC::~SrvUploadAckSNAC() { }

QByteArray SrvUploadAckSNAC::getHash() {
	return m_md5;
}

void SrvUploadAckSNAC::parse(Buffer& b) {
	Byte len;

	b.advance(4); // Unknown	
	b >> len;
	b.readArray(m_md5, len);

	b.removeFromBegin();
}

	// SrvAIMIconResponseSNAC 
SrvAIMIconResponseSNAC::SrvAIMIconResponseSNAC()
	: SNAC_SSBI(SSBI_SRV_AIMRESPONSE, true) { }

SrvAIMIconResponseSNAC::~SrvAIMIconResponseSNAC() { }

UIN SrvAIMIconResponseSNAC::getUin(){
	return m_uin;
}

QByteArray SrvAIMIconResponseSNAC::getHash() {
	return m_md5;
}

QByteArray SrvAIMIconResponseSNAC::getIcon() {
	return m_icon;
}

void SrvAIMIconResponseSNAC::parse(Buffer& b) {
	Byte mlen;
	Word ilen;

	m_uin.parse(b); // get UIN

	b.advance(3); // Unknown data

	b >> mlen;
	b.readArray(m_md5, mlen);

	b >> ilen;
	b.readArray(m_icon, ilen);

	b.removeFromBegin();
}

	// SrvICQIconResponseSNAC 
SrvICQIconResponseSNAC::SrvICQIconResponseSNAC()
	: SNAC_SSBI(SSBI_SRV_ICQRESPONSE, true) { }
SrvICQIconResponseSNAC::~SrvICQIconResponseSNAC() { }

UIN SrvICQIconResponseSNAC::getUin(){
	return m_uin;
}

QByteArray SrvICQIconResponseSNAC::getHash() {
	return m_md5;
}

QByteArray SrvICQIconResponseSNAC::getIcon() {
	return m_icon;
}

void SrvICQIconResponseSNAC::parse(Buffer& b) {
	Byte mlen;
	Word ilen;

	m_uin.parse(b); // get UIN

	b.advance(3);

	b >> mlen;
	b.readArray(m_md5, mlen);

	b.advance(4); // command, bitmask ?

	b >> mlen;
	b.advance(mlen); // repeated MD5

	b >> ilen;
	b.readArray(m_icon, ilen); // icon data :-)

	b.removeFromBegin();
}

	// CliUploadIconSNAC
CliUploadIconSNAC::CliUploadIconSNAC(QString fname)
	: SNAC_SSBI(SSBI_CLI_UPLOAD, true) { 
	
	// FIXME: all this code doesn't check errors. Add checks.
	
	QFile f(fname);

	f.open(QIODevice::ReadOnly);

	m_data << (Word) 0x0001; // ref number?

	m_data << (Word) f.size();

	QByteArray a = f.readAll();
	m_data << a;
}

CliUploadIconSNAC::~CliUploadIconSNAC() { }
	
	// CliAIMRequestIconSNAC
CliAIMRequestIconSNAC::CliAIMRequestIconSNAC(UIN uin, QByteArray md5)
	: SNAC_SSBI(SSBI_CLI_AIMREQUEST, true) { 
	uin.appendUin(m_data);

	m_data << (Byte) 0x01;
	m_data << (Word) 0x0001;
	m_data << (Byte) 0x01;

	m_data << (Word) md5.size();
	m_data << md5;
}

CliAIMRequestIconSNAC::~CliAIMRequestIconSNAC() { }
	
	// CliICQRequestIconSNAC
CliICQRequestIconSNAC::CliICQRequestIconSNAC(UIN uin, QByteArray md5)
	: SNAC_SSBI(SSBI_CLI_ICQREQUEST, true) { 
	uin.appendUin(m_data);

	m_data << (Byte) 0x01;
	m_data << (Word) 0x0001;
	m_data << (Byte) 0x01;

	m_data << (Word) md5.size();
	m_data << md5;
}

CliICQRequestIconSNAC::~CliICQRequestIconSNAC() { }


}
