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

#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include "libimmsnp.h"
#include <string>

namespace libimmsnp {
class Commands {
public:
	Commands ();
	std::string split (std::string msg);
	std::string VER (int idtr);
	std::string CVR (int idtr, std::string msnPassport);
	std::string USRIni (int idtr, std::string msnPassport);
	std::string USRFin (int idtr, std::string ticket);
	std::string SYN (int idtr, std::string synchversion);
	std::string GCF (int idtr);
	std::string CHG (int idtr, std::string clientID, std::string initialStatus);
	std::string QRY (int idtr, std::string challenge);
	std::string REA (int idtr,std::string msnPassport, std::string newname);
	std::string ANS (int idtr, std::string msnPassport, std::string ticket, std::string sesionId );
	std::string MSG (int idtrChat, std::string chatMsg );
	std::string PNG ();
	std::string OUT ();
};
}
#endif // _COMMANDS_H_
