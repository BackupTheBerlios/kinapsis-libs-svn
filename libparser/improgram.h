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

using namespace std;

#ifndef _IMProgram_
#define _IMProgram_

//Clase padre abstracta para las clases derivadas Kopete, Pidgin, Psi...

class IMProgram{
        private:

        protected: //inherited
            QString m_name;
            QString m_version;
            QList<QString> m_metacontacts;
            QList<QString> m_accounts;
            QList<QString> m_protocols; //asocia a una cuenta su conf
            QList<QString> m_logs;
            //cuentas
            
        public:
            IMProgram();
            IMProgram(QString nom, QString ver);
            IMProgram(QString nom, QString ver, QList<QString> acc, QList<QString> pro, 
                                                QList<QString> con, QList<QString> lg);
            ~IMProgram();

            virtual QString getName()=0;
            virtual QString getVersion()=0;
            virtual void setName(QString name)=0;
            virtual void setVersion(QString version)=0;
            virtual void parser()=0; //rellena los atrib. contacts, protocols, accounts y logs
            virtual QList<QString> getMetaContacts()=0;
            virtual QList<QString> getAccounts()=0;
            virtual QList<QString> getProtocols()=0;
            virtual QList<QString> getLogs()=0;
            virtual void saveLogs()=0;

}; //fin declaracion clase IMProgram (Instant Message Program)

#endif
