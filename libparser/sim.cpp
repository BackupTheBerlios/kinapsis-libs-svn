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

#include "sim.h"

/* Public methods */

Sim::Sim()
: IMProgram()
{ }

Sim::Sim(QString nombre, QString version)
: IMProgram(nombre,version)
{ }
            
Sim::Sim(QString nombre, QString version, QList<QString> accounts, 
                QList<QString> protocols, QList<QString> metacontacts, QList<QString> logs)
: IMProgram(nombre,version,accounts,protocols,metacontacts,logs)
{ }

Sim::~Sim(){ }

QString Sim::getName(){
        return m_name;
}

QString Sim::getVersion(){
        return m_version;
}

void Sim::setName(QString name){
        m_name=name;
}

void Sim::setVersion(QString version){
        m_version=version;
}

void Sim::parser(){
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

QList<QString> Sim::getMetaContacts(){
        return m_metacontacts;
}

QList<QString> Sim::getAccounts(){
        return m_accounts;
}

QList<QString> Sim::getProtocols(){
        return m_protocols;
}

QList<QString> Sim::getLogs(){
        return m_logs;
}

void Sim::saveLogs(){
        // save logs in kinapsis format
        return;
}

/* Private methods */

void Sim::processMetacontacts(QDomElement e){
        QDomAttr a;
        QString tmp;
        bool ok;

        return;
}

void Sim::processLogs(QDomElement e, QString protocol, QString account, QString date){
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

void Sim::processLine(QString line){
        int pos=-1;
        QRegExp rx;
        QString tmp;

        return;
}
