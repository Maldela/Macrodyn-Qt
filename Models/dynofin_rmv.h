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
	qreal z_0;
	int type;
	int theta_type;
	qreal my;
	qreal z_trans;
	qreal trans_x[Trans_x_Max];
	qreal trans_a[Trans_x_Max];
	qreal trans_b[Trans_x_Max];		
    int mc_flag;
    QString zvar_expr;
    QString mc_matrix;
	markov_chain * mc;
	rand_var * zvar;			
};
class dynofin_rmv : public baseModel {

protected:

	qreal w1;
	qreal w2;
	qreal thetaA1;
	qreal thetaA2;	
	qreal thetaA1_t;
	qreal thetaA2_t;	
	qreal thetaB1;
	qreal thetaB2;	
	qreal p;
	qreal gam_1;
	qreal var_1;
	qreal exp_1;
	qreal gam_2;
	qreal var_2;
	qreal exp_2;	
	qreal ( * pf ) (qreal w, qreal d, qreal p);		
	qreal r_fin, r_fin_0;
	qreal d_fin, d_fin_0;
	qreal w1_0, w2_0, thetaA1_0, thetaA2_0, thetaB1_0, thetaB2_0, p_0, gam_0, var_0, expe_0;
	int n_states;
    QString in_string;
public:	
	dynofin_rmv();
	void loadParamset(QDataStream&);	
  	void saveParamset(QDataStream&);
    void save_st_fin_paramset(QDataStream&,st_fin_paramset*);
    void saveParamsetWithNames(QDataStream&);
    void save_st_fin_paramsetWithNames(QDataStream&,st_fin_paramset*);
    void printParamset();
    void print_st_fin_paramset(st_fin_paramset*);
    void noise_iteration(st_fin_paramset*);
    void read_sim(QDataStream&,st_fin_paramset*);
    void delete_st_fin_paramset_stochptr(st_fin_paramset*);
    void iteration(const qint64&);
    void initialize();
    qreal* setLabels(const QString&);
    void sendStateSpace(int &,const qreal***);
    qreal* sendModelVar();
    void sendParameters(int&,qreal**);
  	void receiveParameters(const qreal*);
        virtual ~dynofin_rmv(void);            //destructor
};
#endif //  _DYNOFIN_RMV_INCLUDED

// eof  
	
