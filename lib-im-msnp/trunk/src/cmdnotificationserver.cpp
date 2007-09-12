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

#include "cmdnotificationserver.h"

namespace libimmsnp {

CHG::CHG(int idtr) : Command ("CHG", idtr) {}
CHG::~CHG(){}
Buffer CHG::makeCmd(){
	// CHG 9 NLN 1073791084\r\n
	// CHG trid statuscode clientid msnobj
	Buffer res;
	res << beginCmd() + " " + m_statusCode + " " + m_capabilities + endCmd();
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


//PRP::PRP(int idtr) : Command ("PRP", idtr) {}
//PRP::~PRP(){}
//Buffer PRP::makeCmd(){
//	// PRP idtr nick\r\n
//	Buffer res;
//	res += beginCmd();
//	res += " MFN " + m_nick;
//	res += endCmd();
//	return res;
//}
//void PRP::addNick (QString nick){m_nick = nick;}
//
XFR::XFR(int idtr) : Command ("XFR", idtr) {}
XFR::~XFR(){}
Buffer XFR::makeCmd(){
	// XFR 15 SB\r\n
	Buffer res;
	res << beginCmd() + " SB" + endCmd();
	return res;
}

OUT::OUT() : Command ("OUT") {}
OUT::~OUT(){}
Buffer OUT::makeCmd(){
	// OUT\r\n
	Buffer res;
	res += beginCmd();
	res += endCmd();
	return res;
}

ADC::ADC(int idtr) : Command ("ADC", idtr) {}
ADC::~ADC(){}
Buffer ADC::makeCmd(){
	// >> ADC 45 FL C=f39c2bb5-8a18-4f40-ae18-75aa595b747e cc57fad0-f245-42c2-a5f7-e13df408cb95\r\n
	// << ADC 45 FL C=f39c2bb5-8a18-4f40-ae18-75aa595b747e cc57fad0-f245-42c2-a5f7-e13df408cb95\r\n
	Buffer res;
	res += beginCmd();
	if (m_list == "FL"){
		if (m_id ==  "") res += " " + m_list + " C=" + m_id + " " + m_groupId;
		if (m_passport !=  "") res += " " + m_list + " N=" + m_passport + " F=" + m_dispName;
	}
	if (m_list == "BL"){
		res += " " + m_list + " N=" + m_passport;
	}

	res += endCmd();
	return res;
}
void ADC::addList (QString list){m_list = list;}
// if new contact
void ADC::addPassport (QString passport){m_passport = passport;}
void ADC::addDisplayName (QString dispName){m_dispName = dispName;}
// if I want to add a contact to a group
void ADC::addId (QString id){m_id = id;}
void ADC::addGroupId (QString groupId){m_groupId = groupId;}

REM::REM(int idtr) : Command ("REM", idtr) {}
REM::~REM(){}
Buffer REM::makeCmd(){
	Buffer res;
	res += beginCmd();
	res += " " + m_list + " " + m_id;
	res += endCmd();
	return res;
}
void REM::addList (QString list){m_list = list;}
void REM::addId (QString id){m_id = id;}

ADG::ADG(int idtr) : Command ("ADG", idtr) {}
ADG::~ADG(){}
Buffer ADG::makeCmd(){
	Buffer res;
	res += beginCmd();
	res += " " + m_name + " 0"; 
	res += endCmd();
	return res;
}
void ADG::addName (QString name){m_name = name;}

RMG::RMG(int idtr) : Command ("RMG", idtr) {}
RMG::~RMG(){}
Buffer RMG::makeCmd(){
	Buffer res;
	res += beginCmd();
	res += " " + m_name; 
	res += endCmd();
	return res;
}
void RMG::addId (QString name){m_name = name;}

}
