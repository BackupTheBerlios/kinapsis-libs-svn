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

#ifndef _MANAGER_
#define _MANAGER_

#include "improgram.h"
//Clase manager para interactuar unicamente con la clase abstracta IMProgram

class Manager{
        private:
            int m_numberIMP;
            QList<IMProgram*> m_programs; //Vector con los ptros a las instancias de Kopete,pidgin...
            bool m_existPrograms;

            void searchIMPrograms(); 
            
        public:
            Manager();
            ~Manager();
            bool existIMPrograms(); 
            QList<IMProgram*> getIMPrograms();
            int getNumberIMPrograms();
    
}; //fin declaracion clase Manager

#endif
