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
    qreal rho ;
    qreal gamma ;
    qreal lambda ;
    qreal g_tm1_0,g_tm1, g_t ;
    qreal thetaOld,thetaEold;
    qreal theta_e_t_0,theta_e_t, theta_e_tp1 ;
    qreal theta_tm1_0,theta_tm1, theta_t ;
    qreal Err ;

    qreal p_t;
    qreal p_tm1;
    qreal p_e_tp1;

    virtual qreal savings(const qreal&);
public:
    martinNeu();
//    virtual ~martinNeu();
    qreal* setLabels(char*);
    void initialize();
    void loadParamset(ifstream&);	
    void sendStateSpace(int &,const qreal***);
    void iteration(const long&);
};

#endif
