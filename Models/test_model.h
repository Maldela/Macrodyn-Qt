///////////////////////////////////////////////////////////////////////////////
//
// Module name:		test_model.h
// Contents:		Class definition of the class test_model
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _test_model_INCLUDED
#define _test_model_INCLUDED

#include "baseModel.h"
#include "distribution.h"

#include "../distri/normal_distri.h"
#include "../distri/gamma_distri.h"

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		test_model
// Derived from:	baseModel
// Purpose:		
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

class test_model : public baseModel {

protected:
    qreal Ed;			// Variable für den Erwartungswert
    qreal Vd;			// Variable für die Varianz
    qreal d;				// Variable für die Zufallsvariable
    qreal d2;
	distribution distri;
	normal_Distri distri2;
	
    qreal U,s1,s2;
	
    qreal x,y,z,a,b,c,x0,y0,z0,dx,dy,dz;

public:
    test_model();			// constructor
    virtual ~test_model();		// destructor
    void loadParamset(QDataStream&);
    void saveParamset(QDataStream&);
    void saveParamsetWithNames(QDataStream&);
    void printParamset();
    void iteration(const qint64&);
    void initialize();
    qreal* sendModelVar();
    qreal* setLabels(const QString&);
    void sendStateSpace(int &,const qreal***);
    void sendParameters(int&,qreal**); 	// write all parameters
                                	// into an array and return the 
                                	// numbers of parameters
                                	
    void receiveParameters(const qreal*);// receive parameter values
};

#endif	// _LOGISTICMAP_INCLUDED

// eof
