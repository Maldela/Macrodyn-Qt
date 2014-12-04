//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/logiDelay.C,v 1.1 2000/08/31 15:59:56 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  logiDelay.C                                                 */
/* Contents:     Member functions of the class logiDelay                     */
/*                                                                            */
/* Last modified: 02.03.1995 (Markus Lohmann)                                 */
/* Modified:      24.02.95 -- sendParameters and receiveParameters added      */
/*                02.03.95 -- logiDelay is a derived from models2D now       */
/*                                                                            */
/******************************************************************************/

#include "logiDelay.h"
#include "../error.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      logiDelay                                                */
/* Member function: iteration                                                 */
/* Purpose:         perform one iteration of the system                       */
/* Last modified:   08.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void logiDelay::iteration(const qint64&)
{
    qreal oldX=x;
       
    x=alpha*x*(1-y)+beta;
    y=oldX;
}

qreal logLogDelay::delayFunc(const qreal& argOne, const qreal& argTwo)
{
    return( (1-beta)*argTwo + beta*argOne );
}

void logLogDelay::iteration(const qint64&)
{

    qreal w=delayFunc(x,y);

    x=y;
    y=alpha*w * (1-w);
}
