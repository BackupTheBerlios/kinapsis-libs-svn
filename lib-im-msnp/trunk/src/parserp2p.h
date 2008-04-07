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

#ifndef _PARSERP2P_H_
#define _PARSERP2P_H_
#include "libimmsnp.h"

namespace libimmsnp {
class ParserP2P {

public:
	ParserP2P ();
	virtual ~ParserP2P();
	QByteArray getFileName() {return m_context;}

	bool isFinished(); 

	QByteArray getData(){return m_data;}
	P2pStep step() {return m_step;}
	void parse (QByteArray);

	int getBHSessionID 	();
	int getBHid		();
	int getBHDataOffset 	();
	int getBHTotalDataSize	();
	int getBHFlag 		();
	int getBHMessageLength	();
	int getBHAckUniqueID 	();
	int getBHAckIdentifier	();
	int getBHAckDataSize 	();

	QByteArray getTo		(){return m_to		;}	
	QByteArray getFrom		(){return m_from	;}	
	QByteArray getBranch		(){return m_branch	;}	
	QByteArray getCallId		(){return m_callId	;}	
	QByteArray getp2pSessionId	(){return m_p2pSessionId;}

	QByteArray getEUF_GUID		(){return m_EUF_GUID	;}	

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
	P2pStep m_step;

	// DATA
	QByteArray m_data;
	
	// BYE
	bool m_closeSession;
};

}
#endif //_PARSERP2P_H_

