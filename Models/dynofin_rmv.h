///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/dynofin_rmv.h,v 1.1 2000/08/31 15:53:58 mmeyer Exp $
//
// Module name:		dynofin_rmv.h
// Contents:		Class definition of the class dynofin_rmv		
// Author:		Michael Meyer
// Last modified:	Thu April 22 12:40:17 METDST 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _DYNOFIN_RMV_INCLUDED
#define _DYNOFIN_RMV_INCLUDED

#include "baseModel.h"
#include "rand_var.h"
#include "markov_chain.h"

const int Trans_x_Max=100;

const int mc_states_Max=10;

struct st_fin_paramset 
{
	real z_0;
	int type;
	int theta_type;
	real my;
	real z_trans;
	real trans_x[Trans_x_Max];
	real trans_a[Trans_x_Max];
	real trans_b[Trans_x_Max];		
        int mc_flag;
	char* zvar_expr;
	char mc_matrix[256];
	markov_chain * mc;
	rand_var * zvar;			
};
class dynofin_rmv : public baseModel {

protected:

	real w1;
	real w2;
	real thetaA1;
	real thetaA2;	
	real thetaA1_t;
	real thetaA2_t;	
	real thetaB1;
	real thetaB2;	
	real p;
	real gam_1;
	real var_1;
	real exp_1;
	real gam_2;
	real var_2;
	real exp_2;	
	real ( * pf ) (real w, real d, real p);		
	real r_fin, r_fin_0;
	real d_fin, d_fin_0;
	real w1_0, w2_0, thetaA1_0, thetaA2_0, thetaB1_0, thetaB2_0, p_0, gam_0, var_0, expe_0;
	int n_states;
    	char in_string[256];
public:	
	dynofin_rmv();
	void loadParamset(ifstream&);	
  	void saveParamset(ofstream&);
    	void save_st_fin_paramset(ofstream&,st_fin_paramset*);
    	void saveParamsetWithNames(ofstream&); 
    	void save_st_fin_paramsetWithNames(ofstream&,st_fin_paramset*);
    	void printParamset();
    	void print_st_fin_paramset(st_fin_paramset*);
    	void noise_iteration(st_fin_paramset*);
    	void read_sim(ifstream&,st_fin_paramset*);
    	void delete_st_fin_paramset_stochptr(st_fin_paramset*);
    	void iteration(const long&);
    	void initialize();
    	real* setLabels(char*);
    	void sendStateSpace(int &,const real***);
    	real* sendModelVar();		
    	void sendParameters(int&,real**);
  	void receiveParameters(const real*);
        virtual ~dynofin_rmv(void);            //destructor
};
#endif //  _DYNOFIN_RMV_INCLUDED

// eof  
	
