///////////////////////////////////////////////////////////////////////////////
//
// 
// Module name:		affine2.h
// Contents:		Class definition of the class affine2
//
// Author:		Marten Hillebrand
// Last modified:	
// By:
//
///////////////////////////////////////////////////////////////////////////////

#include "baseModel.h"
#include "rand_var.h"

///////////////////////////////////////////////////////////////////////////////
//
// 
// Class name:		affine2
// Derived from:	baseModel
// Purpose:		defines a system of two affine maps, each of which is chosen with a specified probability
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

class affine2: public baseModel {

protected:
    qreal a1;				// f1(x) = a1*x
    qreal a2;				// f2(x) = 1 + a2*x
	qreal a;					//	needed for the particular case where a2 = 1-a1
	qreal x;					// state variable
    qreal x0;				// initial value for x
    qreal p;					//probability that map 1 is chosen
	qreal urv;			//a random variable
	int count1, count2; //counter for map1 and map2
	bool incase1;
	rand_var *zvar;
    char zvar_expr[256];

public:
    affine2();			// constructor
    virtual ~affine2();	//destructor
	void loadParamset(ifstream&);	
    void saveParamset(ofstream&);
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


