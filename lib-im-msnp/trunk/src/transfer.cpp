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

#include "transfer.h"
namespace libimmsnp {
Transfer::Transfer () {
	m_filename = "newFile.txt";
	m_type = "";
	m_step = P2P_NULL;
}
Transfer::~Transfer() {}

QByteArray int2byte (int num, int len){
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

void Transfer::setBHSessionID 	 	(int sessionID 		) {m_bh_sessionID	= int2byte(sessionID	,8);}
void Transfer::setBHIdentifier 	 	(int identifier 	) {m_bh_identifier 	= int2byte(identifier 	,8);}
void Transfer::setBHDataOffset 	 	(int dataOffset 	) {m_bh_dataOffset 	= int2byte(dataOffset 	,16);}
void Transfer::setBHTotalDataSize	(int totalDataSize 	) {m_bh_totalDataSize 	= int2byte(totalDataSize,16);}
void Transfer::setBHMessageLength	(int messageLength 	) {m_bh_messageLength	= int2byte(messageLength,8);}
void Transfer::setBHFlag 		(int flag 		) {m_bh_flag 	 	= int2byte(flag 	,8);}
void Transfer::setBHAckIdentifier	(int ackIdentifier	) {m_bh_ackIdentifier	= int2byte(ackIdentifier,8);}
void Transfer::setBHAckUniqueID 	(int ackUniqueID 	) {m_bh_ackUniqueID 	= int2byte(ackUniqueID 	,8);}
void Transfer::setBHAckDataSize 	(int ackDataSize 	) {m_bh_ackDataSize 	= int2byte(ackDataSize 	,16);}

void Transfer::createMyIdentifier (){
	m_myIdentifier = rand()%0x0FFFFFF0 + 4;
}

QByteArray Transfer::incMyIdentifier(int inc){
	m_myIdentifier += inc;
	return int2byte(m_myIdentifier, 8);
}
}
#include "transfer.moc"
