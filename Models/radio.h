#ifndef RADIO_H
#define RADIO_H

//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/radio.h,v 1.1 2000/08/31 16:09:15 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  radio.h                                                 */
/* Contents:     Class definition of the class radio                     */
/*                                                                            */
/* Last Modified: 02.03.1995 (Markus Lohmann)                                 */
/* Modified:      24.02.95 -- sendParameters and receiveParameters added      */
/*                02.03.95 -- radio is a derived from models2D now       */
/*                                                                            */
/******************************************************************************/

#include "models2D.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    radio                                                  */
/* Derived from:  models2D                                                    */
/* Purpose:       defines the model that is taken from the "International     */
/*                Journal of Chaos and Bifurcation, Vol. 4, No. 2, 1994,      */
/*                Noninvertible Two-Dimensional Maps arising in Radiophysics" */
/*                by Maistrenko, Vladimir; Maistrenko, Yuri and Sushko, Irina */
/*                                                                            */
/* Last Modified: 02.03.1995 (Markus Lohmann)                                 */
/* Modified:      24.02.95 -- sendParameters and receiveParameters added      */
/*                02.03.95 -- radio is a derived from models2D now       */
/*                                                                            */
/******************************************************************************/

class radio : public models2D {
    qreal logisticMap(qreal&);	// logistic map computing with
					// parameter alpha
public:
    void iteration(const qint64&);
};

#endif
