///////////////////////////////////////////////////////////////////////////////
//
// Module name:		cobweb.h
// Contents:		Class definition of the class cobweb
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _cobweb_INCLUDED
#define _cobweb_INCLUDED

#include "baseModel.h"
#include "rand_var.h"

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		cobweb
// Derived from:	baseModel
// Purpose:		simple onedimensional dynamics
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

class cobweb : public baseModel {

protected:
	real p;		//real price
	real p_e;	//expected price
	real p_e_old;	//old expected price
	real p_e_0;	//initial expectation
	real q;		//equiblibrium quantity
	real e_t;	//prediction error
	real dummy;	//differing pupose dummy variable
	
	real lambda;	
	real a;
	real b;
	real w;

public:
    cobweb();			// constructor
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


///////////////////////////////////////////////////////////////////////////////
//
// Class name:		cobweb_RLS
// Derived from:	cobweb
// Purpose:		simple onedimensional dynamics with recursive
//			least squares in expectations
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

class cobweb_RLS : public cobweb {
private:
	real theta_0;
	real theta_t;
	real X_0;
	real X_t;
	real lambda_t;
	real p_0;
	real p_t1;

public:
	void loadParamset( ifstream& inFile );
	void initialize();
	void iteration( const long& );
	real* setLabels(char*);
};


///////////////////////////////////////////////////////////////////////////////
//
// Class name:		rdemand_cobweb
// Derived from:	cobweb
// Purpose:		simple onedimensional dynamics with stochastically
//			disturbed demand parameter a
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

class rdemand_cobweb : public cobweb {
private:
	char zvar_expr[256];		//expression which defines a random variable
	rand_var *zvar;			//a random variable
	char zvar_name[256];		//name of the stochastified parameter
	real *zvar_ptr;			//pointer to the org/stoch parameter
	real buffer_a;			//buffer to store original value
	
public:
	rdemand_cobweb();			//constructor
	void loadParamset( ifstream& );		//load parameterset
	void iteration( const long& );		//iterate ones
	void initialize(void);			//initialize
};

#endif	
// eof
