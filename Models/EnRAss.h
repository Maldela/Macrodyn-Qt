///////////////////////////////////////////////////////////////////////////////
//
//
// Module name:		EnRAss.h
// Contents:		Class definition of the class EnRAss
//			
//
// Author:		Marc Mueller
// Last modified:	99/05/15
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

#ifndef EnRAss_H
#define EnRAss_H

#include "baseModel.h"
#include "rand_var.h"


class EnRAss : public baseModel {

protected:
	real	N;			// finite number of consumers
	real	e;			// initial endowment
	real	R;			// risk free rate of return
	real	x_;			// fixed number of shares
	real	c_a;			// consum of a
	real	alpha,alphamin;		// risk aversion
	real	theta0,theta,thetaOld;	// price expectation
	real	*thetaPtr;
	real	thetaE,thetaDiff;	// error free theta
	real	dmax,d,dOld,dmid;	// random dividend
	real	*dPtr;
	real	p0,pmax,p,pOld,pmid;	// stockprice
	real	*pPtr;
	real	MEP;			// 
	real	eps,epsmid,avEps,relEps,avRelEps;// error function
	char	learntype[32];		// Name of update function
	int	mem;			// Memory length

	rand_var * zvar;		// ptr random function   

	real get_new_d();		// get new random dividend

	real (*_learnAgols) (real*,real*,int&,long&);
	real (EnRAss::*learn) (real*,real*,int&,long&);
	void learn_init ();
	real  learnAgols (real*,real*,int&,long&);
	real  seroError (real*,real*,int&,long&);
	real  constTheta (real*,real*,int&,long&);
	real  rational (real*,real*,int&,long&);

public:

	EnRAss();
	~EnRAss();
	real* setLabels(char*);
	void initialize();
	void loadParamset(ifstream&);	
	void saveParamsetWithNames(ofstream&); 
//	void printParamset();
	void sendStateSpace(int &,const real***);
	void iteration(const long&);

};

#endif
