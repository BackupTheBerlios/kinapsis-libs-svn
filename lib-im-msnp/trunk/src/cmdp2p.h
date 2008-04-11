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
#include "libimmsnp.h"

namespace libimmsnp {
class P2P : public Command {

public:
	P2P (int);
	virtual ~P2P();
	void setClientIp(QString ip) {m_clientIp = ip;}
	void setClientPort(QString port) {m_clientPort = port;}

	void setBHSessionID 	 (QByteArray sessionID 		) {m_bh_sessionID	= sessionID	;}
	void setBHIdentifier 	 (QByteArray identifier 	) {m_bh_identifier 	= identifier 	;}
	void setBHDataOffset 	 (QByteArray dataOffset 	) {m_bh_dataOffset 	= dataOffset 	;}
	void setBHTotalDataSize	 (QByteArray totalDataSize 	) {m_bh_totalDataSize 	= totalDataSize ;}
	void setBHMessageLength	 (QByteArray messageLength 	) {m_bh_messageLength	= messageLength ;}
	void setBHFlag 		 (QByteArray flag 		) {m_bh_flag 	 	= flag 	 	;}
	void setBHAckIdentifier	 (QByteArray ackIdentifier	) {m_bh_ackIdentifier	= ackIdentifier ;}
	void setBHAckUniqueID 	 (QByteArray ackUniqueID 	) {m_bh_ackUniqueID 	= ackUniqueID 	;}
	void setBHAckDataSize 	 (QByteArray ackDataSize 	) {m_bh_ackDataSize 	= ackDataSize 	;}

	void setCsEq (int cseq) {m_CsEq = cseq;}

	void setStep(P2pStep step) {m_step = step;}
	void setCmd(P2pCmd cmd) {m_cmd = cmd;}
	void setFrom (QByteArray from) {m_from = from;}
	void setTo (QByteArray to) {m_to = to;}

	void setBranch(QByteArray branch) {m_branch = branch;}
	void setCallId(QByteArray callId) {m_callId = callId;}
	void setp2pSessionId(QByteArray p2pSessionId) {m_p2pSessionId = p2pSessionId;}

	QByteArray make();

	QByteArray makeCmd(){return QByteArray("");}
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
	QString m_clientIp;
	QString m_clientPort;
	P2pStep m_step;	
	P2pCmd m_cmd;
	// BYE
	bool m_closeSession;
};

}
#endif //_CMDP2P_H_

