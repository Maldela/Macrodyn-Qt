// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/constantLag.C,v 1.1 2000/08/31 15:24:51 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  constantLag.C                                                */
/* Contents:     Member functions of the class constantLag                    */
/*               This is an extension of the default model. It uses           */
/*               constant lags, i.e. expInflRate=theta[t-lag]                 */
/*               expectations to obtain the expected inflation rate.          */
/*                                                                            */
/* Last Modified: 24.02.1995 (Markus Lohmann)                                 */
/*                                                                            */
/******************************************************************************/

#include "constantLag.h"
#include "../error.h"

//extern void fatalError(const char*, const char*);

/******************************************************************************/
/*                                                                            */
/* Class name:      constantLag                                               */
/* Member function: expectedInflationRate                                     */
/* Purpose:         computes the expected inflation rate                      */
/* Last modified:   31.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal constantLag::expectedInflationRate(const long t)
{
    long T=MIN(t,tau);
    if( !tau )
	return 1;
    return( theta[T-1] );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      constantLag                                               */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system (Figs. 7, 8)   */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void constantLag::iteration(const long& t)
{
    qreal ptratex;
    char state[5];
    qreal ztnot;
    qreal ytnot;
    qreal xtnot;
    qreal ct;

    ptratex=expectedInflationRate(t);
    notProd(ztnot,ytnot);
    notCom(xtnot,ct,ptratex);
    empAndOut(xtnot,ytnot);
    detDiseq(xtnot,ytnot,state);
    wageAndPrice(xtnot,ytnot,ztnot,state);
    dynamics();
}
