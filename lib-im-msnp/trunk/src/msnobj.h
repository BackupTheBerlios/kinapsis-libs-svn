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

#ifndef _MSNOBJ_H_
#define _MSNOBJ_H_

namespace libimmsnp {
class MSNOBJ{

public:
	MSNOBJ ();
	//MSNOBJ (QByteArray obj);
	virtual ~MSNOBJ();
	QByteArray getMsnObj();

	void setCreator(QByteArray creator)	{m_creator = creator;}
	void setType(int type)			{m_type = type;}
	void setSize(int size)			{m_size = size;}
	void setLocation(QByteArray location) 	{m_location = location;}
	void setFriendly(QByteArray friendly) 	{m_friendly = friendly;}
	
	QByteArray getCreator()	{return m_creator;}
	int getType()		{return m_type;}
	int getSize()		{return m_size;}
	QByteArray getLocation() 	{return m_location;}
	QByteArray getFriendly() 	{return m_friendly;}
	
private:
	//<msnobj Creator="buddy@hotmail.com" Size="24539" Type="3" Location="TFR2C.tmp" Friendly="AAA=" SHA1D="trC8SlFx2sWQxZMIBAWSEnXc8oQ=" SHA1C="U32o6bosZzluJq82eAtMpx5dIEI="/>


	QByteArray m_creator;
	int m_size;
	int m_type;
	// * 1: Avatar (Unknown, but already present since Messenger 6.0)
	// * 2: Custom Emoticons
	// * 3: User Tile (Static display pictures only)
	// * 4: Shared File (Unknown, but already present since Messenger 6.0)
	// * 5: Backgrounds (Static backgrounds only)
	// * 6: History (Unknown)
	// * 7: Deluxe Display Pictures (Dynamic display pictures)
	// * 8: Wink
	// * 9: Map File (A map file contains a list of items in the store)
	// * 10: Dynamic Backgrounds (Animated backgrounds)
	// * 11: Voice Clip
	// * 12: Plug-In State (Saved state of Add-ins)
	// * 13: Roaming Objects (For example your roaming display picture)
	// * 14: Signature Sound (Signature sounds introcued in Messenger 9.0) 
	QByteArray m_location; 
	QByteArray m_friendly; 
	QByteArray m_SHA1D; 
	QByteArray m_SHA1C; 
};

}
#endif //_MSNOBJ_H_

