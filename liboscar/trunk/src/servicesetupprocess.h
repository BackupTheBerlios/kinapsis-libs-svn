/***************************************************************************
 *   Copyright (C) 2007 by Luis Cidoncha                                   *
 *   luis.cidoncha@gmail.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef _SERVICESETUPPROCESS_H_
#define _SERVICESETUPPROCESS_H_

#define NUM_OF_SERVICES 5

#include "liboscar.h"
#include "client.h"
#include "snac_contact.h"
#include "snac_location.h"
#include "snac_icbm.h"
#include "snac_roster.h"
#include "snac_bos.h"
#include <qobject.h>

namespace liboscar {

class ServiceSetupProcess : public QObject {
Q_OBJECT

public:
	ServiceSetupProcess(Client* c);
	virtual ~ServiceSetupProcess();

public slots:
	void requestServiceSetup();
	void locationLimits(SrvReplyLocationSNAC srl);
	void BLMLimits(SrvReplyBuddySNAC srb);
	void ICBMLimits(SrvReplyICBMSNAC sri);
	void PRMLimits(SrvReplyBOSSNAC srb);
	void SSILimits(SrvReplyListsSNAC srr);

signals:
	void serviceSetupDone();

private:
	void requestLocation();
	void requestBLM();
	void requestICBM();
	void requestPRM();
	void requestSSI();

	void done();

	int m_count;
	Client* m_parent;
};

}

#endif // _SERVICESETUPPROCESS_H_
