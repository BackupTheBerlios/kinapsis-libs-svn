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
	virtual ~P2P();
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
	
	// BYE
	bool m_closeSession;
};

}
#endif //_CMDP2P_H_

