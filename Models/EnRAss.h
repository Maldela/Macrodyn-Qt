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
	qreal	N;			// finite number of consumers
	qreal	e;			// initial endowment
	qreal	R;			// risk free rate of return
	qreal	x_;			// fixed number of shares
	qreal	c_a;			// consum of a
	qreal	alpha,alphamin;		// risk aversion
	qreal	theta0,theta,thetaOld;	// price expectation
	qreal	*thetaPtr;
	qreal	thetaE,thetaDiff;	// error free theta
	qreal	dmax,d,dOld,dmid;	// random dividend
	qreal	*dPtr;
	qreal	p0,pmax,p,pOld,pmid;	// stockprice
	qreal	*pPtr;
	qreal	MEP;			// 
	qreal	eps,epsmid,avEps,relEps,avRelEps;// error function
	char	learntype[32];		// Name of update function
    long	mem;			// Memory length

	rand_var * zvar;		// ptr random function   

	qreal get_new_d();		// get new random dividend

    qreal (*_learnAgols) (qreal*,qreal*,const long&,const long&);
    qreal (EnRAss::*learn) (qreal*,qreal*,const long&,const long&);
    void   learn_init ();
    qreal  learnAgols (qreal*,qreal*,const long&,const long&);
    qreal  seroError (qreal*,qreal*,const long&,const long&);
    qreal  constTheta (qreal*,qreal*,const long&,const long&);
    qreal  rational (qreal*,qreal*,const long&,const long&);

public:

	EnRAss();
	~EnRAss();
	qreal* setLabels(char*);
	void initialize();
	void loadParamset(ifstream&);	
	void saveParamsetWithNames(ofstream&); 
//	void printParamset();
	void sendStateSpace(int &,const qreal***);
	void iteration(const long&);

};

#endif
