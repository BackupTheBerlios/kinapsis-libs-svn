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

#ifndef _CMDP2P_H_
#define _CMDP2P_H_
#include "command.h" 

namespace libimmsnp {
class P2P : public Command {

public:
	P2P (int);
	// TODO: check this P2P()
	P2P ();
	virtual ~P2P();
	void setClientIp(QString ip) {m_clientIp = ip;}
	void setClientPort(QString port) {m_clientPort = port;}
	
	int getId();	
	QByteArray getType() {return m_ContentType;}	
	void setAccepted() { m_accepted = 1;}
	void setDenied() { m_accepted = -1;}
	int isAccepted() {return m_accepted;}
	void setPath(QByteArray path) {m_path = path;}
	QByteArray getPath() {return m_path;}
	QByteArray getData() {return m_data;}
	QByteArray getEUF_GUID() {return m_EUF_GUID;}
	
	bool isAckMsg() {return m_prevAck == m_ackIdentifier;}
	
	int getDataOffset(); 
	int getTotalDataSize();
	int getMessageLength();
	int getP2pSessionId();
	int getAckUniqueID();
	//int get() {return m_;}
	//int get() {return m_;}

	QByteArray makeCmd();
	void parse (QByteArray);
private:
	QByteArray m_sessionID;	
	QByteArray m_identifier;	
	QByteArray m_dataOffset;
	QByteArray m_totalDataSize;
	QByteArray m_flag;		
	QByteArray m_messageLength;
	QByteArray m_ackUniqueID;	
	QByteArray m_ackIdentifier; 
	QByteArray m_ackDataSize;	

	QByteArray m_prevAck; 

	QByteArray m_p2pType;
	QByteArray m_to;
	QByteArray m_from;
	QByteArray m_branch; 
	int m_CsEq;
	QByteArray m_callId;
	QByteArray m_maxForwards;
	QByteArray m_ContentType;
	QByteArray m_ContentLength;

	// Invitation
	QByteArray m_EUF_GUID;
	QByteArray m_p2pSessionId;
	QByteArray m_appId;
	QByteArray m_context;
	
	int m_accepted;
	QByteArray m_path;
		
	// initial dc
	QByteArray m_netId;
	QByteArray m_bridges;
	QByteArray m_connType;
	QByteArray m_tcpConnType;
	QByteArray m_UPnpNat;
	QByteArray m_Icf;
	QByteArray m_hashedNonce;
	QByteArray m_sChannelState;
	QByteArray m_capabilities;

	QString m_clientIp;
	QString m_clientPort;
	
	// DATA
	QByteArray m_data;
	
	// BYE
	bool m_closeSession;
};

}
#endif //_CMDP2P_H_

