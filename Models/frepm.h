///////////////////////////////////////////////////////////////////////////////
//
// $Header: frepm.h Thu Jul  6 16:54:07 METDST 2000 mmeyer 
//
// Module name:		frepm.h
// Contents:		Class definition of the class 
//
// Author:		
// Last modified:	Thu Jul  6 16:54:53 METDST 2000
// By:			
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _FREPM_INCLUDED
#define _FREPM_INCLUDED

#include "baseModel.h"
#include "rand_var.h"
#include "../matrix.h"


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		frepm
// Derived from:	baseModel
// Purpose:		defines the samuelson
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

class frepm : public baseModel {

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
	qreal p1, m011, m021, i011, i021, i11, i21, m11, m21, c11, c21;	// entered values from sim-file
	qreal p2, m012, m022, i012, i022, i12, i22, m12, m22, c12, c22;
	qreal p3, m013, m023, i013, i023, i13, i23, m13, m23, c13, c23;
	qreal p4, m014, m024, i014, i024, i14, i24, m14, m24, c14, c24;
	qreal p5, m015, m025, i015, i025, i15, i25, m15, m25, c15, c25;
	qreal p6, m016, m026, i016, i026, i16, i26, m16, m26, c16, c26;
	qreal p7, m017, m027, i017, i027, i17, i27, m17, m27, c17, c27;
	qreal p8, m018, m028, i018, i028, i18, i28, m18, m28, c18, c28;
	qreal p9, m019, m029, i019, i029, i19, i29, m19, m29, c19, c29;
	qreal p10, m0110, m0210, i0110, i0210, i110, i210, m110, m210, c110, c210;
	qreal x01, x02;				// initial values
		
	unsigned short N;   // number of lines in sim-file
	int flag;			// 0 if value out of range
	void check();		// checks if values are out of range


public:
    frepm();				// constructor
    virtual ~frepm();		// destructor
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

#endif //  _frepm_INCLUDED
