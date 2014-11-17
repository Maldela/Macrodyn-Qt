///////////////////////////////////////////////////////////////////////////////
//
//
// Module name:	MerA.h
// Contents:		Class definition of the class MerA
//			
//
// Author:		Marc Mueller
// Last modified:	99/11/30 Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

#ifndef MerA_H
#define MerA_H

#include "baseModel.h"
#include "markov_chain.h"
#include "../strnchr.h"
#include<stdlib.h>

class MerA : public baseModel {

protected:
	int  info;
	int  perfectPredictor;
	int  K;			// number of assets
	real *xAll;			// fixed number of shares 
	real R;			// risk free rate of return
	real *p;
real pOld;
	real *q;
	real *qShift;
	real *qOld;
	real *qmin;
	real *qmax;
	real *p0;
	real *q0;
 	real *ferr;
real deltap, deltaq, mef;

	rand_var * zvar;		// ptr random function   
	markov_chain * markov;		// ptr markov chain

real mean_ferr,expectation_ferr,variance_ferr,standardDeviation_ferr;
real mean_p,expectation_p,variance_p,standardDeviation_p;
real mean_deltap,expectation_deltap,variance_deltap,standardDeviation_deltap;
real mean_deltaq,expectation_deltaq,variance_deltaq,standardDeviation_deltaq;
real mean_d,expectation_d,variance_d,standardDeviation_d;

	real *d;
	real *d0;
	real *dmid;
	real *dmin;
	real *dmax;
	real get_new_d(int &);		// get new random dividend

	real *tau1x;

public:

	MerA();
	virtual ~MerA();
	real* setLabels(char*);
	void initialize();
	void loadParamset(ifstream&);	
	void saveParamsetWithNames(ofstream&); 
	void sendStateSpace(int &,const real***);
	void iteration(const long&);

};

#endif
