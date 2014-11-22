//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/newMacro.C,v 1.1 2000/08/31 16:07:10 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  newMacro.C                                              */
/* Contents:     Member functions of the class newMacro. This model is an*/
/*               extension of the default model. It uses "labour market first"*/
/*                                                                            */
/* Last Modified: 14.02.1995 (Markus Lohmann)                                 */
/*                                                                            */
/******************************************************************************/

#include "newMacro.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      newMacro                                             */
/* Member function: notProd                                                   */
/* Purpose:         computes the notional demand for labor and the notional   */
/*                  supply of goods of the producer                           */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void newMacro::notProd(qreal &ztnot,qreal &yteff)
{
    ztnot= exp( log(A/wtqreal) / (1-B) );
    employment=MIN(Lmax,ztnot);
    yteff= prodFunction(employment);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      newMacro                                             */
/* Member function: notCom                                                    */
/* Purpose:         computes the desired consumption of the consumers         */
/*                  (eq. 2.12)                                                */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void newMacro::notCom(qreal &xteff,qreal &ct,qreal &ptratex)
{
    qreal sigma = rho / (rho-1);
  
    ct =  1/ ( 1 + exp( log(delta)/(1-rho) + sigma*log(ptratex) ) );
    xteff=( g + beta * mtqreal ) +  ct*wtqreal*employment*(1-tax);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      newMacro                                             */
/* Member function: wageAndPrice                                              */
/* Purpose:         wage and price adjustment (eqs. 3.6, 3.7)                 */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void newMacro::wageAndPrice(qreal &xteff, qreal &yteff,qreal &ztnot,
				 char *)

{
    qreal yDiff=xteff-yteff;
    qreal lDiff=ztnot-Lmax;
    
    if( yDiff < 0 )
	ptrate= 1 + kappa*(yDiff/yteff) ;
    else
	ptrate= 1 + gamm*(yDiff/xteff) ;
    
    if( lDiff < 0 )
	wtrate= 1 + lambda*(lDiff/Lmax) ;
    else
	wtrate= 1 + my*(lDiff/ztnot) ;
    
    if (ptrate<1e-5)
	ptrate=1e-5;
    if (wtrate<1e-5)
	wtrate=1e-5;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      newMacro                                             */
/* Member function: dynamics                                                  */
/* Purpose:         describes the dynamics of the system                      */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void newMacro::dynamics()
{
    mtqreal  = ( MIN(output,g+mtqreal) + (1-tax)*wtqreal*employment - output )
	      / ptrate;
    wtqreal  = wtqreal * (wtrate / ptrate);
    for(int i=0; i<=tau; i++ )
	theta[tau+1-i]=theta[tau-i];     /* p(t) -> price[t+1] */
    theta[0]=ptrate;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system (Figs. 7, 8)   */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void newMacro::iteration(const long& t)
{
    qreal ptratex;
    qreal ztnot;
    qreal yteff;
    qreal xteff;
    qreal ct;
    
    notProd(ztnot,yteff);
    ptratex=expectedInflationRate(t);
    notCom(xteff,ct,ptratex);
    output=MIN(xteff,yteff);
    wageAndPrice(xteff,yteff,ztnot,NULL);
    dynamics();
}
