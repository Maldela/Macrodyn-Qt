/******************************************************************************/
/*                                                                            */
/* Module name:  perturbedDelayedLogisticMap.C                                */
/* Contents:     Member functions of the class perturbedDelayedLogisticMap    */
/*                                                                            */
/* Modified:	  Mon Jun 15 10:41:39 METDST 1998 by Marc Mueller             */
/* Last Modified: Mon Jun 15 10:41:39 METDST 1998 (Marc Mueller)              */
/*                                                                            */
/******************************************************************************/

#include "perturbedDelayedLogisticMap.h"
/******************************************************************************/
/*                                                                            */
/* Class name:      perturbedDelayedLogisticMap                               */
/* Member function: iteration                                                 */
/* Purpose:         perform one iteration of the system                       */
/*                                                                            */
/* Modified:	  Mon Jun 15 10:41:39 METDST 1998 by Marc Mueller             */
/* Last Modified: Mon Jun 15 10:41:39 METDST 1998 (Marc Mueller)              */
/*                                                                            */
/******************************************************************************/

void perturbedDelayedLogisticMap::iteration(const long&)
{
    qreal oldX=x;

    x=y;
    y=alpha*y*(1-oldX)+beta;
}
