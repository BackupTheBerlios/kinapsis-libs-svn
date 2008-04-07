#include <iostream>
#include <QString>
#include <QtDebug>

using namespace std;

#include "manager.h"

int main(){
        bool existImp;
        bool hay;
        QList<IMProgram*> programs;
        QList<QString> accounts;
        QList<QString> protocols;
        QList<QString> metacontacts;
        QList<QString> logs;
        IMProgram* p;
        Manager manager;

        existImp = manager.existIMPrograms(); 

        if (existImp){
                programs = manager.getIMPrograms();

                QListIterator<IMProgram*> i(programs);
                int j = 0;
                while (i.hasNext()){
                    p = programs[j];
                    qDebug() << "PROGRAM FOUND:";
                    qDebug() << "\t" << p->getName() << p->getVersion() << endl;
                    p->parser(); //esta llamada rellena los atrib. accounts, protocols, contacts y logs
                    accounts = p->getAccounts();
                    protocols = p->getProtocols();
                    metacontacts = p->getMetaContacts();
                    logs = p->getLogs();
                    qDebug() << "\tAccounts:";
                    qDebug() << "\t" << accounts << endl;
                    qDebug() << "\tProtocols:";
                    qDebug() << "\t" << protocols << endl;
                    qDebug() << "\tMeta contacts:";
                    qDebug() << "\t" << metacontacts << endl;
                    qDebug() << "\tLogs:";
                    qDebug() << "\t" << logs << endl;
                    p->saveLogs(); //esta llamada salva el atrib. logs en el fichero log de kinapsis
                    i.next();
                    j++;
                }
        }
        else{
                cout << "No found previous Instant Message Programs installed\n"; 
        }
}//fin programa para testear clase manager->improgram-> y derivadas(kopete,pidgin,etc)
