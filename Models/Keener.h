///////////////////////////////////////////////////////////////////////////////
//
// Module name:		Keener.h
// Contents:		Class definition of the class Keener
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _Keener_INCLUDED
#define _Keener_INCLUDED

#include "baseModel.h"
#include "distribution.h"

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		Keener
// Derived from:	baseModel
// Purpose:		simple onedimensional dynamics
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

class Keener : public baseModel {

protected:
	real	x_0;
	real	a;
	real	b;
	real	Modulo;
	real	lambda;
	real	bias;
	long	myseed;
	
	real x;			// state variable
	distribution	distri;

public:
    Keener();			// constructor
    virtual ~Keener();		// destructor
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

#endif	

// eof
