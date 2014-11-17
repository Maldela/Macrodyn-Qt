///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/growth_model.h,v 1.3 2002/01/08 09:04:26 mmeyer Exp mmeyer $
//
// Module name:		growth_model.h
// Contents:		Class definition of the class growth_model,
//			a dynamik growth model including caldor
//
// Author:		Uli Middelberg
// Last modified:	Mon May 11 12:40:17 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _GROWTH_MODEL_INCLUDED
#define _GROWTH_MODEL_INCLUDED

#include "baseModel.h"
#include "rand_var.h"
//#include <compat.h>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		growth_model
// Derived from:	baseModel
// Purpose:		defines the basic growth_model
//
// Author:		Uli Middelberg
// Last modified:	Mon May 11 12:40:08 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

class growth_model : public baseModel {

protected:
    			// original state variables
    real K;		// kapital
    real L;		// labour
    real ln_K;		// ln of K
    real ln_L;		// ln of L
    real d_K_kL;	// difference K_t - k_fix L_t
    real k;		// transformed state variable k:=K/L
    real k_fix;		// fixed point of k

    real k_0;		// initial value
    
    			// derived state variables
    real y;		// output
    real w;		// wage
    real r;		// rent
    real xBundle;

				// production function
    real ( * pf ) ( real k, real a, real b, real c, real alpha );	
    				// a pointer to the production function
    real ( * pf_prime) ( real k, real a, real b, real c, real alpha );
    				// and its derivative
    real ( * pf_second_prime) ( real k, real a, real b, real c, real alpha );
    				// and its derivative
    real pf_type;		// the current type of the production function
    real a, b, c, alpha;	// original parameters
    real A, B, C, D;		// parameters as ][0,1][ normalized input
    void pf_init ( void );	// maps (A,B,C) -> (a,b,c) and sets pf, pf_prime
    
    virtual real sf_y ( real );	// savings function for the entire economy
    real syr;			// parameter for sf_y
    virtual real sf_w ( real ); // savings function for the worker w.r.t the wage
    real swr;			// parameter for sf_w
    virtual real sf_r ( real ); // savings function for the shareholder w.r.t the rent
    real srr;			// parameter for sf_r
    
     			// derived state variables
    real sav_y;		// savings w.r.t. the output
    real sav_w;		// savings w.r.t. the wage
    real sav_r;		// savings w.r.t. the rent

    real n;		// exogenous growth of labour suppling people
   
    real delta_p;	// depreciation of kapital
	real s_hat;
	real Ef_sc;

public:
    growth_model();			// constructor
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

};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rgrowth_model
// Derived from:        growth_model
// Purpose:             defines the growth_model with random variables
//
// Author:              Uli Middelberg
// Last modified:       Tue Aug 19 14:26:59 METDST 1997
// By:                  Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class rgrowth_model : public growth_model {
private:
    char zvar_expr[256];        // expression which defines a random variable
    rand_var * zvar;            // a random variable    
    char zvar_name[256];        // name of the stochastified parameter
    real * zvar_ptr;		// pointer to the stochastic parameter
    
public:
    rgrowth_model();			// constructor
    void loadParamset(ifstream&);	// load parameterset from a file
    void iteration(const long&);	// perform one iteration 
    void initialize(void);		// initialize the model
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          depreciate
// Derived from:        growth_model
// Purpose:             defines the growth_model with several kinds
//			of chaotic depreciation rates
//			delta_p becomes a state variable now
//
// Author:              Uli Middelberg
// Last modified:       Fri Jun 12 14:48:41 METDST 1998
// By:                  Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
const int trans_x_max=100; 		// max. number of i in trans_x[i]

class depreciate : public growth_model {

private:
    real _z;				// variable for the original process
    real _z_0;				// initial value for z
    char _z_var_name[256];		// lexical name of the stochastified parameter
    real * _z_var_ptr;			// pointer to the stochastified parameter

    real trans(real z,real* x,real* a,real* b); // transforms z to delta_p
    real * trans_x;			// pointer to array classes
    real * trans_a;			// parameter for the transformation
    real * trans_b;			// parameter for the transformation

    real (* theta) (real z, real m);	// theta defines the basic dynamic for z
    real theta_type;			// the current type of the theta function
    real my;				// parameters for theta
    void theta_init (void);		// sets theta to a specific type
    
public:
    depreciate();			// constructor
    void sendStateSpace(int &,const real***);
    real * setLabels(char*);		// get a variable by a name
    void initialize(void);		// initialize the model
    void loadParamset(ifstream&);	// load parameterset from a file
    void saveParamset(ofstream&);
    void printParamset();
    void iteration(const long&);	// perform one iteration 
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          pasinetti
// Derived from:        growth_model
// Purpose:             defines the growth_model with several kinds
//			of chaotic depreciation rates
//			delta_p becomes a state variable now
//
// Author:              Marc Mueller
// Last modified:       Mon Dec  7 18:54:51 MET 1998
// By:                  Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

class pasinetti : public growth_model {
private:
    //// k_0                               initial value shareholder
    real k_1;                           // initial value worker
    real kr,kw,lohn,kap_an,kap_ka,eink_an,kap_ges;		//Kapital KA, Kapital AN, Lohn AN
    real lohn_od,kap_an_od,kap_ka_od;
    real Q_r;
    real Q_w;
    real Q_r_n;
    real Q_w_n;
    real Quot_kr;
    real Quot_kw; 
    real k_pas;
    real E;
    real E_n;
	real E_f_pr_n, bif_point;
	real E_steady_leon;
	real E_steady_quad; 
 	real E_steady_leon_prime;
	real E_steady_quad_prime;     
	real y_ini,y_ini_prime;
    real y_quad,y_quad_prime,y_quad_prime2;
    real y_leon,y_leon_prime,y_leon_prime2;
    real xBundle;
	real YBundle;
    real steady_quad;
	real steady_leon;
	real steady_kc_leon;
	real steady_kw_leon;
	real steady_kc_quad;
	real steady_kw_quad;
    real steady_kc;
    real steady_kw;
	real steady_cd;
	real steady_kc_cd;
	real steady_kw_cd;
	real steady_ces;
	real y_steady;
	real y_prime_steady;
	real E_f;
	

	real Q;
	real Konsum_C, Konsum_K, Konsum_A, Konsum_C_n, Konsum_A_n, Z, Z2;

public:
    pasinetti();			// constructor
	virtual ~pasinetti();
    void loadParamset(ifstream&);	// load parameterset from a file
    void saveParamset(ofstream&);
    void printParamset();
    real * setLabels(char*);		// get a variable by a name
    void initialize(void);		// initialize the model
    void iteration(const long&);	// perform one iteration 
};



#endif //  _GROWTH_MODEL_INCLUDED

// eof
