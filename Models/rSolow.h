///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/rSolow.h,v 1.1 2000/08/31 16:08:55 mmeyer Exp $
//
// Module name:		rSolow.h
// Contents:		Class definition of the class rSolow,
//			a dynamik growth model 
//
// Author:		Michael Meyer
// Last modified:	Tue April 04 12:40:17 METDST 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _RSOLOW_INCLUDED
#define _RSOLOW_INCLUDED

#include "baseModel.h"
#include "rand_var.h"
#include "markov_chain.h"


const int trans_x_Max=100;

const int mc_states_max=10;

struct st_paramset 
{
    qreal z_0;
	int type;
	int theta_type;
    qreal my;
    qreal z_trans;
    qreal trans_x[trans_x_Max];
    qreal trans_a[trans_x_Max];
    qreal trans_b[trans_x_Max];
        int mc_flag;
	char* zvar_expr;
	char mc_matrix[256];
	markov_chain * mc;
	rand_var * zvar;
	
		
};


class rSolow : public baseModel {

protected:


st_paramset *a_paramset; 
st_paramset *b_paramset;
st_paramset *c_paramset;
st_paramset *d_paramset;
st_paramset *n_paramset;
st_paramset *s_paramset ;
st_paramset *delta_p_paramset;

    qreal K;		// kapital
    qreal L;		// labour
    qreal k;		// transformed state variable k:=K/L
    qreal consumption;   // consumption
    qreal av_cons;       // average consumption
    
    qreal k_0;		// initial value
    
    			// derived state variables
    qreal y;		// output
    qreal w;		// wage
    qreal r;		// rent

				// production function
    qreal ( * pf ) ( qreal k, qreal a, qreal b, qreal c, qreal alpha );
    				// a pointer to the production function
    qreal ( * pf_prime) ( qreal k, qreal a, qreal b, qreal c, qreal alpha );
    				// and its derivative

    qreal pf_type;		// the current type of the production function
    qreal a, b, c, alpha;	// original parameters
    qreal A, B, C, D;		// parameters as ][0,1][ normalized input
    void pf_init ( void );	// maps (A,B,C) -> (a,b,c) and sets pf, pf_prime
    
    qreal s;			// Savings
    
    qreal n;		// exogenous growth of labour suppling people
   
    qreal delta_p;	// depreciation of kapital
    int n_states;
    char in_string[256];
   
    qreal a_0, b_0, c_0, d_0, n_0, s_0, delta_p_0;	// original parameters
    qreal cons_sum;
    int time_t;       

public:
    rSolow();			// constructor
    void loadParamset(ifstream&);	
    void saveParamset(ofstream&);
    void save_st_Paramset(ofstream&,st_paramset*);
    void saveParamsetWithNames(ofstream&); // add parametset to printerfile (so far only for xpm)
    void save_st_ParamsetWithNames(ofstream&,st_paramset*);
    void printParamset();
    void print_st_Paramset(st_paramset*);
    void noise_iteration(st_paramset*);
    void read_sim(ifstream&,st_paramset*);
    void delete_st_paramset_stochptr(st_paramset*);
    void iteration(const long&);
    void initialize();
    qreal* setLabels(char*);
    void sendStateSpace(int &,const qreal***);

    qreal* sendModelVar();		// for compatibity reasons only
    void sendParameters(int&,qreal**);
    void receiveParameters(const qreal*);
    virtual ~rSolow(void);            //destructor

};
///////////////////////////////////////////////////////////////////////////////
//
// class name:		RBC_delta_1.h
// Direved from:	rSolow
// Purpose:		 
//
// Author:		Michael Meyer
// Last modified:	Tue April 04 12:40:17 METDST 1999
// By:			Michael Meyer
//
////////////////////////////////////////////////////////////////////////////

class RBC_delta_1 : public rSolow {
protected:
	st_paramset *beta_paramset; 
	st_paramset *theta_RBC_paramset; 
	st_paramset *rho_paramset; 
	st_paramset *epsilon_paramset; 
	st_paramset *alpha_paramset; 

    qreal l_RBC;
    qreal z_RBC;
    qreal c_RBC;
    qreal k_RBC;
    qreal beta;
    qreal theta_RBC;
    qreal rho;
    qreal epsilon;
    qreal alpha_RBC;
    qreal z_RBC_0, c_RBC_0, k_RBC_0;
    qreal beta_0, theta_RBC_0, rho_0, epsilon_0, alpha_RBC_0;
	
public:
	RBC_delta_1();			//constructor
   	void loadParamset(ifstream&);	
  	void saveParamset(ofstream&);  
	void saveParamsetWithNames(ofstream&); // add parametset to printerfile (so far only for xpm)	
   	void printParamset();	   	
   	void iteration(const long&);
   	void initialize();
    	virtual ~RBC_delta_1(void);            //destructor
    qreal* setLabels(char*);
};
#endif //  _RSOLOW_INCLUDED

// eof
