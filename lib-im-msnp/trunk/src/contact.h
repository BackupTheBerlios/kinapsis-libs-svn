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

#ifndef _CONTACT_H_
#define _CONTACT_H_

#include "libimmsnp.h"
#include <qobject.h>
#include <qstring.h>

namespace libimmsnp {

class Contact {

public:
	Contact();
	Contact(QString passport, QString groupName, QString nickName, QString msnId, QString groupId);
	
	virtual ~Contact();
	
	void setPassport(QString);
	void setGroupName(QString);
	void setNickName(QString);
	void setId(QString);
	void setGroupId (QString);
	void setStatus (State);
	void setDisplayName (QString);
	void setPersMsg (QString);
	void setCapabilities (QString);
	void setList (QString list);

	QString getPassport(){return m_passport;};
	QString getGroupName(){return m_groupName;};
	QString getNickName(){return m_nickName;};
	QString getId(){return m_id;};
	QString getGroupId(){return m_groupId;};
	State getStatus() {return m_status;};
	QString getCapabilities() {return m_capabilities;};
	QString getList(){return m_list;};
	QString getDisplayName(){return m_displayName;};
	

private:
	QString	m_passport; 	
	QString	m_groupName;  	
	QString	m_nickName; 			
	QString	m_id; 		
	QString	m_groupId;		
	State	m_status; 
	QString	m_displayName;	
	QString	m_persMsg;		
	QString	m_capabilities ;	
	QString m_list;
};

class Group {
public:
       Group ();
       Group (QString groupName, QString groupId);
       void setName (QString name) {m_name = name;}
       void setId (QString id) {m_id = id;}
       QString getName () {return m_name;};
       QString getId () {return m_id;};
       virtual ~Group();
private:
       QString m_name;
       QString m_id;
};

}

#endif // _CONTACT_H_
