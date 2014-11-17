///////////////////////////////////////////////////////////////////////////////
//
// Module name:		HicksModel.h
// Contents:		Class definition of the class HicksModel
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _HicksModel_INCLUDED
#define _HicksModel_INCLUDED

#include "baseModel.h"
#include "distribution.h"

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		HicksModel
// Derived from:	baseModel
// Purpose:		simple onedimensional dynamics
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

class HicksModel : public baseModel {

protected:
	real y1_0;			// parameter
	real y2_0;
	real If;
	real Iaut;
	real Yc;
	real v;
	real m;
	
	distribution distri;
	real lambda;
	real noise;
	long myseed;
	
	real y2;			// state variable
	real y1;			// old state
	real Iind;
	real It;
	real Ct;

public:
    HicksModel();			// constructor
    virtual ~HicksModel();		// destructor
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
