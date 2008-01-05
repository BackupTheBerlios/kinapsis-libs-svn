/***************************************************************************
 *   Copyright (C) 2005-2008 by Luis Cidoncha                              *
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
#include "servertlv.h"
#include "snac_service.h"
#include "snac_location.h"
#include "snac_contact.h"
#include "snac_icbm.h"
#include "snac_bos.h"
#include "snac_interval.h"
#include "snac_roster.h"
#include "snac_newuser.h"
#include "snac_icq.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define POSITIVE_MASK 0x7fff

namespace liboscar {

OscarParser::OscarParser() : Parser() { 
	srand(time(NULL));
}

void OscarParser::parse(){
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
			qDebug("Invalid FLAP header. Discarding data");
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
				qDebug("FLAP on unknown channel: %d", ch);
				break;
		}
	}
}

void OscarParser::parseCh1(Buffer& buf){
	DWord dw = 0;

	buf >> dw;

	if (dw != 0x00000001){
		qDebug("Unknown header on channel 1: %d", dw);
		return ;
	}

	qDebug("emit hello");
	emit recvHello();

}

void OscarParser::parseCh2(Buffer& buf){
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
			parseCh2Roster(buf);
			break;
		case SNAC_FAM_ICQ:
			parseCh2ICQ(buf);
			break;
		case SNAC_FAM_NEWUSER:
			parseCh2NewUser(buf);
			break;
		default:
			qDebug("SNAC from an unknown family %d", family);
			break;
	}
}

void OscarParser::parseCh4(Buffer& buf){

	Word id, l, err, w;
	Byte b;
	unsigned int i;
	QString reason;

	ErrorTLV errt;
	ServerTLV servt;
	QByteArray cookie;

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
			case 0x000b:
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
					cookie.append((char)b);
				}
				buf.removeFromBegin();
				break;
			case 0x0008:
			case 0x0009:
				errt.parse(buf);
				buf.removeFromBegin();
				break;
			case 0x0013:
				buf >> w; // XXX: Registration status
				buf.removeFromBegin();
				break;
			case 0x0054:
				buf.advance(l); // XXX: Change password URL
				buf.removeFromBegin();
				break;
			default:
				qDebug("Unknown TLV on channel 4");
				// ignore unknown data
				buf.advance(l);
				buf.removeFromBegin();
				break;
		}
	}

	if (servt.getServer().isEmpty()) // Got an unexpected disconnection
		emit serverDisconnected(reason, errt.getError());
	else // We got the BOS && cookie info :-)
		emit receivedBOS(servt.getServer(), servt.getPort(), cookie);
}

void OscarParser::parseCh5(Buffer& buf){
	if (buf.len() != 0)
		qDebug("Unknown extra data on channel 5");
	sendKeepAlive();
}

void OscarParser::parseCh2Service(Buffer& buf) {
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
	SrvVersionsSNAC svs(&m_fam);

	buf >> command;
	buf >> flags;
	buf >> reference;

	if (flags == 0x8000){ // unknown extra data TODO: parse it in SNAC
		Word len;
		Byte by;

		buf >> len;
		while (len--) buf >> by;
	}

	buf.removeFromBegin();

	switch (command) {
		case SERVICE_SRV_SERVICE_ERR:
			sse.parse(buf);
			qDebug("Error on channel 2 family 1: %d", sse.getError());
			break;
		case SERVICE_SRV_FAMILIES:
			sf.parse(buf); // Got families supported by server
			emit serverFamilies(sf);
			break;
		case SERVICE_SRV_REDIRECT:
			srd.parse(buf);
			break;
		case SERVICE_SRV_RATES:
			sra.parse(buf);
			emit serverRateLimits(sra);
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
			emit receivedBOS(smr.getServer(), smr.getPort(), QByteArray());
			break;
		case SERVICE_SRV_MOTD:
			smotd.parse(buf);
			break;
		case SERVICE_SRV_VERSIONS:
			svs.parse(buf); // Update families versions
			emit serverServicesVersion(svs);
			break;
		default:
			qDebug("Unknown command on SNAC Service family: %02x", command);
			break;
	}
}

void OscarParser::parseCh2Location(Buffer& buf) {

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
			qDebug("Error on channel 2 family 2: %d", sle.getError());
			break;
		case LOCATION_SRV_REPLYLOCATION:
			srls.parse(buf);
			emit locationLimits(srls);
			break;
		case LOCATION_SRV_USERINFO:
			suis.parse(buf);
			emit serverUserInfo(suis);
			break;
		default:
			qDebug("Unknown command on SNAC Location family");
			break;
	}
}

void OscarParser::parseCh2Contact(Buffer& buf) {

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
			qDebug("Error on channel 2 family 3: %d", cse.getError());
			break;
		case CONTACT_SRV_REPLYBUDDY:
			rbs.parse(buf);
			emit BLMLimits(rbs);
			break;
		case CONTACT_SRV_REFUSED:
			// TODO: report refused event
			srs.parse(buf);
			break;
		case CONTACT_SRV_USERONLINE:
			uos.parse(buf);
			emit statusChanged(uos.getUin(), uos.getStatus());
			break;
		case CONTACT_SRV_USEROFFLINE:
			uofs.parse(buf);
			emit statusChanged(uofs.getUin(), STATUS_OFFLINE);
			break;
		default:
			qDebug("Unknown command on SNAC Contact family");
			break;
	}
}

void OscarParser::parseCh2ICBM(Buffer& buf) {

	Word command, flags;
	DWord reference;

	SrvICBMErrSNAC ise;
	SrvReplyICBMSNAC irs;
	SrvRecvMsg irm;
	SrvMissedICBMSNAC imi;
	SrvAckMsgSNAC iam;
	CliAckMsgSNAC cam;
	SrvCliTypingSNAC scts;

	buf >> command;
	buf >> flags;
	buf >> reference;

	buf.removeFromBegin();

	switch (command) {
		case ICBM_SRV_ICBM_ERR:
			ise.parse(buf);
			qDebug("Error on channel 2 family 4: %d", ise.getError());
			break;
		case ICBM_SRV_REPLYICBM:
			irs.parse(buf);
			emit ICBMLimits(irs);
			break;
		case ICBM_SRV_RECVMSG:
			irm.parse(buf);
			emit newMessage(irm.getMessage());
			break;
		case ICBM_SRV_MISSEDICBM:
			imi.parse(buf);
			// TODO: report
			break;
		case ICBM_SRV_SRVACKMSG:
			iam.parse(buf);
			break;
		case ICBM_CLI_ACKMSG:
			cam.parse(buf);
			// TODO: more checks
			emit awayMessageArrived(cam.getMessage().getUin(), cam.getMessage().getText());
			break;
		case ICBM_SRV_CLI_TYPING:
			scts.parse(buf);
			emit typingEventArrived(scts.getUin(), scts.getType());
			break;
		default:
			qDebug("Unknown command on SNAC ICBM family");
			break;
	}
}

void OscarParser::parseCh2BOS(Buffer& buf) {

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
			PRMLimits(brb);
			break;
		default:
			qDebug("Unknown command on SNAC BOS family");
			break;
	}
}

void OscarParser::parseCh2Interval(Buffer& buf) {

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

void OscarParser::parseCh2Roster(Buffer& buf) {

	Word command, flags;
	DWord reference;

	SrvReplyListsSNAC srl;
	SrvReplyRosterSNAC srr;
	SrvUpdateAckSNAC sua;
	SrvReplyRosterOkSNAC srro;
	SrvAuthReqSNAC sar;
	SrvAuthReplySNAC sare;
	SrvAddedYouSNAC say;

	buf >> command;
	buf >> flags;
	buf >> reference;

	if (flags == 0x8000){ // unknown extra data TODO: parse it in SNAC
		Word len;
		Byte by;

		buf >> len;
		while (len--) buf >> by;
	}

	buf.removeFromBegin();

	switch (command) {
		// TODO: handle things && report
		case ROSTER_SRV_REPLYLISTS:
			srl.parse(buf);
			emit SSILimits(srl);
			break;
		case ROSTER_SRV_REPLYROSTER:
			emit rosterArrived(buf);
			break;
		case ROSTER_SRV_UPDATEACK:
			// Response from the server to one of our actions.
			sua.parse(buf);
			emit rosterServerAck(sua.getAck());
			break;
		case ROSTER_SRV_REPLYROSTEROK:
			// FIXME: well, this should never arrive, but do it well
			srro.parse(buf);
			emit rosterArrived(buf);
			break;
		case ROSTER_SRV_AUTHREQ:
			// Someone is asking for auth to add us to his/her roster
			sar.parse(buf);
			emit authReq(sar.getUin(), sar.getReason());
			break;
		case ROSTER_SRV_AUTHREPLY:
			sare.parse(buf);
			break;
		case ROSTER_SRV_ADDEDYOU:
			say.parse(buf);
			emit addedYou(say.getUin());
			break;
		default:
			qDebug("Unknown command on SNAC Roster family");
			break;
	}
}

void OscarParser::parseCh2ICQ(Buffer& buf) {

	Word command, flags;
	DWord reference;

	SrvICQErrSNAC ies;
	SrvMetaReplySNAC mrs;

	buf >> command;
	buf >> flags;
	buf >> reference;

	buf.removeFromBegin();

	switch (command) {
		case ICQ_SRV_ICQ_ERR:
			ies.parse(buf);
			qDebug("Error on channel 2 family 15: %d", ies.getError());
			break;
		case ICQ_SRV_METAREPLY:
			mrs.parse(buf);
			emit serverMetaReply(mrs);
			break;
		default:
			qDebug("Unknown command on SNAC ICQ family");
			break;
	}
}

void OscarParser::parseCh2NewUser(Buffer& buf) {

	Word command, flags;
	DWord reference;

	SrvRegRefusedSNAC srrs;
	SrvReplyLoginSNAC srls;
	SrvReplyUINSNAC srus;
	SrvReplyMD5SNAC srms;
	SrvReqSecureIdSNAC srsis;

	buf >> command;
	buf >> flags;
	buf >> reference;

	buf.removeFromBegin();

	switch (command) {
		case NEWUSER_SRV_REGREFUSED:
			srrs.parse(buf);
			break;
		case NEWUSER_SRV_REPLYLOGIN:
			srls.parse(buf);
			break;
		case NEWUSER_SRV_REPLYUIN:
			srus.parse(buf);
			emit newUin(srus.getUin());
			break;
		case NEWUSER_SRV_REPLYMD5:
			srms.parse(buf);
			break;
		case NEWUSER_SRV_REQSECUREID:
			srsis.parse(buf);
			break;
		default:
			qDebug("Unknown command on SNAC NewUser family");
			break;
	}
}

void OscarParser::sendKeepAlive(){
	// TODO: 
	FLAP f(0x05, Connection::getNextSeqNumber(), 0);
	//m_client->send(f.pack());
}

OscarParser::~OscarParser() { }


}

#include "parser.moc"
