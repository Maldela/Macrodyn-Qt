///////////////////////////////////////////////////////////////////////////////
//
//
// Module name:		EnRAssH.h
// Contents:		Class definition of the class EnRAssH
//			
//
// Author:		Marc Mueller
// Last modified:	
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

#ifndef EnRAssH_H
#define EnRAssH_H

#include "baseModel.h"
#include "rand_var.h"

class EnRAssH : public baseModel {

protected:
	qreal	N;			// finite number of consumers
	qreal	e;			// initial endowment
	qreal	R;			// risk free rate of return
	qreal	x_;			// fixed number of shares
	qreal	c_a;			// consum of a
    qreal	alpha,alphaqMin;		// risk aversion
	qreal	theta0,theta,thetaOld;	// price expectation
	qreal	*thetaPtr;
	qreal	thetaE,thetaDiff;	// error free theta
    qreal	dmax,d,dOld,dmid;	// random dividend
	qreal	*dPtr;
    qreal	p0,pqMax,p,pOld,pmid;	// stockprice
	qreal	*pPtr;
	qreal	MEP;			// 
	qreal	eps,epsmid,avEps,relEps,avRelEps;// error function
    QString	learntype;		// Name of update function
	int	mem;			// Memory length

	rand_var * zvar;		// ptr random function   

	qreal get_new_d();		// get new random dividend

    qreal (*_learnAgols) (qreal*,qreal*,const qint64&,const qint64&);
    qreal (EnRAssH::*learn) (qreal*,qreal*,const qint64&,const qint64&);
    void   learn_init ();
    qreal  learnAgols (qreal*,qreal*,const qint64&,const qint64&);
    qreal  seroError (qreal*,qreal*,const qint64&,const qint64&);
    qreal  constTheta (qreal*,qreal*,const qint64&,const qint64&);
    qreal  rational (qreal*,qreal*,const qint64&,const qint64&);

public:

	EnRAssH();
	~EnRAssH();
    qreal* setLabels(const QString&);
	void initialize();
	void loadParamset(QTextStream&);	
	void saveParamsetWithNames(QTextStream&); 
//	void printParamset();
	void sendStateSpace(int &,const qreal***);
	void iteration(const qint64&);

};

#endif
