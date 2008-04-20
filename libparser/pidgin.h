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
#include <QString>
#include <QList>
#include <QDomNode>

using namespace std;

#ifndef _Pidgin_
#define _Pidgin_

#include "improgram.h"

class Pidgin : public IMProgram {
        private:
            void processLine(QString line);
            void processMetacontacts(QDomElement e);
            void processLogs(QDomElement e, QString protocol, QString account, QString date);
            QList<QString> m_listGroups;
            
        public:
            Pidgin();
            Pidgin(QString nombre, QString version);
            Pidgin(QString nombre, QString version, QList<QString> accounts, 
                   QList<QString> protocols, QList<QString> metacontacts, QList<QString> logs);
            ~Pidgin();

            QString getName();
            QString getVersion();
            void setName(QString name);
            void setVersion(QString version);
            void parser();
            QList<QString> getMetaContacts();
            QList<QString> getAccounts();
            QList<QString> getProtocols();
            QList<QString> getLogs();
            void saveLogs();

};

#endif
