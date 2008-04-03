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

                //XML para los metacontactos
                //TODO
                dir.append(home);
                dir.append("/.kde/share/apps/kopete");
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
                        // if (n.isElement())
                        // try to convert the node to an element.
                        QDomElement e = n.toElement();
                        if(!e.isNull()) {
                        // // the node really is an element.
                        // qDebug() << e.tagName() << endl; 
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
                dir.append("/.kde/share/config");
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
                QString tmp;
//                rx.setPatternSyntax(QRegExp::Wildcard);
//FIXME: ICQ, AOL
//
                  if (line.size() > 0){
                    //account
                    rx.setPattern("(^AccountId=)([a-z|A-Z|0-9]+@[a-z|A-Z|0-9]+.[a-z|A-Z]+)");
                    pos = rx.indexIn(line,0);
                    if (pos > -1){
                            accounts << rx.cap(2);
                            tmp.clear();
                            tmp.append("Account:");
                            tmp.append(rx.cap(2));
                            protocols << tmp;
                            return;
                    }
                    //Plain text
                    rx.setPattern("(^AllowPlainTextPassword=)([a-z|A-Z]+)");  
                    pos = rx.indexIn(line,0);
                    if (pos > -1){
                            tmp.clear();
                            tmp.append("PlainTextPassword:");
                            tmp.append(rx.cap(2));
                            protocols << tmp;
                            return;
                    }
                    //port (jabber)
                    rx.setPattern("(^Port=)([0-9]+)");  
                    pos = rx.indexIn(line,0);
                    if (pos > -1){
                            tmp.clear();
                            tmp.append("Port:");
                            tmp.append(rx.cap(2));
                            protocols << tmp;
                            return;
                    }
                    //protocol
                    rx.setPattern("(^Protocol=)([a-z|A-Z]+)");  
                    pos = rx.indexIn(line,0);
                    if (pos > -1){
                            tmp.clear();
                            tmp.append("Protocol:");
                            tmp.append(rx.cap(2));
                            protocols << tmp;
                            return;
                    }
                    //server (jabber)
                    rx.setPattern("(^Server=)([a-z|A-Z|0-9|.]+)");  
                    pos = rx.indexIn(line,0);
                    if (pos > -1){
                            tmp.clear();
                            tmp.append("Server:");
                            tmp.append(rx.cap(2));
                            protocols << tmp;
                            return;
                    }
                    //ssl-tls
                    rx.setPattern("(^UseSSL=)([a-z|A-Z]+)");  
                    pos = rx.indexIn(line,0);
                    if (pos > -1){
                            tmp.clear();
                            tmp.append("SSL:");
                            tmp.append(rx.cap(2));
                            protocols << tmp;
                            return;
                    }
                    //MFN  FIXME: posibilidad de caracteres raros
                    rx.setPattern("(^MFN=)([a-z|A-Z|0-9]+)");  
                    pos = rx.indexIn(line,0);
                    if (pos > -1){
                            tmp.clear();
                            tmp.append("MFN:");
                            tmp.append(rx.cap(2));
                            protocols << tmp;
                            return;
                    }
                    //serverName (MSN)
                    rx.setPattern("(^serverName=)([a-z|A-Z|0-9|.]+)");  
                    pos = rx.indexIn(line,0);
                    if (pos > -1){
                            tmp.clear();
                            tmp.append("Server:");
                            tmp.append(rx.cap(2));
                            protocols << tmp;
                            return;
                    }
                    //serverPort (MSN)
                    rx.setPattern("(^serverPort=)([0-9]+)");  
                    pos = rx.indexIn(line,0);
                    if (pos > -1){
                            tmp.clear();
                            tmp.append("Port:");
                            tmp.append(rx.cap(2));
                            protocols << tmp;
                            return;
                    }

                }
            }

// fin kopete.cpp
