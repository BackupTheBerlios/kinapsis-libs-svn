#include <iostream>
#include <QtDebug>
#include <QDomNode>
#include <QFile>
#include <QDir>

using namespace std;

#include "kopete.h"

//Clase Kopete derivada de la clase padre IMProgram

/* Public methods */

            Kopete::Kopete()
            : IMProgram()
            { }

            Kopete::Kopete(QString nombre, QString version)
            : IMProgram(nombre,version)
            { }
            
            Kopete::Kopete(QString nombre, QString version, QList<QString> accounts, 
                   QList<QString> protocols, QList<QString> metacontacts, QList<QString> logs)
            : IMProgram(nombre,version,accounts,protocols,metacontacts,logs)
            { }

            Kopete::~Kopete(){ }
            
            QString Kopete::getName(){
                return name;
            }
            
            QString Kopete::getVersion(){
                return version;
            }
            
            void Kopete::setName(QString name){
                this->name=name;
            }
            
            void Kopete::setVersion(QString version){
                this->version=version;
            }

            void Kopete::parser(){
            // rellena los atrib. metacontacts, accounts, protocols y logs
                QFile file;
                QDomDocument doc;
                QString home = QDir::homePath();
                QString dir;

                    if (version == "3.5"){
                            //XML para los metacontactos
                            //TODO
                            dir.clear();
                            dir.append(home);
                            dir.append("/.kde3.5/share/apps/kopete");
                            QDir::setCurrent(dir);
                            file.setFileName("contactlist.xml");
                            if (!file.open(QIODevice::ReadOnly)){
                                    return; //FIXME
                            }
                            if (!doc.setContent(&file)) {
                                    file.close();
                                    return;//FIXME
                            }

                            QDomElement docElem = doc.documentElement();
                            QDomNode n = docElem.firstChild();
                            while(!n.isNull()) {
//                                    if (n.isElement())
                                    QDomElement e = n.toElement(); // try to convert the node to an element.
                                    if(!e.isNull()) {
//                                            qDebug() << e.tagName() << endl; // the node really is an element.
                                    }
                                    n = n.nextSibling();
                            } 
                            file.close();
                            //XML para los logs
                            //TODO
                            //buscar en dir ../kopete/logs
                            //MSNProtocol->cuentas,
                            //JabberProtocol->cuentas 
                            //etc, etc

                            //kdeRC format para las cuentas y protocolos
                            //TODO-ING
                            dir.clear();
                            dir.append(home);
                            dir.append("/.kde3.5/share/config");
                            QDir::setCurrent(dir);
                            file.setFileName("kopeterc");
                            if (file.exists()){
                                    file.open(QIODevice::ReadOnly | QIODevice::Text);
                                    QTextStream in(&file);
                                    while (!in.atEnd()) {
                                            QString line = in.readLine();
                                            processLine(line);
                                    }
                                    file.close();
                            }
                    }
                    else if (version == "4.0"){
                            //XML para los metacontactos
                            //TODO
                            dir.clear();
                            dir.append(home);
                            dir.append("/.kde4.0/share/apps/kopete");
                            QDir::setCurrent(dir);
                            file.setFileName("contactlist.xml");
                            if (!file.open(QIODevice::ReadOnly)){
                                    return; //FIXME
                            }
                            if (!doc.setContent(&file)) {
                                    file.close();
                                    return;//FIXME
                            }

                            QDomElement docElem = doc.documentElement();
                            QDomNode n = docElem.firstChild();
                            while(!n.isNull()) {
//                                    if (n.isElement())
                                    QDomElement e = n.toElement(); // try to convert the node to an element.
                                    if(!e.isNull()) {
//                                            qDebug() << e.tagName() << endl; // the node really is an element.
                                    }
                                    n = n.nextSibling();
                            } 
                            file.close();
                            //XML para los logs
                            //TODO
                            //buscar en dir ../kopete/logs
                            //MSNProtocol->cuentas,
                            //JabberProtocol->cuentas 
                            //etc, etc

                            //kdeRC format para las cuentas y protocolos
                            //TODO-ING
                            dir.clear();
                            dir.append(home);
                            dir.append("/.kde4.0/share/config");
                            QDir::setCurrent(dir);
                            file.setFileName("kopeterc");
                            if (file.exists()){
                                    file.open(QIODevice::ReadOnly | QIODevice::Text);
                                    QTextStream in(&file);
                                    while (!in.atEnd()) {
                                            QString line = in.readLine();
                                            processLine(line);
                                    }
                                    file.close();
                            }
                    }
            }


            QList<QString> Kopete::getMetaContacts(){
                    return metacontacts;
            }

            QList<QString> Kopete::getAccounts(){
                    return accounts;
            }

            QList<QString> Kopete::getProtocols(){
                    return protocols;
            }

            QList<QString> Kopete::getLogs(){
                    return logs;
            }

            void Kopete::saveLogs(){
                // salva en fichero log en el estilo de kinapsis el atributo logs
            }

/* Private methods */

            void Kopete::processLine(QString line){
                int pos=-1;
                QRegExp rx;
                QString account;
                QString port;
                QString a;
//                rx.setPatternSyntax(QRegExp::Wildcard);

                  if (line.size() > 0){
                    //Revisar si dominios pueden tener numeros 
                    rx.setPattern("(AccountId)(=)([a-z|A-Z|0-9]+@[a-z|A-Z]+.[a-z|A-Z]+)");  
                    pos = rx.indexIn(line,0);
                    if (pos > -1){
//                            a = rx.cap(1);
//                            port = rx.cap(2);
                            accounts << rx.cap(3);
//                            qDebug() << a;
//                            qDebug() << port;
//                            qDebug() << account;
                    }
                }
            }

// fin kopete.cpp
