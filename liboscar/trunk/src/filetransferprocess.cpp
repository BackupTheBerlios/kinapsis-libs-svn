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
#include "filereceiveservice.h"

namespace liboscar {

FileTransferProcess::FileTransferProcess(Service* s) { 
	m_parent = s;
}

FileTransferProcess::~FileTransferProcess() { }

//
// USER ACTIONS
//

void FileTransferProcess::acceptFileTransfer(QWord cookie, bool accept){

	FTStatus st = m_transfers[cookie]; // recover data

	if (!accept)
		this->cancelFT(cookie);
	else
		this->startFT(st);
}

//
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
			fs.setType(RECEIVING); 
			fs.setCookie(m.getCookie()); 

			m_transfers[m.getCookie()] = fs; // new FT transfer

			emit notifyNewFTRequest(m.getCookie(), m.getUin(), m.getFTData().getFileName());
		}
	}
}

void FileTransferProcess::ftconnProgress(unsigned int id, int partial, int total) {
	QWord c = getCookieFromId(id);

	emit ftProgress(c, partial, total);
}

void FileTransferProcess::fileEnded(unsigned int id) {
	// TODO: must handle more than one file
	QWord c = getCookieFromId(id);

	emit ftEnded(c);
}

void FileTransferProcess::ftconnSuccessful(unsigned int id) {
	// We connected to something. Let's see...
	QWord c = getCookieFromId(id);
	
	if (m_transfers[c].getStatus() == DIRECT) {
		// Woho, we connected to the sender. Send an accept.
		this->acceptFT(m_transfers[c]);
	}
	else if (m_transfers[c].getStatus() == PROXY2) {
		// We connected to the proxy server. Do nothing (Included
		// because is clearer to me)
		return;
	}
	else if (m_transfers[c].getStatus() == PROXY1) {
		// We connected to the proxy server. Do nothing (Included
		// because is clearer to me)
		return;
	}
}

void FileTransferProcess::proxyAckArrived(unsigned int id, QHostAddress ip, Word port) {
	QWord c = getCookieFromId(id);

	if (m_transfers[c].getStatus() != PROXY2){
		qDebug("[FileTransferProcess]: ack from a non-PROXY2 conn. ¿?¿?");
		return;
	}
	// We got a PROXY2 response :-)
	// Send an REQUEST msg
	Message m;

	m.setCookie(c);
	m.setUin(m_transfers[c].getUin());
	m.setFormat(2);
	m.setType(TYPE_FILEREQ);
	m.setRequest(REQUEST);

	// Request specific data
	m.getFTData().setReqNumber(2); // the second one
	m.getFTData().setClientIP(0); // not client, connect to proxy
	m.getFTData().setProxyIP(ip.toString());
	m.getFTData().setListeningPort(port);
	m.getFTData().setProxyUsed(true);
	m.getFTData().setFileCount(m_transfers[c].getFTData().getFileCount());
	m.getFTData().setFileSize(m_transfers[c].getFTData().getFileSize());
	m.getFTData().setFileName(m_transfers[c].getFTData().getFileName());
	m_parent->send(m.pack());
}

void FileTransferProcess::proxyReadyArrived(unsigned int id) {
	QWord c = getCookieFromId(id);

	if (m_transfers[c].getStatus() != PROXY1){
		qDebug("[FileTransferProcess]: ready from a non-PROXY1 conn. ¿?¿?");
		return;
	}
	// We got a PROXY1 response :-)
	// Send an ACCEPT msg
	this->acceptFT(m_transfers[c]);
}

void FileTransferProcess::handleServiceEnd(unsigned int id, ConnectionResult r) {
	QWord c = getCookieFromId(id);
	// A service (FT) ended, delete it from the transfers
	
	FTStatus st = m_transfers[c];

	delete	m_transfers[c].getService();
	st.setService(0);

	if (!r.successful()) // There was a problem connecting. Try another thing
		this->startFT(st);
	else
		m_transfers.remove(c); // old transfer
}

//
// OTHER
//

QWord FileTransferProcess::getCookieFromId(unsigned int id) {
	FTMap::const_iterator i = m_transfers.constBegin();
	FTStatus s;

	while (i != m_transfers.constEnd()) {
		s = i.value();
		if (s.getService()) {
			if (s.getService()->getId() == id)
				return s.getCookie();
		}
		++i;
	}
	qDebug("[FileTransferProcess]: !!!!!!!! Received a signal from an not registered service.");
	return Q_UINT64_C(0xFFFFFFFFFFFFFFFF);
}

void FileTransferProcess::cancelFT(QWord cookie) {
	// Cancel the FT
	Message m;
	FTStatus st = m_transfers[cookie];

	m.setCookie(cookie);
	m.setUin(st.getUin());
	m.setFormat(2);
	m.setType(TYPE_FILEREQ);
	m.setRequest(CANCEL);
	m_parent->send(m.pack());

	m_transfers.remove(cookie); // delete from transfers
}

void FileTransferProcess::acceptFT(FTStatus st) {
	// Accept (to the sender) the FT
	Message m;

	m.setCookie(st.getCookie());
	m.setUin(st.getUin());
	m.setFormat(2);
	m.setType(TYPE_FILEREQ);
	m.setRequest(ACCEPT);
	m_parent->send(m.pack());
}

void FileTransferProcess::addNewReceive(FTStatus& st) {
	FileReceiveService* frs;

	frs = new FileReceiveService(st);

	st.setService(frs);
	
	QObject::connect(frs, SIGNAL(ftProgress(unsigned int,int,int)), this, SLOT(ftconnProgress(unsigned int,int,int)));
	QObject::connect(frs, SIGNAL(fileEnded(unsigned int)), this, SLOT(fileEnded(unsigned int)));
	QObject::connect(frs, SIGNAL(connectionSuccessful(unsigned int)), this, SLOT(ftconnSuccessful(unsigned int)));
	QObject::connect(frs, SIGNAL(proxyAck(unsigned int, QHostAddress, Word)), 
			this, SLOT(proxyAckArrived(unsigned int, QHostAddress, Word)));
	QObject::connect(frs, SIGNAL(proxyReady(unsigned int)), this, SLOT(proxyReadyArrived(unsigned int)));
	QObject::connect(frs, SIGNAL(serviceEnded(unsigned int, ConnectionResult)), this, SLOT(handleServiceEnd(unsigned int, ConnectionResult)));
}

void FileTransferProcess::startFT(FTStatus& st) {
	// Check the status and increment it. Take the decisions and launch a
	// service. TODO: currently only accepts receiving fts
	
	switch (st.getStatus()) {
		case PENDING:
			if (st.getType() == RECEIVING) {
				// So we accepted a pending connection
				// Create Service, connect signals and connect to
				// server.
				if (st.getFTData().getProxyUsed())
					st.setStatus(PROXY1); // sender requested a proxy
				else
					st.setStatus(DIRECT);
				st.setType(RECEIVING);

				this->addNewReceive(st);

				if (st.getStatus() == PROXY1)
					st.getService()->connect(st.getFTData().getProxyIP(), 5190); // Proxy always use 5190
				else
					st.getService()->connect(st.getFTData().getClientIP(), st.getFTData().getListeningPort());
			}
			break;
		case DIRECT:
			if (st.getType() == RECEIVING) {
				// We tried to connect to the sender, but we couldn't.
				// Start a stage2 proxy.
				st.setStatus(PROXY2);
				st.setType(RECEIVING);
				this->addNewReceive(st);
				st.getService()->connect("ars.oscar.aol.com", 5190); // Proxy always use 5190
			}
			break;
		case PROXY2:
		case PROXY3:
		case REDIRECT:
		case PROXY1:
		default:
			qDebug("[FileTransferProcess]: unimplemented transfer mode.");
			break;
	}
}

}

#include "filetransferprocess.moc"
