#include <iostream>

using namespace std;

#include "improgram.h"
#include "kopete.h"

//Clase abstracta padre para las clases derivadas Kopete, Pidgin, Psi...

            IMProgram::IMProgram()
            :   name("Null"),version("Null"),accounts(),protocols(),metacontacts(),logs()
            {}

            IMProgram::IMProgram(QString nom, QString ver)
            :   name(nom),version(ver)
            {}

            IMProgram::IMProgram(QString nom, QString ver, QList<QString> acc, QList<QString> pro, 
                                                QList<QString> con, QList<QString> lg)
            :   name(nom),version(ver),accounts(acc),protocols(pro),metacontacts(con),logs(lg)
            {}

            IMProgram::~IMProgram()
            {}
        
            //Classes containing pure virtual methods are termed "abstract;" they cannot be instantiated directly, 
            //and a subclass of an abstract class can only be instantiated directly if all inherited pure virtual 
            //methods have been implemented by that class or a parent class. Pure virtual methods typically have 
            //a declaration (signature) and no definition (implementation)
            //Although an implementation of the pure virtual function is typically not provided 
            //in an abstract class, it may be included. Every non-abstract child class is still 
            //required to override the method, but the implementation provided by the abstract class may be called
            
// fin imp.cpp
