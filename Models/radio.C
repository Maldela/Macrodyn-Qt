//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/radio.C,v 1.1 2000/08/31 16:09:12 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  radio.C                                                 */
/* Contents:     Member functions of the class radio                     */
/*                                                                            */
/* Last modified: 02.03.1995 (Markus Lohmann)                                 */
/* Modified:      24.02.95 -- sendParameters and receiveParameters added      */
/*                02.03.95 -- radio is a derived from models2D now       */
/*                                                                            */
/******************************************************************************/

#include "radio.h"
#include "../error.h"
/******************************************************************************/
/*                                                                            */
/* Class name:      radio                                                */
/* Member function: logisticMap                                               */
/* Purpose:         computes the logstic map with parameter alpha             */
/* Last modified:   08.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal radio::logisticMap(qreal& in)
{
    return alpha*in*(1-in);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      radio                                                */
/* Member function: iteration                                                 */
/* Purpose:         perform one iteration of the system                       */
/* Last modified:   08.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void radio::iteration(const qint64&)
{
    qreal oldX=x;
    qreal oldY=y;
    
    x=beta*oldY;
    y=(logisticMap(oldX)+logisticMap(oldY))/(1+beta);
}
