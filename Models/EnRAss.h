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
    QString	learntype;		// Name of update function
    qint64	mem;			// Memory length

	rand_var * zvar;		// ptr random function   

	qreal get_new_d();		// get new random dividend

    qreal (*_learnAgols) (qreal*,qreal*,const qint64&,const qint64&);
    qreal (EnRAss::*learn) (qreal*,qreal*,const qint64&,const qint64&);
    void   learn_init ();
    qreal  learnAgols (qreal*,qreal*,const qint64&,const qint64&);
    qreal  seroError (qreal*,qreal*,const qint64&,const qint64&);
    qreal  constTheta (qreal*,qreal*,const qint64&,const qint64&);
    qreal  rational (qreal*,qreal*,const qint64&,const qint64&);

public:

	EnRAss();
	~EnRAss();
    qreal* setLabels(const QString&);
	void initialize();
	void loadParamset(QDataStream&);	
	void saveParamsetWithNames(QDataStream&); 
//	void printParamset();
	void sendStateSpace(int &,const qreal***);
	void iteration(const qint64&);

};

#endif
