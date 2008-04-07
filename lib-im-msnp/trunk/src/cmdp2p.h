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
	
	int getStatus(){return m_status;}
	
	int getId();	
	QByteArray getType() {return m_ContentType;}	
	void setAccepted() { m_accepted = 1;}
	void setDenied() { m_accepted = -1;}
	int isAccepted() {return m_accepted;}
	void setPath(QByteArray path) {m_path = path;}

	QByteArray getPath() {return m_path;}
	QByteArray getFileName() {return m_context;}
	QByteArray getData() {return m_data;}
	QByteArray getEUF_GUID() {return m_EUF_GUID;}
	
	bool isAckMsg() {return m_prevAck == m_bh_ackIdentifier;}
	
	int getBHsessionID();	
	int getBHid(); 		
	int getBHdataOffset();
	int getBHtotalDataSize();
	int getBHflag();	
	int getBHmessageLength();
	int getBHackUniqueID();
	int getBHackIdentifier(); 	
	int getBHackDataSize();

	int getP2pSessionId();
	//int get() {return m_;}
	//int get() {return m_;}

	QByteArray makeCmd();
	QByteArray makeBId();
	QByteArray makeAck();
	QByteArray make200ok();
	QByteArray makeDecline();
	void parse (QByteArray);
private:
	// BINARY HEADER
	QByteArray m_bh_sessionID;	
	QByteArray m_bh_identifier;	
	QByteArray m_bh_dataOffset;
	QByteArray m_bh_totalDataSize;
	QByteArray m_bh_flag;		
	QByteArray m_bh_messageLength;
	QByteArray m_bh_ackUniqueID;	
	QByteArray m_bh_ackIdentifier; 
	QByteArray m_bh_ackDataSize;	

	QByteArray m_prevAck; 

	// MSG HEADER
	QByteArray m_p2pType;
	QByteArray m_to;
	QByteArray m_from;
	QByteArray m_branch; 
	int m_CsEq;
	QByteArray m_callId;
	QByteArray m_maxForwards;
	QByteArray m_ContentType;
	QByteArray m_ContentLength;

	// INVITATION
	QByteArray m_EUF_GUID;
	QByteArray m_p2pSessionId;
	QByteArray m_appId;
	QByteArray m_context;
	
	// DIRECT CONNECT
	QByteArray m_dc_bridges;
	QByteArray m_dc_netId;
	QByteArray m_dc_connType;
	QByteArray m_dc_tcpConnType;
	QByteArray m_dc_UPnpNat;
	QByteArray m_dc_Icf;
	QByteArray m_dc_hashedNonce;
	QByteArray m_dc_sChannelState;
	QByteArray m_dc_capabilities;
	QByteArray m_dc_sessionID;

	// MORE STUFF
	int m_status;
	QString m_clientIp;
	QString m_clientPort;
	int m_accepted;
	QByteArray m_path;

	// DATA
	QByteArray m_data;
	
	// BYE
	bool m_closeSession;
};

}
#endif //_CMDP2P_H_

