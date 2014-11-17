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

	real K, k;				// State variable capital (intensity)
	real k_old;				// Capital intensity from the previous period
	real C, c;				// Consumption (per capita)
	real Y, y;				// Aggregate output(per capita)
	real k_rate;			// Growth rate of capital intensity
	real ln_k_rate;			// Growth rate of capital intensity
	real k_diff;			// First differences of capital intensity
	real k_ln;				// Log of k
   
	real k_rate_old;		// Growth rate of capital intensity from the previous period
	real k_diff_old;		// First differences of capital intensity from the previous period
	real k_ln_old;			// Log of k from the previous period
   	real k_0;				// Initial value for k
	real Z_0;				// Initial value for Z
	real w;					// Real wage
	real r;					// Real interest rate



    real epsilon;			// A random variable which is uniformly distributed on the intervall [;]
	real eta;				// A random variable which is a function of epsilon
	real Z;					// Random perturbation of production 
	real Z_ln;				// Logarithm of Z


	rand_var *zvar;
    char zvar_expr[256];

	// 2. Parameters


	real alpha;				// Parameter of the production function
	real beta;				// Discount factor
	real mu, gamma;			// Parameters which govern the stochastic evolution of Z
	real epsilon_param;		// Parameters which govern the stochastic evolution of Z
	

public:
    RBC_basic_model();			// constructor
    virtual ~RBC_basic_model();	//destructor
	void loadParamset(ifstream&);	
    void saveParamset(ofstream&);
    void printParamset();
    void iteration(const long&);
    void initialize();
    real* sendModelVar();
    real* setLabels(char*);
    void sendStateSpace(int &,const real***);
    void sendParameters(int&,real**); 	// write all parameters
                                	// into an array and return the 
                                	// numbers of parameters
                                	
    void receiveParameters(const real*);// receive parameter values 
};


