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
    qreal x;					// state variable
    qreal x0;				// initial value for x
	qreal r;					//intercept
   	qreal a;					// f_a(x) = a*(x+r)

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
    qreal* sendModelVar();
    qreal* setLabels(char*);
    void sendStateSpace(int &,const qreal***);
    void sendParameters(int&,qreal**); 	// write all parameters
                                	// into an array and return the 
                                	// numbers of parameters
                                	
    void receiveParameters(const qreal*);// receive parameter values 
};

