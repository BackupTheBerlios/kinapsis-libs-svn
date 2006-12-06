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
		ConnBadMd5Digest		// error 540

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
	
		
}
#endif // LIBIMMSNP_H__
