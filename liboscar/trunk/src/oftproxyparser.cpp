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


#include "oftproxyparser.h"

namespace liboscar {

OFTProxyParser::OFTProxyParser() { }

OFTProxyParser::~OFTProxyParser() { }

void OFTProxyParser::parse() {
	// Parses an OFTProxy command
	// everybody's in the place? let's go
	
	Word len, w;
	Buffer b;

	while (m_buf.len()){
		m_buf.gotoBegin();

		m_buf >> len;

		/* Copy command to local buffer */
		b.wipe();
		b << m_buf;

		b.setLength(len);
		b.setPosition(2);
		b.removeFromBegin();
		b.gotoBegin();
	
		/* remove the command from the parser buffer */
		m_buf.gotoBegin();

		b >> w;
		if (w != 0x044a)
			qDebug("[OFTProxyParser]: strange, the version is not 0x044a: %02x", w);

		Byte cmd;
		b >> cmd;

		OFTProxyError operr;
		OFTProxyAck opack;
		OFTProxyReady oprea;

		switch (cmd){
			case OPID_ERROR:
				operr.parse(b);
				emit proxyError(operr);
				break;
			case OPID_ACK:
				opack.parse(b);
				emit proxyAck(opack);
				break;
			case OPID_READY:
				oprea.parse(b);
				emit proxyReady(oprea);
				break;
			case OPID_INITSEND:
			case OPID_INITRECV:
				qDebug("[OFTProxyParser]: strange, got an client proxy command from the server: %02x", cmd);
				break;
			default:
				qDebug("[OFTProxyParser]: got an unknown proxy command: %02x", cmd);
				return;
		}

	}

}

}

#include "oftproxyparser.moc"
