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
    qreal z_0;
	int type;
	int theta_type;
    qreal my;
    qreal z_trans;
    qreal trans_x[trans_x_MAX];
    qreal trans_a[trans_x_MAX];
    qreal trans_b[trans_x_MAX];
        int mc_flag;
	char* zvar_expr;
	char mc_matrix[256];
	markov_chain * mc;
	rand_var * zvar;			
};

class  rOLG_wt : public baseModel {

protected:

	int dof;
    qreal b_exponential;
    qreal a_pareto;
    qreal c_pareto;

    qreal Y_olg;
    qreal Y_olg_E,Y_olg_V,Y_olg_S;
    qreal W_olg;
    qreal W_olg_E,W_olg_V,W_olg_S;
    qreal M_olg;
    qreal M_olg_E,M_olg_V,M_olg_S;
    qreal K_olg_0;               // initial value
    qreal Ez_0;		    // initial value
    qreal K_olg;
    qreal K_olg_E,K_olg_V,K_olg_S;
    qreal K_olg_V_n;
    qreal K_olg_zaehl;
    qreal av_Ez;
    qreal Ez;
    qreal R_olg;
    qreal a_m;
    qreal b_m;
    qreal c_olg;
    qreal m_olg;
    qreal A_olg;
    qreal delta_olg;
    qreal n_olg;
    qreal alpha_olg;
    qreal z_st;
    qreal r;
    qreal r_E,r_V,r_S;
    qreal S_olg;
    qreal c1;
    qreal c2;
    int n_states;
    char in_string[256];
    qreal z_st_n;
    qreal z_st_0;              // original parameter
    qreal T;
    qreal TT;
 
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
    void iteration(const qint64&);
    void expection(qreal,qreal);
    void initialize();
    qreal* setLabels(char*);
    void sendStateSpace(int &,const qreal***);

    qreal* sendModelVar();		// for compatibity reasons only
    void sendParameters(int&,qreal**);
    void receiveParameters(const qreal*);
    virtual ~rOLG_wt(void);            //destructor

};

#endif //  _ROLG_WT_INCLUDED

// eof
