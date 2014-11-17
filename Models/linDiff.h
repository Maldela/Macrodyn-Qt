#ifndef LINDIFF_H
#define LINDIFF_H

//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/linDiff.h,v 1.1 2000/08/31 15:59:37 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  linDiff.h                                                    */
/* Contents:     Class definition of the class linDiff                        */
/*                                                                            */
/* Last Modified: 02.03.1995 (Markus Lohmann)                                 */
/* Modified:      24.02.95 -- sendParameters and receiveParameters added      */
/*                02.03.95 -- linDiff is a derived from models2D now          */
/*                                                                            */
/******************************************************************************/

#include "models2D.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    linDiff                                                     */
/* Derived from:  models2D                                                    */
/* Purpose:       defines the 2D linear difference equation map               */
/*                                                                            */
/* Last modified: 20.02.1995 (Markus Lohmann)                                 */
/*                                                                            */
/******************************************************************************/

class linDiff : public models2D {
public:
    void iteration(const long&);
};
#endif
