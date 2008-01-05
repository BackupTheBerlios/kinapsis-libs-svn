/***************************************************************************
 *   Copyright (C) 2008 by Luis Cidoncha                                   *
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


#include "filetransferprocess.h"

namespace liboscar {

FileTransferProcess::FileTransferProcess(Service* s) { 
	m_parent = s;
}

FileTransferProcess::~FileTransferProcess() { }

//
// USER ACTIONS
//

void FileTransferProcess::acceptFileTransfer(QWord cookie, bool accept){
	// TODO: if !accept then send CANCEL message
	
	// So we accepted the FT. Well, first try to connect to the sender
	// Let's do that in a service.	
	
	if (m_transfers[cookie].getFTData().getProxyUsed())
		m_transfers[cookie].setStatus(PROXY1); // sender requested a proxy
	else
		m_transfers[cookie].setStatus(DIRECT);


	// TODO: move this to another function, and make it check the status...,
	// so we can reuse the code for every connection mode
	
	if (m_transfers[cookie].getFTData().getProxyUsed())
		m_frs = new FileReceiveService(true);
	else
		m_frs = new FileReceiveService();

	QObject::connect(m_frs, SIGNAL(connectionEnded(ConnectionResult)), this, SLOT(ftconnEnded(ConnectionResult)));
	QObject::connect(m_frs, SIGNAL(connectionSuccessful()), this, SLOT(ftconnSuccessful()));
	QObject::connect(m_frs, SIGNAL(ftProgress(int)), this, SLOT(ftconnProgress(int)));

	if (m_transfers[cookie].getFTData().getProxyUsed())
		m_frs->connect(m_transfers[cookie].getFTData().getProxyIP(), m_transfers[cookie].getFTData().getListeningPort());
	else
		m_frs->connect(m_transfers[cookie].getFTData().getClientIP(), m_transfers[cookie].getFTData().getListeningPort());
	// END TODO:

	// XXX: this code probably DOES NOT go here. Move to it's place
	Message m;

	m.setCookie(cookie);
	m.setUin(m_transfers[cookie].getUin());
	m.setFormat(2);
	m.setType(TYPE_FILEREQ);

	if (!accept){
		m.setRequest(CANCEL);
		m_transfers.remove(cookie); // No longer valid FT
	}
	else
		m.setRequest(ACCEPT);

	m_parent->send(m.pack());
	// END XXX:
}

// SLOTS
//

void FileTransferProcess::messageArrived(Message m) {

	if (m.getType() == TYPE_FILEREQ){
		// TODO: check TYPE
		if (m_transfers.contains(m.getCookie())){
			//TODO: Already existing transfer. Deal with it (the
			//message)
		}
		else {
			FTStatus fs;
			fs.setFTData(m.getFTData());
			fs.setUin(m.getUin());
			fs.setStatus(PENDING); 

			m_transfers[m.getCookie()] = fs; // new FT req

			emit notifyNewFTRequest(m.getCookie(), m.getUin(), m.getFTData().getFileName());
		}
	}
}

void FileTransferProcess::ftconnEnded(ConnectionResult r){
	// TODO: handle. The (single conn support ATM) FT connection ended
	
}

void FileTransferProcess::ftconnSuccessful(){
	// TODO: this means that the connect method for this FT worked. Update
	// status
	
}

void FileTransferProcess::ftconnProgress(int percent){
	// TODO: emit the progress with the cookie
	
	qDebug("[FTProgress]: %d\n",percent);
}

}

#include "filetransferprocess.moc"
