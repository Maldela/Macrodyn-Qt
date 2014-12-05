///////////////////////////////////////////////////////////////////////////////
//
// 
// Module name:		RBC_basic_model.h
// Contents:		Class definition of the class RBC_basic_model
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
// Class name:		RBC_basic_model
// Derived from:	baseModel
// Purpose:		defines a system of n>=1 affine maps, each of which is chosen with a specified probability
//
// Author:		Marten Hillebrand
// Last modified: Don Jan 29 15:02:23 CET 2004	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

class RBC_basic_model: public baseModel {

protected:
	

	

	// 1. Variables

	// Note: Labor N is normalized to unity -> Variables in intensity form have the same value 

	qreal K, k;				// State variable capital (intensity)
	qreal k_old;				// Capital intensity from the previous period
	qreal C, c;				// Consumption (per capita)
	qreal Y, y;				// Aggregate output(per capita)
	qreal k_rate;			// Growth rate of capital intensity
	qreal ln_k_rate;			// Growth rate of capital intensity
	qreal k_diff;			// First differences of capital intensity
	qreal k_ln;				// Log of k
   
	qreal k_rate_old;		// Growth rate of capital intensity from the previous period
	qreal k_diff_old;		// First differences of capital intensity from the previous period
	qreal k_ln_old;			// Log of k from the previous period
   	qreal k_0;				// Initial value for k
	qreal Z_0;				// Initial value for Z
	qreal w;					// qreal wage
	qreal r;					// qreal interest rate



    qreal epsilon;			// A random variable which is uniformly distributed on the intervall [;]
	qreal eta;				// A random variable which is a function of epsilon
	qreal Z;					// Random perturbation of production 
	qreal Z_ln;				// Logarithm of Z


	rand_var *zvar;
    QString zvar_expr;

	// 2. Parameters


	qreal alpha;				// Parameter of the production function
	qreal beta;				// Discount factor
	qreal mu, gamma;			// Parameters which govern the stochastic evolution of Z
	qreal epsilon_param;		// Parameters which govern the stochastic evolution of Z
	

public:
    RBC_basic_model();			// constructor
    virtual ~RBC_basic_model();	//destructor
	void loadParamset(QDataStream&);	
    void saveParamset(QDataStream&);
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


