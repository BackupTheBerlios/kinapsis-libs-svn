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

#ifndef _TRANSFER_H_
#define _TRANSFER_H_

#include "libimmsnp.h"
#include <QObject>
namespace libimmsnp {

class Transfer : public  QObject{

Q_OBJECT

public:
	Transfer ();
	virtual ~Transfer();

	void setBHSessionID 	 (int sessionID		);
	void setBHIdentifier 	 (int identifier 	);
	void setBHDataOffset 	 (int dataOffset 	);
	void setBHTotalDataSize	 (int totalDataSize 	);
	void setBHMessageLength	 (int messageLength 	);
	void setBHFlag 		 (int flag 		);
	void setBHAckIdentifier	 (int ackIdentifier	);
	void setBHAckUniqueID 	 (int ackUniqueID 	);
	void setBHAckDataSize 	 (int ackDataSize 	);

	void setContext	(QByteArray data) {m_context = data;}
	void addContext	(QByteArray data) {m_context += data;}

	QByteArray getBHIdentifier  () {return m_bh_identifier;}
	QByteArray getBHTotalDataSize  () {return m_bh_totalDataSize;}
	QByteArray getBHAckIdentifier  () {return m_bh_ackIdentifier;}
	QByteArray getBHSessionID  () {return m_bh_sessionID;}

	QByteArray getContext () {return m_context;}

	void setTo	  	(QByteArray to		) {m_to		 = to		;}
	void setFrom	  	(QByteArray from	) {m_from	 = from		;}
	void setBranch	  	(QByteArray branch	) {m_branch	 = branch	;}
	void setCallId	  	(QByteArray callId	) {m_callId	 = callId	;}
	void setp2pSessionId	(QByteArray p2pSessionId) {m_p2pSessionId= p2pSessionId	;}

	QByteArray getTo		(){return m_to		;}	
	QByteArray getFrom		(){return m_from	;}	
	QByteArray getBranch		(){return m_branch	;}	
	QByteArray getCallId		(){return m_callId	;}	
	QByteArray getp2pSessionId	(){return m_p2pSessionId;}

	void createMyIdentifier	();
	void createAckIdentifier();
	QByteArray incMyIdentifier(int inc);
	QByteArray incAckIdentifier(int inc);

	void setFileName (QByteArray fn) {m_filename = fn;}
	void setType (QByteArray type) {m_type = type;}
	void setStep (P2pStep step) {m_step = step;}
	void setDestination(QByteArray dest){m_destination = dest;}
	void addData(QByteArray data){m_data += data;}
	void setP2pType(P2pType type){m_p2pType = type;}
	
	QByteArray getFileName() {return m_filename;}
	QByteArray getType(){return m_type;}
	P2pStep getStep(){return m_step;}
	QByteArray getDestination(){return m_destination;}
	QByteArray getData(){return m_data;}
	P2pType getP2pType(){return m_p2pType;}

	void setHasTransfered() {m_hasTransfered = true;}
	bool hasTransfered() {return m_hasTransfered;}

private:
	QByteArray m_filename;
	QByteArray m_type;
	P2pStep m_step;
	QByteArray m_destination;
	QByteArray m_data;
	QByteArray m_context;

	bool m_hasTransfered;	

	int m_myIdentifier;
	int m_myAckIdentifier; 

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
	QByteArray m_to;
	QByteArray m_from;
	QByteArray m_branch; 
	QByteArray m_callId;

	// INVITATION
	QByteArray m_EUF_GUID;
	QByteArray m_p2pSessionId;
	P2pType m_p2pType;
};

}
#endif //_TRANSFER_H_

