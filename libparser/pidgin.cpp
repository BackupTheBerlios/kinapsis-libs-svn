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

#include <iostream>
#include <QtDebug>
#include <QFile>
#include <QDir>

using namespace std;

#include "pidgin.h"

/* Public methods */

Pidgin::Pidgin()
: IMProgram()
{ }

Pidgin::Pidgin(QString nombre, QString version)
: IMProgram(nombre,version)
{ }
            
Pidgin::Pidgin(QString nombre, QString version, QList<QString> accounts, 
                QList<QString> protocols, QList<QString> metacontacts, QList<QString> logs)
: IMProgram(nombre,version,accounts,protocols,metacontacts,logs)
{ }

Pidgin::~Pidgin(){ }

QString Pidgin::getName(){
        return m_name;
}

QString Pidgin::getVersion(){
        return m_version;
}

void Pidgin::setName(QString name){
        m_name=name;
}

void Pidgin::setVersion(QString version){
        m_version=version;
}

QList<QString> Pidgin::getMetaContacts(){
        return m_metacontacts;
}

QList<QString> Pidgin::getAccounts(){
        return m_accounts;
}

QList<QString> Pidgin::getProtocols(){
        return m_protocols;
}

QList<QString> Pidgin::getLogs(){
        return m_logs;
}

void Pidgin::saveLogs(){
        // save logs in kinapsis format
        return;
}

void Pidgin::parser(){
        // fill attributes: metacontacts, accounts, protocols & logs
        QFile file;
        QDomDocument doc;
        QString home = QDir::homePath();
        QString dir;
        QString tmp;
        QDir directory;
        QString protocol;
        QString account;
        
        //XML for accounts & protocols
        dir.append(home);
        dir.append("/.purple");
        QDir::setCurrent(dir);
        file.setFileName("accounts.xml");
        if (!file.open(QIODevice::ReadOnly)){
                return; //FIXME
        }
        if (!doc.setContent(&file)) {
                file.close();
                return;//FIXME
        }

        QDomElement docElem = doc.documentElement();
        QDomNode n = docElem.firstChild();
        while(!n.isNull()) {
                QDomElement e = n.toElement();
                if(!e.isNull()) {
                        processAccountsProtocols(e);
                }
                n = n.nextSibling();
        } 
        file.close();

        //XML for metacontacts
        dir.append(home);
        dir.append("/.purple");
        QDir::setCurrent(dir);
        file.setFileName("blist.xml");
        if (!file.open(QIODevice::ReadOnly)){
                return; //FIXME
        }
        if (!doc.setContent(&file)) {
                file.close();
                return;//FIXME
        }

        docElem = doc.documentElement();
        n = docElem.firstChild();
        while(!n.isNull()) {
                QDomElement e = n.toElement();
                if(!e.isNull()) {
                        processMetacontacts(e);
                }
                n = n.nextSibling();
        } 
        file.close();

        /*MSN logs*/
        dir.clear();
        dir.append(home);
        dir.append("/.purple/logs");
        directory.setCurrent(dir);
        if (directory.exists("msn")){
                //accounts search
                QListIterator<QString> i(m_accounts);
                tmp.append("msn/");
                protocol.append("MSN");
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
                                //dont return neither . nor ..
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
                        tmp.append("/msn/");
                }
        }
        /*Jabber logs*/
//        dir.clear();
//        dir.append(home);
//        dir.append("/.kde/share/apps/kopete/logs");
//        directory.setPath(dir);
//        if (directory.exists("JabberProtocol")){
//                QListIterator<QString> i(m_accounts);
//                tmp.clear();
//                tmp.append(dir);
//                tmp.append("/JabberProtocol/");
//                protocol.clear();
//                protocol.append("Jabber");
//                while (i.hasNext()){
//                        QString acc;
//                        QString aux;
//                        aux.append(i.peekNext());
//                        account.clear();
//                        account.append(aux);
//                        acc.append(i.next());
//                        acc.replace(QString("."), QString("-"));
//                        tmp.append(acc);
//                        directory.setPath(tmp);
//                        if (directory.exists(tmp)){
//                                QStringList files;
//                                files = directory.entryList();
//                                for (int j=2; j<files.size(); j++){
//                                        QString f = files.at(j);
//                                        QString date = f.section('.',-2,-2);
//                                        QDir::setCurrent(tmp);
//                                        file.setFileName(files.at(j));
//                                        file.open(QIODevice::ReadOnly | QIODevice::Text);
//                                        doc.setContent(&file);
//                                        QDomElement logElem = doc.documentElement();
//                                        QDomNode n = logElem.firstChild();
//                                        while(!n.isNull()) {
//                                                QDomElement e = n.toElement();
//                                                if(!e.isNull()) {
//                                                        processLogs(e,protocol,account,date);
//                                                }
//                                                n = n.nextSibling();
//                                        } 
//                                        file.close();
//                                }
//                        }
//                        tmp.clear();
//                        tmp.append(dir);
//                        tmp.append("/JabberProtocol/");
//                }
//        }
       /* AIM logs */
//        dir.clear();
//        dir.append(home);
//        dir.append("/.kde/share/apps/kopete/logs");
//        directory.setPath(dir);
//        if (directory.exists("AIMProtocol")){
//                QListIterator<QString> i(m_accounts);
//                tmp.clear();
//                tmp.append(dir);
//                tmp.append("/AIMProtocol/");
//                protocol.clear();
//                protocol.append("AIM");
//                while (i.hasNext()){
//                        QString acc;
//                        QString aux;
//                        aux.append(i.peekNext());
//                        account.clear();
//                        account.append(aux);
//                        acc.append(i.next());
//                        acc.replace(QString("."), QString("-"));
//                        tmp.append(acc);
//                        directory.setPath(tmp);
//                        if (directory.exists(tmp)){
//                                QStringList files;
//                                files = directory.entryList();
//                                for (int j=2; j<files.size(); j++){
//                                        QString f = files.at(j);
//                                        QString date = f.section('.',-2,-2);
//                                        QDir::setCurrent(tmp);
//                                        file.setFileName(files.at(j));
//                                        file.open(QIODevice::ReadOnly | QIODevice::Text);
//                                        doc.setContent(&file);
//                                        QDomElement logElem = doc.documentElement();
//                                        QDomNode n = logElem.firstChild();
//                                        while(!n.isNull()) {
//                                                QDomElement e = n.toElement();
//                                                if(!e.isNull()) {
//                                                        processLogs(e,protocol,account,date);
//                                                }
//                                                n = n.nextSibling();
//                                        } 
//                                        file.close();
//                                }
//                        }
//                        tmp.clear();
//                        tmp.append(dir);
//                        tmp.append("/AIMProtocol/");
//                }
//        }
       /* ICQ/AOL logs */
//        dir.clear();
//        dir.append(home);
//        dir.append("/.kde/share/apps/kopete/logs");
//        directory.setPath(dir);
//        if (directory.exists("ICQProtocol")){
//                QListIterator<QString> i(m_accounts);
//                tmp.clear();
//                tmp.append(dir);
//                tmp.append("/ICQProtocol/");
//                protocol.clear();
//                protocol.append("ICQ");
//                while (i.hasNext()){
//                        QString acc;
//                        QString aux;
//                        aux.append(i.peekNext());
//                        account.clear();
//                        account.append(aux);
//                        acc.append(i.next());
//                        acc.replace(QString("."), QString("-"));
//                        tmp.append(acc);
//                        directory.setPath(tmp);
//                        if (directory.exists(tmp)){
//                                QStringList files;
//                                files = directory.entryList();
//                                m_logs << aux;
//                                for (int j=2; j<files.size(); j++){
//                                        QString f = files.at(j);
//                                        QString date = f.section('.',-2,-2);
//                                        QDir::setCurrent(tmp);
//                                        file.setFileName(files.at(j));
//                                        file.open(QIODevice::ReadOnly | QIODevice::Text);
//                                        doc.setContent(&file);
//                                        QDomElement logElem = doc.documentElement();
//                                        QDomNode n = logElem.firstChild();
//                                        while(!n.isNull()) {
//                                                QDomElement e = n.toElement();
//                                                if(!e.isNull()) {
//                                                        processLogs(e,protocol,account,date);
//                                                }
//                                                n = n.nextSibling();
//                                        } 
//                                        file.close();
//                                }
//                        }
//                        tmp.clear();
//                        tmp.append(dir);
//                        tmp.append("/ICQProtocol/");
//                }
//        }

}


/* Private methods */

void Pidgin::processAccountsProtocols(QDomElement e){
        QDomAttr a;
        QString tmp;
        QString aux;
        bool ok;

        QDomNodeList accounts = e.elementsByTagName("name");
        if (!accounts.isEmpty()){
                QDomElement elem = accounts.item(0).toElement();
                tmp.append(elem.text());
                aux = tmp.section('/', -2, -2);
                if (aux.size() == 0)
                    m_accounts << tmp;
                else
                    m_accounts << aux;
        }
}

void Pidgin::processMetacontacts(QDomElement e){
        QDomAttr a;
        QString tmp;
        bool ok;

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

void Pidgin::processLogs(QDomElement e, QString protocol, QString account, QString date){
        QDomAttr a;
        QString tmp;
        QString from;
        QString msg;
        QString time;
        QString prot;
        QString acco;
        QString d;

        return;
}

void Pidgin::processLine(QString line){
        int pos=-1;
        QRegExp rx;
        QString tmp;

        return;
}
