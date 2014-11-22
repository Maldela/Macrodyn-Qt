#ifndef LOGIDELAY_H
#define LOGIDELAY_H

//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/logiDelay.h,v 1.1 2000/08/31 15:59:59 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  logiDelay.h                                                 */
/* Contents:     Class definition of the class logiDelay                     */
/*                                                                            */
/* Last Modified: 02.03.1995 (Markus Lohmann)                                 */
/* Modified:      24.02.95 -- sendParameters and receiveParameters added      */
/*                02.03.95 -- logiDelay is a derived from models2D now       */
/*                                                                            */
/******************************************************************************/

#include "models2D.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    logiDelay                                                  */
/* Derived from:  models2D                                                    */
/* Purpose:       defines the model that is taken from the "International     */
/*                Journal of Chaos and Bifurcation, Vol. 4, No. 2, 1994,      */
/*                Noninvertible Two-Dimensional Maps arising in Radiophysics" */
/*                by Maistrenko, Vladimir; Maistrenko, Yuri and Sushko, Irina */
/*                                                                            */
/* Last Modified: 02.03.1995 (Markus Lohmann)                                 */
/* Modified:      24.02.95 -- sendParameters and receiveParameters added      */
/*                02.03.95 -- logiDelay is a derived from models2D now       */
/*                                                                            */
/******************************************************************************/

class logiDelay : public models2D {
public:
    void iteration(const long&);
};

class logLogDelay : public models2D {
protected:
    qreal delayFunc(const qreal&, const qreal&);
public:
    void iteration(const long&);
};

#endif
