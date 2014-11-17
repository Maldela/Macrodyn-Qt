#ifndef HENON_H
#define HENON_H

//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/henon.h,v 1.1 2000/08/31 15:57:00 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  henon.h                                                      */
/* Contents:     Class definition of the classes henon and modifiedHenon      */
/*                                                                            */
/* Last Modified: 08.12.1995 (Markus Lohmann)                                 */
/* Modified:      24.02.95 -- sendParameters and receiveParameters added (ML) */
/*                02.03.95 -- henon is derived from models2D (ML)             */
/*                08.12.95 -- quadHenon added (ML)                            */
/*                                                                            */
/******************************************************************************/

#include "models2D.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    henon                                                       */
/* Derived from:  models2D                                                    */
/* Purpose:       defines the henon map                                       */
/*                                                                            */
/* Last modified: 20.02.1995 (Markus Lohmann)                                 */
/*                                                                            */
/******************************************************************************/

class henon : public models2D {
public:
    void iteration(const long&);
};

/******************************************************************************/
/*                                                                            */
/* Class name:    quadHenon                                                   */
/* Derived from:  models2D                                                    */
/* Purpose:       defines the quadratic henon map                             */
/*                                                                            */
/* Last modified: 08.12.1995 (Markus Lohmann)                                 */
/*                                                                            */
/******************************************************************************/

class quadHenon : public models2D {
public:
    void iteration(const long&);
};
#endif
