///////////////////////////////////////////////////////////////////////////////
//
// 
// Module name:		affine1.h
// Contents:		Class definition of the class affine1
//
// Author:		Marten Hillebrand
// Last modified:	
// By:
//
///////////////////////////////////////////////////////////////////////////////

#include "baseModel.h"
#include "rand_var.h"

///////////////////////////////////////////////////////////////////////////////
//
// 
// Class name:		affine1
// Derived from:	baseModel
// Purpose:		defines a family of affine maps
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

class affine1: public baseModel {

protected:
    real x;					// state variable
    real x0;				// initial value for x
	real r;					//intercept
   	real a;					// f_a(x) = a*(x+r)

	rand_var *zvar;
    char zvar_expr[256];

public:
    affine1();			// constructor
    virtual ~affine1();	//destructor
	void loadParamset(ifstream&);	
    void saveParamset(ofstream&);
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


