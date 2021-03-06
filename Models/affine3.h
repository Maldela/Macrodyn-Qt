///////////////////////////////////////////////////////////////////////////////
//
// 
// Module name:		affine3.h
// Contents:		Class definition of the class affine3
//
// Author:		Andreas Starke
// Last modified:	
// By:
//
///////////////////////////////////////////////////////////////////////////////

#include "baseModel.h"
#include "rand_var.h"

///////////////////////////////////////////////////////////////////////////////
//
// 
// Class name:		affine3
// Derived from:	baseModel
// Purpose:		defines a system of two affine maps, each of which is 
//			chosen with a specified probability and distrubed
//			with a uniform stochastic parameter
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

class affine3: public baseModel {

protected:
	qreal a;		// x_1 = f1(x_0)=(a+e)x_0 with p
			// x_1 = f2(x_0)=(a-1+e)x_0+1 with 1-p
	qreal x;		// state variable
	qreal x0;	// initial value for x
	qreal p;		// parametric probability that map 1 is chosen
	qreal eta;	// parametric value for e~[-eta,eta]
	qreal urv_p;	// the random variable p
	qreal urv_e;	// the random variable e
	int count1, count2;	// counter for map1 and map2
	rand_var *zvar1;
	rand_var *zvar2;
    QString zvar_expr1;

public:
	affine3();				// constructor
	virtual ~affine3();			//destructor
	void loadParamset(QTextStream&);	
	void saveParamset(QTextStream&);
	void printParamset();
	void iteration(const qint64&);
	void initialize();
	qreal* sendModelVar();
    qreal* setLabels(const QString&);
    void sendStateSpace(int &, QList<qreal *> *);
	void sendParameters(int&,qreal**); 	// write all parameters
                         	       		// into an array and return the 
                                		// numbers of parameters                                	
    void receiveParameters(const QList<qreal>&);	// receive parameter values
};
