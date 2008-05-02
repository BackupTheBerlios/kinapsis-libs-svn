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
#include <QStringList>

#ifndef _IMPROGRAM_H_
#define _IMPROGRAM_H_

class IMProgram{
        public:
            IMProgram(){};
            IMProgram(QString nam, QString ver):m_name(nam), m_version(ver){};
            ~IMProgram(){};
            virtual QString name()=0;
            virtual QString version()=0;
            virtual void setName(QString name)=0;
            virtual void setVersion(QString version)=0;
            virtual void parser()=0;
            virtual QStringList metacontacts()=0;
            virtual QStringList accounts()=0;
            virtual QStringList protocols()=0;
            virtual QStringList logs()=0;
            virtual void saveLogs()=0;

        protected:
            QString m_name;
            QString m_version;
            QStringList m_metacontacts;
            QStringList m_accounts;
            QStringList m_protocols;
            QStringList m_logs;
};
#endif //_IMPROGRAM_H_
