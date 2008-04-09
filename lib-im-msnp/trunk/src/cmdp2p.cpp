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

#include <QRegExp>
#include <QDebug>
#include <QtEndian>
#include <QTextCodec>
#include "cmdp2p.h"
namespace libimmsnp {
P2P::P2P (int idtr):Command ("MSG", idtr, "")  {
	m_bh_sessionID 		= QByteArray::fromHex("00 00 00 00");	
	m_bh_identifier 	= QByteArray::fromHex("00 00 00 00");	
	m_bh_dataOffset 	= QByteArray::fromHex("00000000 00000000");
	m_bh_totalDataSize 	= QByteArray::fromHex("00000000 00000000");
	m_bh_flag 		= QByteArray::fromHex("00 00 00 00");		
	m_bh_messageLength 	= QByteArray::fromHex("00 00 00 00");
	m_bh_ackUniqueID 	= QByteArray::fromHex("00 00 00 00");	
	m_bh_ackIdentifier	= QByteArray::fromHex("00 00 00 00"); 
	m_bh_ackDataSize 	= QByteArray::fromHex("00000000 00000000");	
	}
P2P::~P2P() {}

int qbyte2int (QByteArray num){
	bool ok;
	QByteArray tmp = num.mid(3,1).toHex().data();
	tmp.append(num.mid(2,1).toHex().data());
	tmp.append(num.mid(1,1).toHex().data());
	tmp.append(num.mid(0,1).toHex().data());
	return tmp.toInt(&ok,16);
}

QByteArray int2qbyte (int num, int len){
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

QByteArray increase(QByteArray num){
	int data = qbyte2int (num);
	data++;
	return int2qbyte(data, 8);
}

QByteArray P2P::make(){
	QByteArray binaryHeader, res = "", data = "", msnslpData= "";
	if (m_cmd == P2PC_200OK){
		msnslpData.append("MSNSLP/1.0 200 OK\r\n");
		msnslpData.append("To: <msnmsgr:" + m_to + ">\r\n");
		msnslpData.append("From: <msnmsgr:"+ m_from + ">\r\n");
		msnslpData.append("Via: MSNSLP/1.0/TLP ;branch={" + m_branch + "}\r\n");
		msnslpData.append("CSeq: 1 \r\n");
		msnslpData.append("Call-ID: {" + m_callId + "}\r\n");		
		msnslpData.append("Max-Forwards: 0\r\n");
		if (m_step == P2P_INVITATION) {
			msnslpData.append("Content-Type: application/x-msnmsgr-sessionreqbody\r\n");
			msnslpData.append("Content-Length: " + QByteArray::number(m_p2pSessionId.size() + 16 )+ "\r\n\r\n");
			msnslpData.append("SessionID: " + m_p2pSessionId + "\r\n\r\n");
		}

		else if (m_step = P2P_NEGOTIATION) {
			msnslpData.append("Content-Type: application/x-msnmsgr-transrespbody\r\n");
			msnslpData.append("Content-Length: 83\r\n\r\n");
			msnslpData.append("Bridge: TCPv1\r\n" );
			msnslpData.append("Listening: false\r\n" );
			msnslpData.append("Nonce: {00000000-0000-0000-0000-000000000000}\r\n\r\n" );
		}

		msnslpData.append(QByteArray::fromHex("00"));
		m_bh_totalDataSize = int2qbyte(msnslpData.size(), 16);
		m_bh_messageLength = int2qbyte(msnslpData.size(), 8);
	}
	binaryHeader =  m_bh_sessionID + m_bh_identifier + m_bh_dataOffset;
	binaryHeader += m_bh_totalDataSize + m_bh_messageLength + m_bh_flag;
	binaryHeader += m_bh_ackIdentifier + m_bh_ackUniqueID + m_bh_ackDataSize;

	data = "MIME-Version: 1.0\r\nContent-Type: application/x-msnmsgrp2p\r\nP2P-Dest: " + m_to + "\r\n\r\n";
	data.append(binaryHeader);
	data.append(msnslpData);
	if (m_cmd == P2PC_200OK || m_cmd == P2PC_ACK){
		data.append(QByteArray::fromHex("00 00 00 00"));
	}
	res = QByteArray(QString(beginCmd() + " D " + QString::number(data.size()) + "\r\n").toUtf8().data());
	qDebug() << "\nENVIO " << QByteArray(res).replace("\n","\\n").replace("\r","\\r") << "MIME-Version: 1.0\\r\\nContent-Type: application/x-msnmsgrp2p\\r\\nP2P-Dest: " + m_to + "\\r\\n\\r\\n" << binaryHeader.toHex() << QByteArray(msnslpData).replace("\n","\\n").replace("\r","\\r");
	res.append(data);
	return res;
}
}
