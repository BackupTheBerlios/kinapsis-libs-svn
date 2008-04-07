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
#include "parserp2p.h"
namespace libimmsnp {
ParserP2P::ParserP2P () {
	m_CsEq = 0;
	m_step = P2P_NULL;
}
ParserP2P::~ParserP2P() {}


int byte2int (QByteArray num){
	bool ok;
	QByteArray tmp = num.mid(3,1).toHex().data();
	tmp.append(num.mid(2,1).toHex().data());
	tmp.append(num.mid(1,1).toHex().data());
	tmp.append(num.mid(0,1).toHex().data());
	return tmp.toInt(&ok,16);
}

int  ParserP2P::getBHSessionID 	 	() {return byte2int(m_bh_sessionID	);}
int  ParserP2P::getBHid			() {return byte2int(m_bh_identifier	);}
int  ParserP2P::getBHDataOffset 	() {return byte2int(m_bh_dataOffset 	);}
int  ParserP2P::getBHTotalDataSize	() {return byte2int(m_bh_totalDataSize	);}
int  ParserP2P::getBHFlag 		() {return byte2int(m_bh_flag 	 	);}
int  ParserP2P::getBHMessageLength	() {return byte2int(m_bh_messageLength	);}
int  ParserP2P::getBHAckUniqueID 	() {return byte2int(m_bh_ackUniqueID 	);}
int  ParserP2P::getBHAckIdentifier	() {return byte2int(m_bh_ackIdentifier	);}
int  ParserP2P::getBHAckDataSize 	() {return byte2int(m_bh_ackDataSize 	);}


bool ParserP2P::isFinished() {
	bool st = (byte2int(m_bh_dataOffset) + byte2int(m_bh_messageLength)) == byte2int(m_bh_totalDataSize);
	qDebug() << byte2int(m_bh_dataOffset) << "+" << byte2int(m_bh_messageLength) << byte2int(m_bh_totalDataSize) << st;
	return st;
}

void ParserP2P::parse(QByteArray data){
	QByteArray binaryHeader = data.mid(data.indexOf("\r\n\r\n")+4, 48);
	//http://msnpiki.msnfanatic.com/index.php/MSNC:Binary_Headers
	// SessionID[4]	Identifier[4]	Data offset[8] 		Total data size[8]	Message length[4]	Flag [4]       	identifier[4]	unique ID[4]    	data size[8]
	// 00 00 00 00 	51 67 07 00	00 00 00 00 00 00 00 00 2a 02 00 00 00 00 00 00 2a 02 00 00 		00 00 00 00   	dd 30 48 00  	00 00 00 00    	00 00 00 00 00 00 00 00
	m_bh_sessionID	 	= binaryHeader.mid(0,4);	
	m_bh_identifier	 	= binaryHeader.mid(4,4);	
	m_bh_dataOffset	 	= binaryHeader.mid(8,8);	
	m_bh_totalDataSize  	= binaryHeader.mid(16,8);	
	m_bh_flag		= binaryHeader.mid(20,4);	
	m_bh_messageLength  	= binaryHeader.mid(24,4);	
	m_bh_ackUniqueID	= binaryHeader.mid(28,4);	
	m_bh_ackIdentifier 	= binaryHeader.mid(32,4);	
	m_bh_ackDataSize	= binaryHeader.mid(36,8);	

//	qDebug () << "#&&# PARSER P2P:  SLP BIN:" << m_bh_sessionID.toHex() << m_bh_identifier.toHex() << m_bh_dataOffset.toHex() << m_bh_totalDataSize.toHex() << m_bh_flag.toHex() << m_bh_messageLength.toHex() << m_bh_ackUniqueID.toHex() << m_bh_ackIdentifier.toHex() << m_bh_ackDataSize.toHex();

	QByteArray invitation = data.mid(data.indexOf("\r\n\r\n")+52);
	int dataLen = invitation.size() - 4;
	m_data = invitation.mid(0,dataLen);
	bool ok;
	QRegExp fx;

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

//	qDebug() << "#&&# PARSER P2P:  SLP DTA" << QByteArray(invitation).replace("\n","\\n").replace("\r","\\r");
	//if (m_data.size() == 0) {
	//	qDebug() << "\n\n## RECIBO ACK " << m_data.size() <<"\n";
	//	m_step = P2P_ACK;
	//}
	
	// INVITATION PHASE
	if (m_ContentType == "application/x-msnmsgr-sessionreqbody"){
		m_step = P2P_INVITATION;
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
			m_context 	=  QByteArray::fromHex(QByteArray::fromBase64(QByteArray(fx.cap(1).toUtf8().data())).mid(20).toHex().replace("00",""));
	}
	
	
	if (m_ContentType == "application/x-msnmsgr-transreqbody"){
		m_step = P2P_NEGOTIATION;
		// REQUEST PHASE
		fx.setMinimal(TRUE);
		fx.setPattern("Bridges?: (.*)\r\n");
		if (fx.indexIn(invitation) != -1)
			m_dc_bridges	= QByteArray(fx.cap(1).toUtf8().data());
		fx.setPattern("NetID: (\\S+)\r\n");
		if (fx.indexIn(invitation) != -1)
			m_dc_netId		= QByteArray(fx.cap(1).toUtf8().data());
		fx.setPattern("Conn-Type: (\\S+)\r\n");
		if (fx.indexIn(invitation) != -1)
			m_dc_connType	= QByteArray(fx.cap(1).toUtf8().data());
		fx.setPattern("TCP-Conn-Type: (\\S+)\r\n");
		if (fx.indexIn(invitation) != -1)
			m_dc_tcpConnType	= QByteArray(fx.cap(1).toUtf8().data());
		fx.setPattern("UPnPNat: (\\S+)\r\n");
		if (fx.indexIn(invitation) != -1)
			m_dc_UPnpNat	= QByteArray(fx.cap(1).toUtf8().data());
		fx.setPattern("ICF: (\\S+)\r\n");
		if (fx.indexIn(invitation) != -1)
			m_dc_Icf		= QByteArray(fx.cap(1).toUtf8().data());
		fx.setPattern("Hashed-Nonce: \\{(\\S+)\\}\r\n");
		if (fx.indexIn(invitation) != -1)
			m_dc_hashedNonce	= QByteArray(fx.cap(1).toUtf8().data());
		fx.setPattern("SessionID: (\\d+)\r\n");
		if (fx.indexIn(invitation) != -1)
			m_dc_sessionID	= QByteArray(fx.cap(1).toUtf8().data());
		fx.setPattern("SChannelState: (\\d+)\r\n");
		if (fx.indexIn(invitation) != -1)
			m_dc_sChannelState	= QByteArray(fx.cap(1).toUtf8().data());
		fx.setPattern("Capabilities-Flags: (\\d+)\r\n)");
		if (fx.indexIn(invitation) != -1)
			m_dc_capabilities	= QByteArray(fx.cap(20).toUtf8().data());
		//qDebug() << "### SLP REQ: " << m_dc_bridges << m_dc_netId << m_dc_connType << m_dc_tcpConnType << m_dc_UPnpNat << m_dc_Icf << m_dc_hashedNonce << m_dc_sChannelState << m_dc_capabilities;
	}
	if (m_ContentType == "application/x-msnmsgr-sessionclosebody"){
		m_closeSession = true;
		//qDebug() << "### SLP BYE: Yes"; 
	}
}

}
