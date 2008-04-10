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

#ifndef LIBIMMSNP_H__
#define LIBIMMSNP_H__

namespace libimmsnp {
	
	enum ConnectionError {
		ConnNoError,
		ConnSocketError,
		ConnUserDisconnected,
		ConnAuthenticationFailed,
		ConnAtOtherPlace,		// Connected at other place with the same account
		ConnNotExpected,		// error 751
		ConnBadMd5Digest,		// error 540
		ConnCloseUnexpect
	};

	enum State {
		STATUS_NLN,
		STATUS_BSY,
		STATUS_IDL,
		STATUS_BRB,
		STATUS_AWY,
		STATUS_PHN,
		STATUS_LUN,
		STATUS_HDN,
		STATUS_OFF
	};
	
	enum ParserOperation {
		PARSER_OK,
		PARSER_HELLO,
		PARSER_BYE,
		PARSER_ERROR
	};

	enum Effect {
		EFFECT_ITALIC,
		EFFECT_BOLD,
		EFFECT_UNDERLINE
	};

	enum MsgType {
		MSG_TXT,
		MSG_IDENTIFICATION,
		MSG_ISTYPING,
		MSG_P2P
	};
	enum P2pStep {
		P2P_NULL,
		P2P_INVITATION,
		P2P_NEGOTIATION,
		P2P_TRANSFER,
		P2P_ACK,
		P2P_BYE
	};
	
	enum P2pCmd {
		P2PC_INVITATION,
		P2PC_NEGOTIATION,
		P2PC_TRANSFER,
		P2PC_200OK,
		P2PC_ACK,
		P2PC_INITID,
	};

}
#endif // LIBIMMSNP_H__
