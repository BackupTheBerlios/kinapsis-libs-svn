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
#include "parserkopete.h"
#include "parserpidgin.h"
#include "parserpsi.h"
#include "parsermercury.h"
#include "parseramsn.h"

/* Public methods */

const bool Manager::existIMPrograms(){
        searchIMPrograms();
        return m_existPrograms;
}

/* Private methods */

const int Manager::searchAux(QString program){
        QFile file;
        QString home = QDir::homePath();
        QString dir;
        QDir directory;
        IMProgram* p;

        dir.clear();
        dir.append(home);
        if (program == "kopete")
                dir.append("/.kde/share/config");
        else if (program == "pidgin")
                dir.append("/.purple");
        else
                return -1;
        directory.setPath(dir);
        if (directory.exists(dir)){
                QDir::setCurrent(dir);
                if (program == "kopete")
                        file.setFileName("kopeterc");
                else if (program == "pidgin")
                        file.setFileName("accounts.xml");
                else
                        return -2;
                if (file.exists()){
                        m_existPrograms = true;
                        if (program == "kopete"){
                                p = new ParserKopete();
                                p->setName("Kopete");
                        }else if (program == "pidgin"){
                                p = new ParserPidgin();
                                p->setName("Pidgin");
                        }
                        else
                                return -3;
                        m_programs.append(p);
                }
        }
        return 0;
}

void Manager::searchIMPrograms(){
        int result;

        /*Kopete*/
//        result=searchAux("kopete");
//        if (result !=0)
//            return;   //FIXME
        /*Pidgin*/
        result=searchAux("pidgin");
        if (result !=0)
            return;   //FIXME
        /*Amsn*/
//        result=searchAux("amsn");
//        if (result !=0)
//            return;   //FIXME
        /*Mercury*/
//        result=searchAux("mercury");
//        if (result !=0)
//            return;   //FIXME
        /*Psi*/
//        result=searchAux("psi");
//        if (result !=0)
//            return;   //FIXME

        if (m_existPrograms==false){
                qDebug() << "Instant Message Programs not found";
        }
        m_numberPrograms=m_programs.count();
}
