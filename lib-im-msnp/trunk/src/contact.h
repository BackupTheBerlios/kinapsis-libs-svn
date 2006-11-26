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

class Contact : public QObject {
Q_OBJECT

public:
	Contact();
	Contact(QString msnPassport, QString groupname, QString friendlyname, QString msnid, QString group);
	
	virtual ~Contact();
	
	void setMsnName(QString msnPassport);
	void setGroupName(QString groupName);
	void setFriendlyName(QString friendlyName);
	void setMsnId(QString msnId);
	void setGroupId (QString groupId);
	void setStatus (PresenceStatus status);
	void setPersMsg (QString persMsg);
	void setCapabilities (QString msnCapabilities);

	QString getMsnName(){return m_msnname;};
	QString getGroupName(){return m_groupname;};
	QString getFriendlyName(){return m_friendlyname;};
	QString getMsnId(){return m_msnid;};
	QString getGroupId(){return m_groupid;};

private:
	QString m_msnname;
	QString m_groupname;
	QString m_friendlyname;
	QString m_msnid;
	QString m_groupid;
	PresenceStatus m_status;
	QString m_persmsg;
	QString m_capabilities;
};

class Group {
public:
       Group ();
       Group (QString groupName, QString groupId);
       QString getName () {return m_name;};
       QString getId () {return m_id;};
       virtual ~Group();
private:
       QString m_name;
       QString m_id;
};

}

#endif // _CONTACT_H_
