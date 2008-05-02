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
#include <QDomNode>

#ifndef _KOPETE_H_
#define _KOPETE_H_

#include "improgram.h"

class Kopete : public IMProgram {
        public:
            Kopete();
            Kopete(QString nombre, QString version);
            ~Kopete();
            void setName(QString name){m_name = name;};
            void setVersion(QString version){m_version = version;};
            void parser();
            void saveLogs();
            inline QString name(){return m_name;};
            inline QString version(){return m_version;};
            QStringList metacontacts(){return m_metacontacts;};
            QStringList accounts(){return m_accounts;};
            QStringList protocols(){return m_protocols;};
            QStringList logs(){return m_logs;};

        private:
            QStringList m_listGroups;

            void processProtocolsAccounts(const QString& line);
            void processMetacontacts(const QDomElement& e);
            void processLogs(const QDomElement& e, const QString& protocol, const QString& account, const QString& date);
};
#endif // _KOPETE_H_
