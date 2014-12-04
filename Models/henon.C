//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/henon.C,v 1.1 2000/08/31 15:56:57 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  henon.C                                                      */
/* Contents:     Member functions of the class henon                          */
/*                                                                            */
/* Last modified: 02.03.1995 (Markus Lohmann)                                 */
/* Modified:      24.02.95 -- sendParameters and receiveParameters added      */
/*                02.03.95 -- henon is a derived from models2D now            */
/*                                                                            */
/* Modified:      16.06.1998 (Anton Stiefenhofer)                             */
/*                Henon Map according to Devaney (1996)                       */
/*                                                                            */
/******************************************************************************/

#include "henon.h"
#include "../error.h"
/******************************************************************************/
/*                                                                            */
/* Class name:      henon                                                     */
/* Member function: iteration                                                 */
/* Purpose:         perform one iteration of the system                       */
/* Last modified:   20.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/* Modified:      16.06.1998 (Anton Stiefenhofer)                             */
/*                Henon Map according to Devaney (1996)                       */
/*                                                                            */
/******************************************************************************/

void henon::iteration(const qint64&)
{
    qreal oldY=y;

    y= alpha - beta * x - y * y;
    x= oldY;

    /* x=1+beta*y-alpha*x*x; */
    /* y=oldX; */
}

/******************************************************************************/
/*                                                                            */
/* Class name:      quadHenon                                                 */
/* Member function: iteration                                                 */
/* Purpose:         perform one iteration of the system                       */
/* Last modified:   08.12.1995 (Markus Lohmann)                               */
/*                  28.06.1998 (Toni Stiefenhofer)                            */
/*                                                                            */
/******************************************************************************/

void quadHenon::iteration(const qint64&)
{
    qreal oldX=x;

    x = (1.0/beta)*(alpha - y - x*x);
    y = oldX;
    
    /* x=y;   ??? */
    /* y=beta-alpha*oldX*oldX-y*y; */
}
