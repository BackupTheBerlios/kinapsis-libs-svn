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
#include <QtDebug>
#include <QFile>
#include <QDir>
#include <QTextStream>

#include "manager.h"
#include "improgram.h"
#include "kopete.h"
#include "pidgin.h"
//#include "psi.h"
//#include "sim.h"
//#include "mercury.h"
//#include "amsn.h"

/* Public methods */

const bool Manager::existIMPrograms(){
        searchIMPrograms();
        return m_existPrograms;
}

/* Private methods */

void Manager::searchIMPrograms(){
        QFile file;
        QString home = QDir::homePath();
        QString dir;
        QDir directory;

        //Kopete
        dir.clear();
        dir.append(home);
        dir.append("/.kde/share/config");
        directory.setPath(dir);
        if (directory.exists(dir)){
            QDir::setCurrent(dir);
            file.setFileName("kopeterc");
            if (file.exists()){
                m_existPrograms = true;
                IMProgram* p = new Kopete();
                p->setName("Kopete");
                m_programs.append(p);
            }
        }

        //Pidgin
        dir.clear();
        dir.append(home);
        dir.append("/.purple");
        directory.setPath(dir);
        if (directory.exists(dir)){
            QDir::setCurrent(dir);
            file.setFileName("accounts.xml");
            if (file.exists()){
                m_existPrograms = true;
                IMProgram* p = new Pidgin();
                p->setName("Pidgin");
                m_programs.append(p);
            }
        }

        //Amsn
//        dir.clear();
//        dir.append(home);
//        dir.append("/.amsn");
//        directory.setPath(dir);
//        if (directory.exists(dir)){
//            QDir::setCurrent(dir);
//            file.setFileName("accounts.xml");
//            if (file.exists()){
//                m_existPrograms = true;
//                IMProgram* p = new Amsn();
//                p->setName("Amsn");
//                m_programs.append(p);
//            }
//        }

        //Mercury
//        dir.clear();
//        dir.append(home);
//        dir.append("/.Mercury");
//        directory.setPath(dir);
//        if (directory.exists(dir)){
//            QDir::setCurrent(dir);
//            file.setFileName("accounts.xml");
//            if (file.exists()){
//                m_existPrograms = true;
//                IMProgram* p = new Mercury();
//                p->setName("Mercury");
//                m_programs.append(p);
//            }
//        }

        //Psi
//        dir.clear();
//        dir.append(home);
//        dir.append("/.psi");
//        directory.setPath(dir);
//        if (directory.exists(dir)){
//            QDir::setCurrent(dir);
//            file.setFileName("accounts.xml");
//            if (file.exists()){
//                m_existPrograms = true;
//                IMProgram* p = new Psi();
//                p->setName("Psi");
//                m_programs.append(p);
//            }
//        }

        //Sim
//        dir.clear();
//        dir.append(home);
//        dir.append("/.sim");
//        directory.setPath(dir);
//        if (directory.exists(dir)){
//            QDir::setCurrent(dir);
//            file.setFileName("accounts.xml");
//            if (file.exists()){
//                m_existPrograms = true;
//                IMProgram* p = new Sim();
//                p->setName("Sim");
//                m_programs.append(p);
//            }
//        }

        if (m_existPrograms==false){
                qDebug() << "Instant Message Programs not found";
        }
        m_numberPrograms=m_programs.count();
}
