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
            IMProgram * p;
            QFile file;
            QString home = QDir::homePath();
            QString dir;

            //TODO: pidgin,psi,gaim,sim,amsn,mercury
            //FIXME: path sacarlo con kde, usar exp. regulares
            //para sacar los ~/.kde*

            dir.append(home);
            dir.append("/.kde/share/config");
            file.setFileName("kopeterc");
            QDir::setCurrent(dir);
            if (file.exists()){
                    m_existPrograms = true;
                    p = new Kopete();
                    p->setName("Kopete");
                    m_programs.append(p);
            }
            else{
                qDebug() << "Kopete not found";
            }
            m_numberIMP=m_programs.count();
    }

// fin manager.cpp
