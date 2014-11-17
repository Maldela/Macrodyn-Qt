///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/rOLG_wt.h,v 1.5 2000/10/18 11:50:59 mhoffman Exp $
//
// Module name:		rOLG_wt.h
// Contents:		Class definition of the class rOLG_wt
//			 
//
// Author:		Michael Meyer
// Last modified:	Tue Aug 03 12:40:17 METDST 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _ROLG_WT_INCLUDED
#define _ROLG_WT_INCLUDED

#include "baseModel.h"
#include "rand_var.h"
#include "markov_chain.h"
#include "distribution.h"

const int trans_x_MAX=100;

const int mc_states_MAX=10;

struct st_olg_paramset 
{
	real z_0;
	int type;
	int theta_type;
	real my;
	real z_trans;
	real trans_x[trans_x_MAX];
	real trans_a[trans_x_MAX];
	real trans_b[trans_x_MAX];		
        int mc_flag;
	char* zvar_expr;
	char mc_matrix[256];
	markov_chain * mc;
	rand_var * zvar;			
};

class  rOLG_wt : public baseModel {

protected:

	int dof;
	real b_exponential;
	real a_pareto;
	real c_pareto;

    real Y_olg;
    real Y_olg_E,Y_olg_V,Y_olg_S;
    real W_olg; 
    real W_olg_E,W_olg_V,W_olg_S;
    real M_olg;
    real M_olg_E,M_olg_V,M_olg_S;
    real K_olg_0;               // initial value  
    real Ez_0;		    // initial value  
    real K_olg;
    real K_olg_E,K_olg_V,K_olg_S;
    real K_olg_V_n; 
    real K_olg_zaehl;
    real av_Ez;		
    real Ez;		      			
    real R_olg;		
    real a_m;		 
    real b_m;		
    real c_olg;
    real m_olg;		    
    real A_olg;		
    real delta_olg;	
    real n_olg;
    real alpha_olg;
    real z_st;
    real r;
    real r_E,r_V,r_S;
    real S_olg;
    real c1;
    real c2;
    int n_states;
    char in_string[256];
    real z_st_n;
    real z_st_0;              // original parameter
    real T;
    real TT;  
 
	distribution *rand_dis;

	double ran_rec;
	double ran_norm;
	double ran_chi;
	double ran_cauchy;
	double ran_student_t;
	double ran_exp;
	double ran_pareto;

public:
    rOLG_wt();			// constructor
    void loadParamset(ifstream&);	
    void saveParamset(ofstream&);
    void save_st_olg_Paramset(ofstream&,st_olg_paramset*);
    void saveParamsetWithNames(ofstream&); // add parametset to printerfile (so far only for xpm)
    void save_st_olg_ParamsetWithNames(ofstream&,st_olg_paramset*);
    void printParamset();
    void print_st_olg_Paramset(st_olg_paramset*);
    void noise_iteration(st_olg_paramset*);
    void read_sim(ifstream&,st_olg_paramset*);
    void delete_st_olg_paramset_stochptr(st_olg_paramset*);
    void iteration(const long&);
    void expection(real,real);
    void initialize();
    real* setLabels(char*);
    void sendStateSpace(int &,const real***);

    real* sendModelVar();		// for compatibity reasons only
    void sendParameters(int&,real**);
    void receiveParameters(const real*);
    virtual ~rOLG_wt(void);            //destructor

};

#endif //  _ROLG_WT_INCLUDED

// eof
