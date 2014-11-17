///////////////////////////////////////////////////////////////////////////////
//
// Module name:		Samuelson_basic.h
// Contents:		Class definition of the class Samuelson_basic
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _Samuelson_basic_INCLUDED
#define _Samuelson_basic_INCLUDED

#include "baseModel.h"
#include "distribution.h"

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		Samuelson_basic
// Derived from:	baseModel
// Purpose:		simple onedimensional dynamics
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

class Samuelson_basic : public baseModel {

protected:
	real	m_0;
	real	v_0;
	real	m;
	real	v;
	real	y1_0;
	real	y2_0;
	real	lambda;
	real	noise;
	real	v_0noise, v_0prob, v_0max, v_0min;
	long	myseed;
	
	real y;			// state variable
	real y1;
	real y2;
	distribution	distri;

public:
    Samuelson_basic();			// constructor
    virtual ~Samuelson_basic();
    void loadParamset(ifstream&);	
    void saveParamset(ofstream&);
    void saveParamsetWithNames(ofstream&);    
    void printParamset();
    virtual void iteration(const long&);
    void initialize();
    real* sendModelVar();
    real* setLabels(char*);
    void sendStateSpace(int &,const real***);
    void sendParameters(int&,real**); 	// write all parameters
                                	// into an array and return the 
                                	// numbers of parameters
                                	
    void receiveParameters(const real*);// receive parameter values 
};


class Samuelson_ar1 : public Samuelson_basic {

protected:
	real a;
	real xi;

public:
	void iteration(const long& );
	void loadParamset(ifstream& );
	void initialize();
	real* setLabels(char* label);
};

#endif	

// eof
