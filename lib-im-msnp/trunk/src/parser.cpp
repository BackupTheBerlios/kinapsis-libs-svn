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

#include <qobject.h>
#include <string>
#include <iostream>

#include "libimmsnp.h"
#include "connection.h"
#include "parser.h"
#include "commands.h"
#include "chat.h"

namespace libimmsnp {
	Parser::Parser(std::string msnPassport, std::string msnPass){
		m_cmd = "";
		m_processingCmd = 0;
		m_chatId = 0;
		m_msnPassport = msnPassport;
		m_msnPass = msnPass;
			
	}
	
	Parser::Parser(std::string msnPassport, std::string msnPass, int chatId){
		m_cmd = "";
		m_processingCmd = 0;
		m_chatId = chatId;
		m_msnPassport = msnPassport;
		m_msnPass = msnPass;
	}
	
	void urldecode (std::string& cmd){
	
		while (1) {
			const unsigned  int pos = cmd.find("%");
			if (pos == std::string::npos) break;
			cmd.replace(pos,1,"_");
			cmd.replace(pos+1,1,"");
			cmd.replace(pos+1,1,"");
		}
	}

	ParserOperation Parser::makeRoaster (msocket* s, std::string bufSyn){

		//Roster msnRoster;
		
		// Get the number of contacts and groups
		QStringList cmdSyn_rn (QStringList::split ("\r\n", QString::fromUtf8 (bufSyn.c_str()))); // split by \r\n
		QStringList::Iterator cmdIterSyn_rn = cmdSyn_rn.begin(); // Make an iterator and start to the first of the list.
		
		QStringList cmdSyn_sp (QStringList::split (" ", *cmdIterSyn_rn));   
		QStringList::Iterator cmdIter = cmdSyn_sp.begin() += 4; // point to numGroups
		
		int numberBuddies = (*(cmdIter)).toInt();
		int numberGroups = (*(++cmdIter)).toInt();

		// Now y have the number of groups and contacts
		QString tmp;
		std::string tmpBuf, tmpBufTot = bufSyn;
		
		// start getting it
		while ((numberBuddies != 0) or (numberGroups !=0)){
			s->recv (tmpBuf);
			tmpBufTot += tmpBuf;
			while (tmpBuf.substr(tmpBuf.size()-1,1) != "\n"){
				s->recv (tmpBuf);
				tmpBufTot += tmpBuf;
			}
			
			QStringList cmd_rn, cmd_sp;
			urldecode (tmpBufTot);	
			
			// split by \r\n and run across it
			cmd_rn = QStringList::split ("\r\n",QString::fromUtf8 (tmpBufTot.c_str()));
			QStringList::Iterator cmdIter_rn = cmd_rn.begin();
			while (cmdIter_rn != cmd_rn.end()){
				
				// split by space each *cmdIter_rn and run across it
				cmd_sp = QStringList::split (" ", *cmdIter_rn);
				QStringList::Iterator cmdIter_sp = cmd_sp.begin();
				
				// If it is a group
				if (*cmdIter_sp == "LSG"){  
					QString groupName (*(++cmdIter_sp));
					QString groupId (*(++cmdIter_sp));
					Group* g = new Group(groupName, groupId);
					emit newGroupArrived (g);
					numberGroups-= 1;
				}
				
				// if it is a contact
				if (*cmdIter_sp == "LST"){
					
					QString msnPassport (*(++cmdIter_sp));
					QString msnFriendlyName (*(++cmdIter_sp));
					QString msnContactId (*(++cmdIter_sp));
					cmdIter_sp += 2;
					QString msnGroupId (*(++cmdIter_sp));
					msnGroupId = msnGroupId.section( ",", 0, 0 ); // I take only the first Group
					if (cmd_sp.size() > 4){
						Contact* c = new Contact (msnPassport.section('=',1),QString(""), msnFriendlyName.section('=',1), msnContactId.section('=',1), msnGroupId.stripWhiteSpace());
						emit newContactArrived (c);
						numberBuddies -= 1;
					}
					else{
						Contact* c = new Contact (msnPassport.section('=',1), QString(""), QString(""), QString(""), QString(""));
						//msnRoster.addContact (c)
						emit newContactArrived (c);
						numberBuddies -= 1;
					}
					
				}
				cmdIter_rn ++;
				//qDebug ("#@@# Contacts  "+ tmp.setNum(numberBuddies) + " Groups: " +  tmp.setNum(numberGroups) );
				
			}
			tmpBufTot = "";
		}
		return PARSER_OK;
		
	}

	int nextCmd (QString m_cmd) {
		
		for (int i = 0 ; i <= m_cmd.length() - 3 ; i++) {
			if ((m_cmd.mid(i,3) == "ILN") or (m_cmd.mid(i,3) == "NLN") or (m_cmd.mid(i,3) == "UBX") or (m_cmd.mid(i,3) == "RNG")  or (m_cmd.mid(i,3) == "MSG") or (m_cmd.mid(i,3) == "BYE") or (m_cmd.mid(i,3) == "FLN")  or (m_cmd.mid(i,3) == "XFR")  ) {
				return i;
			}
		}
		return -1;
	}
		
	ParserOperation Parser::feed (std::string cmd){
		// variables
		int finThisCmd;
		QString thisCmd;
		QStringList cmdList;
		QStringList::Iterator cmdListIter;
		// for "ILN" "NLN"
		QString msnPresenceState;
		// for "ILN" "NLN" "UBX"
		QString msnPassport;
		
		// replace  spaces and %..
		urldecode (cmd);
		// add what i've receive
		m_cmd += QString::fromUtf8 (cmd.c_str());
		
		// next command
		int iniThisCmd; 
		iniThisCmd = nextCmd (m_cmd); // command position
		m_cmd = m_cmd.mid (iniThisCmd, m_cmd.length() - iniThisCmd);
		
//		while ((iniThisCmd = nextCmd (m_cmd)) != -1) {
			if ((m_cmd.mid(0, 3) == "ILN") or (m_cmd.mid(0, 3) == "NLN")) {
				m_processingCmd = 1;
				
				// NLN BRB msnPassport INITIAL_MSG ID_USER TRASH\r\n
				// ILN 9 AWY msnPassport INITIAL_MSG ID_USER TRASH\r\n
	
				// the end of ILN or NLN is a \n
				finThisCmd = m_cmd.find("\n") + 1;
				thisCmd = m_cmd.mid (0, finThisCmd);
				
				cmdList = QStringList::split (" ",thisCmd);
				cmdListIter = cmdList.begin ();
					
				if (thisCmd.mid(0, 3) == "ILN") cmdListIter +=1 ;
				msnPresenceState = *(++cmdListIter);
				msnPassport = *(++cmdListIter);
				QString msnPersMsg = *(++cmdListIter);
				QString msnCapabilities = *(++cmdListIter);
	
				if (msnPassport != ""){
					if (msnPresenceState == "NLN"){
						m_processingCmd = 0;
						emit statusChanged (msnPassport, PresenceNLN, msnPersMsg, msnCapabilities);
					}
					
					if (msnPresenceState == "BSY"){
						m_processingCmd = 0;
						emit statusChanged (msnPassport, PresenceBSY, msnPersMsg, msnCapabilities);
					}
					
					if (msnPresenceState == "IDL"){
						m_processingCmd = 0;
						emit statusChanged (msnPassport, PresenceIDL, msnPersMsg, msnCapabilities);
					}
					
					if (msnPresenceState == "BRB"){
						m_processingCmd = 0;
						emit statusChanged (msnPassport, PresenceBRB, msnPersMsg, msnCapabilities);
					}
					
					if (msnPresenceState == "AWY"){
						m_processingCmd = 0;
						emit statusChanged (msnPassport, PresenceAWY, msnPersMsg, msnCapabilities);
					}
					
					if (msnPresenceState == "PHN"){
						m_processingCmd = 0;
						emit statusChanged (msnPassport, PresencePHN, msnPersMsg, msnCapabilities);
					}
					
					if (msnPresenceState == "LUN"){
						emit statusChanged (msnPassport, PresenceLUN, msnPersMsg, msnCapabilities);
						m_processingCmd = 0;
					}
					
					if (msnPresenceState == "HDN"){
						emit statusChanged (msnPassport, PresenceHDN, msnPersMsg, msnCapabilities);
						m_processingCmd = 0;
					}
				}
				
				// i clean the buffer
				if (m_processingCmd == 0) {
					m_cmd = m_cmd.mid (finThisCmd, m_cmd.length() - finThisCmd);
				}
			}
			if (m_cmd.mid(0, 3) == "UBX") {
				// UBX msnPassport 117\r\n<Data><PSM>PERSONAL_MSG</PSM><CurrentMedia></CurrentMedia></Data>
				
				// The end of UBX is a payload
				m_processingCmd = 1;
				
				finThisCmd = m_cmd.find("\r") + 2;
				thisCmd = m_cmd.mid (0, finThisCmd);
				
				cmdList = QStringList::split (" ",thisCmd);
				cmdListIter = cmdList.begin ();
				
				cmdList =  QStringList::split (" ",thisCmd);
				cmdListIter = cmdList.begin ();
				msnPassport = *(++cmdListIter);
				int payload = (*(++cmdListIter)).toInt();
				if (payload == 0) {
					finThisCmd += 2;
					m_cmd = m_cmd.mid (finThisCmd, m_cmd.length() - finThisCmd);
					return PARSER_OK;
				}

				// Now i get the payload
				QString msnPersMsg = m_cmd.mid (finThisCmd, m_cmd.find("</Data>") + 7 - finThisCmd);
				if (m_cmd.find("</Data>")) {
					m_processingCmd = 0;
					std::cout << "###~~~ PAYLOAD "<< payload << "  " << msnPersMsg.latin1() << "\n"; 
					msnPersMsg = msnPersMsg.section ("<PSM>", 1, 1); // personal message starts
					msnPersMsg = msnPersMsg.section ("</PSM>", 0, 0); // personal message ends ## CHANGE
				        emit personalMessage (msnPassport, msnPersMsg);
				}
				
				// If I've processed the command complettely -> I clean the buffer
				if (m_processingCmd == 0) {
					finThisCmd += m_cmd.find("</Data>") + 7; // ## CHANGE
					m_cmd = m_cmd.mid (finThisCmd, m_cmd.length() - finThisCmd);
				}
			}
			if (m_cmd.mid(0, 3) == "RNG") {
				// RNG 979532535 64.4.37.19:1863 CKI 971327.191161103 msnPassport          msnFriendlyName\r\n
				// RNG sessid    address         authtype ticket      invitepassport       invitename\r\n
				m_processingCmd = 1;
				QString sessid, hostPort, authtype, ticket, msnFriendlyName;
				
				// The end of RNG is a \n
				finThisCmd = m_cmd.find("\r");
				thisCmd = m_cmd.mid (0, finThisCmd);
				
				cmdList = QStringList::split (" ",thisCmd);
				cmdListIter = cmdList.begin ();
					
				sessid = *(++cmdListIter);
				hostPort = *(++cmdListIter);
				authtype = *(++cmdListIter);
				ticket = *(++cmdListIter);
				msnPassport = *(++cmdListIter);
				msnFriendlyName = *(++cmdListIter);
				if (msnFriendlyName != ""){
					emit chatRequest (hostPort, msnPassport, ticket, sessid);
					m_processingCmd = 0;
				}
				// i clean the buffer
				if (m_processingCmd == 0) {
					m_cmd = m_cmd.mid (finThisCmd, m_cmd.length() - finThisCmd);
				}
			}
			if (m_cmd.mid(0, 3) == "MSG") {
				qDebug ("## && FULL CMD: " + m_cmd);
				QString msnFriendlyName;
				// MSG destMsnMail destMsnFriendlyName 126\r\nMIME-Version: 1.0\r\nContent-Type: text/plain; charset=UTF-8\r\nX-MMS-IM-Format: FN=Helvetica; EF=; CO=000000; CS=0; PF=22\r\n\r\nhola
				// The end of UBX is a payload
				m_processingCmd = 1;
				
				// MSG destMsnMail destMsnFriendlyName 126
				finThisCmd = m_cmd.find("\r") ;
				// thisCmd = MSG destMsnMail destMsnFriendlyName 126
				thisCmd = m_cmd.mid (0, finThisCmd);
				
				// I make a list of MSG msnPassport destMsnFriendlyName 126\r
				cmdList = QStringList::split (" ",thisCmd);
				cmdListIter = cmdList.begin ();
				
				cmdList =  QStringList::split (" ",thisCmd);
				cmdListIter = cmdList.begin ();
				msnPassport = *(++cmdListIter);
				msnFriendlyName = *(++cmdListIter);
				int payload = (*(++cmdListIter)).toInt();

				// Now i get the payload
				//   MSG destMsnMail destMsnFriendlyName 126\r\n
				QString chatMsg = m_cmd.mid (finThisCmd + 2, payload);
				

				if (chatMsg.length() == payload) {
					// `MIME-Version: 1.0\r\nContent-Type: text/plain; charset=UTF-8\r\nX-MMS-IM-Format: FN=Helvetica; EF=; CO=000000; CS=0; PF=22\r\n\r\nhola'
					
					m_processingCmd = 0;
					cmdList =  QStringList::split ("\r\n",chatMsg);
					cmdListIter = cmdList.begin ();
					if (*(++cmdList.begin ()) == "Content-Type: text/plain; charset=UTF-8") {
						int iniChatMsg = chatMsg.find("\r\n\r\n");
						chatMsg = chatMsg.mid (iniChatMsg + 4,(chatMsg.length() - iniChatMsg));
						emit chatArrivedMessage (m_chatId, msnPassport, chatMsg);
					}
					if (*(++cmdList.begin ()) == "Content-Type: text/x-clientcaps"){
						cmdListIter += 2;
						QString chatMsnClient = (*cmdListIter).mid (13, (*cmdListIter).length());
						QString chatIsLogging = (*(++cmdListIter)).mid (14, (*cmdListIter).length());
						qDebug ("###" + chatMsnClient + " " + chatIsLogging);
						emit chatInfo (m_chatId, chatMsnClient, chatIsLogging );
					}
					if (*(++cmdList.begin ()) == "Content-Type: text/x-msmsgscontrol"){
						cmdListIter += 2;
						QString chatMsnPassport = (*cmdListIter).mid (12, (*cmdListIter).length());
						emit chatIsTyping (m_chatId, chatMsnPassport);
					}
				}
				
				// If I've processed the command complettely -> I clean the buffer
				if (m_processingCmd == 0) {
					finThisCmd += (payload + 2);
					m_cmd = m_cmd.mid (finThisCmd, m_cmd.length() - finThisCmd);
				}
				
			}
			if (m_cmd.mid(0, 3) == "BYE") {
				
				//BYE chatMsnPassport\r\n
				
				m_processingCmd = 0;
				finThisCmd = m_cmd.find("\r");
				thisCmd = m_cmd.mid (0, finThisCmd);
				
				cmdList = QStringList::split (" ",thisCmd);
				cmdListIter = cmdList.begin ();
					
				QString chatMsnPassport = *(++cmdList.begin());
				if (chatMsnPassport != "") m_processingCmd = 0;

				emit chatLeavedTheRoom (m_chatId, chatMsnPassport);
					
				// i clean the buffer
				if (m_processingCmd == 0) {
					m_cmd = m_cmd.mid (finThisCmd, m_cmd.length() - finThisCmd);
				}
				return PARSER_BYE;
				
			}
			
			if (m_cmd.mid(0, 3) == "FLN") {
				
				//FLN msnPassport\r\n
				m_processingCmd = 0;
				finThisCmd = m_cmd.find("\r");
				thisCmd = m_cmd.mid (0, finThisCmd);
				
				cmdList = QStringList::split (" ",thisCmd);
				cmdListIter = cmdList.begin ();
					
				msnPassport = *(++cmdList.begin());
				if (msnPassport != "") m_processingCmd = 0;

				emit contactDisconnected (msnPassport);
					
				// i clean the buffer
				if (m_processingCmd == 0) {
					m_cmd = m_cmd.mid (finThisCmd, m_cmd.length() - finThisCmd);
				}
			}

			if (m_cmd.mid(0, 3) == "XFR") {
				std::cout << "\n\nINICIANDO XFR\n";
				// XFR 57   SB 207.46.26.114:1863 CKI      1481854017.25321957.215204203"
				// XFR idtr SB address            authtype ticket\r\n
				m_processingCmd = 1;
				QString hostPort, authtype, ticket;
				
				// The end of XFR is a \n
				finThisCmd = m_cmd.find("\r");
				thisCmd = m_cmd.mid (0, finThisCmd);
				qDebug ("thisCmd: " + thisCmd);
				
				cmdList = QStringList::split (" ",thisCmd);
				cmdListIter = cmdList.begin () +=3;

				hostPort = *(cmdListIter);
				authtype = *(++cmdListIter);
				ticket = *(++cmdListIter);
				qDebug ("params : "+ hostPort +" "+ authtype +" "+ ticket);
				emit chatCreated (hostPort, ticket);
				m_processingCmd = 0;

				// i clean the buffer
				if (m_processingCmd == 0) {
					m_cmd = m_cmd.mid (finThisCmd, m_cmd.length() - finThisCmd);
				}
			}
			// OUT OTH\r\n = Connected from other place
//		}		
	}
}
#include "parser.moc"
