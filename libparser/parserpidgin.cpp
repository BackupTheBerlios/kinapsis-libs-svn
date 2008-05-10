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

#include "parserpidgin.h"

/* Public methods */

void ParserPidgin::saveLogs(){
        // save logs in kinapsis format
        return;
}

int ParserPidgin::parserLogsAux(QString prot){
        QString dir;
        QString tmp;
        QDir directory;
        QString account;
        QString protocol;
        QString home = QDir::homePath();
        QFile file;
        QDomDocument doc;

//TODO: chats
        dir.append(home);
        dir.append("/.purple/logs");
        directory.setCurrent(dir);
        tmp.append(dir);
        if (prot == "msn"){
                if (directory.exists("msn")){
                        tmp.append("/msn/");
                        protocol.append("MSN");
                }
        } else if (prot == "jabber"){
                if (directory.exists("jabber")){
                        tmp.append("/jabber/");
                        protocol.append("Jabber");
                }
        } else if (prot == "icq"){
                if (directory.exists("icq")){
                        tmp.append("/icq/");
                        protocol.append("ICQ");
                }
        } else if (prot == "aim"){
                if (directory.exists("aim")){
                        tmp.append("/aim/");
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
                tmp.append(acc);
                directory.setPath(tmp);
                if (directory.exists(tmp)){
                        QStringList contactslogs;
                        contactslogs = directory.entryList();
                        for (int k=2; k<contactslogs.size(); k++){
                            QStringList files;
                            QString dirPath;
                            QDir dirAct;
                            dirPath.append(tmp);
                            dirPath.append("/");
                            dirPath.append(contactslogs.at(k));
                            dirAct.setPath(dirPath);
                            dirAct.setCurrent(dirPath);
                            files = dirAct.entryList();
                            //neither . nor .. FIXME
                            for (int j=2; j<files.size(); j++){
                                QString f = files.at(j);
                                QString date = f.section('.',0,0);
                                QDir::setCurrent(dirPath);
                                file.setFileName(files.at(j));
                                if (file.exists()){
                                        file.open(QIODevice::ReadOnly | QIODevice::Text);
                                        QTextStream in(&file);
                                        while (!in.atEnd()) {
                                                QString line = in.readLine();
                                                processLogs(line, protocol, account, date);
                                        }
                                        file.close();
                                }
                            }
                        }
                }
                tmp.clear();
                tmp.append(dir);
                if (prot == "msn"){
                        tmp.append("/msn/");
                } else if (prot == "jabber"){
                        tmp.append("/jabber/");
                } else if (prot == "icq"){
                        tmp.append("/icq/");
                } else if (prot == "aim"){
                        tmp.append("/aim/");
                } else {
                        return -2;
                }
        }
        return 0;
}

void ParserPidgin::parser(){
        // fill attributes: metacontacts, accounts, protocols & logs
        QFile file;
        QDomDocument doc;
        QString home = QDir::homePath();
        QString dir;
        int result;

        //XML format for metacontacts
        dir.append(home);
        dir.append("/.purple");
        QDir::setCurrent(dir);
        file.setFileName("blist.xml");
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

        //XML format for accounts and protocols
        file.setFileName("accounts.xml");
        if (!file.open(QIODevice::ReadOnly)){
                return;
        }
        if (!doc.setContent(&file)) {
                file.close();
                return;
        }
        QDomElement docElemAcc = doc.documentElement();
        QDomNode nodeAcc = docElemAcc.firstChild();
        while(!nodeAcc.isNull()) {
                QDomElement e = nodeAcc.toElement();
                if(!e.isNull()) {
                        processAccountsProtocols(e);
                }
                nodeAcc = nodeAcc.nextSibling();
        } 
        file.close();

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

void ParserPidgin::processAccountsProtocols(const QDomElement& e){
        QString tmp;
        QString aux;
        QString pro;
        QString acc;
        QString passwd;

        QDomNodeList protocol = e.elementsByTagName("protocol");
        if (!protocol.isEmpty()){
                QDomElement elem = protocol.item(0).toElement();
                pro.append(elem.text());
                pro = pro.section('-', -1, -1);
        }

        QDomNodeList password = e.elementsByTagName("password");
        if (!password.isEmpty()){
                QDomElement elem = password.item(0).toElement();
                passwd.append(elem.text());
        }

        QDomNodeList accounts = e.elementsByTagName("name");
        if (!accounts.isEmpty()){
                QDomElement elem = accounts.item(0).toElement();
                tmp.append(elem.text());
                aux = tmp.section('/', -2, -2);
                if (aux.size() == 0){
                    m_accounts << tmp;
                    acc = tmp;
                }
                else{
                    m_accounts << aux;
                    acc = aux;
                }
        }

        QDomNodeList protocols = e.elementsByTagName("settings");
        if (!protocols.isEmpty()){
                QDomElement elem = protocols.item(0).toElement();
                tmp.clear();
                tmp.append("\033[01;31m");
                tmp.append("\nAccount: ");
                tmp.append("\033[00m");
                tmp.append(acc);
                m_protocols << tmp;
                tmp.clear();
                tmp.append("Protocol: ");
                tmp.append(pro);
                m_protocols << tmp;
                tmp.clear();
                tmp.append("Password: ");
                tmp.append(passwd);
                m_protocols << tmp;
                QDomNodeList settings = elem.elementsByTagName("setting");
                for(int j=0; j < settings.length(); j++){
                    QDomElement setting = settings.item(j).toElement();
                    if (setting.attributeNode("name").value().compare("server")==0){
                        tmp.clear();
                        tmp.append("Server: ");
                        tmp.append(" ");
                        tmp.append(setting.text());
                         m_protocols << tmp;
                    }
                    if (setting.attributeNode("name").value().compare("port")==0){
                        tmp.clear();
                        tmp.append("Port: ");
                        tmp.append(" ");
                        tmp.append(setting.text());
                        m_protocols << tmp;
                    }
                    if (setting.attributeNode("name").value().compare("require_tls")==0){
                        tmp.clear();
                        tmp.append("Tls: ");
                        tmp.append(" ");
                        tmp.append(setting.text());
                        m_protocols << tmp;
                    }
                    if (setting.attributeNode("name").value().compare("old_ssl")==0){
                        tmp.clear();
                        tmp.append("Ssl: ");
                        tmp.append(" ");
                        tmp.append(setting.text());
                        m_protocols << tmp;
                    }
                    if (setting.attributeNode("name").value().compare("auth_plain_in_clear")==0){
                        tmp.clear();
                        tmp.append("Plain Text: ");
                        tmp.append(" ");
                        tmp.append(setting.text());
                        m_protocols << tmp;
                    }
                }
        }

}

void ParserPidgin::processMetacontacts(const QDomElement& e){
        QString tmp;

        QDomNodeList groups = e.elementsByTagName("group");
        if (!groups.isEmpty()){
                for (int j=0; j<groups.length(); j++){
                    QDomElement elem = groups.item(j).toElement();
                    tmp.append(elem.text());
                    QDomNodeList cont = elem.elementsByTagName("contact");
                    if (!cont.isEmpty()){
                        for (int i=0; i<cont.length(); i++){
                            QDomElement ct = cont.item(i).toElement();
                        }
                    }
                }
        }
        m_metacontacts << tmp;
}

void ParserPidgin::processLogs(const QString& line, const QString& protocol, 
                                     const QString& account, const QString& date){
        QString aux = line;
        QString tmp;
        QString rest;
        QString from;
        QString msg;
        QString time;
        QString prot;
        QString acco;
        QString d;


        if (aux.section(' ', 0, 0) == "Conversation")
                return;
        prot.append(protocol);
        acco.append(account);
        d.append(date);

        //FIXME: messages with returns from a copy-paste,
        // the line starts with the message
        time = line.section(' ', 0, 0);
        rest = line.section(' ', 1);   
        from = rest.section(':', 0, 0);
        msg = rest.section(':', 1);

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
