/***************************************************************************
 *   Copyright (C) 2008 by David Menéndez González                         *
 *   narayan82@gmail.com                                                   *
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
#include <QtDebug>
#include <QFile>
#include <QDir>

#include "parserkopete.h"

/* Public methods */

ParserKopete::ParserKopete() : IMProgram() {
    m_listGroups.insert(0,"NullGroup");
}

ParserKopete::ParserKopete(QString name, QString version) : IMProgram(name, version) { 
    m_listGroups.insert(0,"NullGroup");
}
            
void ParserKopete::saveLogs(){
        // save logs in kinapsis format
        return;
}

int ParserKopete::parserLogsAux(QString prot){
        QString dir;
        QString tmp;
        QDir directory;
        QString account;
        QString protocol;
        QString home = QDir::homePath();
        QFile file;
        QDomDocument doc;

        dir.append(home);
        dir.append("/.kde/share/apps/kopete/logs");
        directory.setCurrent(dir);
        tmp.append(dir);
        if (prot == "msn"){
                if (directory.exists("MSNProtocol")){
                        tmp.append("/MSNProtocol/");
                        protocol.append("MSN");
                }
        } else if (prot == "jabber"){
                if (directory.exists("JabberProtocol")){
                        tmp.append("/JabberProtocol/");
                        protocol.append("Jabber");
                }
        } else if (prot == "icq"){
                if (directory.exists("ICQProtocol")){
                        tmp.append("/ICQProtocol/");
                        protocol.append("ICQ");
                }
        } else if (prot == "aim"){
                if (directory.exists("AIMProtocol")){
                        tmp.append("/AIMProtocol/");
                        protocol.append("AIM");
                }
        } else {
                return -1;
        }
        QListIterator<QString> i(m_accounts);
        while (i.hasNext()){
                QString acc;
                QString aux;
                aux.append(i.peekNext());
                account.clear();
                account.append(aux);
                acc.append(i.next());
                acc.replace(QString("."), QString("-"));
                tmp.append(acc);
                directory.setPath(tmp);
                if (directory.exists(tmp)){
                        QStringList files;
                        files = directory.entryList();
                        //neither . nor .. FIXME
                        for (int j=2; j<files.size(); j++){
                                QString f = files.at(j);
                                QString date = f.section('.',-2,-2);
                                QDir::setCurrent(tmp);
                                file.setFileName(files.at(j));
                                file.open(QIODevice::ReadOnly | QIODevice::Text);
                                doc.setContent(&file);
                                QDomElement logElem = doc.documentElement();
                                QDomNode n = logElem.firstChild();
                                while(!n.isNull()) {
                                        QDomElement e = n.toElement();
                                        if(!e.isNull()) {
                                                processLogs(e,protocol,account,date);
                                        }
                                        n = n.nextSibling();
                                } 
                                file.close();
                        }
                }
                tmp.clear();
                tmp.append(dir);
                if (prot == "msn"){
                        tmp.append("/MSNrotocol/");
                } else if (prot == "jabber"){
                        tmp.append("/JabberProtocol/");
                } else if (prot == "icq"){
                        tmp.append("/ICQProtocol/");
                } else if (prot == "aim"){
                        tmp.append("/AIMProtocol/");
                } else {
                        return -2;
                }
        }
        return 0;
}

void ParserKopete::parser(){
        // fill attributes: metacontacts, accounts, protocols & logs
        QFile file;
        QDomDocument doc;
        QString home = QDir::homePath();
        QString dir;
        int result;

        //XML for metacontacts
        dir.append(home);
        dir.append("/.kde/share/apps/kopete");
        QDir::setCurrent(dir);
        file.setFileName("contactlist.xml");
        if (!file.open(QIODevice::ReadOnly)){
                return;
        }
        if (!doc.setContent(&file)) {
                file.close();
                return;
        }
        QDomElement docElem = doc.documentElement();
        QDomNode n = docElem.firstChild();
        while(!n.isNull()) {
                QDomElement e = n.toElement();
                if(!e.isNull()) {
                        processMetacontacts(e);
                }
                n = n.nextSibling();
        } 
        file.close();

        //kderc format for accounts and protocols
        dir.clear();
        dir.append(home);
        dir.append("/.kde/share/config");
        QDir::setCurrent(dir);
        file.setFileName("kopeterc");
        if (file.exists()){
                file.open(QIODevice::ReadOnly | QIODevice::Text);
                QTextStream in(&file);
                while (!in.atEnd()) {
                        QString line = in.readLine();
                        processProtocolsAccounts(line);
                }
                file.close();
        }

        //MSN
        result = parserLogsAux("msn");
        //Jabber
        result = parserLogsAux("jabber");
        //AIM
        result = parserLogsAux("aim");
        //ICQ
        result = parserLogsAux("icq");
}


/* Private methods */

void ParserKopete::processMetacontacts(const QDomElement& e){
        QDomAttr a;
        QString tmp;
        bool ok;
        QDomElement element = e;

        //Former elements: kopete-group
        a = element.attributeNode("groupId");
        if (!a.value().isEmpty()){
                //Element inside kopete-group
                QDomNodeList display = element.elementsByTagName("display-name");
                if (!display.isEmpty()){
                        QDomElement elem = display.item(0).toElement();
                        m_listGroups.insert(a.value().toInt(&ok, 10), elem.text());
                }
                return;
        }

        //Latter elements: meta-contact
        a = element.attributeNode("contactId");
        if (!a.value().isEmpty()){
                tmp.append("\033[01;31m");
                tmp.append("\nContactId: ");
                tmp.append("\033[00m");
                tmp.append(a.value());

                //Element inside meta-contact
                QDomNodeList display = element.elementsByTagName("display-name");
                if (!display.isEmpty()){
                        QDomElement elem = display.item(0).toElement();
                        tmp.append("\033[01;34m");
                        tmp.append(" GlobalDisplayName: ");
                        tmp.append("\033[00m");
                        tmp.append(elem.text());
                }


                //Element inside meta-contact
                QDomNodeList group = element.elementsByTagName("groups");
                if (!group.isEmpty()){
                        QDomElement g = group.item(0).toElement();
                        tmp.append("\033[01;30m");
                        tmp.append(" Group: ");
                        tmp.append("\033[00m");
                        QDomNodeList gro = g.elementsByTagName("group");
                        if (!gro.isEmpty()){
                                QDomElement elGr = gro.item(0).toElement();
                                int id=(elGr.attributeNode("id").value().toInt());
                                tmp.append(m_listGroups.at(id));
                        }
                }

                //Element inside meta-contact
                QDomNodeList nodeList = element.elementsByTagName("plugin-data");
                for(int i=0; i < nodeList.length(); i++){
                        QDomElement el = nodeList.item(i).toElement();
                        tmp.append("\033[01;35m");
                        tmp.append(" Meta: ");
                        tmp.append("\033[00m");
                        tmp.append(el.attributeNode("plugin-id").value());

                        //Element inside meta-contact->plugin-data
                        QDomNodeList pluginDataFields = el.elementsByTagName("plugin-data-field");
                        for(int j=0; j < pluginDataFields.length(); j++){
                                QDomElement key = pluginDataFields.item(j).toElement();
                                if (key.attributeNode("key").value().compare("accountId")==0){
                                        tmp.append(" ");
                                        tmp.append(key.text());
                                }
                                if (key.attributeNode("key").value().compare("contacttId")==0){
                                        tmp.append(" ");
                                        tmp.append(key.text());
                                }
                                if (key.attributeNode("key").value().compare("displayName")==0){
                                        tmp.append(" ");
                                        tmp.append(key.text());
                                }
                        }
                        tmp.append(el.attribute("key"));
                }
                m_metacontacts << tmp;
        }
}

void ParserKopete::processProtocolsAccounts(const QString& line){
        int pos=-1;
        QRegExp rx;
        QString tmp;

        if (line.size() > 0){
                //account
                rx.setPattern("(^AccountId=)(([a-z|A-Z|0-9]+@[a-z|A-Z|0-9]+.[a-z|A-Z]+)|([a-z|A-Z|0-9]+))");
                pos = rx.indexIn(line,0);
                if (pos > -1){
                        m_accounts << rx.cap(2);
                        tmp.clear();
                        tmp.append("\033[01;31m");
                        tmp.append("\nAccount: ");
                        tmp.append("\033[00m");
                        tmp.append(rx.cap(2));
                        m_protocols << tmp;
                        return;
                }
                //Plain text
                rx.setPattern("(^AllowPlainTextPassword=)([a-z|A-Z]+)");  
                pos = rx.indexIn(line,0);
                if (pos > -1){
                        tmp.clear();
                        tmp.append("PlainTextPassword: ");
                        tmp.append(rx.cap(2));
                        m_protocols << tmp;
                        return;
                }
                //port
                rx.setPattern("(^Port=)([0-9]+)");  
                pos = rx.indexIn(line,0);
                if (pos > -1){
                        tmp.clear();
                        tmp.append("Port: ");
                        tmp.append(rx.cap(2));
                        m_protocols << tmp;
                        return;
                }
                //protocol
                rx.setPattern("(^Protocol=)([a-z|A-Z]+)");  
                pos = rx.indexIn(line,0);
                if (pos > -1){
                        tmp.clear();
                        tmp.append("Protocol: ");
                        tmp.append(rx.cap(2));
                        m_protocols << tmp;
                        return;
                }
                //server
                rx.setPattern("(^Server=)([a-z|A-Z|0-9|.]+)");  
                pos = rx.indexIn(line,0);
                if (pos > -1){
                        tmp.clear();
                        tmp.append("Server: ");
                        tmp.append(rx.cap(2));
                        m_protocols << tmp;
                        return;
                }
                //ssl-tls
                rx.setPattern("(^UseSSL=)([a-z|A-Z]+)");  
                pos = rx.indexIn(line,0);
                if (pos > -1){
                        tmp.clear();
                        tmp.append("SSL: ");
                        tmp.append(rx.cap(2));
                        m_protocols << tmp;
                        return;
                }
                //MFN  FIXME: cheack weird characters
                rx.setPattern("(^MFN=)([a-z|A-Z|0-9]+)");  
                pos = rx.indexIn(line,0);
                if (pos > -1){
                        tmp.clear();
                        tmp.append("MFN: ");
                        tmp.append(rx.cap(2));
                        m_protocols << tmp;
                        return;
                }
                //serverName
                rx.setPattern("(^serverName=)([a-z|A-Z|0-9|.]+)");  
                pos = rx.indexIn(line,0);
                if (pos > -1){
                        tmp.clear();
                        tmp.append("Server: ");
                        tmp.append(rx.cap(2));
                        m_protocols << tmp;
                        return;
                }
                //serverPort
                rx.setPattern("(^serverPort=)([0-9]+)");  
                pos = rx.indexIn(line,0);
                if (pos > -1){
                        tmp.clear();
                        tmp.append("Port: ");
                        tmp.append(rx.cap(2));
                        m_protocols << tmp;
                        return;
                }

        }
}

void ParserKopete::processLogs(const QDomElement& e, const QString& protocol, const QString& account, const QString& date){
        QString tmp;
        QString from;
        QString msg;
        QString time;
        QString prot;
        QString acco;
        QString d;

        if(e.tagName().compare("head")==0)
                return;//omit element head

        prot.append(protocol);
        acco.append(account);
        d.append(date);
//        from.append(e.attribute("nick"));
//        from.append(" - ");
        from.append(e.attribute("from"));
        msg.append(e.text().toUtf8()); //FIXME: check weird characters
        time.append(e.attribute("time"));

        tmp.append("\033[01;31m");
        tmp.append("\nPROTOCOL: ");
        tmp.append("\033[00m");
        tmp.append(protocol);
        tmp.append("\033[01;34m");
        tmp.append(" ACCOUNT: ");
        tmp.append("\033[00m");
        tmp.append(account);
        tmp.append("\033[01;29m");
        tmp.append(" FROM: ");
        tmp.append("\033[00m");
        tmp.append(from);
        tmp.append("\033[01;35m");
        tmp.append(" DATE: ");
        tmp.append("\033[00m");
        tmp.append(d);
        tmp.append("\033[01;33m");
        tmp.append(" TIME: ");
        tmp.append("\033[00m");
        tmp.append(time);
        tmp.append("\033[01;30m");
        tmp.append(" MSG: ");
        tmp.append("\033[00m");
        tmp.append(msg);
        m_logs << tmp;
}
