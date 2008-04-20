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
#include <QTextStream>

using namespace std;

#include "manager.h"
#include "improgram.h"
#include "kopete.h"
#include "pidgin.h"


/* Public methods */

Manager::Manager()
: m_numberIMP(0), m_existPrograms(false)
{ }

Manager::~Manager()
{ }

bool Manager::existIMPrograms(){
        searchIMPrograms();
        return m_existPrograms;
}

QList<IMProgram*> Manager::getIMPrograms(){
        return m_programs;
}

int Manager::getNumberIMPrograms(){
        return m_numberIMP;
}

/* Private methods */

void Manager::searchIMPrograms(){
//        IMProgram * p;
        QFile file;
        QString home = QDir::homePath();
        QString dir;

        //TODO: pidgin,psi,gaim,sim,amsn,mercury

        dir.append(home);
        dir.append("/.kde/share/config");
        file.setFileName("kopeterc");
        QDir::setCurrent(dir);
        if (file.exists()){
                m_existPrograms = true;
                IMProgram* p = new Kopete();
                p->setName("Kopete");
                m_programs.append(p);
        }

        dir.clear();
        dir.append(home);
        dir.append("/.purple");
        file.setFileName("accounts.xml");
        QDir::setCurrent(dir);
        if (file.exists()){
                m_existPrograms = true;
                IMProgram* p = new Pidgin();
                p->setName("Pidgin");
                m_programs.append(p);
        }

        if (m_existPrograms==false){
                qDebug() << "Instant Message Programs not found";
        }
        m_numberIMP=m_programs.count();
}
