#ifndef PUREEXCHANGE_H
#define PUREEXCHANGE_H
//$Header: pureExchange.h
/******************************************************************************/
/*                                                                            */
/* Module name:  pureExchange.h                                               */
/* Contents:     Class definition of the class pureExchange 	              */
/*                                                                            */
/* Last Modified: 07.09.1995 (Markus Lohmann)                                 */
/* Modified:      07.09.95 excOlgAdapt and excOlgGeoExp added (M. Lohmann)    */
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"

class pureExchange : public baseModel {
  protected:
	real job_type;				// current type of job (used for jbo 83)
	real err;					// error function : theta_t - theE_Old
	real sf_type;				// the current type of the savings function
    real theta_e_m1;            // initial inflation rate for period -1                
	real theta_e_null;			// initial inflation rate for period 0
	real gamma;					// gross rate of currency growth
	real p0;					// initial price
	real w1, w2;				// initial endowment for t,t+1	
	real g0;					// initial parameter for exp.Infl.rate OLS
	real g_tm1;					// parameter for exp.Infl.rate OLS
	real rho, delta;			// parameter for the savings function
    real theta_tm1;		        // inflation rate in t-1
    real theta_t;		        // inflation rate in t
	real theE_Old;				// expected inflation rate for period t
	real theE_You;			// expected inflation rate for period t+1
	real thetaOld;
	real theE_Old2;
	real theE_You2;
	real pt;					// price in t
	real p_tm1;					// price in t-1
    void expInflRateYoung();  	// algorithm for theE_You                      
    real (*savingsFunc)(real,real,real,real,real);    //pointer to the savings function  
	void sf_init ();			// sets savingsFunc

public:
    pureExchange();			   		 // default constructor
    virtual ~pureExchange();		    // destructor   
    void loadParamset(ifstream&);	    // load parameterset from a file
    void saveParamset(ofstream&);	    // write parameterset into a file
    void printParamset();		    	// print parameterset on the screen
    void iteration(const long&);    	// perform one iteration    
	real* setLabels(char*);		    	// return a pointer to a variable
					    				// or a parameter specified by its
					    				// name
    void initialize();			    	// initialize the model, define
					    				// the initial state
    real* sendModelVar();		    	// return the main model variable
    void sendStateSpace(int &,const real***); 	// return pointers to all model
					    						// variables and the dimension
					    						// of the model
    void sendParameters(int&,real**); 	// write all parameters
                                		// into an array and return the numbers
										// of parameters
    void receiveParameters(const real*); 		// receive parameter values 
};

#endif // PUREEXCHANGE_H

// eof
