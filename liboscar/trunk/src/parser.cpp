/***************************************************************************
 *   Copyright (C) 2005 by Luis Cidoncha                                   *
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


#include "parser.h"
#include "flap.h"
#include "errortlv.h"
#include "passwordtlv.h"
#include "servertlv.h"
#include "snac_service.h"
#include "snac_location.h"
#include "snac_contact.h"
#include "snac_icbm.h"
#include "snac_bos.h"
#include "snac_interval.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define POSITIVE_MASK 0x7fff

namespace liboscar {

Parser::Parser(Client *c){ 
	srand(time(NULL));
	m_seq = (Word) POSITIVE_MASK * (rand()/RAND_MAX); /* Keep it positive */
	m_client = c;
	m_cap.setDefault();
}

void Parser::add(Byte *data, int len){
	int i;

	for (i=0; i < len; i++)
		m_buf << data[i];
}

void Parser::parse(){
	Byte b = 0, ch = 0;
	Word w = 0;
	Buffer buf;

	while (m_buf.len()){
		/* We have more data in the buffer */
		m_buf.gotoBegin();
	
		if (m_buf.len() < 6) /* Yet haven't enough data */
			return;
	
		m_buf >> b;
	
		if (b != 0x2a){
			qDebug(QString("Invalid FLAP header. Discarding data"));
			m_buf.wipe();
			return; 
		}
		
		m_buf >> ch;
		m_buf >> w; /* Sequence number */
		m_buf >> w; /* Data lenght */
	
		if (m_buf.len() < (w + 6))
			return;
	
		/* Copy the FLAP to a local buffer */
		buf.wipe();
		buf << m_buf;
	
		buf.setLength(w+6);
		buf.setPosition(6);
		buf.removeFromBegin();
		buf.gotoBegin();
	
		/* remove the FLAP from the parser buffer */
		m_buf.gotoBegin();
		m_buf.remove(w + 6);
	
		switch (ch){
			case 0x01:
				parseCh1(buf);
				break;
			case 0x02:
				parseCh2(buf);
				break;
			case 0x04:
				parseCh4(buf);
				break;
			case 0x05:
				parseCh5(buf);
				break;
			default:
				qDebug(QString("FLAP on unknown channel: %1").arg(ch));
				break;
		}
	}
}

void Parser::parseCh1(Buffer& buf){
	FLAP f(0x01, getNextSeqNumber(), 0); // Unknown length
	DWord dw = 0;
	UnformattedTLV *tlv;
	PasswordTLV* pt;

	buf >> dw;

	if (dw != 0x00000001){
		qDebug("Unknown header on channel 1");
		return ;
	}

	f.data() << (DWord) 0x00000001; // Hello header

	if (m_client->state() == CLI_CONNECTED){ // Send cookie
		tlv = new UnformattedTLV(TLV_TYPE_COOKIE);
		m_cookie.gotoBegin();
		tlv->data() << m_cookie;
		f.addTLV(tlv);
	}

	if ((m_client->state() == CLI_AUTHING) || (m_client->state() == CLI_CONNECTED)){

		// UIN
		tlv = new UnformattedTLV(TLV_TYPE_UIN);
		tlv->data() << m_client->getUIN().getUin();
		f.addTLV(tlv);

		// Password
		pt = new PasswordTLV();
		pt->setPassword(m_client->m_password);
		f.addTLV(pt);
		
		// Version
		tlv = new UnformattedTLV(TLV_TYPE_VERSION);
		tlv->data() << TLV_VERSION_ICQ2003B;
		f.addTLV(tlv); 

		// ClientId
		tlv = new UnformattedTLV(TLV_TYPE_CLIENTID);
		tlv->data() << (Word) 0x010a;
		f.addTLV(tlv); 

		// Versionmajor
		tlv = new UnformattedTLV(TLV_TYPE_VERMAJOR);
		tlv->data() << TLV_VERMAJOR;
		f.addTLV(tlv); 

		// Versionminor
		tlv = new UnformattedTLV(TLV_TYPE_VERMINOR);
		tlv->data() << TLV_VERMINOR_ICQ2003B;
		f.addTLV(tlv); 

		// Lesser
		tlv = new UnformattedTLV(TLV_TYPE_LESSER);
		tlv->data() << TLV_LESSER;
		f.addTLV(tlv); 

		// Build
		tlv = new UnformattedTLV(TLV_TYPE_BUILD);
		tlv->data() << TLV_BUILD_ICQ2003B;
		f.addTLV(tlv); 

		// Distrib
		tlv = new UnformattedTLV(TLV_TYPE_DISTRIBUTION);
		tlv->data() << TLV_DISTRIBUTION;
		f.addTLV(tlv); 

		// Language
		tlv = new UnformattedTLV(TLV_TYPE_LANGUAGE);
		tlv->data() << (Word) 0x656e; // en
		f.addTLV(tlv); 

		// Country
		tlv = new UnformattedTLV(TLV_TYPE_COUNTRY);
		tlv->data() << (Word) 0x7573; // us
		f.addTLV(tlv); 

	}

	m_client->send(f.pack());
}

void Parser::parseCh2(Buffer& buf){
	Word family;

	buf >> family;

	switch (family) {
		case SNAC_FAM_SERVICE:
			parseCh2Service(buf);
			break;
		case SNAC_FAM_LOCATION:
			parseCh2Location(buf);
			break;
		case SNAC_FAM_CONTACT:
			parseCh2Contact(buf);
			break;
		case SNAC_FAM_ICBM:
			parseCh2ICBM(buf);
			break;
		case SNAC_FAM_BOS:
			parseCh2BOS(buf);
			break;
		case SNAC_FAM_INTERVAL:
			parseCh2Interval(buf);
			break;
		case SNAC_FAM_ROSTER:
			break;
		case SNAC_FAM_OLDICQ:
			break;
		case SNAC_FAM_NEWUSER:
			break;
		default:
			qDebug(QString("SNAC from an unknown family %1").arg(family));
			break;
	}
}

void Parser::parseCh4(Buffer& buf){

	Word id, l, err;
	Byte b;
	unsigned int i;
	QString reason;

	ErrorTLV errt;
	ServerTLV servt;

	while (buf.len()){
		buf >> id;
		buf >> l;
		switch (id){
			case 0x0001:
				// Ignore UIN
				buf.advance(l);
				buf.removeFromBegin();
				break;
			case 0x0004:
				for (i=0; i < l; i++){
					buf >> b;
					reason.append(b);
				}
				buf.removeFromBegin();
				break;
			case 0x0005:
				servt.parse(buf);
				buf.removeFromBegin();
				break;
			case 0x0006:
				for (i=0; i < l; i++){
					buf >> b;
					m_cookie << b;
				}
				buf.removeFromBegin();
				break;
			case 0x0008:
				errt.parse(buf);
				buf.removeFromBegin();
				break;
			default:
				qDebug("Unknown TLV on channel 4");
				buf.wipe();
				break;
		}
	}

	if (servt.getServer().isEmpty()){ // Got an unexpected disconnection
		emit serverDisconnected(reason, errt.getError());
	}
	else // We got the BOS && cookie info :-)
		emit receivedBOS(servt.getServer(), servt.getPort());
}

void Parser::parseCh5(Buffer& buf){
	if (buf.len() != 0)
		qDebug("Unknown extra data on channel 5");
	sendKeepAlive();
}

void Parser::parseCh2Service(Buffer& buf) {
	Word command, flags;
	DWord reference;

	SrvServiceErrSNAC sse;
	SrvFamiliesSNAC sf(&m_fam);
	SrvRedirectSNAC srd;
	SrvRatesSNAC sra;
	SrvRateExceededSNAC sre;
	SrvServerPauseSNAC ssp;
	SrvReplyInfoSNAC sri;
	SrvMigrationReqSNAC smr;
	SrvMOTDSNAC smotd;
	SrvFamilies2SNAC sf2(&m_fam);

	buf >> command;
	buf >> flags;
	buf >> reference;

	buf.removeFromBegin();

	switch (command) {
		case SERVICE_SRV_SERVICE_ERR:
			sse.parse(buf);
			qDebug(QString("Error on channel 2 family 1: %1").arg(sse.getError()));
			break;
		case SERVICE_SRV_FAMILIES:
			sf.parse(buf); // Got families supported by server
			break;
		case SERVICE_SRV_REDIRECT:
			srd.parse(buf);
			break;
		case SERVICE_SRV_RATES:
			sra.parse(buf);
			break;
		case SERVICE_SRV_RATEEXCEEDED:
			sre.parse(buf);
			break;
		case SERVICE_SRV_SERVERPAUSE:
			ssp.parse(buf);
			break;
		case SERVICE_SRV_REPLYINFO:
			sri.parse(buf);
			break;
		case SERVICE_SRV_MIGRATIONREQ:
			smr.parse(buf);
			emit receivedBOS(smr.getServer(), smr.getPort());
			break;
		case SERVICE_SRV_MOTD:
			smotd.parse(buf);
			break;
		case SERVICE_SRV_FAMILIES2:
			sf2.parse(buf); // Update families versions
			break;
		default:
			qDebug("Unknown command on SNAC Service family");
			break;
	}
	// React to commands
	if (command == SERVICE_SRV_FAMILIES) {
		// Got family list: request versions numbers
		FLAP f(0x02, getNextSeqNumber(), 0);
		CliFamiliesSNAC *clis = new CliFamiliesSNAC; // TODO: make this get Families info
		f.addSNAC(clis);
		m_client->send(f.pack());
	}
	else if (command == SERVICE_SRV_MOTD){
		// Request rates
		FLAP f(0x02, getNextSeqNumber(), 0);
		CliRatesRequestSNAC *clir = new CliRatesRequestSNAC;
		f.addSNAC(clir);
		m_client->send(f.pack());
	}
	else if (command == SERVICE_SRV_RATES){
		// Ack rates
		FLAP f(0x02, getNextSeqNumber(), 0);
		CliAckRatesSNAC *clia = new CliAckRatesSNAC;
		f.addSNAC(clia);
		m_client->send(f.pack());
	}

}

void Parser::parseCh2Location(Buffer& buf) {

	Word command, flags;
	DWord reference;

	SrvLocationErrSNAC sle;
	SrvUserInfoSNAC suis;
	SrvReplyLocationSNAC srls;

	buf >> command;
	buf >> flags;
	buf >> reference;

	buf.removeFromBegin();

	switch (command) {
		case LOCATION_SRV_LOCATION_ERR:
			sle.parse(buf);
			qDebug(QString("Error on channel 2 family 2: %1").arg(sle.getError()));
			break;
		case LOCATION_SRV_REPLYLOCATION:
			srls.parse(buf);
			m_cap.setMaxCap(srls.getMaxCap());
			break;
		case LOCATION_SRV_USERINFO:
			suis.parse(buf);
			// TODO: handle user info :-)
			break;
		default:
			qDebug("Unknown command on SNAC Location family");
			break;
	}
}

void Parser::parseCh2Contact(Buffer& buf) {

	Word command, flags;
	DWord reference;

	SrvContactErrSNAC cse;
	SrvReplyBuddySNAC rbs;
	SrvRefusedSNAC srs;
	SrvUserOnlineSNAC uos;
	SrvUserOfflineSNAC uofs;

	buf >> command;
	buf >> flags;
	buf >> reference;

	buf.removeFromBegin();

	switch (command) {
		case CONTACT_SRV_CONTACT_ERR:
			cse.parse(buf);
			qDebug(QString("Error on channel 2 family 3: %1").arg(cse.getError()));
			break;
		case CONTACT_SRV_REPLYBUDDY:
			rbs.parse(buf);
			// XXX Ignoring limitations
			break;
		case CONTACT_SRV_REFUSED:
			// TODO: report refused event
			srs.parse(buf);
			break;
		case CONTACT_SRV_USERONLINE:
			// TODO: report status event
			uos.parse(buf);
			break;
		case CONTACT_SRV_USEROFFLINE:
			// TODO: report offline event
			uofs.parse(buf);
			break;
		default:
			qDebug("Unknown command on SNAC Contact family");
			break;
	}
}

void Parser::parseCh2ICBM(Buffer& buf) {

	Word command, flags;
	DWord reference;

	SrvICBMErrSNAC ise;
	SrvReplyICBMSNAC irs;
	SrvRecvMsg irm;
	SrvMissedICBMSNAC imi;
	SrvAckMsgSNAC iam;

	buf >> command;
	buf >> flags;
	buf >> reference;

	buf.removeFromBegin();

	switch (command) {
		case ICBM_SRV_ICBM_ERR:
			ise.parse(buf);
			qDebug(QString("Error on channel 2 family 4: %1").arg(ise.getError()));
			break;
		case ICBM_SRV_REPLYICBM:
			irs.parse(buf);
			// TODO: report
			break;
		case ICBM_SRV_RECVMSG:
			irm.parse(buf);
			// TODO: report
			break;
		case ICBM_SRV_MISSEDICBM:
			imi.parse(buf);
			// TODO: report
			break;
		case ICBM_SRV_SRVACKMSG:
			iam.parse(buf);
			break;
		default:
			qDebug("Unknown command on SNAC ICBM family");
			break;
	}
}

void Parser::parseCh2BOS(Buffer& buf) {

	Word command, flags;
	DWord reference;

	SrvReplyBOSSNAC brb;

	buf >> command;
	buf >> flags;
	buf >> reference;

	buf.removeFromBegin();

	switch (command) {
		case BOS_SRV_REPLYBOS:
			brb.parse(buf);
			break;
		default:
			qDebug("Unknown command on SNAC BOS family");
			break;
	}
}

void Parser::parseCh2Interval(Buffer& buf) {

	Word command, flags;
	DWord reference;

	SrvSetIntervalSNAC isi;

	buf >> command;
	buf >> flags;
	buf >> reference;

	buf.removeFromBegin();

	switch (command) {
		case INTERVAL_SRV_SETINTERVAL:
			isi.parse(buf);
			// TODO: reply the SNAC
			break;
		default:
			qDebug("Unknown command on SNAC Interval family");
			break;
	}
}

void Parser::sendKeepAlive(){
	FLAP f(0x05, getNextSeqNumber(), 0);
	m_client->send(f.pack());
}

Word Parser::getNextSeqNumber(){
	m_seq = ++m_seq & POSITIVE_MASK;
	return m_seq;
}

Parser::~Parser() { }


}

#include "parser.moc"
