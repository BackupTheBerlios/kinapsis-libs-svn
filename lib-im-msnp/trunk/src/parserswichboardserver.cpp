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
#include <QRegExp>
#include <QFile>
#include "parserswichboardserver.h"
#include "context.h"

namespace libimmsnp {
ParserSB::ParserSB (QString address, int port, int chatId, QString msnPassport, QString ticket, QString sessid, Client* c){
	m_idtr = 1;
	m_address = address;
	m_port = port;
	m_socket = 0;
	m_client = c;
	m_chatId = chatId;
	m_msnPassport = msnPassport;
	m_ticket = ticket;
	m_sessid = sessid;
	m_hasCommand = false;
	m_isParsing = false;
	m_endChat = false;
	qRegisterMetaType<Transfer*>("Transfer");
}

int toInt (QByteArray num){
	bool ok;
	QByteArray tmp = num.mid(3,1).toHex().data();
	tmp.append(num.mid(2,1).toHex().data());
	tmp.append(num.mid(1,1).toHex().data());
	tmp.append(num.mid(0,1).toHex().data());
	return tmp.toInt(&ok,16);
}
QByteArray toByte (int num){
	QByteArray tmp2 = QByteArray::number(num,16);
	while (tmp2.size() < 8) tmp2.insert(0,"0");
	QByteArray tmp3;
	QByteArray a;
	a = QByteArray::fromHex(tmp2);
	tmp3 = a.mid(3,1).toHex().data();
	tmp3.append(a.mid(2,1).toHex().data());
	tmp3.append(a.mid(1,1).toHex().data());
	tmp3.append(a.mid(0,1).toHex().data());
	return tmp3;
}

void ParserSB::run(){
	m_socket = new msocket ();
	m_socket->connect(m_address, m_port);

	if (m_sessid != ""){ // they start the chat
		ANS ans(m_idtr++);
		ans.addPassport (m_msnPassport);
		ans.addTicket (m_ticket);
		ans.addSessId (m_sessid);
		m_socket->send(ans.makeCmd());
	}

	else {	// I start the chat
		USRchat usr (m_idtr++);
		usr.addPassport (m_msnPassport);
		usr.addTicket (m_ticket);
		m_socket->send(usr.makeCmd());
	}

	QByteArray data;
	//qDebug("MSN::ParserSB:: Chat %i START",m_chatId);
	while (!m_endChat && ((m_socket->recv(data)) != -1)){
		feed (data);
		data.clear();
		if (!m_isParsing){parse();}
	}
	//qDebug("MSN::ParserSB:: END Connecton with buddy: %s at:%s",m_buddy.toUtf8().data(), m_socket->getHost().toUtf8().data());
	delete m_socket;
	m_socket = 0;
}

void ParserSB::send (Command& c){
	m_socket->send(c.makeCmd());
}
void ParserSB::sendFile(QString file, QString dest){
	P2P initFT = 		P2P(nextIdtr());	
	initFT.setCmd		(P2PC_INITFT);
	initFT.setTo		(dest.toUtf8());
	//qDebug() << "ENVIO: INITBID" << bid.make().toHex();
	m_socket->send(initFT.make());
}

void ParserSB::feed (QByteArray b){
	m_hasCommand = true;
	m_buf.append(b);
}

void ParserSB::parseIro(){
	// IRO 1 1 1 XXXXXXX@gmail.com XXXXX 1073741860\r\n 
	// IRO trid rooster roostercount passport friendlyname
	QRegExp rx;
	rx.setPattern("(^IRO \\d+ (\\d+) (\\d+) (\\S+) (\\S+) (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		QString rooster = rx.cap(2);
		QString rosterCount = rx.cap(3);
		m_buddy = rx.cap(4);
		QString fName = rx.cap(5);

		m_buf.remove(0,rx.cap(1).size());
	}
	else m_hasCommand = false;
}
void ParserSB::parseAns(){
	//ANS trid "OK"\r\n
	QRegExp rx;
	rx.setPattern("(^ANS (\\d+) OK\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		// TODO: 
		m_buf.remove(0,rx.cap(1).size());
		MSG m;
		m.addTrid(nextIdtr());
		m.addType(MSG_IDENTIFICATION);
		m.addClientName(m_client->getClientName());
		m.addClientVersion(m_client->getClientVer());
		m.addClientIsLogging(m_client->getClientIsLogging());
		m_socket->send(m.makeCmd());
	}
	else m_hasCommand = false;
}
void ParserSB::acceptFileTransfer (Transfer* msg, QByteArray path) {
	//m_FTList[msg.getBHid()].setDestination(path);
	msg->setDestination(path);
	qDebug() << "@############# Transferencia Aceptada en:" << msg->getDestination();
	P2P bid = P2P(nextIdtr());	
	bid.setCmd(P2PC_INITID);
	bid.setTo		(msg->getFrom());
	bid.setBHIdentifier	(msg->incMyIdentifier(0));
	bid.setBHTotalDataSize	(msg->getBHTotalDataSize());
	bid.setBHAckIdentifier	(msg->getBHIdentifier());
	bid.setBHAckUniqueID		(msg->getBHAckIdentifier());
	bid.setBHAckDataSize		(msg->getBHTotalDataSize());
	bid.setBHFlag				(QByteArray::fromHex("02 00 00 00"));
	//qDebug() << "ENVIO: INITBID" << bid.make().toHex();
	m_socket->send(bid.make());

	P2P ok200 = P2P(nextIdtr());	
	ok200.setStep (P2P_INVITATION);
	ok200.setCmd(P2PC_200OK);
	ok200.setFrom			(msg->getTo());
	ok200.setCsEq					(1);
	ok200.setTo			(msg->getFrom());
	ok200.setBranch			(msg->getBranch());
	ok200.setCallId			(msg->getCallId());
	ok200.setp2pSessionId		(msg->getp2pSessionId());
	
	ok200.setBHIdentifier		(msg->incMyIdentifier(0));
	ok200.setBHAckIdentifier	(msg->incAckIdentifier(1));

	//qDebug() << "ENVIO: 200 OK" << ok200.make().toHex();

	m_socket->send(ok200.make());
}

void ParserSB::parseMsg () {
	// MSG XXXXXXXX@gmail.com XXXXXX 590\r\nMIME-Version: 1.0\r\nContent-Type: application/x-msnmsgrp2p\r\nP2P-Dest: XXXXXXXX@hotmail.com\r\n\r\n......
	// MSG XXXXXXXX@hotmail.com XXXX 128\r\nMIME-Version: 1.0\r\nContent-Type: text/plain; charset=UTF-8\r\nX-MMS-IM-Format: FN=Helvetica; EF=; CO=000000; CS=0; PF=22\r\n\r\nholaaa
	QRegExp rx;
	QByteArray data;
	rx.setPattern("(^MSG (\\S+) (\\S+) (\\d+)\r\n)MIME-Version: 1.0\r\nContent-Type: (\\S+)[\r\n|;]"); 
	if (rx.indexIn(m_buf) != -1){
		QString header = rx.cap(1);
		QString senderPassport = rx.cap(2);
		QString msgType = rx.cap(5);
		QString payload = rx.cap(4);
		data.append(m_buf.mid(rx.cap(1).size(), payload.toInt()));
		if (data.size() == payload.toInt()) {
			m_buf.remove(0, header.size() + payload.toInt());

			if (msgType == QString("text/x-clientcaps")){
				rx.setPattern("\r\n\r\nClient-Name: ([\\S|\\s]*)\r\nChat-Logging: (\\S)[\r\n]+$");
				if (rx.indexIn(data.data()) != -1){
					//qDebug("MSG chat info client:%s logging:%s",rx.cap(1).toUtf8().data(), rx.cap(2).toUtf8().data());
					emit chatInfo (m_chatId, rx.cap(1), rx.cap(2));
					return;
				}
			}

			if (msgType == QString("text/x-msmsgscontrol")){
				rx.setPattern("\r\nTypingUser: (\\S+)[\r\n]+$");
				if (rx.indexIn(data.data()) != -1){
					//qDebug("MSG Typing user:%s ",rx.cap(1).toUtf8().data());
					emit chatIsTyping ( m_chatId,senderPassport);
					return;
				}
			}


			if (msgType == QString("text/plain")){
				QString font; 
				QString effect;
				QString color;
				QString charSet;
				QString fontType; 
				QString codif;
				MSG msg ;
				qRegisterMetaType<MSG>("MSG");

				rx.setPattern("charset=(\\S+)");
				if (rx.indexIn(data.data()) != -1){
					codif = rx.cap(1);
				}

				// X-MMS-IM-Format: FN=Arial; EF=BI; CO=0
				rx.setPattern("\r\nX-MMS-IM-Format: FN=(\\S+); EF=(\\S*); CO=(\\S*); CS=(\\S*); PF=(\\d*)");
				if (rx.indexIn(data.data()) != -1){
					font = rx.cap(1);
					effect = rx.cap(2);
					color = rx.cap(3);
					charSet = rx.cap(4);
					fontType = rx.cap(5);
				}
				rx.setPattern("\r\nX-MMS-IM-Format: FN=(\\S+); EF=(\\S*); CO=(\\S*)");
				if (rx.indexIn(data.data()) != -1){
					font = rx.cap(1);
					effect = rx.cap(2);
					color = rx.cap(3);
				}
				msg.addColor(color);
				msg.addFont(font);
				if (effect.contains('B')) msg.addEffect(EFFECT_BOLD);
				if (effect.contains('I')) msg.addEffect(EFFECT_ITALIC);
				if (effect.contains('U')) msg.addEffect(EFFECT_UNDERLINE);

				msg.addMsg( data.mid (data.indexOf("\r\n\r\n")+4));

				//qDebug("MSG Codificacion:%s Fuente:%s Efectos:%s Color:%s Character sets:%s Font Type:%s msg:%s",codif.toUtf8().data(), font.toUtf8().data(), effect.toUtf8().data(), color.toUtf8().data(), charSet.toUtf8().data(), fontType.toUtf8().data() ,msg.dataDebug());
				emit chatArrivedMessage(m_chatId, senderPassport, msg);
				return;

			}

			if (msgType == QString("text/x-msnmsgr-datacast")){
				// Zumbidos
				rx.setPattern("\r\n\r\nID: 1[\r\n]+$");
				if (rx.indexIn(data.data()) != -1){
					qDebug("MSN::ParserSB::Log #%i: MSG Zumbido", m_chatId);
				}
				// Guiños
				// Content-Type: text/x-msnmsgr-datacast\r\nMessage-ID: {879ED7A5-DF1C-BEAA-6D8B-65F6137734C5}\r\nChunks: 3\r\n\r\nID: 2\r\nData: <msnobj Creator="vaticano666@hotmail.com" Size="95742" Type="8" Location="TFR1B4.tmp" Friendly="QgByAGUAYQBrACAARABhAG4AYwBlAHIA" SHA1D="i++iSwaoqftUJZ3j+9d2jccqtmo=" SHA1C="oCshAefPuHtWCODyB/eX+103nOY=" stamp="MIIIngYJ.....
				// MSG vaticano666@hotmail.com Pedro 1266\r\nMessage-ID: {879ED7A5-DF1C-BEAA-6D8B-65F6137734C5}\r\nChunk: 1\r\n\r\n6CAcoATQBpAGMAcgBvAHMAbwBmA...
				// MSG vaticano666@hotmail.com Pedro 837\r\nMessage-ID: {879ED7A5-DF1C-BEAA-6D8B-65F6137734C5}\r\nChunk: 2\r\n\r\nzwj7QM7Rs/4p...
				rx.setPattern("^MIME-Version: 1.0\r\nContent-Type: text/x-msnmsgr-datacast\r\nMessage-ID: \\{(\\S+)\\}\r\nChunks: (\\d+)\r\n\r\nID: (\\d+)\r\nData: <msnobj Creator=\"(\\S+)\" Size=\"(\\d+)\" Type=\"(\\d+)\" Location=\"(\\S+)\" Friendly=\"(\\S+)\" SHA1D=\"(\\S+)\" SHA1C=\"(\\S+)\" stamp=\"(\\S+)$");
				if (rx.indexIn(data.data()) != -1){
					QString messageID	= rx.cap(1);
					QString chunks		= rx.cap(2);  
					QString id		= rx.cap(3); 
					QString creator		= rx.cap(4); 
					QString size		= rx.cap(5); 
					QString type		= rx.cap(6); 
					QString location	= rx.cap(7);    
					QString friendly	= rx.cap(8);    
					QString sha1D		= rx.cap(9);  
					QString sha1C		= rx.cap(10); 
					QString dataWink	= rx.cap(11); 	
					printf ("MSN::ParserSB::Log #%i: MSG Guiño %s,%s\n",m_chatId, messageID.toUtf8().data(), dataWink.toUtf8().data() );
				}
			}

			if (msgType == QString("application/x-msnmsgrp2p")){
				rx.setPattern("\r\nP2P-Dest: (\\S+)\r\n\r\n");
				if (rx.indexIn(data) != -1){
					ParserP2P p = ParserP2P();
					p.parse(data);

					if (p.getData().size() == 0){
						// Es un ACK 
						qDebug() << "RECIBO ACK" << p.getBHid() << m_FTList.size() << m_FTList.keys();
						int desp = 0;
						while (++desp){
							if (m_FTList.contains(p.getBHid()-desp)){
								if (m_FTList[p.getBHid()-desp]->getP2pType() == P2PT_EMOTICON){
										Transfer* t = m_FTList[p.getBHid()-desp];
										P2P bid = 					P2P(nextIdtr());	
										bid.setCmd					(P2PC_ACK);
										bid.setBHSessionID		(toByte(t->getp2pSessionId().toInt()));
										bid.setTo					(t->getFrom());
										bid.setBHIdentifier		(t->incMyIdentifier(0));
										bid.setBHTotalDataSize	(t->getBHTotalDataSize());
										bid.setBHAckIdentifier	(t->getBHIdentifier());
										bid.setBHAckUniqueID		(t->getBHAckIdentifier());
										bid.setBHAckDataSize		(t->getBHTotalDataSize());
										bid.setBHFlag				(QByteArray::fromHex("02 00 00 00"));
										bid.setBinaryFooter		(QByteArray::fromHex("00 00 00 01"));
			
										//qDebug() << "ENVIO: INITBID" << bid.make().toHex();
										m_socket->send				(bid.make());
										m_FTList[p.getBHid()-desp]->setP2pType(P2PT_NULL);
								}
								break;
							}
						}
						return ;
					}
					
					// si no está lo añado, junto con sus datos
					if (! m_FTList.contains(p.getBHid())) {
						qDebug() << "NO LO Contiene" << p.getBHid() << p.step() << m_FTList.keys();
						int desp = 0;
						if (p.step() == P2P_NEGOTIATION){
							while (++desp){
								if (m_FTList.contains(p.getBHid()-desp)){
									m_FTList.insert((p.getBHid()), m_FTList[p.getBHid()-desp]);
									m_FTList.remove(p.getBHid()-desp);
									m_FTList[p.getBHid()]->setStep(P2P_NEGOTIATION);
									qDebug() << "ACTUALIZADO" << m_FTList.keys();
									break;
								}
							}
							qDebug() << "NO ACTUALIZADO" << m_FTList.keys();
						}
						desp = 0;
						if (p.step() == P2P_TRANSFER){
							while (++desp){
								if (m_FTList.contains(p.getBHid()-desp)){
									m_FTList.insert((p.getBHid()), m_FTList[p.getBHid()-desp]);
									m_FTList.remove(p.getBHid()-desp);
									m_FTList[p.getBHid()]->setStep(P2P_TRANSFER);
									qDebug() << "ACTUALIZADO" << m_FTList.keys();
									break;
								}
							}
							qDebug() << "NO ACTUALIZADO" << m_FTList.keys();
						}
						if (p.step() == P2P_BYE){
						desp = 0;
							while (++desp){
								if (m_FTList.contains(p.getBHid()-desp)){
									m_FTList.insert((p.getBHid()), m_FTList[p.getBHid()-desp]);
									m_FTList.remove(p.getBHid()-desp);
									m_FTList[p.getBHid()]->setStep(P2P_BYE);
									qDebug() << "ACTUALIZADO" << m_FTList.keys();
									break;
								}
							}
							qDebug() << "NO ACTUALIZADO" << m_FTList.keys();
						}
						if (p.step() == P2P_INVITATION){
							Transfer* t = new Transfer();
							t->setStep					(p.step());
							t->createMyIdentifier	();
							t->createAckIdentifier	();
							t->addData					(p.getData());
							t->setBHIdentifier		(p.getBHid());
							t->setBHAckIdentifier	(p.getBHAckIdentifier());
							t->setBHTotalDataSize	(p.getBHTotalDataSize());
							t->setTo	   				(p.getTo	      	()); 
							t->setFrom	    			(p.getFrom	   	());
							t->setBranch				(p.getBranch      ());    
							t->setCallId				(p.getCallId      ());	    
							t->setp2pSessionId  		(p.getp2pSessionId());  
							t->setContext  			(p.getContext		());  
							if (p.getEUF_GUID() == "5D3E02AB-6190-11D3-BBBB-00C04F795683"){ 
								qDebug() << "## FILETRANSFER with id " << t->incMyIdentifier(0).toHex();
								t->setP2pType(P2PT_FILE);
							}
							if (p.getEUF_GUID() == "A4268EEC-FEC5-49E5-95C3-F126696BDBF6"){
								qDebug() << "##  EMOTICONO\n";
								t->setP2pType(P2PT_EMOTICON);
							}
							if (p.getEUF_GUID() == "4BD96FC0-AB17-4425-A14A-439185962DC8"){
								qDebug() << "##  WEBCAM\n";
								t->setP2pType(P2PT_WEBCAM);
							}
								
							m_FTList.insert (p.getBHid(), t);
						}
					}
					// Una vez que lo he metido
					if (m_FTList.contains(p.getBHid())) {
						qDebug() << "LO Contiene" << p.step() << p.getBHid()  << m_FTList.keys();
						Transfer* t = m_FTList[p.getBHid()];
						if (t->getStep() == P2P_INVITATION) {
							qDebug() << "INVITACION"  << p.getBHid()  << m_FTList.keys();
							if (p.isFinished()){
								if (t->getP2pType() == P2PT_FILE){
									Context c1 = Context();
									c1.parse(t->getContext());
									t->setFileName			(c1.getName());
									m_FTList.insert (p.getBHid(), t);
									qDebug() << "INNVITACION FIN"  << p.getBHid() << t->getFileName();
									emit incomingFileTransfer (m_chatId, t);
								}
								if (t->getP2pType() == P2PT_EMOTICON){
									m_FTList.insert (p.getBHid(), t);
										P2P bid = P2P(nextIdtr());	
										bid.setCmd(P2PC_INITID);
										bid.setTo					(t->getFrom());
										bid.setBHIdentifier		(t->incMyIdentifier(0));
										bid.setBHTotalDataSize	(t->getBHTotalDataSize());
										bid.setBHAckIdentifier	(t->getBHIdentifier());
										bid.setBHAckUniqueID		(t->getBHAckIdentifier());
										bid.setBHAckDataSize		(t->getBHTotalDataSize());
										bid.setBHFlag				(QByteArray::fromHex("02 00 00 00"));
										//qDebug() << "ENVIO: INITBID" << bid.make().toHex();
										m_socket->send				(bid.make());

										P2P ok200 = 				P2P(nextIdtr());	
										ok200.setStep 				(P2P_INVITATION);
										ok200.setCmd				(P2PC_200OK);
										ok200.setFrom				(t->getTo());
										ok200.setCsEq				(1);
										ok200.setTo					(t->getFrom());
										ok200.setBranch			(t->getBranch());
										ok200.setCallId			(t->getCallId());
										ok200.setp2pSessionId	(t->getp2pSessionId());
										
										ok200.setBHIdentifier	(t->incMyIdentifier(0));
										ok200.setBHAckIdentifier(t->incAckIdentifier(1));

										//qDebug() << "ENVIO: 200 OK" << ok200.make().toHex();

										m_socket->send(ok200.make());
								}
								if (t->getP2pType() == P2PT_WEBCAM){
									m_FTList.insert (p.getBHid(), t);
								}
								t->setStep(P2P_NEGOTIATION);
							}
							else {
								t->addContext  (p.getData());  
								qDebug() << "INVITACION RECIBIENDO"  << p.getBHid();
							}
						}
						else if (t->getStep() == P2P_NEGOTIATION){
							if (p.isFinished()){
								qDebug() << "NEGOCIACIONES FIN 1"  << p.getBHid();
								P2P ack = P2P(nextIdtr());	
								ack.setCmd(P2PC_ACK);
								ack.setBHIdentifier		(t->incMyIdentifier(1));
								ack.setBHTotalDataSize	(t->getBHTotalDataSize());
								ack.setBHMessageLength	(t->getBHTotalDataSize().mid(0,4));
								ack.setBHAckIdentifier	(t->incMyIdentifier(1));
								//qDebug() << "ENVIO: ACK" << ack.make().toHex();
								m_socket->send(ack.make());

								P2P neg = P2P(nextIdtr());	
								neg.setStep					(P2P_NEGOTIATION);
								neg.setCmd					(P2PC_200OK);
								neg.setFrom					(p.getTo());
								neg.setTo					(p.getFrom());
								neg.setBranch				(p.getBranch());
								neg.setCallId				(p.getCallId());
								neg.setp2pSessionId		(p.getp2pSessionId());
								neg.setBHIdentifier		(m_FTList[p.getBHid()]->incMyIdentifier(0));
								neg.setBHAckIdentifier	(m_FTList[p.getBHid()]->incAckIdentifier(1));
								//qDebug() << "ENVIO: 200 OK NEGOTIATION" << neg.make().toHex();
								m_socket->send				(neg.make());
								t->setStep(P2P_TRANSFER);
								
							}
							else {
								qDebug() << "NEGOCIACIONES RECIBIENDO"  << p.getBHid();
							}
						}	
						else if (t->getStep() ==  P2P_TRANSFER){
							qDebug() << "TRANSFERENCIA RECIBIENDO" << p.getBHDataOffset() << p.getBHMessageLength() << p.getBHTotalDataSize() << t->getDestination(); 
                  	emit fileTransferProgress(m_chatId, p.getBHid(), (p.getBHDataOffset() + p.getBHMessageLength()), p.getBHTotalDataSize());
							QFile * fd =  new QFile(t->getDestination());
                  	if (fd->open(QIODevice::Append)){
                  	      qDebug() << "TRANSFERENCIA ESCRIBIENDO" << p.getBHid();
                  	      fd->write(p.getData());
                  	      fd->close();
                  	}
							if (p.isFinished()){
	                  	emit fileTransferFinished(m_chatId, p.getBHid());
								t->setHasTransfered();
								qDebug() << "Finalizado" << p.getBHid();
								
								t->setBHSessionID			(p.getBHSessionID()); 
								t->setBHTotalDataSize 	(p.getBHTotalDataSize());
								t->setBHIdentifier		(p.getBHid());    
								t->setBHAckIdentifier	(p.getBHAckIdentifier());	    
								
								P2P ackFin = P2P(nextIdtr());	
								ackFin.setCmd						(P2PC_ACK);
								ackFin.setBHSessionID 			(t->getBHSessionID());
								ackFin.setBHIdentifier			(t->incMyIdentifier(1));
								ackFin.setBHTotalDataSize		(t->getBHTotalDataSize());
								ackFin.setBHFlag					(QByteArray::fromHex("02 00 00 00"));
								ackFin.setBHAckIdentifier		(t->getBHIdentifier());
							  	ackFin.setBHAckUniqueID 		(t->getBHAckIdentifier());
								ackFin.setBHAckDataSize 		(t->getBHTotalDataSize());
								ackFin.setTo						(t->getFrom());
								qDebug() << "ENVIO: ACK a "<< t->getFrom() << ackFin.make().toHex();
								m_socket->send(ackFin.make());
							}
						}
						else if (t->getStep() ==  P2P_BYE){
								if (!t->hasTransfered()){
									qDebug() << "\n\n\n\nCANCELADOOOO \n\n\n\n";
									emit fileTransferCanceled(m_chatId, p.getBHid());
								}
								m_FTList.remove(p.getBHid());
								qDebug() << "ELIMINADO" << m_FTList.keys();
								qDebug() << "ENVIADO Y RECIBIDO CORRECTAMENTE";
						}
					}
				}
			}
		}
	}
	else {
		rx.setPattern("(^MSG (\\S+) (\\S+) (\\d+)\r\n)"); 
		if (rx.indexIn(m_buf) != -1){
			QString header = rx.cap(1);
			QString senderPassport = rx.cap(2);
			QString payload = rx.cap(4);
			qDebug ("%s, %s, %s",header.toUtf8().data(),senderPassport.toUtf8().data(),payload.toUtf8().data());
			data.clear();
			data.append(m_buf.mid(header.size(), payload.toInt()));
			// MSG vaticano666@hotmail.com Pedro 837\r\nMessage-ID: {879ED7A5-DF1C-BEAA-6D8B-65F6137734C5}\r\nChunk: 2\r\n\r\nzwj7QM7Rs/4...
			if (data.size() == payload.toInt()) {
				m_buf.remove(0, header.size() + payload.toInt());
				rx.setPattern("^Message-ID: \\{(\\S+)\\}\r\nChunk: (\\d+)\r\n\r\n(\\S+)");
				if (rx.indexIn(data.data()) != -1){
					QString messageID2	= rx.cap(1);
					QString chunks2		= rx.cap(2);  
					QString dataWink2	= rx.cap(3); 
					printf ("MSN::ParserSB::Log #%i: MSG Guiño %s,%s\n",m_chatId, messageID2.toUtf8().data(), dataWink2.toUtf8().data() );
				}
			}
			else
				m_hasCommand = false;
		}
		else
			m_hasCommand = false;
	}
}

void ParserSB::parseUsr(){
	// << USR 827 OK XXXXXXXXX@hotmail.com XXXX\r\n
	// >> CAL 828 XXXXXXX@gmail.com\r\n
	QRegExp rx;
	rx.setPattern("(^USR \\d+ OK \\S+ \\S+\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		m_buf.remove(0,rx.cap(1).size());
		CAL cal(m_idtr++);
		cal.addPassport (m_client->nextChatPassport());
		m_socket->send(cal.makeCmd());
	}
	else m_hasCommand = false;
}

void ParserSB::parseCal(){
	// << CAL 828 RINGING 482462925\r\n
	// >> CAL 32 XXXXXXXX@gmail.com\r\n
	QRegExp rx;
	rx.setPattern("(^CAL \\d+ RINGING (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		m_buf.remove(0,rx.cap(1).size());
	}
	else m_hasCommand = false;
}

void ParserSB::parseJoi(){
	// << JOI XXXXXXXXXXXX@gmail.com XXXXXXXX 1073741860.73741860.912301
	QRegExp rx;
	rx.setPattern("(^JOI (\\S+) (\\S+) (\\S+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		QString email = rx.cap(2);
		QString fName = rx.cap(3);
		QString capabilities = rx.cap(4);
		m_buf.remove(0,rx.cap(1).size());
		MSG m;
		m.addType(MSG_IDENTIFICATION);
		m.addClientName(m_client->getClientName());
		m.addClientVersion(m_client->getClientVer());
		m.addClientIsLogging(m_client->getClientIsLogging());
		//m_socket->send(m.makeCmd());
		//qDebug("MSN::ParserSB::Log #%i::NEW CHAT with %s fname:%s capab:%s", m_chatId,email.toUtf8().data(), fName.toUtf8().data(), capabilities.toUtf8().data());
		// TODO: remove three lines below
	}
	else m_hasCommand = false;
}

void ParserSB::parseAck(){
	// ACK 3\r\n
	QRegExp rx;
	rx.setPattern("(^ACK (\\d+)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		m_buf.remove(0,rx.cap(1).size());
	}
	else m_hasCommand = false;
}


void ParserSB::parseBye () {
	// BYE XXXXXXXXX@hotmail.com 1\r\n 
	QRegExp rx;
	rx.setPattern("(^BYE (\\S+)[ ]?(\\d*)\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		m_buf.remove(0, rx.cap(1).size());
		m_endChat = true;
		emit chatLeavedTheRoom (m_chatId,rx.cap(2));
		m_socket->close();
	}
	else m_hasCommand = false;
}

void ParserSB::parseError () {
	// 215 1\r\n
	QRegExp rx;
	rx.setPattern("(^(\\d{3}) \\d+\r\n)"); 
	if (rx.indexIn(m_buf) != -1){
		qDebug ("MSN::ParserSB::Log #%i ## ERROR : %s", m_chatId, rx.cap(2).toUtf8().data());
		QString reason = rx.cap(2);
		m_buf.remove(0,rx.cap(1).size());
		if (reason == "200") {
		        qDebug("MSN::ERROR::ParserNS : Invalid Syntax");
		}
		if (reason == "201") {
		        qDebug("MSN::ERROR::ParserNS : Invalid parameter");
		}
		if (reason == "205") {
		        qDebug("MSN::ERROR::ParserNS : Invalid principal");
		}
		if (reason == "206") {
		        qDebug("MSN::ERROR::ParserNS : Domain name missing");
		}
		if (reason == "207") {
		        qDebug("MSN::ERROR::ParserNS : Already logged in");
		}
		if (reason == "208") {
		        qDebug("MSN::ERROR::ParserNS : Invalid principal");
		}
		if (reason == "209") {
		        qDebug("MSN::ERROR::ParserNS : Nickname change illegal");
		}
		if (reason == "210") {
		        qDebug("MSN::ERROR::ParserNS : Principal list full");
		}
		if (reason == "213") {
		        qDebug("MSN::ERROR::ParserNS : Invalid rename request?");
		}
		if (reason == "215") {
			m_socket->close();	
			m_endChat = true;
		}
		if (reason == "216") {
		        qDebug("MSN::ERROR::ParserNS : Principal not on list");
		}
		if (reason == "217") {
		        qDebug("MSN::ERROR::ParserNS : Principal not online");
		}
		if (reason == "218") {
		        qDebug("MSN::ERROR::ParserNS : Already in mode");
		}
		if (reason == "219") {
		        qDebug("MSN::ERROR::ParserNS : Principal is in the opposite list");
		}
		if (reason == "223") {
		        qDebug("MSN::ERROR::ParserNS : Too many groups");
		}
		if (reason == "224") {
		        qDebug("MSN::ERROR::ParserNS : Invalid group");
		}
		if (reason == "225") {
		        qDebug("MSN::ERROR::ParserNS : Principal not in group");
		}
		if (reason == "227") {
		        qDebug("MSN::ERROR::ParserNS : Group not empty");
		}
		if (reason == "228") {
		        qDebug("MSN::ERROR::ParserNS : Group with same name already exists");
		}
		if (reason == "229") {
		        qDebug("MSN::ERROR::ParserNS : Group name too long");
		}
		if (reason == "230") {
		        qDebug("MSN::ERROR::ParserNS : Cannot remove group zero");
		}
		if (reason == "231") {
		        qDebug("MSN::ERROR::ParserNS : Invalid group");
		}
		if (reason == "240") {
		        qDebug("MSN::ERROR::ParserNS : Empty domain");
		}
		if (reason == "280") {
		        qDebug("MSN::ERROR::ParserNS : Switchboard failed");
		}
		if (reason == "281") {
		        qDebug("MSN::ERROR::ParserNS : Transfer to switchboard failed");
		}
		if (reason == "282") {
		        qDebug("MSN::ERROR::ParserNS : P2P Error?");
		}
		if (reason == "300") {
		        qDebug("MSN::ERROR::ParserNS : Required field missing");
		}
		if (reason == "302") {
		        qDebug("MSN::ERROR::ParserNS : Not logged in");
		}
		if (reason == "402") {
		        qDebug("MSN::ERROR::ParserNS : Error accessing contact list");
		}
		if (reason == "403") {
		        qDebug("MSN::ERROR::ParserNS : Error accessing contact list");
		}
		if (reason == "420") {
		        qDebug("MSN::ERROR::ParserNS : Invalid Account Permissions");
		}
		if (reason == "500") {
		        qDebug("MSN::ERROR::ParserNS : Internal server error");
		}
		if (reason == "501") {
		        qDebug("MSN::ERROR::ParserNS : Database server error");
		}
		if (reason == "502") {
		        qDebug("MSN::ERROR::ParserNS : Command disabled");
		}
		if (reason == "510") {
		        qDebug("MSN::ERROR::ParserNS : File operation failed");
		}
		if (reason == "511") {
		        qDebug("MSN::ERROR::ParserNS : User account you are attempting to connect with is banned.");
		}
		if (reason == "520") {
		        qDebug("MSN::ERROR::ParserNS : Memory allocation failed");
		}
		if (reason == "540") {
		        qDebug("MSN::ERROR::ParserNS : Challenge response failed");
		}
		if (reason == "600") {
		        qDebug("MSN::ERROR::ParserNS : Server is busy");
		}
		if (reason == "601") {
		        qDebug("MSN::ERROR::ParserNS : Server is unavailable");
		}
		if (reason == "602") {
		        qDebug("MSN::ERROR::ParserNS : Peer nameserver is down");
		}
		if (reason == "603") {
		        qDebug("MSN::ERROR::ParserNS : Database connection failed");
		}
		if (reason == "604") {
		        qDebug("MSN::ERROR::ParserNS : Server is going down");
		}
		if (reason == "605") {
		        qDebug("MSN::ERROR::ParserNS : Server unavailable");
		}
		if (reason == "700") {
		        qDebug("MSN::ERROR::ParserNS : Could not create connection");
		}
		if (reason == "710") {
		        qDebug("MSN::ERROR::ParserNS : Bad CVR parameters sent");
		}
		if (reason == "711") {
		        qDebug("MSN::ERROR::ParserNS : Write is blocking");
		}
		if (reason == "712") {
		        qDebug("MSN::ERROR::ParserNS : Session is overloaded");
		}
		if (reason == "713") {
		        qDebug("MSN::ERROR::ParserNS : Calling too rapidly");
		}
		if (reason == "714") {
		        qDebug("MSN::ERROR::ParserNS : Too many sessions");
		}
		if (reason == "715") {
		        qDebug("MSN::ERROR::ParserNS : Not expected");
		}
		if (reason == "717") {
		        qDebug("MSN::ERROR::ParserNS : Bad friend file");
		}
		if (reason == "731") {
		        qDebug("MSN::ERROR::ParserNS : Not expected");
		}
		if (reason == "800") {
		        qDebug("MSN::ERROR::ParserNS : Changing too rapidly");
		}
		if (reason == "910") {
		        qDebug("MSN::ERROR::ParserNS : Server too busy");
		}
		if (reason == "911") {
		        qDebug("MSN::ERROR::ParserNS : Server is busy");
		}
		if (reason == "912") {
		        qDebug("MSN::ERROR::ParserNS : Server too busy");
		}
		if (reason == "913") {
		        qDebug("MSN::ERROR::ParserNS : Not allowed when hidden");
		}
		if (reason == "914") {
		        qDebug("MSN::ERROR::ParserNS : Server unavailable");
		}
		if (reason == "915") {
		        qDebug("MSN::ERROR::ParserNS : Server unavailable");
		}
		if (reason == "916") {
		        qDebug("MSN::ERROR::ParserNS : Server unavailable");
		}
		if (reason == "917") {
		        qDebug("MSN::ERROR::ParserNS : Authentication failed");
		}
		if (reason == "918") {
		        qDebug("MSN::ERROR::ParserNS : Server too busy");
		}
		if (reason == "919") {
		        qDebug("MSN::ERROR::ParserNS : Server too busy");
		}
		if (reason == "920") {
		        qDebug("MSN::ERROR::ParserNS : Not accepting new principals");
		}
		if (reason == "921") {
		        qDebug("MSN::ERROR::ParserNS : Server too busy");
		}
		if (reason == "922") {
		        qDebug("MSN::ERROR::ParserNS : Server too busy");
		}
		if (reason == "923") {
		        qDebug("MSN::ERROR::ParserNS : Kids' Passport without parental consent");
		}
		if (reason == "924") {
		        qDebug("MSN::ERROR::ParserNS : Passport account not yet verified");
		}
		if (reason == "928") {
		        qDebug("MSN::ERROR::ParserNS : Bad ticket");
		}
		if (reason == "931") {
		        qDebug("MSN::ERROR::ParserNS : Account not on this server");
		}
	}
	else m_hasCommand = false;
}

void ParserSB::parse (){
	m_isParsing = true;
	QString cmd;
	while (m_buf.size() && m_hasCommand){
		qDebug("MSN::ParserSB::Log #%i::BUFFER <%s>", m_chatId,QByteArray(m_buf).replace("\\r\\n", "\\\\r\\\\n").replace("\r\n", "\\r\\n").data());
		cmd = m_buf.mid(0,3).data();
		if (cmd == "IRO"){
			qDebug ("MSN::ParserSB::Log #%i : Parsing IRO", m_chatId);
			parseIro();
		}

		else if (cmd == "ANS"){
			qDebug ("MSN::ParserSB::Log #%i : Parsing ANS", m_chatId);
			parseAns();
		}

		else if (cmd == "MSG"){
			qDebug ("MSN::ParserSB::Log #%i : Parsing MSG", m_chatId);
			parseMsg();

		}

		else if (cmd == "ACK"){
			qDebug ("MSN::ParserSB::Log #%i : Parsing ACK", m_chatId);
			parseAck();

		}

		else if (cmd == "USR"){
			qDebug ("MSN::ParserSB::Log #%i : Parsing USR", m_chatId);
			parseUsr();

		}

		else if (cmd == "CAL"){
			qDebug ("MSN::ParserSB::Log #%i : Parsing CAL", m_chatId);
			parseCal();

		}

		else if (cmd == "JOI"){
			qDebug ("MSN::ParserSB::Log #%i : Parsing JOI", m_chatId);
			parseJoi();

		}
		
		else if (cmd == "BYE"){
			qDebug ("MSN::ParserSB::Log #%i : Parsing bye", m_chatId);
			parseBye();
		}

		else if (cmd.contains(QRegExp::QRegExp("^\\d{3}"))){
			parseError();
		}
		else break;			
	}	
	m_isParsing = false;
}
}
#include "parserswichboardserver.moc"
