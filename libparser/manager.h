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
#include <QString>

#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "improgram.h"

class Manager{
        public:
            Manager() : m_numberPrograms(0), m_existPrograms(false) { };
            ~Manager() { };
            const bool existIMPrograms(); 
            inline const QList<IMProgram*>& imPrograms() const  {return m_programs;};
            inline const int& numberPrograms() const            {return m_numberPrograms;};

        private:
            int m_numberPrograms;
            QList<IMProgram*> m_programs;
            bool m_existPrograms;

            void searchIMPrograms(); 
            const int searchAux(QString program); 
}; 
#endif // _MANAGER_H_
