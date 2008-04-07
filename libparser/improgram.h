#include <iostream>
#include <QString>
#include <QList>

using namespace std;

#ifndef _IMProgram_
#define _IMProgram_

//Clase padre abstracta para las clases derivadas Kopete, Pidgin, Psi...

class IMProgram{
        private:

        protected: //inherited
            QString m_name;
            QString m_version;
            QList<QString> m_metacontacts;
            QList<QString> m_accounts;
            QList<QString> m_protocols; //asocia a una cuenta su conf
            QList<QString> m_logs;
            //cuentas
            
        public:
            IMProgram();
            IMProgram(QString nom, QString ver);
            IMProgram(QString nom, QString ver, QList<QString> acc, QList<QString> pro, 
                                                QList<QString> con, QList<QString> lg);
            ~IMProgram();

            virtual QString getName()=0;
            virtual QString getVersion()=0;
            virtual void setName(QString name)=0;
            virtual void setVersion(QString version)=0;
            virtual void parser()=0; //rellena los atrib. contacts, protocols, accounts y logs
            virtual QList<QString> getMetaContacts()=0;
            virtual QList<QString> getAccounts()=0;
            virtual QList<QString> getProtocols()=0;
            virtual QList<QString> getLogs()=0;
            virtual void saveLogs()=0;

}; //fin declaracion clase IMProgram (Instant Message Program)

#endif
