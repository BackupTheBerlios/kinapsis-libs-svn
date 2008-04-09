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

#ifndef _Kopete_
#define _Kopete_

#include "improgram.h"

class Kopete : public IMProgram {
        private:
            void processLine(QString line);
            void processMetacontacts(QDomElement e);
            
        public:
            Kopete();
            Kopete(QString nombre, QString version);
            Kopete(QString nombre, QString version, QList<QString> accounts, 
                   QList<QString> protocols, QList<QString> metacontacts, QList<QString> logs);
            ~Kopete();

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

}; //fin declaracion clase Kopete derivada de IMProgram

#endif
