#include <QStringList>
#include <QtDebug>

#include "manager.h"

int main(){
        bool existImp;
        bool hay;
        QList<IMProgram*> programs;
        QStringList accounts;
        QStringList protocols;
        QStringList metacontacts;
        QStringList logs;
        IMProgram* p;
        Manager manager;

        existImp = manager.existIMPrograms(); 

        if (existImp){
                programs = manager.imPrograms();

                QListIterator<IMProgram*> i(programs);
                int j = 0;
                while (i.hasNext()){
                        p = programs[j];
                        qDebug() << ("\033[01;32m");
                        qDebug() << "PROGRAM FOUND:";
                        qDebug() << "\t" << p->name();
                        if (p->version()!= "")
                            qDebug() << p->version() << endl;
                        qDebug() << ("\033[00m");
                        p->parser();
                        accounts = p->accounts();
                        protocols = p->protocols();
                        metacontacts = p->metacontacts();
                        logs = p->logs();
                        qDebug() << ("\033[01;33m");
                        qDebug() << "\tAccounts:";
                        qDebug() << ("\033[00m");
                        qDebug() << "\t" << accounts << endl;
                        qDebug() << ("\033[01;33m");
                        qDebug() << "\tProtocols:";
                        qDebug() << ("\033[00m");
                        qDebug() << "\t" << protocols << endl;
                        qDebug() << ("\033[01;33m");
                        qDebug() << "\tMetacontacts:";
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
                qDebug() << "No found previous Instant Message Programs installed\n"; 
        }
}
