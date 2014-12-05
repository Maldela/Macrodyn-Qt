///////////////////////////////////////////////////////////////////////////////
//
// Module name:		ELS.h
// Contents:		Class definition of the class ELS
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _ELS_INCLUDED
#define _ELS_INCLUDED

#include "baseModel.h"
#include "rand_var.h"
#include "markov_chain.h"

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		ELS
// Derived from:	baseModel
// Purpose:		simple onedimensional dynamics
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

class ELS : public baseModel {

protected:
	qreal c0;			// parameter
	qreal c1;
	qreal qreal_money;
	qreal G;
	qreal T;
	qreal alpha;
	qreal y0;
	
	qreal y;				// state variable

public:
    ELS();			// constructor
    void loadParamset(QDataStream&);	
    void saveParamset(QDataStream&);
    void saveParamsetWithNames(QDataStream&);    
    void printParamset();
    void iteration(const qint64&);
    void initialize();
    qreal* sendModelVar();
    qreal* setLabels(const QString&);
    void sendStateSpace(int &,const qreal***);
    void sendParameters(int&,qreal**); 	// write all parameters
                                	// into an array and return the 
                                	// numbers of parameters
                                	
    void receiveParameters(const qreal*);// receive parameter values 
};

#endif	// _LOGISTICMAP_INCLUDED

// eof
