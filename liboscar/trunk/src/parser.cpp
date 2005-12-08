/***************************************************************************
 *   Copyright (C) 2005 by Luis Cidoncha                                   *
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


#include "parser.h"
#include <stdlib.h>
#include <time.h>

#define POSITIVE_MASK 0x7fff

namespace liboscar {

Parser::Parser(){ 
	srand(time(NULL));
	m_seq = (Word) POSITIVE_MASK * (rand()/RAND_MAX); /* Keep it positive */
}

void Parser::add(QString data){
	m_buf << data;
}

void Parser::parse(){
	Byte b = 0, ch = 0;
	Word w = 0;
	Buffer buf;

	/* We have more data in the buffer */
	m_buf.gotoBegin();

	if (m_buf.len() < 6) /* Yet haven't enough data */
		return;

	m_buf >> b;

	if (b != 0x2a){
		qDebug(QString("Invalif FLAP header. Discarding data"));
		m_buf.wipe();
		return; 
	}
	
	m_buf >> ch;
	m_buf >> w; /* Sequence number */
	m_buf >> w; /* Data lenght */

	if (m_buf.len() < (w + 6))
		return;

	/* Copy the FLAP to a local buffer */
	buf << m_buf;
	buf.setLength(w + 6);
	buf.gotoBegin();
	buf.remove(6); /* remove header */

	/* remove the FLAP from the parser buffer */
	m_buf.gotoBegin();
	m_buf.remove(w + 6);

	switch (ch){
		case 0x01:
			parseCh1(buf);
			break;
		case 0x02:
			parseCh2(buf);
			break;
		case 0x04:
			parseCh4(buf);
			break;
		case 0x05:
			parseCh5(buf);
			break;
		default:
			qDebug(QString("FLAP on unknown channel: %1").arg(ch));
			break;
	}

}

void Parser::parseCh1(Buffer& buf){
}

void Parser::parseCh2(Buffer& buf){
}

void Parser::parseCh4(Buffer& buf){
}

void Parser::parseCh5(Buffer& buf){
}

Word Parser::getNextSeqNumber(){
	m_seq = ++m_seq & POSITIVE_MASK;
	return m_seq;
}

Parser::~Parser() { }


}

#include "parser.moc"
