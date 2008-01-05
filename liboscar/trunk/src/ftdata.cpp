/***************************************************************************
 *   Copyright (C) 2006-2008 by Luis Cidoncha                              *
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

#include "ftdata.h"

namespace liboscar {

FTData::FTData() {
	m_msg = "";
}

FTData::~FTData() { }

void FTData::setMessage(QString m){
	m_msg = m;
}

QString FTData::getMessage(){
	return m_msg;
}

void FTData::setLocale(QString l){
	m_loc = l;
}

QString FTData::getLocale(){
	return m_loc;
}

void FTData::setEncoding(QString e){
	m_enc = e;
}

QString FTData::getEncoding(){
	return m_enc;
}

void FTData::setFileEncoding(QString f){
	m_fileenc = f;
}

QString FTData::getFileEncoding(){
	return m_fileenc;
}

void FTData::setProxyUsed(bool u){
	m_proxyused = u;
}

bool FTData::getProxyUsed(){
	return m_proxyused;
}

void FTData::setProxyIP(DWord i){ 
	m_proxyip = QString::number(i);
}

void FTData::setProxyIP(QString i){ 
	m_proxyip = i;
}


QString FTData::getProxyIP() {
	return m_proxyip;
}

void FTData::setClientIP(DWord i) {
	m_clientip = QString::number(i);
}

void FTData::setClientIP(QString i) {
	m_clientip = i;
}

QString FTData::getClientIP() {
	return m_clientip;
}

void FTData::setExternalIP(DWord i) {
	m_externalip = QString::number(i);
}

void FTData::setExternalIP(QString i) {
	m_externalip = i;
}

QString FTData::getExternalIP() {
	return m_externalip;
}

void FTData::setListeningPort(QString p) {
	m_listenport = p.toUInt();
}

void FTData::setListeningPort(int p) {
	m_listenport = p;
}

unsigned int FTData::getListeningPort() {
	return m_listenport;
}

void FTData::setMultiple(bool m){
	m_mul = m;
}

bool FTData::getMultiple(){
	return m_mul;
}

void FTData::setFileCount(Word c) {
	m_count = c;
}

Word FTData::getFileCount() {
	return m_count;
}

void FTData::setFileSize(DWord s) {
	m_size = s;
}

DWord FTData::getFileSize() {
	return m_size;
}

void FTData::setFileName(QString n) {
	m_filename = n;
}

QString FTData::getFileName() {
	return m_filename;
}

}
