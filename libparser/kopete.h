#include <iostream>
#include <QString>
#include <QList>

using namespace std;

#ifndef _Kopete_
#define _Kopete_

#include "improgram.h"

class Kopete : public IMProgram {
        private:
            void processLine(QString line);
            
        public:
            Kopete();
            Kopete(QString nombre, QString version);
            Kopete(QString nombre, QString version, QList<QString> accounts, 
                   QList<QString> protocols, QList<QString> metacontacts, QList<QString> logs);
            ~Kopete();

            QString getName();
            QString getVersion();
            void setName(QString name);
            void setVersion(QString version);
            void parser();
            QList<QString> getMetaContacts();
            QList<QString> getAccounts();
            QList<QString> getProtocols();
            QList<QString> getLogs();
            void saveLogs();

}; //fin declaracion clase Kopete derivada de IMProgram

#endif
