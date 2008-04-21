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

#include "mercury.h"

/* Public methods */

Mercury::Mercury()
: IMProgram()
{ }

Mercury::Mercury(QString nombre, QString version)
: IMProgram(nombre,version)
{ }
            
Mercury::Mercury(QString nombre, QString version, QList<QString> accounts, 
                QList<QString> protocols, QList<QString> metacontacts, QList<QString> logs)
: IMProgram(nombre,version,accounts,protocols,metacontacts,logs)
{ }

Mercury::~Mercury(){ }

QString Mercury::getName(){
        return m_name;
}

QString Mercury::getVersion(){
        return m_version;
}

void Mercury::setName(QString name){
        m_name=name;
}

void Mercury::setVersion(QString version){
        m_version=version;
}

void Mercury::parser(){
        // fill attributes: metacontacts, accounts, protocols & logs
        QFile file;
        QDomDocument doc;
        QString home = QDir::homePath();
        QString dir;
        QString tmp;
        QDir directory;
        QString protocol;
        QString account;
        
        return;
}

QList<QString> Mercury::getMetaContacts(){
        return m_metacontacts;
}

QList<QString> Mercury::getAccounts(){
        return m_accounts;
}

QList<QString> Mercury::getProtocols(){
        return m_protocols;
}

QList<QString> Mercury::getLogs(){
        return m_logs;
}

void Mercury::saveLogs(){
        // save logs in kinapsis format
        return;
}

/* Private methods */

void Mercury::processMetacontacts(QDomElement e){
        QDomAttr a;
        QString tmp;
        bool ok;

        return;
}

void Mercury::processLogs(QDomElement e, QString protocol, QString account, QString date){
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

void Mercury::processLine(QString line){
        int pos=-1;
        QRegExp rx;
        QString tmp;

        return;
}
