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
	real Ed;			// Variable für den Erwartungswert
	real Vd;			// Variable für die Varianz
	real d;				// Variable für die Zufallsvariable
	real d2;
	distribution distri;
	normal_Distri distri2;
	
	real U,s1,s2;
	
	real x,y,z,a,b,c,x0,y0,z0,dx,dy,dz;

public:
    test_model();			// constructor
    virtual ~test_model();		// destructor
    void loadParamset(ifstream&);
    void saveParamset(ofstream&);
    void saveParamsetWithNames(ofstream&);
    void printParamset();
    void iteration(const long&);
    void initialize();
    real* sendModelVar();
    real* setLabels(char*);
    void sendStateSpace(int &,const real***);
    void sendParameters(int&,real**); 	// write all parameters
                                	// into an array and return the 
                                	// numbers of parameters
                                	
    void receiveParameters(const real*);// receive parameter values 
};

#endif	// _LOGISTICMAP_INCLUDED

// eof
