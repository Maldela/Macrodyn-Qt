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
    qreal p;		//qreal price
    qreal p_e;	//expected price
    qreal p_e_old;	//old expected price
    qreal p_e_0;	//initial expectation
    qreal q;		//equiblibrium quantity
    qreal e_t;	//prediction error
    qreal dummy;	//differing pupose dummy variable
	
    qreal lambda;
    qreal a;
    qreal b;
    qreal w;

public:
    cobweb();			// constructor
    void loadParamset(ifstream&);
    void saveParamset(ofstream&);
    void saveParamsetWithNames(ofstream&);
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
    qreal theta_0;
    qreal theta_t;
    qreal X_0;
    qreal X_t;
    qreal lambda_t;
    qreal p_0;
    qreal p_t1;

public:
	void loadParamset( ifstream& inFile );
	void initialize();
	void iteration( const long& );
    qreal* setLabels(char*);
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
    qreal *zvar_ptr;			//pointer to the org/stoch parameter
    qreal buffer_a;			//buffer to store original value
	
public:
	rdemand_cobweb();			//constructor
	void loadParamset( ifstream& );		//load parameterset
	void iteration( const long& );		//iterate ones
	void initialize(void);			//initialize
};

#endif	
// eof
