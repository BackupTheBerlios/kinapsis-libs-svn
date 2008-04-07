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
	QByteArray binaryHeader, res, data, msnslpData;
	if (m_cmd == P2PC_200OK){
		msnslpData.append("MSNSLP/1.0 200 OK\r\n");
		msnslpData.append("To: <msnmsgr:" + m_from + ">\r\n");
		msnslpData.append("From: <msnmsgr:"+ m_to + ">\r\n");
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

	data = "MIME-Version: 1.0\r\nContent-Type: application/x-msnmsgrp2p\r\nP2P-Dest: " + m_from + "\r\n\r\n";
	data.append(binaryHeader);
	data.append(msnslpData);
	data.append(QByteArray::fromHex("00 00 00 00"));

	res = QByteArray(QString(beginCmd() + " D " + QString::number(data.size()) + "\r\n").toUtf8().data());
	qDebug() << "\nENVIO " << QByteArray(res).replace("\n","\\n").replace("\r","\\r") << "MIME-Version: 1.0\\r\\nContent-Type: application/x-msnmsgrp2p\\r\\nP2P-Dest: " + m_from + "\\r\\n\\r\\n" << binaryHeader.toHex() << QByteArray(msnslpData).replace("\n","\\n").replace("\r","\\r");
	res.append(data);
	return res;
}

//QByteArray P2P::makeCmd() {
//	QByteArray res;
//	QByteArray data;
//	QByteArray binaryHeader;
//	QByteArray msnslpData;
//	QByteArray data200;
//	QByteArray resData200;
//
//	if (m_p2pType == "INVITE" and m_ContentType == "application/x-msnmsgr-sessionreqbody") {
//		// ACK for invite 
//		data = "MIME-Version: 1.0\r\nContent-Type: application/x-msnmsgrp2p\r\nP2P-Dest: " + m_from + "\r\n\r\n";
//		binaryHeader = m_bh_sessionID + increase(m_bh_identifier) + m_bh_dataOffset + m_bh_totalDataSize + m_bh_totalDataSize.mid (0,4) + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("69 44 1e 00 ") + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("0000 0000 0000 0000");
//
//		data.append(binaryHeader);
//		res = QByteArray(QString(beginCmd() + " D " + QString::number(data.size()) + "\r\n").toUtf8().data());
//		res.append(data);
//		// 200 OK
//	
//		data200 ="MIME-Version: 1.0\r\nContent-Type: application/x-msnmsgrp2p\r\nP2P-Dest: " + m_from + "\r\n\r\n";
//
//		msnslpData.append("MSNSLP/1.0 200 OK\r\n");
//		msnslpData.append("To: <msnmsgr:" + m_to + ">\r\n");
//		msnslpData.append("From: <msnmsgr:"+ m_from + ">\r\n");
//		msnslpData.append("Via: MSNSLP/1.0/TLP ;branch={" + m_branch + "}\r\n");
//		msnslpData.append("CSeq: 1 \r\n");
//		msnslpData.append("Call-ID: {" + m_callId + "}\r\n");		
//		msnslpData.append("Max-Forwards: 0\r\n");
//		msnslpData.append("Content-Type: application/x-msnmsgr-sessionreqbody\r\n");
//		msnslpData.append("Content-Length: " + QByteArray::number(m_p2pSessionId.size() + 16 )+ "\r\n\r\n");
//		msnslpData.append("SessionID: " + m_p2pSessionId + "\r\n\r\n");
//		//qDebug() << "#### 200 OK ##" << QByteArray(msnslpData).replace("\r\n","\\r\\n");
//		//qDebug() << msnslpData.size() << increase(m_identifier).toHex() << int2qbyte(msnslpData.size()+2, 16).toHex() << int2qbyte(msnslpData.size()+2, 8).toHex();
//
//		binaryHeader = QByteArray::fromHex("00 00 00 00") + increase(m_bh_identifier) + m_bh_dataOffset + int2qbyte(msnslpData.size()+2, 16) + int2qbyte(msnslpData.size()+2, 8) + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("69 44 1e 00") + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("00 00 00 00 00 00 00 00");
//		msnslpData.append(QByteArray::fromHex("00000"));
//		resData200 = QByteArray(QString("MSG 4 D " + QString::number(data200.size()) + "\r\n").toUtf8().data());
//		//qDebug() << "#### 200 OK ##" << QByteArray(msnslpData).replace("\r\n","\\r\\n") << QByteArray(data200).replace("\r\n","\\r\\n") << binaryHeader.toHex() << QByteArray(msnslpData).replace("\r\n","\\r\\n");
//		data200.append(binaryHeader);
//		data200.append(msnslpData);
//		data200.append(QByteArray::fromHex("000000"));
//		resData200 = QByteArray(QString("MSG 4 D " + QString::number(data200.size()) + "\r\n").toUtf8().data());
//		resData200.append(data200);
//		// qDebug() << "#### ACK ##"<< res.toHex();
//		// qDebug() << "#### 200 OK ##"<< resData200.toHex();
//		res.append(resData200);
//		return res;
//	}
//	else {
//		if (m_p2pType == "INVITE" and m_ContentType == "application/x-msnmsgr-transreqbody") {
//			if (m_dc_connType == "Direct-Connect") 
//				qDebug() << "Inicio Conexion Directa misma IP mismo Puerto";
//			if (m_dc_connType == "Firewall") 
//				qDebug() << "Inicio conexión con Firewall";
//				//MSNSLP/1.0 200 OK\r\n
//				//To: <msnmsgr:probando_msnpy2@hotmail.com>\r\n
//				//From: <msnmsgr:probando_msnpy@hotmail.com>\r\n
//				//Via: MSNSLP/1.0/TLP ;branch={20187971-E9D6-498A-BFFF-232B2FEA9AE1}\r\n
//				//CSeq: 1 \r\n
//				//Call-ID: {C39C6A94-D263-4CD7-933D-DF6179A4E714}\r\n
//				//Max-Forwards: 0\r\n
//				//Content-Type: application/x-msnmsgr-transrespbody\r\n
//				//Content-Length: 157\r\n\r\n
//				//Bridge: TCPv1\r\n
//				//Listening: true\r\n
//				//Hashed-Nonce: {74A6CD4C-7774-ECE4-C66C-27609828426B}\r\n
//				//IPv4Internal-Addrs: 10.0.0.10 10.10.1.178\r\n
//				//IPv4Internal-Port: 1140\r\n\r\n
//				//.....
//				QByteArray msnslpDataData;
//				msnslpDataData.append("Bridge: TCPv1\r\n");
//				msnslpDataData.append("Listening: true\r\n");
//				msnslpDataData.append("Hashed-Nonce: {74A6CD4C-7774-ECE4-C66C-27609828426B}\r\n");
//                                msnslpDataData.append("IPv4Internal-Addrs: " + m_clientIp + "\r\n");
//                                msnslpDataData.append("IPv4Internal-Port: " + m_clientPort + "\r\n\r\n");
//
//				data = "MIME-Version: 1.0\r\nContent-Type: application/x-msnmsgrp2p\r\nP2P-Dest: " + m_from + "\r\n\r\n";
//				binaryHeader = m_bh_sessionID + increase(m_bh_identifier) + m_bh_dataOffset + m_bh_totalDataSize + m_bh_totalDataSize.mid (0,4) + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("69 44 1e 00 ") + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("0000 0000 0000 0000");
//
//				data.append(binaryHeader);
//				res = QByteArray(QString(beginCmd() + " D " + QString::number(data.size()) + "\r\n").toUtf8().data());
//				res.append(data);
//
//				data200 ="MIME-Version: 1.0\r\nContent-Type: application/x-msnmsgrp2p\r\nP2P-Dest: " + m_from + "\r\n\r\n";
//
//				msnslpData.append("MSNSLP/1.0 200 OK\r\n");
//				msnslpData.append("To: <msnmsgr:" + m_from + ">\r\n");
//				msnslpData.append("From: <msnmsgr:"+ m_to + ">\r\n");
//				msnslpData.append("Via: MSNSLP/1.0/TLP ;branch={" + m_branch + "}\r\n");
//				msnslpData.append("CSeq: 1 \r\n");
//				msnslpData.append("Call-ID: {" + m_callId + "}\r\n");		
//				msnslpData.append("Max-Forwards: 0\r\n");
//				msnslpData.append("Content-Type: application/x-msnmsgr-transrespbody\r\n");
//				msnslpData.append("Content-Length: " + QString::number(msnslpDataData.size()) + "\r\n\r\n");
//				msnslpData.append(msnslpDataData);
//
//				binaryHeader = QByteArray::fromHex("00 00 00 00") + increase(m_bh_identifier) + m_bh_dataOffset + int2qbyte(msnslpData.size()+2, 16) + int2qbyte(msnslpData.size()+2, 8) + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("69 44 1e 00") + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("00 00 00 00 00 00 00 00");
//				msnslpData.append(QByteArray::fromHex("00000"));
//				resData200 = QByteArray(QString("MSG 4 D " + QString::number(data200.size()) + "\r\n").toUtf8().data());
//				//qDebug() << "#### 200 OK ##" << QByteArray(msnslpData).replace("\r\n","\\r\\n") << QByteArray(data200).replace("\r\n","\\r\\n") << binaryHeader.toHex() << QByteArray(msnslpData).replace("\r\n","\\r\\n");
//				data200.append(binaryHeader);
//				data200.append(msnslpData);
//				data200.append(QByteArray::fromHex("000000"));
//				resData200 = QByteArray(QString("MSG 8 D " + QString::number(data200.size()) + "\r\n").toUtf8().data());
//				resData200.append(data200);
//				//qDebug() << "#### ACK ##"<< res.toHex();
//				//qDebug() << "#### 200 OK ##"<< resData200.toHex();
//				res.append(resData200);
//				return res;
//
//			if (m_dc_connType == "Port-Restrict-NAT") 
//				qDebug() << "Inicio conexión por misma IP diferente Puerto";
//			if (m_dc_connType == "IP-Restrict-NAT") 
//
//				// << 00 00 00 00  c5 c9 02 00  00 00 00 00  00 00 00 00 
//				// << cd 01 00 00  00 00 00 00  cd 01 00 00  00 00 00 00 
//				// << 17 30 07 00  00 00 00 00  00 00 00 00  00 00 00 00 
//
//				// ACK 
//				// >> 00 00 00 00  e9 bb 05 00  00 00 00 00  00 00 00 00 
//				// >> cd 01 00 00  00 00 00 00  00 00 00 00  02 00 00 00 
//				// >> c5 c9 02 00  17 30 07 00  cd 01 00 00  00 00 00 00
//
//				data = "MIME-Version: 1.0\r\nContent-Type: application/x-msnmsgrp2p\r\nP2P-Dest: " + m_from + "\r\n\r\n";
//				binaryHeader = m_bh_sessionID + increase(m_bh_identifier) + m_bh_dataOffset + m_bh_totalDataSize + m_bh_totalDataSize.mid (0,4) + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("69 44 1e 00 ") + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("0000 0000 0000 0000");
//
//				data.append(binaryHeader);
//				res = QByteArray(QString(beginCmd() + " D " + QString::number(data.size()) + "\r\n").toUtf8().data());
//				res.append(data);
//
//				// 200 OK
//				// >> 00 00 00 00  ea bb 05 00  00 00 00 00  00 00 00 00 
//				// >> 95 01 00 00  00 00 00 00  95 01 00 00  00 00 00 00 
//				// >> e6 e2 24 00  00 00 00 00  00 00 00 00  00 00 00 00
//
//
//				// MSG probando_msnpy3@hotmail.com probando_msnpy3@hotmail.com 498\r\n
//				// MIME-Version: 1.0\r\n
//				// Content-Type: application/x-msnmsgrp2p\r\n
//				// P2P-Dest: probando_msnpy@hotmail.com\r\n
//				// ................[.......[.........E.............
//				// MSNSLP/1.0 200 OK\r\n
//				// To: <msnmsgr:probando_msnpy@hotmail.com>\r\n
//				// From: <msnmsgr:probando_msnpy3@hotmail.com>\r\n
//				// Via: MSNSLP/1.0/TLP ;branch={CB64D584-F815-D581-35C7-ED0435A3FF95}\r\n
//				// CSeq: 1\r\n
//				// Call-ID: {2EA1A9A2-FD64-BF04-379C-2FE1A4F85805}\r\n
//				// Max-Forwards: 0\r\n
//				// Content-Type: application/x-msnmsgr-sessionreqbody\r\n
//				// Content-Length: 83\r\n
//				// Bridge: TCPv1\r\n
//				// Listening: false\r\n
//				// Nonce: {00000000-0000-0000-0000-000000000000}\r\n
//				// .....
//
//
//				data200 ="MIME-Version: 1.0\r\nContent-Type: application/x-msnmsgrp2p\r\nP2P-Dest: " + m_from + "\r\n\r\n";
//
//				msnslpData.append("MSNSLP/1.0 200 OK\r\n");
//				msnslpData.append("To: <msnmsgr:" + m_from + ">\r\n");
//				msnslpData.append("From: <msnmsgr:"+ m_to + ">\r\n");
//				msnslpData.append("Via: MSNSLP/1.0/TLP ;branch={" + m_branch + "}\r\n");
//				msnslpData.append("CSeq: 1 \r\n");
//				msnslpData.append("Call-ID: {" + m_callId + "}\r\n");		
//				msnslpData.append("Max-Forwards: 0\r\n");
//				msnslpData.append("Content-Type: application/x-msnmsgr-transrespbody\r\n");
//				msnslpData.append("Content-Length: 83\r\n\r\n");
//				msnslpData.append("Bridge: TCPv1\r\n" );
//				msnslpData.append("Listening: false\r\n" );
//				msnslpData.append("Nonce: {00000000-0000-0000-0000-000000000000}\r\n" );
//				//qDebug() << "#### 200 OK ##" << QByteArray(msnslpData).replace("\r\n","\\r\\n");
//				//qDebug() << msnslpData.size() << increase(m_identifier).toHex() << int2qbyte(msnslpData.size()+2, 16).toHex() << int2qbyte(msnslpData.size()+2, 8).toHex();
//
//				binaryHeader = QByteArray::fromHex("00 00 00 00") + increase(m_bh_identifier) + m_bh_dataOffset + int2qbyte(msnslpData.size()+2, 16) + int2qbyte(msnslpData.size()+2, 8) + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("69 44 1e 00") + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("00 00 00 00 00 00 00 00");
//				msnslpData.append(QByteArray::fromHex("00000"));
//				resData200 = QByteArray(QString("MSG 4 D " + QString::number(data200.size()) + "\r\n").toUtf8().data());
//				//qDebug() << "#### 200 OK ##" << QByteArray(msnslpData).replace("\r\n","\\r\\n") << QByteArray(data200).replace("\r\n","\\r\\n") << binaryHeader.toHex() << QByteArray(msnslpData).replace("\r\n","\\r\\n");
//				data200.append(binaryHeader);
//				data200.append(msnslpData);
//				data200.append(QByteArray::fromHex("000000"));
//				resData200 = QByteArray(QString("MSG 8 D " + QString::number(data200.size()) + "\r\n").toUtf8().data());
//				resData200.append(data200);
//				//qDebug() << "#### ACK ##"<< res.toHex();
//				//qDebug() << "#### 200 OK ##"<< resData200.toHex();
//				res.append(resData200);
//				return res;
//				qDebug() << "Inicio conexión por distinta IP mismo Puerto";
//			if (m_dc_connType == "Symmetric-NAT" or m_dc_connType == "Unknown-NAT")
//				qDebug() << "Inicio conexion por distinta IP y distinto Puerto";
//			return QByteArray();
//		}
//		else 
//			return QByteArray();
//	}
//}
}
