/***************************************************************************
 *   Copyright (C) 2006-2008 by Luis Cidoncha                              *
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


#include "oftparser.h"
#include "qtextcodec.h"

namespace liboscar {

OFTParser::OFTParser() { }

OFTParser::~OFTParser() { }

void OFTParser::parse() {
	// Parses an OFT command

	OFTHeader head;
	Byte b;
	Word w, len;
	DWord dw;
	QWord qw;


	m_buf >> dw;

	if (dw != 0x4f465432) return; // this is a version we don't understand (OFT2)

	m_buf >> len; // total len of data

	len -= 6;

	m_buf >> w; // the type of this header
	head.setType(w);

	m_buf >> qw; // cookie 
	head.setCookie(qw);

	m_buf.advance(4); //encrypt & compress

	m_buf >> w; //File total
	head.setTotalFiles(w);

	m_buf >> w; //File left
	head.setFilesLeft(w);

	m_buf >> w; //Parts total
	head.setTotalParts(w);

	m_buf >> w; //Parts left
	head.setPartsLeft(w);

	m_buf >> dw; // total Size
	head.setTotalSize(dw);

	m_buf >> dw; // Size left
	head.setSizeLeft(dw);

	m_buf.advance(4); //mod time

	m_buf >> dw; //checksum
	head.setChecksum(dw);

	m_buf.advance(16); // another ckecksum, fork size, creation time ...

	m_buf >> dw; // bytes recv
	head.setBytesReceived(dw);

	m_buf >> dw; // recv chk
	head.setReceivedChk(dw);

	m_buf.advance(32); // IDString, unknown meaning

	m_buf >> b; head.setFlags(b); //flags

	m_buf.advance(87); // list offset, dummy block, and macfileinfo

	m_buf >> w; // encoding
	head.setEncoding(w);

	m_buf.advance(2); // subencoding (always 0x0000)

	int l = len - 192; // lenght of the filename. The total lenght minus 192 bytes we readed
	QByteArray fname;

	m_buf.readArray(fname, l); //raw data of the filename

	while (fname.endsWith((char) 0x00)) // remove null bytes
		fname.chop(1);

	QTextCodec *c;

	switch(head.getEncoding()) {
		case ASCII:
			c = QTextCodec::codecForName("ASCII");
			break;
		case UCS2BE:
			c = QTextCodec::codecForName("UTF-16BE");
			break;
		case LOCAL:
			c = QTextCodec::codecForName("ISO-8859-1");
			break;
		default:
			qDebug("Wrong enconding on OFT header");
			break;
	}

	if (c)
		head.setFilename(c->toUnicode(fname));
	else
		head.setFilename(fname); // who cares? ...

	emit headerArrived(head);
}

}

#include "oftparser.moc"
