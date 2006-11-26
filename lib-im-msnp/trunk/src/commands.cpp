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

#include "commands.h"
#include <sstream>

namespace libimmsnp {

	Commands::Commands (){
	}

	std::string Commands::split (std::string msg){
		return msg;
	}
	
	std::string Commands::VER (int idtr){
		std::ostringstream  tmp;
		tmp << "VER " << idtr << " MSNP12 MSNP11 MSNP10 CVR0\r\n";
		return tmp.str();
	}
	
	std::string Commands::CVR (int idtr, std::string msnPassport){
		std::ostringstream  tmp;
		tmp << "CVR " << idtr << " 0x0409 winnt 5.1 i386 MSNMSGR 7.5.0324 msmsgs " << msnPassport << "\r\n";
		return tmp.str();
	}

	std::string Commands::USRIni (int idtr, std::string msnPassport){
		std::ostringstream  tmp;
		tmp << "USR " << idtr << " TWN I " << msnPassport << "\r\n";
		return tmp.str();
	}
		
	std::string Commands::USRFin (int idtr, std::string ticket){
		std::ostringstream  tmp;
		tmp << "USR " << idtr << " TWN S " << ticket << "\r\n";
		return tmp.str();
	}
	
	std::string Commands::SYN (int idtr, std::string synchversion){
		std::ostringstream  tmp;
		tmp << "SYN " << idtr << " " << synchversion << " 0 \r\n";
		return tmp.str();
	}
	
	std::string Commands::GCF (int idtr){
		std::ostringstream  tmp;
		tmp << "GCF "<< idtr <<" Shields.xml";
		return tmp.str();
	}
	
	std::string Commands::CHG (int idtr, std::string msnContactCapabilities, std::string initialStatus){
		std::ostringstream  tmp;
		tmp << "CHG " << idtr << " " << initialStatus << " " << msnContactCapabilities << "\r\n";
		
		return tmp.str();
	}
	
	std::string Commands::QRY (int idtr, std::string challenge){
		std::ostringstream  tmp;
		tmp << "QRY " << idtr << " PROD0090YUAUV{2B 32\r\n" << challenge;
		return tmp.str();
	}

	// Change of presence name
	// REA trid your_email newname\r\n
	std::string Commands::REA (int idtr, std::string msnPassport, std::string msnFriendlyName) {
		std::ostringstream  tmp;
		tmp << "REA " << idtr << " " << msnPassport << " " << msnFriendlyName << "\r\n";
		return tmp.str();
	}
	
	// Respuesta a un RNG para iniciar un chat
	// >>> ANS trid your_passport ticket sessid\r\n
	// <<< ANS trid "OK"\r\n
	std::string Commands::ANS (int idtr, std::string msnPassport, std::string ticket, std::string sesionId ) {
		std::ostringstream  tmp;
		tmp << "ANS " << idtr << " " << msnPassport << " " << ticket << " " << sesionId << "\r\n";
		return tmp.str();
	}
	
	std::string Commands::MSG (int idtrChat, std::string chatMsg ) {
		std::ostringstream  payload, fullMsg;
		int len;
		payload << "MIME-Version: 1.0\r\nContent-Type: text/plain; charset=UTF-8\r\nX-MMS-IM-Format: FN=Helvetica; EF=; CO=000000; CS=0; PF=22\r\n\r\n"<< chatMsg;
		len = (payload.str()).length();
		fullMsg << "MSG " << idtrChat << " A "<< len <<"\r\n" << payload.str(); 
		return fullMsg.str();
	}

	std::string Commands::PNG() {
		return "PNG\r\n";
	}
	
	std::string Commands::OUT () {
		return "OUT\r\n";
	}
	
	
}
