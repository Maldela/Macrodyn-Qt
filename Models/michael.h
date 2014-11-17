///////////////////////////////////////////////////////////////////////////////
//
// $Header: michael.h,v 1.13 99/03/15 14:24:43 mmeyer Exp $
//
// Module name:		michael.h
// Contents:		Class definition of the class growth_model_m,
//			a dynamik growth model 
//
// Author:		Michael Meyer
// Last modified:	Mon Mar 15 12:40:17 METDST 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _MICHAEL_INCLUDED
#define _MICHAEL_INCLUDED

#include "baseModel.h"


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		growth_model_m
// Derived from:	baseModel
// Purpose:		defines the basic growth_model
//
// Author:		Michael Meyer
// Last modified:	Mon Mar 15 12:40:08 METDST 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

class michael : public baseModel {

protected:
    			// original state variables
    real K;		// kapital
    real L;		// labour
    real k;		// transformed state variable k:=K/L

    real k_0;		// initial value
    real z;
    real z_0;
    real tau;
    real gamma;
    real rho;
    
    
    			// derived state variables
    real y;		// output
    real w;		// wage
    real r;		// rent


				// production function
    real ( * pf ) ( real k, real z, real b, real r );	
    				// a pointer to the production function
    real ( * pf_prime) ( real k, real z, real b, real r );
    				// and its derivative

    real pf_type;		// the current type of the production function
    real a, b, c, alpha;	// original parameters
    real A, B, C, D;		// parameters as ][0,1][ normalized input
    void pf_init ( void );	// maps (A,B,C) -> (a,b,c) and sets pf, pf_prime
    
    virtual real sf_y ( real );	// savings function for the entire economy
    real syr;			// parameter for sf_y
    
    
     			
    real sav_y;		// savings w.r.t. the output

    real n;		// exogenous growth of labour suppling people
   
     

public:
    michael();			// constructor
    void loadParamset(ifstream&);	
    void saveParamset(ofstream&);
    void printParamset();
    void iteration(const long&);
    void initialize();
    real* setLabels(char*);
    void sendStateSpace(int &,const real***);

    real* sendModelVar();		// for compatibity reasons only
    void sendParameters(int&,real**);
    void receiveParameters(const real*);
    virtual ~michael(void);
};

#endif //  _MICHAEL_INCLUDED

// eof
