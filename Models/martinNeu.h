#ifndef MARTINNEU_H
#define MARTINNEU_H

/******************************************************************************/
/* Module name:   martinNeu.h                                                 */
/* Purpose:       defines jan's dynamical system                              */
/*                                                                            */
/* Modified:      20.10.1999 Marc Mueller                                     */
/* Last Modified: 20.10.1999 Marc Mueller   	                              */
/******************************************************************************/

#include "baseModel.h"
#include "rand_var.h"

class martinNeu : public baseModel {
protected:
    rand_var * zvar;
    real rho ;
    real gamma ;
    real lambda ;
    real g_tm1_0,g_tm1, g_t ;
    real thetaOld,thetaEold;
    real theta_e_t_0,theta_e_t, theta_e_tp1 ;
    real theta_tm1_0,theta_tm1, theta_t ;
    real Err ;

    real p_t;
    real p_tm1;
    real p_e_tp1;

    virtual real savings(const real&);
public:
    martinNeu();
//    virtual ~martinNeu();
    real* setLabels(char*);
    void initialize();
    void loadParamset(ifstream&);	
    void sendStateSpace(int &,const real***);
    void iteration(const long&);
};

#endif
