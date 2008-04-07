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
