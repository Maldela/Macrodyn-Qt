///////////////////////////////////////////////////////////////////////////////
//
// Module name:		Samuelson_basic.h
// Contents:		Class definition of the class Samuelson_basic
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _Samuelson_basic_INCLUDED
#define _Samuelson_basic_INCLUDED

#include "baseModel.h"
#include "distribution.h"

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		Samuelson_basic
// Derived from:	baseModel
// Purpose:		simple onedimensional dynamics
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

class Samuelson_basic : public baseModel {

protected:
    qreal	m_0;
    qreal	v_0;
    qreal	m;
    qreal	v;
    qreal	y1_0;
    qreal	y2_0;
    qreal	lambda;
    qreal	noise;
    qreal	v_0noise, v_0prob, v_0max, v_0min;
	qint64	myseed;
	
    qreal y;			// state variable
    qreal y1;
    qreal y2;
	distribution	distri;

public:
    Samuelson_basic();			// constructor
    virtual ~Samuelson_basic();
    void loadParamset(ifstream&);	
    void saveParamset(ofstream&);
    void saveParamsetWithNames(ofstream&);    
    void printParamset();
    virtual void iteration(const qint64&);
    void initialize();
    qreal* sendModelVar();
    qreal* setLabels(char*);
    void sendStateSpace(int &,const qreal***);
    void sendParameters(int&,qreal**); 	// write all parameters
                                	// into an array and return the 
                                	// numbers of parameters
                                	
    void receiveParameters(const qreal*);// receive parameter values
};


class Samuelson_ar1 : public Samuelson_basic {

protected:
    qreal a;
    qreal xi;

public:
	void iteration(const qint64& );
	void loadParamset(ifstream& );
	void initialize();
    qreal* setLabels(char* label);
};

#endif	

// eof
