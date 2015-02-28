///////////////////////////////////////////////////////////////////////////////
//
// 
//
// Module name:		boehmhuels.h
// Contents:		Class definition of the class logistic
//
// Author:		Oliver Claas
// Last modified:	
// By:			Oliver Claas
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _BOEHMHUELSMAP_INCLUDED
#define _BOEHMHUELSMAP_INCLUDED

#include "baseModel.h"
#include "rand_var.h"
#include "markov_chain.h"

///////////////////////////////////////////////////////////////////////////////
//
// 
//
// Class name:		boehmhuels
// Derived from:	baseModel
// Purpose:		defines the logistic map
//
// Author:		Oliver Claas
// Last modified:	
// By:			Oliver Claas
//
///////////////////////////////////////////////////////////////////////////////

class boehmhuels : public baseModel
{
    Q_OBJECT

protected:
    qreal alpha,delta,beta,ybar,r;	// parameter
    qreal xt,yt;			// state variables
    qreal x0,y0;			// initial values for x, y
    qreal pt,rett,absrett,sqrett;    // statistics (prices, returns, absolute returns, sqared returns)


public:
	boehmhuels();				// constructor
    void loadParamset(QTextStream& );
    void saveParamset(QTextStream& );
	void printParamset();
    void iteration(const qint64&);
	void initialize();
    qreal* sendModelVar();
    qreal* setLabels(const QString&);
    void sendStateSpace(int &, QList<qreal *> *);
    void sendParameters(int&,qreal**);	// write all parameters
						// into an array and return the 
						// numbers of parameters
                                	
    void receiveParameters(const QList<qreal>&);	// receive parameter values
};

#endif	// _BOEHMHUELSMAP_INCLUDED

// eof
