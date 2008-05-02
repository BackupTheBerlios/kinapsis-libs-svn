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

#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "improgram.h"

class Manager{
        public:
            Manager();
            ~Manager();
            bool existIMPrograms(); 
            QList<IMProgram*> imPrograms(){return m_programs;};
            int numberPrograms(){return m_numberPrograms;};

        private:
            int m_numberPrograms;
            QList<IMProgram*> m_programs;
            bool m_existPrograms;

            void searchIMPrograms(); 
}; 
#endif //_MANAGER_H_
