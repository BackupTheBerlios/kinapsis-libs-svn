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


#include "filereceiveservice.h"
#include "oftparser.h"
#include "oftproxyparser.h"

namespace liboscar {

FileReceiveService::FileReceiveService(FTStatus st){
	m_st = st;
}

FileReceiveService::~FileReceiveService() { }

void FileReceiveService::registerMeta() { }

void FileReceiveService::disconnect() {
	m_conn->disconnect();
}

void FileReceiveService::usePP(){
	Parser* nparser = new OFTProxyParser();
	if (m_parser){
		QObject::disconnect(m_parser,0,this,0);
		m_conn->changeParser(nparser);
		delete m_parser;
	}
	m_parser = nparser;
	QObject::connect(m_parser, SIGNAL(proxyError(OFTProxyError)), this, SLOT(handleProxyError(OFTProxyError)));
	QObject::connect(m_parser, SIGNAL(proxyAck(OFTProxyAck)), this, SLOT(handleProxyAck(OFTProxyAck)));
	QObject::connect(m_parser, SIGNAL(proxyReady(OFTProxyReady)), this, SLOT(handleProxyReady(OFTProxyReady)));
}

void FileReceiveService::useOP(){
	Parser* nparser = new OFTParser();
	if (m_parser){
		QObject::disconnect(m_parser,0,this,0);
		m_conn->changeParser(nparser);
		delete m_parser;
	}
	m_parser = nparser;
	QObject::connect(m_parser, SIGNAL(headerArrived(OFTHeader)), this, SLOT(handleDCheader(OFTHeader)));
	QObject::connect(m_parser, SIGNAL(rawData(Buffer)), this, SLOT(handleDCdata(Buffer)));
}

void FileReceiveService::create() {
	if (!m_parser){ 
		if (m_st.getStatus() > REDIRECT) // Any proxy stage here
			this->usePP();
		else // DC stage
			this->useOP();
	}
}

//
// SLOTS
//
	//
	// CONN
	//
	
void FileReceiveService::handleConnect() {

	// Here's the stuff. If this is an Proxy1 or Proxy2, we must interact
	// with the proxy until we got accepted.
	emit connectionSuccessful(this->getId());
	
	// FIXME: not ok for REDIRECT, PROXY3.
	
	if (m_st.getStatus() == PROXY1)
		this->sendProxyInitRecv();
	else if (m_st.getStatus() == PROXY2)
		this->sendProxyInitSend();
}

	//
	// PROXY
	//
	
void FileReceiveService::handleProxyError(OFTProxyError pe) {
	// TODO: disconnect with error
}

void FileReceiveService::handleProxyAck(OFTProxyAck pa) {
	if (m_st.getStatus() == PROXY2){
		emit proxyAck(this->getId(), pa.getProxyIP(), pa.getPort()); // this will be used for sending the reply ICBM
		this->useOP(); // finished with the proxy. Time for the OFT2 protocol
	}
	else
		qDebug("[FileReceiveService]: What?? Proxy send us an ACK after an InitSend. Very Strange.");
}

void FileReceiveService::handleProxyReady(OFTProxyReady pr) {
	if (m_st.getStatus() == PROXY1){
		emit proxyReady(this->getId());
		this->useOP(); // finished with the proxy. Time for the OFT2 protocol
	}
	else
		qDebug("[FileReceiveService]: What?? Proxy send us an Ready after an InitRecv. Very Strange.");
}

	//
	// DC
	//
	
void FileReceiveService::handleDCheader(OFTHeader head) {
	// We're receiving so a PROMPT must be what we get
	if (head.getType() == OFTPROMPT){
		m_file = new QFile(this);	
		m_file->setFileName(head.getFilename()); // FIXME: base dir?

		m_head = head;

		if (m_file->exists()) { // This could be a partial or done FT
			// TODO: check chksum && size
		}
		// Open file for writing
		m_file->open(QIODevice::WriteOnly);

		// ACK the file
		m_head.setType(OFTACK);
		m_head.setReceivedChk(0xFFFF0000);
		this->send(head.pack());
	}
	else
		qDebug("[FileReceiveService]: Got a non-PROMPT OFT2 header. That was unexpected.");
}

void FileReceiveService::handleDCdata(Buffer b) {
	int cnt=0;

	if ((cnt = m_file->write(b.toByteArray())) == -1){
		qDebug("[FileReceiveService]: failed writing to file.");
		return ;
	}
	if (cnt != b.len()) {
		qDebug("[FileReceiveService]: failed writing all the data to file.");
		return ;
	}

	m_head.setBytesReceived(m_head.getBytesReceived() + cnt);
	// TODO: calculate new chk

	emit ftProgress(this->getId(), m_head.getBytesReceived(), m_head.getTotalSize());

	if (m_head.getBytesReceived() == m_head.getTotalSize()){
		// We're done :-)
		m_file->close();
		emit fileEnded(this->getId());

		m_head.setType(OFTDONE);
		if (m_head.getFilesLeft() == 1)
			m_head.setFlags(OFTFLAGDONE);
		this->send(m_head.pack());

		if (m_head.getFilesLeft() == 1)
			this->disconnect(); // finish it!
	}
}


//
// PRIVATE
//

void FileReceiveService::sendProxyInitRecv(){
	OFTProxyInitRecv opir(m_st.getUin(), m_st.getFTData().getListeningPort(), m_st.getFTData().getCookie());
	this->send(opir.pack());
}

void FileReceiveService::sendProxyInitSend(){
	OFTProxyInitSend opis(m_st.getUin(), m_st.getFTData().getCookie());
	this->send(opis.pack());
}

}

#include "filereceiveservice.moc"
