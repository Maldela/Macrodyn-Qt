///////////////////////////////////////////////////////////////////////////////
//
// Module name:		ELS.h
// Contents:		Class definition of the class ELS
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _ELS_INCLUDED
#define _ELS_INCLUDED

#include "baseModel.h"
#include "rand_var.h"
#include "markov_chain.h"

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		ELS
// Derived from:	baseModel
// Purpose:		simple onedimensional dynamics
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

class ELS : public baseModel {

protected:
	real c0;			// parameter
	real c1;
	real real_money;
	real G;
	real T;
	real alpha;
	real y0;
	
	real y;				// state variable

public:
    ELS();			// constructor
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
