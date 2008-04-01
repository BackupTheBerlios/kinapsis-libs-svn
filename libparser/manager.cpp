#include <iostream>
#include <QtDebug>
#include <QFile>
#include <QDir>
#include <QTextStream>

using namespace std;

#include "manager.h"
#include "improgram.h"
#include "kopete.h"

//Clase Manager para interactuar unicamente con la clase IMProgram

/* Public methods */

    Manager::Manager()
    : numberIMP(0), existPrograms(false)
    { }

    Manager::~Manager()
    { }

    bool Manager::existIMPrograms(){
            searchIMPrograms();
            return existPrograms;
    }

    QList<IMProgram*> Manager::getIMPrograms(){
            return programs;
    }

    int Manager::getNumberIMPrograms(){
            return numberIMP;
    }

/* Private methods */

    void Manager::searchIMPrograms(){
            IMProgram * p;
            QFile file;
            QString home = QDir::homePath();
            QString dir;

            //TODO: pidgin,psi,gaim,sim,amsn,mercury
            //FIXME: path sacarlo con kde, usar exp. regulares
            //para sacar los ~/.kde*

            //Busqueda de kopete 3.5
            dir.append(home);
            dir.append("/.kde3.5/share/config");
            QDir::setCurrent(dir);
            file.setFileName("kopeterc");
            if (file.exists()){
                 existPrograms = true;
                 p = new Kopete("kopete", "3.5");
                 programs.append(p);
                 numberIMP=numberIMP++;
            }
            else{
                qDebug() << "Kopete in kde3.5 not found";
            }

            //Busqueda de kopete 4.0
            dir.clear();
            dir.append(home);
            dir.append("/.kde4.0/share/config");
            QDir::setCurrent(dir);
            file.setFileName("kopeterc");
            if (file.exists()){
                 existPrograms = true;
                 p = new Kopete("kopete", "4.0");
                 programs.append(p);
                 numberIMP=numberIMP++;
            }
            else{
                qDebug() << "Kopete in kde4.0 not found";
            }
    }

// fin manager.cpp
