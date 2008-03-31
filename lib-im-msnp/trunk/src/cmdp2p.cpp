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
#include "cmdp2p.h"
namespace libimmsnp {
//P2P::P2P ():Command ("MSG", 0, "")  {
//	m_accepted = 0;
//	}
P2P::P2P (int idtr):Command ("MSG", idtr, "")  {
	m_accepted = 0;
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
int P2P::getDataOffset() {return qbyte2int(m_dataOffset);}
int P2P::getTotalDataSize() {return qbyte2int(m_totalDataSize);}
int P2P::getMessageLength() {return qbyte2int(m_messageLength);}
int P2P::getP2pSessionId() {return qbyte2int(m_p2pSessionId);}

QByteArray increase(QByteArray num){
	int data = qbyte2int (num);
	data++;
	return int2qbyte(data, 8);
}
void P2P::parse(QByteArray data){
	
	QByteArray binaryHeader = data.mid(data.indexOf("\r\n\r\n")+4, 48);
	//http://msnpiki.msnfanatic.com/index.php/MSNC:Binary_Headers
	// Little-endian abcd = dbca in memory
	// \r\n\r\n
	// SessionID[4]	Identifier[4]	Data offset[8] 		Total data size[8]	Message length[4]	Flag [4]       	identifier[4]	unique ID[4]    	data size[8]
	// 00 00 00 00 	51 67 07 00	00 00 00 00 00 00 00 00 2a 02 00 00 00 00 00 00 2a 02 00 00 		00 00 00 00   	dd 30 48 00  	00 00 00 00    	00 00 00 00 00 00 00 00
	m_sessionID	 = binaryHeader.mid(0,4);	
	m_identifier	 = binaryHeader.mid(4,4);	
	m_dataOffset	 = binaryHeader.mid(8,8);	
	m_totalDataSize  = binaryHeader.mid(16,8);	
	m_flag		 = binaryHeader.mid(20,4);	
	m_messageLength  = binaryHeader.mid(24,4);	
	m_ackUniqueID	 = binaryHeader.mid(28,4);	
	m_ackIdentifier  = binaryHeader.mid(32,4);	
	m_ackDataSize	 = binaryHeader.mid(36,8);	
	//qDebug() << "### SLP BIN DATA" << binaryHeader.toHex();
	//qDebug () << "SLP BIN: sid"<< m_sessionID.toHex() << "Id" << m_identifier.toHex() << "dataOffset" << m_dataOffset.toHex() << "totalDataSize" << m_totalDataSize.toHex() << "messageLength" << m_messageLength.toHex() << "flag" << m_flag.toHex() << "ackIdentifier" << m_ackIdentifier.toHex() << "ackUniqueID" << m_ackUniqueID.toHex() << "ackDataSize" << m_ackDataSize.toHex();

	QByteArray invitation = data.mid(data.indexOf("\r\n\r\n")+52);
	m_data = invitation;
	//INVITE MSNMSGR:probando_msnpy@hotmail.com MSNSLP/1.0\r\nTo: <msnmsgr:probando_msnpy@hotmail.com>\r\nFrom: <msnmsgr:vaticano666@hotmail.com>\r\nVia: MSNSLP/1.0/TLP ;branch={39844FA1-2352-F681-92ED-5180CBD34F21}\r\nCSeq: 0\r\nCall-ID: {D4F03DF4-E61A-5A2E-2588-ACEE1BA9706A}\r\nMax-Forwards: 0\r\nContent-Type: application/x-msnmsgr-sessionreqbody\r\nContent-Length: 199\r\n\r\nEUF-GUID: {4BD96FC0-AB17-4425-A14A-439185962DC8}\r\nSessionID: 94449907\r\nAppID: 4\r\nContext: ewBCADgAQgBFADcAMABEAEUALQBFADIAQwBBAC0ANAA0ADAAMAAtAEEARQAwADMALQA4ADgARgBGADgANQBCADkARgA0AEUAOAB9AA==\r\n\r\n 00 00 00 00 00
	bool ok;
	QRegExp fx;
	//qDebug() << "### SLP INV DATA" << QByteArray(invitation).replace("\n","\\n").replace("\r","\\r");
	//fx.setPattern("(^(\\S+) MSNMSGR:\\S+ MSNSLP/1.0\r\nTo: <msnmsgr:(\\S+)>\r\nFrom: <msnmsgr:(\\S+)>\r\nVia: MSNSLP/1.0/TLP ;branch=\\{(\\S+)\\}\r\nCSeq: (\\d+)\r\nCall-ID: \\{(\\S+)\\}\r\nMax-Forwards: (\\d+)\r\nContent-Type: (\\S+)\r\nContent-Length: (\\d+)\r\n\r\n)");
	fx.setPattern("^(\\S+) MSNMSGR");
	if (fx.indexIn(invitation) != -1)
		m_p2pType 	= QByteArray(fx.cap(1).toUtf8().data());		
	fx.setPattern("To: <msnmsgr:(\\S+)>");
	if (fx.indexIn(invitation) != -1)
		m_to 		= QByteArray(fx.cap(1).toUtf8().data());
	fx.setPattern("From: <msnmsgr:(\\S+)>");
	if (fx.indexIn(invitation) != -1)
		m_from 		= QByteArray(fx.cap(1).toUtf8().data());
	fx.setPattern("branch=\\{(\\S+)\\}");
	if (fx.indexIn(invitation) != -1)
		m_branch	= QByteArray(fx.cap(1).toUtf8().data());
	fx.setPattern("CSeq: (\\d+)\r\n");
	if (fx.indexIn(invitation) != -1)
		m_CsEq 		= QByteArray(fx.cap(1).toUtf8().data()).toInt();
	fx.setPattern("Call-ID: \\{(\\S+)\\}\r\n");
	if (fx.indexIn(invitation) != -1)
		m_callId	= QByteArray(fx.cap(1).toUtf8().data());
	fx.setPattern("Max-Forwards: (\\d+)\r\n");
	if (fx.indexIn(invitation) != -1)
		m_maxForwards 	= QByteArray(fx.cap(1).toUtf8().data());
	fx.setPattern("Content-Type: (\\S+)\r\n");
	if (fx.indexIn(invitation) != -1)
		m_ContentType 	= QByteArray(fx.cap(1).toUtf8().data());
	fx.setPattern("Content-Length: (\\d+)\r\n");
	if (fx.indexIn(invitation) != -1)
		m_ContentLength = QByteArray(fx.cap(1).toUtf8().data());
	//qDebug() << "SLP MSG: " << m_p2pType << m_to << m_from << m_branch << m_CsEq << m_callId << m_maxForwards << m_ContentType << m_ContentLength;
	
	// INVITATION PHASE
	if (m_ContentType == "application/x-msnmsgr-sessionreqbody"){
		fx.setPattern("EUF-GUID: \\{(\\S+)\\}");
		if (fx.indexIn(invitation) != -1)
			m_EUF_GUID = QByteArray(fx.cap(1).toUtf8().data());
		fx.setPattern("SessionID: (\\d+)\r\n");
		if (fx.indexIn(invitation) != -1)
			m_p2pSessionId 	= QByteArray(fx.cap(1).toUtf8().data());
		fx.setPattern("AppID: (\\d+)\r\n)");
		if (fx.indexIn(invitation) != -1)
			m_appId = QByteArray(fx.cap(1).toUtf8().data());
		fx.setPattern("Context: (\\S+)");
		if (fx.indexIn(invitation) != -1)
			m_context 	= QByteArray(fx.cap(1).toUtf8().data());
		qDebug() << "SLP INV: " << m_EUF_GUID << m_p2pSessionId << m_identifier.mid(1,1).toInt(&ok,16);
	}
	

	if (m_ContentType == "application/x-msnmsgr-transreqbody"){
		// REQUEST PHASE
		fx.setMinimal(TRUE);
		fx.setPattern("Bridges?: (.*)\r\n");
		if (fx.indexIn(invitation) != -1)
			m_bridges	= QByteArray(fx.cap(1).toUtf8().data());
		fx.setPattern("NetID: (\\S+)\r\n");
		if (fx.indexIn(invitation) != -1)
			m_netId		= QByteArray(fx.cap(1).toUtf8().data());
		fx.setPattern("Conn-Type: (\\S+)\r\n");
		if (fx.indexIn(invitation) != -1)
			m_connType	= QByteArray(fx.cap(1).toUtf8().data());
		fx.setPattern("TCP-Conn-Type: (\\S+)\r\n");
		if (fx.indexIn(invitation) != -1)
			m_tcpConnType	= QByteArray(fx.cap(1).toUtf8().data());
		fx.setPattern("UPnPNat: (\\S+)\r\n");
		if (fx.indexIn(invitation) != -1)
			m_UPnpNat	= QByteArray(fx.cap(1).toUtf8().data());
		fx.setPattern("ICF: (\\S+)\r\n");
		if (fx.indexIn(invitation) != -1)
			m_Icf		= QByteArray(fx.cap(1).toUtf8().data());
		fx.setPattern("Hashed-Nonce: \\{(\\S+)\\}\r\n");
		if (fx.indexIn(invitation) != -1)
			m_hashedNonce	= QByteArray(fx.cap(1).toUtf8().data());
		fx.setPattern("SessionID: (\\d+)\r\n");
		if (fx.indexIn(invitation) != -1)
			m_sessionID	= QByteArray(fx.cap(1).toUtf8().data());
		fx.setPattern("SChannelState: (\\d+)\r\n");
		if (fx.indexIn(invitation) != -1)
			m_sChannelState	= QByteArray(fx.cap(1).toUtf8().data());
		fx.setPattern("Capabilities-Flags: (\\d+)\r\n)");
		if (fx.indexIn(invitation) != -1)
			m_capabilities	= QByteArray(fx.cap(20).toUtf8().data());
		//qDebug() << "SLP REQ: " << m_bridges << m_netId << m_connType << m_tcpConnType << m_UPnpNat << m_Icf << m_hashedNonce << m_sChannelState << m_capabilities;
	}
	if (m_ContentType == "application/x-msnmsgr-sessionclosebody"){
		m_closeSession = true;
		qDebug() << "SLP BYE: Yes"; 
	}
}

QByteArray P2P::makeCmd() {
	QByteArray res;
	QByteArray data;
	QByteArray binaryHeader;
	QByteArray msnslpData;
	QByteArray data200;
	QByteArray resData200;

	if (m_p2pType == "INVITE" and m_ContentType == "application/x-msnmsgr-sessionreqbody") {
		// ACK for invite 
		data = "MIME-Version: 1.0\r\nContent-Type: application/x-msnmsgrp2p\r\nP2P-Dest: " + m_from + "\r\n\r\n";
		binaryHeader = m_sessionID + increase(m_identifier) + m_dataOffset + m_totalDataSize + m_totalDataSize.mid (0,4) + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("69 44 1e 00 ") + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("0000 0000 0000 0000");

		data.append(binaryHeader);
		res = QByteArray(QString(beginCmd() + " D " + QString::number(data.size()) + "\r\n").toUtf8().data());
		res.append(data);
		// 200 OK
	
		data200 ="MIME-Version: 1.0\r\nContent-Type: application/x-msnmsgrp2p\r\nP2P-Dest: " + m_from + "\r\n\r\n";

		msnslpData.append("MSNSLP/1.0 200 OK\r\n");
		msnslpData.append("To: <msnmsgr:" + m_from + ">\r\n");
		msnslpData.append("From: <msnmsgr:"+ m_to + ">\r\n");
		msnslpData.append("Via: MSNSLP/1.0/TLP ;branch={" + m_branch + "}\r\n");
		msnslpData.append("CSeq: 1 \r\n");
		msnslpData.append("Call-ID: {" + m_callId + "}\r\n");		
		msnslpData.append("Max-Forwards: 0\r\n");
		msnslpData.append("Content-Type: application/x-msnmsgr-sessionreqbody\r\n");
		msnslpData.append("Content-Length: " + QByteArray::number(m_p2pSessionId.size() + 16 )+ "\r\n\r\n");
		msnslpData.append("SessionID: " + m_p2pSessionId + "\r\n\r\n");
		//qDebug() << "#### 200 OK ##" << QByteArray(msnslpData).replace("\r\n","\\r\\n");
		//qDebug() << msnslpData.size() << increase(m_identifier).toHex() << int2qbyte(msnslpData.size()+2, 16).toHex() << int2qbyte(msnslpData.size()+2, 8).toHex();

		binaryHeader = QByteArray::fromHex("00 00 00 00") + increase(m_identifier) + m_dataOffset + int2qbyte(msnslpData.size()+2, 16) + int2qbyte(msnslpData.size()+2, 8) + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("69 44 1e 00") + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("00 00 00 00 00 00 00 00");
		msnslpData.append(QByteArray::fromHex("00000"));
		resData200 = QByteArray(QString("MSG 4 D " + QString::number(data200.size()) + "\r\n").toUtf8().data());
		//qDebug() << "#### 200 OK ##" << QByteArray(msnslpData).replace("\r\n","\\r\\n") << QByteArray(data200).replace("\r\n","\\r\\n") << binaryHeader.toHex() << QByteArray(msnslpData).replace("\r\n","\\r\\n");
		data200.append(binaryHeader);
		data200.append(msnslpData);
		data200.append(QByteArray::fromHex("000000"));
		resData200 = QByteArray(QString("MSG 4 D " + QString::number(data200.size()) + "\r\n").toUtf8().data());
		resData200.append(data200);
		// qDebug() << "#### ACK ##"<< res.toHex();
		// qDebug() << "#### 200 OK ##"<< resData200.toHex();
		res.append(resData200);
		return res;
	}
	else {
		if (m_p2pType == "INVITE" and m_ContentType == "application/x-msnmsgr-transreqbody") {
			if (m_connType == "Direct-Connect") 
				qDebug() << "Inicio Conexion Directa misma IP mismo Puerto";
			if (m_connType == "Firewall") 
				qDebug() << "Inicio conexión con Firewall";
				//MSNSLP/1.0 200 OK\r\n
				//To: <msnmsgr:probando_msnpy2@hotmail.com>\r\n
				//From: <msnmsgr:probando_msnpy@hotmail.com>\r\n
				//Via: MSNSLP/1.0/TLP ;branch={20187971-E9D6-498A-BFFF-232B2FEA9AE1}\r\n
				//CSeq: 1 \r\n
				//Call-ID: {C39C6A94-D263-4CD7-933D-DF6179A4E714}\r\n
				//Max-Forwards: 0\r\n
				//Content-Type: application/x-msnmsgr-transrespbody\r\n
				//Content-Length: 157\r\n\r\n
				//Bridge: TCPv1\r\n
				//Listening: true\r\n
				//Hashed-Nonce: {74A6CD4C-7774-ECE4-C66C-27609828426B}\r\n
				//IPv4Internal-Addrs: 10.0.0.10 10.10.1.178\r\n
				//IPv4Internal-Port: 1140\r\n\r\n
				//.....
				QByteArray msnslpDataData;
				msnslpDataData.append("Bridge: TCPv1\r\n");
				msnslpDataData.append("Listening: true\r\n");
				msnslpDataData.append("Hashed-Nonce: {74A6CD4C-7774-ECE4-C66C-27609828426B}\r\n");
                                msnslpDataData.append("IPv4Internal-Addrs: " + m_clientIp + "\r\n");
                                msnslpDataData.append("IPv4Internal-Port: " + m_clientPort + "\r\n\r\n");

				data = "MIME-Version: 1.0\r\nContent-Type: application/x-msnmsgrp2p\r\nP2P-Dest: " + m_from + "\r\n\r\n";
				binaryHeader = m_sessionID + increase(m_identifier) + m_dataOffset + m_totalDataSize + m_totalDataSize.mid (0,4) + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("69 44 1e 00 ") + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("0000 0000 0000 0000");

				data.append(binaryHeader);
				res = QByteArray(QString(beginCmd() + " D " + QString::number(data.size()) + "\r\n").toUtf8().data());
				res.append(data);

				data200 ="MIME-Version: 1.0\r\nContent-Type: application/x-msnmsgrp2p\r\nP2P-Dest: " + m_from + "\r\n\r\n";

				msnslpData.append("MSNSLP/1.0 200 OK\r\n");
				msnslpData.append("To: <msnmsgr:" + m_from + ">\r\n");
				msnslpData.append("From: <msnmsgr:"+ m_to + ">\r\n");
				msnslpData.append("Via: MSNSLP/1.0/TLP ;branch={" + m_branch + "}\r\n");
				msnslpData.append("CSeq: 1 \r\n");
				msnslpData.append("Call-ID: {" + m_callId + "}\r\n");		
				msnslpData.append("Max-Forwards: 0\r\n");
				msnslpData.append("Content-Type: application/x-msnmsgr-transrespbody\r\n");
				msnslpData.append("Content-Length: " + QString::number(msnslpDataData.size()) + "\r\n\r\n");
				msnslpData.append(msnslpDataData);

				binaryHeader = QByteArray::fromHex("00 00 00 00") + increase(m_identifier) + m_dataOffset + int2qbyte(msnslpData.size()+2, 16) + int2qbyte(msnslpData.size()+2, 8) + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("69 44 1e 00") + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("00 00 00 00 00 00 00 00");
				msnslpData.append(QByteArray::fromHex("00000"));
				resData200 = QByteArray(QString("MSG 4 D " + QString::number(data200.size()) + "\r\n").toUtf8().data());
				//qDebug() << "#### 200 OK ##" << QByteArray(msnslpData).replace("\r\n","\\r\\n") << QByteArray(data200).replace("\r\n","\\r\\n") << binaryHeader.toHex() << QByteArray(msnslpData).replace("\r\n","\\r\\n");
				data200.append(binaryHeader);
				data200.append(msnslpData);
				data200.append(QByteArray::fromHex("000000"));
				resData200 = QByteArray(QString("MSG 8 D " + QString::number(data200.size()) + "\r\n").toUtf8().data());
				resData200.append(data200);
				//qDebug() << "#### ACK ##"<< res.toHex();
				//qDebug() << "#### 200 OK ##"<< resData200.toHex();
				res.append(resData200);
				return res;

			if (m_connType == "Port-Restrict-NAT") 
				qDebug() << "Inicio conexión por misma IP diferente Puerto";
			if (m_connType == "IP-Restrict-NAT") 

				// << 00 00 00 00  c5 c9 02 00  00 00 00 00  00 00 00 00 
				// << cd 01 00 00  00 00 00 00  cd 01 00 00  00 00 00 00 
				// << 17 30 07 00  00 00 00 00  00 00 00 00  00 00 00 00 

				// ACK 
				// >> 00 00 00 00  e9 bb 05 00  00 00 00 00  00 00 00 00 
				// >> cd 01 00 00  00 00 00 00  00 00 00 00  02 00 00 00 
				// >> c5 c9 02 00  17 30 07 00  cd 01 00 00  00 00 00 00

				data = "MIME-Version: 1.0\r\nContent-Type: application/x-msnmsgrp2p\r\nP2P-Dest: " + m_from + "\r\n\r\n";
				binaryHeader = m_sessionID + increase(m_identifier) + m_dataOffset + m_totalDataSize + m_totalDataSize.mid (0,4) + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("69 44 1e 00 ") + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("0000 0000 0000 0000");

				data.append(binaryHeader);
				res = QByteArray(QString(beginCmd() + " D " + QString::number(data.size()) + "\r\n").toUtf8().data());
				res.append(data);

				// 200 OK
				// >> 00 00 00 00  ea bb 05 00  00 00 00 00  00 00 00 00 
				// >> 95 01 00 00  00 00 00 00  95 01 00 00  00 00 00 00 
				// >> e6 e2 24 00  00 00 00 00  00 00 00 00  00 00 00 00


				// MSG probando_msnpy3@hotmail.com probando_msnpy3@hotmail.com 498\r\n
				// MIME-Version: 1.0\r\n
				// Content-Type: application/x-msnmsgrp2p\r\n
				// P2P-Dest: probando_msnpy@hotmail.com\r\n
				// ................[.......[.........E.............
				// MSNSLP/1.0 200 OK\r\n
				// To: <msnmsgr:probando_msnpy@hotmail.com>\r\n
				// From: <msnmsgr:probando_msnpy3@hotmail.com>\r\n
				// Via: MSNSLP/1.0/TLP ;branch={CB64D584-F815-D581-35C7-ED0435A3FF95}\r\n
				// CSeq: 1\r\n
				// Call-ID: {2EA1A9A2-FD64-BF04-379C-2FE1A4F85805}\r\n
				// Max-Forwards: 0\r\n
				// Content-Type: application/x-msnmsgr-sessionreqbody\r\n
				// Content-Length: 83\r\n
				// Bridge: TCPv1\r\n
				// Listening: false\r\n
				// Nonce: {00000000-0000-0000-0000-000000000000}\r\n
				// .....


				data200 ="MIME-Version: 1.0\r\nContent-Type: application/x-msnmsgrp2p\r\nP2P-Dest: " + m_from + "\r\n\r\n";

				msnslpData.append("MSNSLP/1.0 200 OK\r\n");
				msnslpData.append("To: <msnmsgr:" + m_from + ">\r\n");
				msnslpData.append("From: <msnmsgr:"+ m_to + ">\r\n");
				msnslpData.append("Via: MSNSLP/1.0/TLP ;branch={" + m_branch + "}\r\n");
				msnslpData.append("CSeq: 1 \r\n");
				msnslpData.append("Call-ID: {" + m_callId + "}\r\n");		
				msnslpData.append("Max-Forwards: 0\r\n");
				msnslpData.append("Content-Type: application/x-msnmsgr-transrespbody\r\n");
				msnslpData.append("Content-Length: 83\r\n\r\n");
				msnslpData.append("Bridge: TCPv1\r\n" );
				msnslpData.append("Listening: false\r\n" );
				msnslpData.append("Nonce: {00000000-0000-0000-0000-000000000000}\r\n" );
				//qDebug() << "#### 200 OK ##" << QByteArray(msnslpData).replace("\r\n","\\r\\n");
				//qDebug() << msnslpData.size() << increase(m_identifier).toHex() << int2qbyte(msnslpData.size()+2, 16).toHex() << int2qbyte(msnslpData.size()+2, 8).toHex();

				binaryHeader = QByteArray::fromHex("00 00 00 00") + increase(m_identifier) + m_dataOffset + int2qbyte(msnslpData.size()+2, 16) + int2qbyte(msnslpData.size()+2, 8) + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("69 44 1e 00") + QByteArray::fromHex("00 00 00 00") + QByteArray::fromHex("00 00 00 00 00 00 00 00");
				msnslpData.append(QByteArray::fromHex("00000"));
				resData200 = QByteArray(QString("MSG 4 D " + QString::number(data200.size()) + "\r\n").toUtf8().data());
				//qDebug() << "#### 200 OK ##" << QByteArray(msnslpData).replace("\r\n","\\r\\n") << QByteArray(data200).replace("\r\n","\\r\\n") << binaryHeader.toHex() << QByteArray(msnslpData).replace("\r\n","\\r\\n");
				data200.append(binaryHeader);
				data200.append(msnslpData);
				data200.append(QByteArray::fromHex("000000"));
				resData200 = QByteArray(QString("MSG 8 D " + QString::number(data200.size()) + "\r\n").toUtf8().data());
				resData200.append(data200);
				//qDebug() << "#### ACK ##"<< res.toHex();
				//qDebug() << "#### 200 OK ##"<< resData200.toHex();
				res.append(resData200);
				return res;
				qDebug() << "Inicio conexión por distinta IP mismo Puerto";
			if (m_connType == "Symmetric-NAT" or m_connType == "Unknown-NAT")
				qDebug() << "Inicio conexion por distinta IP y distinto Puerto";
			return QByteArray();
		}
		else 
			return QByteArray();
	}
}
//void P2P::addProductId (QString productId) {
//	m_productId = productId;
//}
//void P2P::addMd5(QString md5){
//	m_md5 = md5; 
//}


}
