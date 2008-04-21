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

#include "psi.h"

/* Public methods */

Psi::Psi()
: IMProgram()
{ }

Psi::Psi(QString nombre, QString version)
: IMProgram(nombre,version)
{ }
            
Psi::Psi(QString nombre, QString version, QList<QString> accounts, 
                QList<QString> protocols, QList<QString> metacontacts, QList<QString> logs)
: IMProgram(nombre,version,accounts,protocols,metacontacts,logs)
{ }

Psi::~Psi(){ }

QString Psi::getName(){
        return m_name;
}

QString Psi::getVersion(){
        return m_version;
}

void Psi::setName(QString name){
        m_name=name;
}

void Psi::setVersion(QString version){
        m_version=version;
}

void Psi::parser(){
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

QList<QString> Psi::getMetaContacts(){
        return m_metacontacts;
}

QList<QString> Psi::getAccounts(){
        return m_accounts;
}

QList<QString> Psi::getProtocols(){
        return m_protocols;
}

QList<QString> Psi::getLogs(){
        return m_logs;
}

void Psi::saveLogs(){
        // save logs in kinapsis format
        return;
}

/* Private methods */

void Psi::processMetacontacts(QDomElement e){
        QDomAttr a;
        QString tmp;
        bool ok;

        return;
}

void Psi::processLogs(QDomElement e, QString protocol, QString account, QString date){
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

void Psi::processLine(QString line){
        int pos=-1;
        QRegExp rx;
        QString tmp;

        return;
}
