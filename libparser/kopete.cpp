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

#include "kopete.h"

//Clase Kopete derivada de la clase padre IMProgram

/* Public methods */

Kopete::Kopete()
: IMProgram()
{ }

Kopete::Kopete(QString nombre, QString version)
: IMProgram(nombre,version)
{ }
            
Kopete::Kopete(QString nombre, QString version, QList<QString> accounts, 
                QList<QString> protocols, QList<QString> metacontacts, QList<QString> logs)
: IMProgram(nombre,version,accounts,protocols,metacontacts,logs)
{ }

Kopete::~Kopete(){ }

QString Kopete::getName(){
        return m_name;
}

QString Kopete::getVersion(){
        return m_version;
}

void Kopete::setName(QString name){
        m_name=name;
}

void Kopete::setVersion(QString version){
        m_version=version;
}

void Kopete::parser(){
        // rellena los atrib. metacontacts, accounts, protocols y logs
        QFile file;
        QDomDocument doc;
        QString home = QDir::homePath();
        QString dir;
        QString tmp;
        QDir directory;
        //TODO: ICQ, AOL

        //XML para los metacontactos
        //TODO
        dir.append(home);
        dir.append("/.kde/share/apps/kopete");
        QDir::setCurrent(dir);
        file.setFileName("contactlist.xml");
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
                // if (n.isElement())
                // try to convert the node to an element.
                QDomElement e = n.toElement();
                if(!e.isNull()) {
                        // // the node really is an element.
                        processMetacontacts(e);
                }
                n = n.nextSibling();
        } 
        file.close();


        //kderc format para las cuentas y protocolos
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
                        processLine(line);
                }
                file.close();
        }

        //XML para los logs
        //
        //MSN
        dir.clear();
        dir.append(home);
        dir.append("/.kde/share/apps/kopete/logs");
        directory.setCurrent(dir);
        if (directory.exists("MSNProtocol")){
                //busqueda de cuentas
                QListIterator<QString> i(m_accounts);
                tmp.append("MSNProtocol/");
                m_logs << "Protocol: MSN >>>";
                while (i.hasNext()){
                        QString acc;
                        QString aux;
                        aux.append("Account:");
                        aux.append(i.peekNext());
                        acc.append(i.next());
                        acc.replace(QString("."), QString("-"));
                        tmp.append(acc);
                        directory.setPath(tmp);
                        if (directory.exists(tmp)){
                                QStringList files;
                                files = directory.entryList();
                                //no devolvemos ni . ni ..
                                m_logs << aux;
                                for (int j=2; j<files.size(); j++){
                                        m_logs << files.at(j);
                                        QDir::setCurrent(tmp);
                                        file.setFileName(files.at(j));
                                        file.open(QIODevice::ReadOnly | QIODevice::Text);
                                        doc.setContent(&file);
                                        QDomElement logElem = doc.documentElement();
                                        QDomNode n = logElem.firstChild();
                                        while(!n.isNull()) {
                                                QDomElement e = n.toElement();
                                                if(!e.isNull()) {
                                                        // // the node really is an element.
                                                        processLogs(e);
                                                }
                                                n = n.nextSibling();
                                        } 
                                        file.close();
                                }
                        }
                        tmp.clear();
                        tmp.append(dir);
                        tmp.append("/MSNProtocol/");
                }
        }
        //Jabber
        dir.clear();
        dir.append(home);
        dir.append("/.kde/share/apps/kopete/logs");
        directory.setPath(dir);
        if (directory.exists("JabberProtocol")){
                //busqueda de cuentas
                QListIterator<QString> i(m_accounts);
                tmp.clear();
                tmp.append(dir);
                tmp.append("/JabberProtocol/");
                m_logs << "Protocol: Jabber >>>";
                while (i.hasNext()){
                        QString acc;
                        QString aux;
                        aux.append("Account:");
                        aux.append(i.peekNext());
                        acc.append(i.next());
                        acc.replace(QString("."), QString("-"));
                        tmp.append(acc);
                        directory.setPath(tmp);
                        if (directory.exists(tmp)){
                                QStringList files;
                                files = directory.entryList();
                                m_logs << aux;
                                for (int j=2; j<files.size(); j++){
                                        m_logs << files.at(j);
                                        QDir::setCurrent(tmp);
                                        file.setFileName(files.at(j));
                                        file.open(QIODevice::ReadOnly | QIODevice::Text);
                                        doc.setContent(&file);
                                        QDomElement logElem = doc.documentElement();
                                        QDomNode n = logElem.firstChild();
                                        while(!n.isNull()) {
                                                QDomElement e = n.toElement();
                                                if(!e.isNull()) {
                                                        // // the node really is an element.
                                                        processLogs(e);
                                                }
                                                n = n.nextSibling();
                                        } 
                                        file.close();
                                }
                        }
                        tmp.clear();
                        tmp.append(dir);
                        tmp.append("/JabberProtocol/");
                }
        }
        //AIM
        dir.clear();
        dir.append(home);
        dir.append("/.kde/share/apps/kopete/logs");
        directory.setPath(dir);
        if (directory.exists("AIMProtocol")){
                //busqueda de cuentas
                QListIterator<QString> i(m_accounts);
                tmp.clear();
                tmp.append(dir);
                tmp.append("/AIMProtocol/");
                m_logs << "Protocol: AIM >>>";
                while (i.hasNext()){
                        QString acc;
                        QString aux;
                        aux.append("Account:");
                        aux.append(i.peekNext());
                        acc.append(i.next());
                        acc.replace(QString("."), QString("-"));
                        tmp.append(acc);
                        directory.setPath(tmp);
                        if (directory.exists(tmp)){
                                QStringList files;
                                files = directory.entryList();
                                m_logs << aux;
                                for (int j=2; j<files.size(); j++){
                                        m_logs << files.at(j);
                                        QDir::setCurrent(tmp);
                                        file.setFileName(files.at(j));
                                        file.open(QIODevice::ReadOnly | QIODevice::Text);
                                        doc.setContent(&file);
                                        QDomElement logElem = doc.documentElement();
                                        QDomNode n = logElem.firstChild();
                                        while(!n.isNull()) {
                                                QDomElement e = n.toElement();
                                                if(!e.isNull()) {
                                                        // // the node really is an element.
                                                        processLogs(e);
                                                }
                                                n = n.nextSibling();
                                        } 
                                        file.close();
                                }
                        }
                        tmp.clear();
                        tmp.append(dir);
                        tmp.append("/AIMProtocol/");
                }
        }
        //ICQ/AOL
        dir.clear();
        dir.append(home);
        dir.append("/.kde/share/apps/kopete/logs");
        directory.setPath(dir);
        if (directory.exists("ICQProtocol")){
                //busqueda de cuentas
                QListIterator<QString> i(m_accounts);
                tmp.clear();
                tmp.append(dir);
                tmp.append("/ICQProtocol/");
                m_logs << "Protocol: ICQ >>>";
                while (i.hasNext()){
                        QString acc;
                        QString aux;
                        aux.append("Account:");
                        aux.append(i.peekNext());
                        acc.append(i.next());
                        acc.replace(QString("."), QString("-"));
                        tmp.append(acc);
                        directory.setPath(tmp);
                        if (directory.exists(tmp)){
                                QStringList files;
                                files = directory.entryList();
                                m_logs << aux;
                                for (int j=2; j<files.size(); j++){
                                        m_logs << files.at(j);
                                        QDir::setCurrent(tmp);
                                        file.setFileName(files.at(j));
                                        file.open(QIODevice::ReadOnly | QIODevice::Text);
                                        doc.setContent(&file);
                                        QDomElement logElem = doc.documentElement();
                                        QDomNode n = logElem.firstChild();
                                        while(!n.isNull()) {
                                                QDomElement e = n.toElement();
                                                if(!e.isNull()) {
                                                        // // the node really is an element.
                                                        processLogs(e);
                                                }
                                                n = n.nextSibling();
                                        } 
                                        file.close();
                                }
                        }
                        tmp.clear();
                        tmp.append(dir);
                        tmp.append("/ICQProtocol/");
                }
        }
}

QList<QString> Kopete::getMetaContacts(){
        return m_metacontacts;
}

QList<QString> Kopete::getAccounts(){
        return m_accounts;
}

QList<QString> Kopete::getProtocols(){
        return m_protocols;
}

QList<QString> Kopete::getLogs(){
        return m_logs;
}

void Kopete::saveLogs(){
        // salva en fichero log en el estilo de kinapsis el atributo logs
        return;
}

/* Private methods */

void Kopete::processMetacontacts(QDomElement e){
        QDomAttr a;
        QString tmp;
        QDomNodeList nodeList;
        QDomElement el;
        a = e.attributeNode("contactId");
        if (!a.value().isEmpty()){
//                tmp.append("\033[01;32m");
                tmp.append(a.value());
//                tmp.append("\033[00m");
        }
        nodeList = e.elementsByTagName("plugin-data");
        for(int i=0; i < nodeList.length(); i++){
            el = nodeList.item(i).toElement();
            tmp.append("<->");
            tmp.append(el.attributeNode("plugin-id").value());
            QDomNodeList pluginDataFields = el.elementsByTagName("plugin-data-field");
            for(int j=0; j < pluginDataFields.length(); j++){
                    QDomElement key = pluginDataFields.item(j).toElement();
                    if (key.attributeNode("key").value().compare("accountId")==0){
                            tmp.append(key.text());
                    }
            }
            tmp.append(">>");
            tmp.append(el.attribute("key"));
        }
        m_metacontacts << tmp;
}

void Kopete::processLogs(QDomElement e){
        QDomAttr a;
        QString tmp;

        tmp.append(" FROM: ");
        tmp.append(e.attribute("nick"));
        tmp.append("-");
        tmp.append(e.attribute("from"));
        tmp.append(" MSG: ");
        tmp.append(e.text().toUtf8()); //FIXME: mal las enyes y demas
        tmp.append(" AT: ");
        tmp.append(e.attribute("time"));
        m_logs << tmp;
}

void Kopete::processLine(QString line){
        int pos=-1;
        QRegExp rx;
        QString tmp;
        //TODO: identity, defaultEncoding, firstPort-lastPort, requireAuth,
        //respectRequireAuth, webAware

        if (line.size() > 0){
                //account
                rx.setPattern("(^AccountId=)(([a-z|A-Z|0-9]+@[a-z|A-Z|0-9]+.[a-z|A-Z]+)|([a-z|A-Z|0-9]+))");
                pos = rx.indexIn(line,0);
                if (pos > -1){
                        m_accounts << rx.cap(2);
                        tmp.clear();
                        tmp.append("Account:");
                        tmp.append(rx.cap(2));
                        m_protocols << tmp;
                        return;
                }
                //Plain text
                rx.setPattern("(^AllowPlainTextPassword=)([a-z|A-Z]+)");  
                pos = rx.indexIn(line,0);
                if (pos > -1){
                        tmp.clear();
                        tmp.append("PlainTextPassword:");
                        tmp.append(rx.cap(2));
                        m_protocols << tmp;
                        return;
                }
                //port (jabber,aim)
                rx.setPattern("(^Port=)([0-9]+)");  
                pos = rx.indexIn(line,0);
                if (pos > -1){
                        tmp.clear();
                        tmp.append("Port:");
                        tmp.append(rx.cap(2));
                        m_protocols << tmp;
                        return;
                }
                //protocol
                rx.setPattern("(^Protocol=)([a-z|A-Z]+)");  
                pos = rx.indexIn(line,0);
                if (pos > -1){
                        tmp.clear();
                        tmp.append("Protocol:");
                        tmp.append(rx.cap(2));
                        m_protocols << tmp;
                        return;
                }
                //server (jabber,aim)
                rx.setPattern("(^Server=)([a-z|A-Z|0-9|.]+)");  
                pos = rx.indexIn(line,0);
                if (pos > -1){
                        tmp.clear();
                        tmp.append("Server:");
                        tmp.append(rx.cap(2));
                        m_protocols << tmp;
                        return;
                }
                //ssl-tls
                rx.setPattern("(^UseSSL=)([a-z|A-Z]+)");  
                pos = rx.indexIn(line,0);
                if (pos > -1){
                        tmp.clear();
                        tmp.append("SSL:");
                        tmp.append(rx.cap(2));
                        m_protocols << tmp;
                        return;
                }
                //MFN  FIXME: posibilidad de caracteres raros
                rx.setPattern("(^MFN=)([a-z|A-Z|0-9]+)");  
                pos = rx.indexIn(line,0);
                if (pos > -1){
                        tmp.clear();
                        tmp.append("MFN:");
                        tmp.append(rx.cap(2));
                        m_protocols << tmp;
                        return;
                }
                //serverName (MSN)
                rx.setPattern("(^serverName=)([a-z|A-Z|0-9|.]+)");  
                pos = rx.indexIn(line,0);
                if (pos > -1){
                        tmp.clear();
                        tmp.append("Server:");
                        tmp.append(rx.cap(2));
                        m_protocols << tmp;
                        return;
                }
                //serverPort (MSN)
                rx.setPattern("(^serverPort=)([0-9]+)");  
                pos = rx.indexIn(line,0);
                if (pos > -1){
                        tmp.clear();
                        tmp.append("Port:");
                        tmp.append(rx.cap(2));
                        m_protocols << tmp;
                        return;
                }

        }
}
