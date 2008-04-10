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

using namespace std;

#include "improgram.h"
#include "kopete.h"

//Clase abstracta padre para las clases derivadas Kopete, Pidgin, Psi...

IMProgram::IMProgram()
:   m_name("Null"),m_version("Null"),m_accounts(),m_protocols(),m_metacontacts(),m_logs()
{}

IMProgram::IMProgram(QString nom, QString ver)
:   m_name(nom),m_version(ver)
{}

IMProgram::IMProgram(QString nom, QString ver, QList<QString> acc, QList<QString> pro, 
                     QList<QString> con, QList<QString> lg)
:   m_name(nom),m_version(ver),m_accounts(acc),m_protocols(pro),m_metacontacts(con),m_logs(lg)
{}

IMProgram::~IMProgram()
{}

//Classes containing pure virtual methods are termed "abstract;" they cannot be instantiated directly, 
//and a subclass of an abstract class can only be instantiated directly if all inherited pure virtual 
//methods have been implemented by that class or a parent class. Pure virtual methods typically have 
//a declaration (signature) and no definition (implementation)
//Although an implementation of the pure virtual function is typically not provided 
//in an abstract class, it may be included. Every non-abstract child class is still 
//required to override the method, but the implementation provided by the abstract class may be called
