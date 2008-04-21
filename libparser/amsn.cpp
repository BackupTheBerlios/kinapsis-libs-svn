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

#include "amsn.h"

/* Public methods */

Amsn::Amsn()
: IMProgram()
{ }

Amsn::Amsn(QString nombre, QString version)
: IMProgram(nombre,version)
{ }
            
Amsn::Amsn(QString nombre, QString version, QList<QString> accounts, 
                QList<QString> protocols, QList<QString> metacontacts, QList<QString> logs)
: IMProgram(nombre,version,accounts,protocols,metacontacts,logs)
{ }

Amsn::~Amsn(){ }

QString Amsn::getName(){
        return m_name;
}

QString Amsn::getVersion(){
        return m_version;
}

void Amsn::setName(QString name){
        m_name=name;
}

void Amsn::setVersion(QString version){
        m_version=version;
}

void Amsn::parser(){
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

QList<QString> Amsn::getMetaContacts(){
        return m_metacontacts;
}

QList<QString> Amsn::getAccounts(){
        return m_accounts;
}

QList<QString> Amsn::getProtocols(){
        return m_protocols;
}

QList<QString> Amsn::getLogs(){
        return m_logs;
}

void Amsn::saveLogs(){
        // save logs in kinapsis format
        return;
}

/* Private methods */

void Amsn::processMetacontacts(QDomElement e){
        QDomAttr a;
        QString tmp;
        bool ok;

        return;
}

void Amsn::processLogs(QDomElement e, QString protocol, QString account, QString date){
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

void Amsn::processLine(QString line){
        int pos=-1;
        QRegExp rx;
        QString tmp;

        return;
}
