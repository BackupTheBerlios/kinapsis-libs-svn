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
#include <QtDebug>
#include <QFile>
#include <QDir>

#include "parseramsn.h"

/* Public methods */

void ParserAmsn::saveLogs(){
        // save logs in kinapsis format
        return;
}

void ParserAmsn::parser(){
        // fill attributes: metacontacts, accounts, protocols & logs
        return;
}

/* Private methods */

const void ParserAmsn::processAccountsProtocols(const QDomElement& e){
        return;
}

const void ParserAmsn::processMetacontacts(const QDomElement& e){
        return;
}

const void ParserAmsn::processLogs(const QDomElement& e, const QString& protocol, 
                                   const QString& account, const QString& date){
        return;
}
