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

#ifndef _Transfer_H_
#define _Transfer_H_
#include "cmdp2p.h"
#include "libimmsnp.h"
namespace libimmsnp {
class Transfer {

public:
	Transfer ();
	virtual ~Transfer();

	void setId (int id) {m_id = id;}
	void setType (QByteArray type) {m_type = type;}
	void setStep (int step) {m_step = step;}
	void setP2P (P2P p2p) {m_ftObj = p2p;}
	void setDestination(QByteArray dest){m_destination = dest;}
	void addData(QByteArray data){m_data += data;}
	
	int getId() {return m_id;}
	QByteArray getType(){return m_type;}
	int getStep(){return m_step;}
	QByteArray getDestination(){return m_destination;}
	P2P getP2P (){return m_ftObj;}
	QByteArray getData(){return m_data;}

private:
	int m_id;
	QByteArray m_type;
	int m_step;
	QByteArray m_destination;
	P2P m_ftObj;	
	QByteArray m_data;
};

}
#endif //_Transfer_H_

