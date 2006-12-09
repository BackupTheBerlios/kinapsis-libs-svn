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

#include "notificationServer.h"

namespace libimmsnp {

CHG::CHG(int idtr) : Command ("CHG", idtr) {}
CHG::~CHG(){}
QString CHG::makeCmd(){
	// CHG 9 NLN 1073791084\r\n
	// CHG trid statuscode clientid msnobj
	QString res;
	res += beginCmd();
	res += " " + m_statusCode + " " + m_capabilities;
	res += endCmd();
	return res;
}
void CHG::addStatusCode (State statusCode) {
	switch(statusCode){
		case STATUS_NLN:
			m_statusCode = "NLN";
			break;
		case STATUS_BSY:
			m_statusCode = "BSY";
			break;
		case STATUS_IDL:
			m_statusCode = "IDL";
			break;
		case STATUS_BRB:
			m_statusCode = "BRB";
			break;
		case STATUS_AWY:
			m_statusCode = "AWY";
			break;
		case STATUS_PHN:
			m_statusCode = "PHN";
			break;
		case STATUS_LUN:
			m_statusCode = "LUN";
			break;
		case STATUS_HDN:
			m_statusCode = "HDN";
			break;
		default:
			m_statusCode = "HDN";
			break;
	}
}
void CHG::addCapabilities (QString capabilities) {m_capabilities = capabilities;}
void CHG::addMsnObj (QString msnObj) {m_msnObj = msnObj;}


PRP::PRP(int idtr) : Command ("PRP", idtr) {}
PRP::~PRP(){}
QString PRP::makeCmd(){
	// PRP idtr nick\r\n
	QString res;
	res += beginCmd();
	res += " MFN " + m_nick;
	res += endCmd();
	return res;
}
void PRP::addNick (QString nick){m_nick = nick;}

XFR::XFR(int idtr) : Command ("XFR", idtr) {}
XFR::~XFR(){}
QString XFR::makeCmd(){
	// XFR 15 SB\r\n
	QString res;
	res += beginCmd();
	res += " SB";
	res += endCmd();
	return res;
}

OUT::OUT() : Command ("OUT") {}
OUT::~OUT(){}
QString OUT::makeCmd(){
	// OUT\r\n
	QString res;
	res += beginCmd();
	res += endCmd();
	return res;
}

ADC::ADC(int idtr) : Command ("ADC", idtr) {}
ADC::~ADC(){}
QString ADC::makeCmd(){
	// 
	QString res;
	res += beginCmd();
	res += " " + m_list + " C=" + m_id + " " + m_groupId;
	res += endCmd();
	return res;
}
void ADC::addId (QString id){m_id = id;}
void ADC::addList (QString list){m_list = list;}
void ADC::addGroupId (QString groupId){m_groupId = groupId;}

REM::REM(int idtr) : Command ("ADC", idtr) {}
REM::~REM(){}
QString REM::makeCmd(){
	// 
	QString res;
	res += beginCmd();
	res += " " + m_list + " " + m_id + " " + m_groupId;
	res += endCmd();
	return res;
}
void REM::addId (QString id){m_id = id;}
void REM::addList (QString list){m_list = list;}
void REM::addGroupId (QString groupId){m_groupId = groupId;}
}
