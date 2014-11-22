///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/logistic.h,v 1.1 2000/08/31 16:00:14 mmeyer Exp $
//
// Module name:		logistic.h
// Contents:		Class definition of the class logistic
//
// Author:		Markus Lohmann
// Last modified:	Fri Jul  4 10:48:34 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _LOGISTICMAP_INCLUDED
#define _LOGISTICMAP_INCLUDED

#include "baseModel.h"
#include "rand_var.h"
#include "markov_chain.h"

///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/logistic.h,v 1.1 2000/08/31 16:00:14 mmeyer Exp $
//
// Class name:		logistic
// Derived from:	baseModel
// Purpose:		defines the logistic map
//
// Author:		Markus Lohmann
// Last modified:	Fri Jul  4 10:49:00 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class logistic : public baseModel {

protected:
    qreal alpha;				// parameter
    qreal x;				// state variable
    qreal x0;				// initial value for x
    qreal x_prime;			// the first derivation at x
    					// used for the calculation of
    					// lyapunov exponents

public:
    logistic();			// constructor
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

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rlogistic
// Derived from:	logistic
// Purpose:		defines the logistic map with random variables
//
// Author:		Uli Middelberg
// Last modified:	Tue May 27 17:42:17 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class rlogistic : public logistic {

protected:
    qreal a,b;			// parameter
    qreal beta;			// parameter used in case of additive noise
 
    char zvar_expr[256];        // expression which defines a random variable
    rand_var * zvar;            // a random variable 

    int  mc_flag;		// flag if either markov chain or zvar
    
    char mc_states[265];	// states and qrealization for each state
    char mc_matrix[1024];	// the transision matrix
    markov_chain * mc;		// a markov chain

    char zvar_name[256];        // name of the stochastified parameter
    qreal * zvar_ptr;		// pointer to the stochastic parameter

public:
    rlogistic();				// constructor
    void iteration(const long&);
    void initialize();
//    qreal* sendModelVar();			// obsolete and unused
    qreal* setLabels(char*);
//  void sendStateSpace(int &,const qreal***);	// obsolete
    void loadParamset(ifstream&);	
    void saveParamset(ofstream&);
    void printParamset();

//    void sendParameters(int&,qreal**); 	// unused
//    void receiveParameters(const qreal*);	// unused 
}; 

#endif	// _LOGISTICMAP_INCLUDED

// eof