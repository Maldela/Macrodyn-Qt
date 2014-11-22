///////////////////////////////////////////////////////////////////////////////
//
// $Header: samuelson1.h Thu Jul  6 16:54:07 METDST 2000 mmeyer 
//
// Module name:		samuelson1.h
// Contents:		Class definition of the class 
//
// Author:		
// Last modified:	Thu Jul  6 16:54:53 METDST 2000
// By:			
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _SAMUELSON1_INCLUDED
#define _SAMUELSON1_INCLUDED

#include "baseModel.h"
#include "rand_var.h"
#include "../matrix.h"


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		samuelson1
// Derived from:	baseModel
// Purpose:		defines the samuelson
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

class samuelson1 : public baseModel {

protected:
	matrixDef *C;		// contains results of calculation
	matrixDef *D;		// contains results of addition
	matrixDef *A;		// currently drawn A-matrix (2x2) 
	matrixDef *b;		// currently drawn b-vector (2x1) 
	matrixDef *V;		// contains all entered values
	matrixDef *x;		// current x-vector (2x1)
	matrixFn *point;	// needed to use functions of matrixFn-class
	rand_var *zvar;		// needed for calculation of random number
    qreal x1_tp1;		// needed to save values of calculation
    qreal x2_tp1;		// needed to save values of calculation
    qreal z;				// random number in [0,1]
    qreal p1, m01, v01, m1, v1;	// entered values from sim-file
    qreal p2, m02, v02, m2, v2;
    qreal p3, m03, v03, m3, v3;
    qreal p4, m04, v04, m4, v4;
    qreal p5, m05, v05, m5, v5;
    qreal p6, m06, v06, m6, v6;
    qreal p7, m07, v07, m7, v7;
    qreal p8, m08, v08, m8, v8;
    qreal p9, m09, v09, m9, v9;
    qreal p10, m010, v010, m10, v10;
    qreal x01, x02;				// initial values
    qreal xBundle;
		
	unsigned short N;   // number of lines in sim-file
	int flag;			// 0 if value out of range
	void check();		// checks if values are out of range


public:
    samuelson1();				// constructor
    virtual ~samuelson1();		// destructor
    void loadParamset(ifstream&);	// load parameterset from a file
    void saveParamset(ofstream&);
    void printParamset();
    void sendParameters(int &,qreal**);
    qreal* sendModelVar(void);
    void receiveParameters(const qreal*);
    qreal* setLabels(char*);		// get a variable by a name
    void initialize(void);		// initialize the model
    void iteration(const long&);	// perform one iteration 	
    void sendStateSpace(int &,const qreal***);

};

#endif //  _SAMUELSON1_INCLUDED
