///////////////////////////////////////////////////////////////////////////////
//
// Module name:		HicksModel.h
// Contents:		Class definition of the class HicksModel
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _HicksModel_INCLUDED
#define _HicksModel_INCLUDED

#include "baseModel.h"
#include "distribution.h"

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		HicksModel
// Derived from:	baseModel
// Purpose:		simple onedimensional dynamics
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

class HicksModel : public baseModel {

protected:
	qreal y1_0;			// parameter
	qreal y2_0;
	qreal If;
	qreal Iaut;
	qreal Yc;
	qreal v;
	qreal m;
	
	distribution distri;
	qreal lambda;
	qreal noise;
	long myseed;
	
	qreal y2;			// state variable
	qreal y1;			// old state
	qreal Iind;
	qreal It;
	qreal Ct;

public:
    HicksModel();			// constructor
    virtual ~HicksModel();		// destructor
    void loadParamset(ifstream&);	
    void saveParamset(ofstream&);
    void saveParamsetWithNames(ofstream&);    
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

#endif	

// eof
