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
                        qDebug() << ("\033[01;32m");
                        qDebug() << "PROGRAM FOUND:";
                        qDebug() << "\t" << p->getName();
                        if (p->getVersion()!= "Null")
                            qDebug() << p->getVersion() << endl;
                        qDebug() << ("\033[00m");
                        p->parser();
                        accounts = p->getAccounts();
                        protocols = p->getProtocols();
                        metacontacts = p->getMetaContacts();
                        logs = p->getLogs();
                        qDebug() << ("\033[01;33m");
                        qDebug() << "\tAccounts:";
                        qDebug() << ("\033[00m");
                        qDebug() << "\t" << accounts << endl;
                        qDebug() << ("\033[01;33m");
                        qDebug() << "\tProtocols:";
                        qDebug() << ("\033[00m");
                        qDebug() << "\t" << protocols << endl;
                        qDebug() << ("\033[01;33m");
                        qDebug() << "\tMeta contacts:";
                        qDebug() << ("\033[00m");
                        qDebug() << "\t" << metacontacts << endl;
                        qDebug() << ("\033[01;33m");
                        qDebug() << "\tLogs:";
                        qDebug() << ("\033[00m");
                        qDebug() << "\t" << logs << endl;
                        p->saveLogs();
                        i.next();
                        j++;
                }
        }
        else{
                cout << "No found previous Instant Message Programs installed\n"; 
        }
}
