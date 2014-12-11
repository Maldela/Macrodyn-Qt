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

class MerA : public baseModel {

protected:
	int  info;
	int  perfectPredictor;
	int  K;			// number of assets
	qreal *xAll;			// fixed number of shares 
	qreal R;			// risk free rate of return
	qreal *p;
qreal pOld;
	qreal *q;
	qreal *qShift;
	qreal *qOld;
    qreal *qmin;
    qreal *qmax;
	qreal *p0;
	qreal *q0;
 	qreal *ferr;
qreal deltap, deltaq, mef;

	rand_var * zvar;		// ptr random function   
	markov_chain * markov;		// ptr markov chain

qreal mean_ferr,expectation_ferr,variance_ferr,standardDeviation_ferr;
qreal mean_p,expectation_p,variance_p,standardDeviation_p;
qreal mean_deltap,expectation_deltap,variance_deltap,standardDeviation_deltap;
qreal mean_deltaq,expectation_deltaq,variance_deltaq,standardDeviation_deltaq;
qreal mean_d,expectation_d,variance_d,standardDeviation_d;

	qreal *d;
	qreal *d0;
	qreal *dmid;
    qreal *dmin;
    qreal *dmax;
	qreal get_new_d(int &);		// get new random dividend

	qreal *tau1x;

public:

	MerA();
	virtual ~MerA();
	qreal* setLabels(const QString&);
	void initialize();
	void loadParamset(QTextStream&);	
	void saveParamsetWithNames(QTextStream&); 
    void receiveParameters(const QList<qreal>&);
	void sendStateSpace(int &,const qreal***);
	void iteration(const qint64&);

};

#endif
