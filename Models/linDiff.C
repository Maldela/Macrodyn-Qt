//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/linDiff.C,v 1.1 2000/08/31 15:59:35 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  linDiff.C                                                    */
/* Contents:     Member functions of the class linDiff                        */
/*                                                                            */
/* Last modified: 02.03.1995 (Markus Lohmann)                                 */
/* Modified:      24.02.95 -- sendParameters and receiveParameters added      */
/*                02.03.95 -- linDiff is a derived from models2D now          */
/*                                                                            */
/******************************************************************************/

#include "linDiff.h"
#include "../logger.h"
/******************************************************************************/
/*                                                                            */
/* Class name:      linDiff                                                   */
/* Member function: iteration                                                 */
/* Purpose:         perform one iteration of the system                       */
/* Last modified:   20.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void linDiff::iteration(const qint64&)
{
    qreal oldX=x;

    x=alpha*oldX+beta*y;
    y=oldX;
}
