/***************************************************************************
 *   Copyright (C) 2008 by David Menéndez González                         *
 *   narayan82@gmail.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
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
            IMProgram() { };
            IMProgram(QString nam, QString ver) : m_name(nam), m_version(ver) { };
            ~IMProgram() { };
            inline const void setName(QString name)       {m_name = name;};
            inline const void setVersion(QString version) {m_version = version;};
            inline const QString& name() const                  {return m_name;};
            inline const QString& version() const               {return m_version;};
            inline const QStringList& metacontacts() const      {return m_metacontacts;};
            inline const QStringList& accounts() const          {return m_accounts;};
            inline const QStringList& protocols() const         {return m_protocols;};
            inline const QStringList& logs() const              {return m_logs;};
            virtual void parser()=0;
            virtual void saveLogs()=0;

        protected:
            QString m_name;
            QString m_version;
            QStringList m_metacontacts;
            QStringList m_accounts;
            QStringList m_protocols;
            QStringList m_logs;
            QStringList m_listGroups;
};
#endif //_IMPROGRAM_H_
