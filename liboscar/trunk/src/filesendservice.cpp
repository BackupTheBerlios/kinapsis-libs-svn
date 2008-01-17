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


#include "filesendservice.h"
#include "oftparser.h"
#include "oftproxyparser.h"
#include "filetransferprocess.h"
#include <qfileinfo.h>
#include <qdatetime.h>

namespace liboscar {

FileSendService::FileSendService(FTStatus st){
	m_st = st;
}

FileSendService::~FileSendService() { }

void FileSendService::registerMeta() { }

void FileSendService::disconnect() {
	if (m_conn)
		m_conn->disconnect();
}

void FileSendService::create() {
	if (!m_parser) {
		if (m_st.getStatus() > REDIRECT)
			this->usePP();
		else
			this->useOP();
	}
}

void FileSendService::receiverAccepted() {
	// Receiver accepted. yuhu!
	this->sendDCPrompt();
}

//
// SLOTS
//

void FileSendService::handleProxyConn() {
	// We connected to the proxy. Say hi.
	if (m_st.getStatus() == PROXY1) {
		OFTProxyInitSend opis(m_st.getUin(), m_st.getCookie());
		this->send(opis.pack());
	}
	else if (m_st.getStatus() == PROXY2) {
		OFTProxyInitRecv opir(m_st.getUin(), m_st.getFTData().getListeningPort(), m_st.getCookie());
		this->send(opir.pack());
	}
}

void FileSendService::handleProxyError(OFTProxyError e) {
	// TODO: disconnect with error
}

void FileSendService::handleProxyAck(OFTProxyAck a) {
	if (m_st.getStatus() == PROXY1){
		emit proxyAck(this->getId(), a.getProxyIP(), a.getPort());
		this->useOP(); // finished with the proxy. Time for the OFT2 protocol
	}
	else
		qDebug("[FileSendService]: What?? (at handleProxyAck)");
}

void FileSendService::handleProxyReady(OFTProxyReady r) {
	if (m_st.getStatus() == PROXY2){
		emit proxyReady(this->getId());
		this->useOP(); // finished with the proxy. Time for the OFT2 protocol
		this->sendDCPrompt(); // XXX: I'm not sure. We sended the accept via BOS.
					// but we're the only capable of a
					// PROMPT
	}
	else
		qDebug("[FileSendService]: What?? (at handleProxyReady)");
}

void FileSendService::handleDCheader(OFTHeader h) {
	if (h.getType() == OFTACK)
		this->sendFile(); // send data, you fools!
	else if (h.getType() == OFTDONE)
		this->fileDone(h);
	else{
		qDebug("[FileSendService]: wow! I was really expecting an something else, and got %d", h.getType());
		return;
	}
}

// 
// OTHER
//

void FileSendService::sendDCPrompt() {
	// FIXME: do for more than one file
	// XXX: OFTHeader can have a fromFTData(FTData)
	m_head.setType(OFTPROMPT);
	m_head.setTotalFiles(m_st.getFTData().getFileCount());
	m_head.setFilesLeft(1);
	m_head.setTotalParts(1);
	m_head.setPartsLeft(1);
	m_head.setTotalSize(m_st.getFTData().getFileSize());
	m_head.setSizeLeft(m_st.getFTData().getFileSize());
	m_head.setBytesReceived(0); // well, sent actually, but I have to choose a name.
	m_head.setFlags(OFTFLAGNEGO);
//	m_head.setEncoding(m_st.getFTData().getEncoding());
	m_head.setEncoding(ASCII); // FIXME: fix all the encoding issues
	m_head.setFileName(m_st.getFTData().getFileName());

	QFileInfo fi(m_st.getFTData().getFileName());
	m_head.setModTime(fi.lastModified().toTime_t());

	m_head.setChecksum(FileTransferProcess::calculateFileChecksum(m_st.getFTData().getFileName()));
	m_head.setReceivedChk(0xffff0000); // weeeell, sentchk
	this->send(m_head.pack()); // send it
}

void FileSendService::sendFile() {
	QFile f(m_st.getFTData().getFileName());

	if (!f.open(QIODevice::ReadOnly))
		return;

	while (!f.atEnd()) {
		// FIXME: this does NOT check that the data is actually sent
		QByteArray a = f.read(1024); // send one kb chunks FIXME: this is temporary
		int w = a.size();
		Buffer b;
		b << a;
		this->send(b);
		m_head.setBytesReceived(m_head.getBytesReceived() + w);
		m_head.setReceivedChk(FileTransferProcess::calculateChunkChecksum(a, w, 
					m_head.getReceivedChk()));
		emit ftProgress(this->getId(), m_head.getBytesReceived(), m_head.getTotalSize());
	}
	f.close(); // nothing more here. Wait for an done.
}

void FileSendService::fileDone(OFTHeader h) {
	// We're so happy :-)
	
	emit fileEnded(this->getId());

	if (h.getReceivedChk() != m_head.getReceivedChk())
		qDebug("[FileSendService]: CHKs do not match!!!");

	this->disconnect();
}

void FileSendService::usePP(){
	Parser* nparser = new OFTProxyParser();
	if (m_parser){
		QObject::disconnect(m_parser,0,this,0);
		m_conn->changeParser(nparser);
		delete m_parser;
	}
	m_parser = nparser;
	QObject::connect(m_conn, SIGNAL(connConnected()), this, SLOT(handleProxyConn()));
	QObject::connect(m_parser, SIGNAL(proxyError(OFTProxyError)), this, SLOT(handleProxyError(OFTProxyError)));
	QObject::connect(m_parser, SIGNAL(proxyAck(OFTProxyAck)), this, SLOT(handleProxyAck(OFTProxyAck)));
	QObject::connect(m_parser, SIGNAL(proxyReady(OFTProxyReady)), this, SLOT(handleProxyReady(OFTProxyReady)));
}

void FileSendService::useOP(){
	Parser* nparser = new OFTParser();
	if (m_parser){
		QObject::disconnect(m_parser,0,this,0);
		m_conn->changeParser(nparser);
		delete m_parser;
	}
	m_parser = nparser;
	QObject::connect(m_parser, SIGNAL(headerArrived(OFTHeader)), this, SLOT(handleDCheader(OFTHeader)));
}

}

#include "filesendservice.moc"
