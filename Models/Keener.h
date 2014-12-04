///////////////////////////////////////////////////////////////////////////////
//
// Module name:		Keener.h
// Contents:		Class definition of the class Keener
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _Keener_INCLUDED
#define _Keener_INCLUDED

#include "baseModel.h"
#include "distribution.h"

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		Keener
// Derived from:	baseModel
// Purpose:		simple onedimensional dynamics
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

class Keener : public baseModel {

protected:
	qreal	x_0;
	qreal	a;
	qreal	b;
	qreal	Modulo;
	qreal	lambda;
	qreal	bias;
	qint64	myseed;
	
	qreal x;			// state variable
	distribution	distri;

public:
    Keener();			// constructor
    virtual ~Keener();		// destructor
    void loadParamset(ifstream&);	
    void saveParamset(ofstream&);
    void saveParamsetWithNames(ofstream&);    
    void printParamset();
    void iteration(const qint64&);
    void initialize();
    qreal* sendModelVar();
    qreal* setLabels(char*);
    void sendStateSpace(int &,const qreal***);
    void sendParameters(int&,qreal**); 	// write all parameters
                                	// into an array and return the 
                                	// numbers of parameters
                                	
    void receiveParameters(const qreal*);// receive parameter values 
};

#endif	

// eof
