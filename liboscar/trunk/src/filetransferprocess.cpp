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
#include "filesendservice.h"
#include <qfileinfo.h>

#define OSCAR_PROXY_SERVER "ars.oscar.aol.com"
#define OSCAR_PROXY_PORT 5190

namespace liboscar {

FileTransferProcess::FileTransferProcess(Service* s) { 
	m_parent = s;
	m_mylport = 0;
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

QWord FileTransferProcess::sendFile(UIN to, QString fname) {
	// TODO: do for more than one file
	QWord cookie;
	Buffer b;
	FTStatus st;
	FTData dt;

	// Do a new cookie for this transfer
	for (int i = 0; i < 8; i++)
		b << (Byte) qrand();
	b >> cookie;
	
	st.setUin(to);
	st.setCookie(cookie);
	st.setType(SENDING);
	st.setStatus(PENDING);

	QFileInfo f(fname);

	dt.setFileCount(1); // FIXME: below
	dt.setMultiple(false); // FIXME: can be the same
	dt.setReqNumber(1); 
	dt.setCookie(cookie); 
	dt.setFileName(fname); 
	dt.setFileSize(f.size());
	dt.setFileEncoding("ASCII"); // FIXME: make it work
	dt.setLocale("en"); // FIXME: make it work
//	dt.setMessage("<ICQ_COOL_FT>"); // yeah yeah, very cool

	st.setFTData(dt);
	this->startFT(st);
}

void FileTransferProcess::setUseProxy(bool p) {
	m_useproxy = p;
}

void FileTransferProcess::setListeningPort(int p) {
	m_mylport = p;
}

void FileTransferProcess::setReceiveDirectory(QString dir) {
	m_dir = dir;
}

//
// SLOTS
//

void FileTransferProcess::messageArrived(Message m) {
	if (m.getType() != TYPE_FILEREQ)
		return;

	if (m_transfers.contains(m.getCookie())){
		// Existing FT. 
		// Maybe we're going to a proxy2.
		if ((m.getRequest() == REQUEST) && 
				(m_transfers[m.getCookie()].getType() == SENDING)){
			this->clearFTService(m.getCookie());
			m_transfers[m.getCookie()].getFTData().setProxyUsed(true);
			m_transfers[m.getCookie()].getFTData().setProxyIP(m.getFTData().getProxyIP());
			m_transfers[m.getCookie()].getFTData().setListeningPort(m.getFTData().getListeningPort());
			m_transfers[m.getCookie()].setStatus(PROXY2); // Lets go
			this->startFT(m_transfers[m.getCookie()]);
		}

		// Maybe we got accepted :-)
		// We only care about accepts from the receiving part.
		// If we're receiving then the sender will send an OFT
		// header after this accept
		if ((m.getRequest() == ACCEPT) && 
				(m_transfers[m.getCookie()].getType() == SENDING))
			((FileSendService*) m_transfers[m.getCookie()].getService())->receiverAccepted();
	}
	else { // this is a new transfer (so, it's a RECEIVING one)
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

	if (m_transfers[c].getType() == RECEIVING) {
		if (m_transfers[c].getStatus() != PROXY2){
			qDebug("[FileTransferProcess]: ack from a receiving non-PROXY2 conn. ¿?¿?");
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
	else {
		if (m_transfers[c].getStatus() != PROXY1){
			qDebug("[FileTransferProcess]: ack from a sending non-PROXY1 conn. ¿?¿?");
			return;
		}
		// We got a PROXY1 response :-)
		// Send an REQUEST msg
		Message m;
	
		m.setCookie(c);
		m.setUin(m_transfers[c].getUin());
		m.setFormat(2);
		m.setType(TYPE_FILEREQ);
		m.setRequest(REQUEST);
	
		// Request specific data
		m.getFTData().setReqNumber(1); // the first
		m.getFTData().setClientIP(0); // not client, connect to proxy
		m.getFTData().setProxyIP(ip.toString());
		m.getFTData().setListeningPort(port);
		m.getFTData().setProxyUsed(true);
		m.getFTData().setFileCount(m_transfers[c].getFTData().getFileCount());
		m.getFTData().setFileSize(m_transfers[c].getFTData().getFileSize());
		m.getFTData().setFileName(m_transfers[c].getFTData().getFileName());
		m_parent->send(m.pack());
	}
}

void FileTransferProcess::proxyReadyArrived(unsigned int id) {
	QWord c = getCookieFromId(id);

	// We got a PROXY1(RECEIVING) or PROXY2(SENDING) response :-)
	// Send an ACCEPT msg
	this->acceptFT(m_transfers[c]);
}

void FileTransferProcess::handleServiceEnd(unsigned int id, ConnectionResult r) {
	QWord c = getCookieFromId(id);
	// A service (FT) ended, delete it from the transfers
	
	this->clearFTService(c);

	if (!r.successful()) // There was a problem connecting. Try another thing
		this->startFT(m_transfers[c]);
	else
		m_transfers.remove(c); // old transfer
}

//
// OTHER
//

DWord FileTransferProcess::calculateChunkChecksum(Buffer buffer, DWord prevcheck){
	return calculateChunkChecksum(buffer.toByteArray(), buffer.len(), prevcheck);
}

	/* The checksum calculation is adapted to C++ from 
	 * Pidgin's code (http://www.pidgin.im). Thank you guys! */
DWord FileTransferProcess::calculateChunkChecksum(const QByteArray buffer, int bufferlen, DWord prevcheck)
{
	DWord check = (prevcheck >> 16) & 0xffff, oldcheck;
	int i;
	unsigned short val;

	for (i=0; i<bufferlen; i++) {
		oldcheck = check;
		if (i&1)
			val = buffer[i];
		else
			val = buffer[i] << 8;
		check -= val;
		/*
		 * The following appears to be necessary.... It happens 
		 * every once in a while and the checksum doesn't fail.
		 */
		if (check > oldcheck)
			check--;
	}
	check = ((check & 0x0000ffff) + (check >> 16));
	check = ((check & 0x0000ffff) + (check >> 16));
	return check << 16;
}

DWord FileTransferProcess::calculateFileChecksum(QString filename) {
	QFile f(filename);
	DWord checksum = 0xffff0000;

	if (f.open(QIODevice::ReadOnly)){
		int bytes;
		QByteArray buffer;
		buffer = f.read(1024);
		while (buffer.size()){
			checksum = calculateChunkChecksum(buffer, buffer.size(), checksum);
			buffer = f.read(1024);
		}
		f.close();
	}

	return checksum;
}

void FileTransferProcess::sendRequest(QWord c) {
	// FIXME: use this function in proxyAckArrived()
	// Send an REQUEST msg
	Message m;
	
	m.setCookie(c);
	m.setUin(m_transfers[c].getUin());
	m.setFormat(2);
	m.setType(TYPE_FILEREQ);
	m.setRequest(REQUEST);
	
	// Request specific data
	m.getFTData().setReqNumber(1); // the first
	m.getFTData().setClientIP(0);
	m.getFTData().setProxyIP(0);
	m.getFTData().setListeningPort(m_mylport);
	m.getFTData().setProxyUsed(false);
	m.getFTData().setFileCount(m_transfers[c].getFTData().getFileCount());
	m.getFTData().setFileSize(m_transfers[c].getFTData().getFileSize());
	m.getFTData().setFileName(m_transfers[c].getFTData().getFileName());
	m_parent->send(m.pack());
}

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
	
	frs->setReceiveDirectory(m_dir);

	QObject::connect(frs, SIGNAL(ftProgress(unsigned int,int,int)), this, SLOT(ftconnProgress(unsigned int,int,int)));
	QObject::connect(frs, SIGNAL(fileEnded(unsigned int)), this, SLOT(fileEnded(unsigned int)));
	QObject::connect(frs, SIGNAL(connectionSuccessful(unsigned int)), this, SLOT(ftconnSuccessful(unsigned int)));
	QObject::connect(frs, SIGNAL(proxyAck(unsigned int, QHostAddress, Word)), 
			this, SLOT(proxyAckArrived(unsigned int, QHostAddress, Word)));
	QObject::connect(frs, SIGNAL(proxyReady(unsigned int)), this, SLOT(proxyReadyArrived(unsigned int)));
	QObject::connect(frs, SIGNAL(serviceEnded(unsigned int, ConnectionResult)), this, SLOT(handleServiceEnd(unsigned int, ConnectionResult)));
}

void FileTransferProcess::addNewSend(FTStatus& st) {
	FileSendService* fss;

	fss = new FileSendService(st);

	st.setService(fss);
	
	QObject::connect(fss, SIGNAL(ftProgress(unsigned int,int,int)), this, SLOT(ftconnProgress(unsigned int,int,int)));
	QObject::connect(fss, SIGNAL(fileEnded(unsigned int)), this, SLOT(fileEnded(unsigned int)));
	QObject::connect(fss, SIGNAL(proxyAck(unsigned int, QHostAddress, Word)), 
			this, SLOT(proxyAckArrived(unsigned int, QHostAddress, Word)));
	QObject::connect(fss, SIGNAL(proxyReady(unsigned int)), this, SLOT(proxyReadyArrived(unsigned int)));
	QObject::connect(fss, SIGNAL(serviceEnded(unsigned int, ConnectionResult)), this, SLOT(handleServiceEnd(unsigned int, ConnectionResult)));
}

void FileTransferProcess::clearFTService(QWord c) {
	FTStatus st = m_transfers[c];
	QObject::disconnect(this, 0, st.getService(), 0);
	delete st.getService();
	m_transfers[c].setService(0);
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
				if (m_useproxy) // User wants the conns to be proxied
					st.setStatus(PROXY2);
				else {
					if (st.getFTData().getProxyUsed())
						st.setStatus(PROXY1); // sender requested a proxy
					else
						st.setStatus(DIRECT);
				}

				this->addNewReceive(st);

				if (st.getStatus() == PROXY1)
					st.getService()->connect(st.getFTData().getProxyIP(), 
							OSCAR_PROXY_PORT); // Proxy always use 5190
				else
					st.getService()->connect(st.getFTData().getClientIP(), 
							st.getFTData().getListeningPort());
			}
			else if (st.getType() == SENDING) {
				// New created FT send. Check the use of the
				// proxy and start it.
				if (m_useproxy)
					st.setStatus(PROXY1);
				else
					st.setStatus(DIRECT);

				this->addNewSend(st);

				if (st.getStatus() == PROXY1){
					st.getFTData().setProxyUsed(true);
					st.getFTData().setProxyIP(OSCAR_PROXY_SERVER);
					st.getFTData().setListeningPort(OSCAR_PROXY_PORT);
					st.getFTData().setClientIP(0);
					st.getService()->connect(OSCAR_PROXY_SERVER, 
							OSCAR_PROXY_PORT);
				}
				else{
					st.getFTData().setProxyUsed(false);
					st.getFTData().setProxyIP(0);
					st.getFTData().setListeningPort(m_mylport);
					st.getFTData().setClientIP(0);
					this->sendRequest(st.getCookie());
					st.getService()->connect(st.getFTData().getListeningPort()); // listen
				}
			}
			break;
		case DIRECT:
			if (st.getType() == RECEIVING) {
				// We tried to connect to the sender, but we couldn't.
				// Start a stage2 proxy.
				st.setStatus(PROXY2);
				st.setType(RECEIVING);
				this->addNewReceive(st);
				st.getService()->connect(OSCAR_PROXY_SERVER, OSCAR_PROXY_PORT);
			}
			break;
		case PROXY2:
			if (st.getType() == SENDING) {
				st.setStatus(PROXY2);
				this->addNewSend(st);
				st.getService()->connect(st.getFTData().getProxyIP(), OSCAR_PROXY_PORT);
			}
			break;
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
