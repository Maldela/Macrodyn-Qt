#ifndef PERTURBEDDELAYEDLOGISTICMAP_H
#define PERTURBEDDELAYEDLOGISTICMAP_H

/******************************************************************************/
/*                                                                            */
/* Module name:  perturbedDelayedLogisticMap.h                                */
/* Contents:     Class definition of the classes perturbedDelayedLogisticMap  */
/*                                                                            */
/* Modified:	  Mon Jun 15 10:41:39 METDST 1998 by Marc Mueller             */
/* Last Modified: Mon Jun 15 10:41:39 METDST 1998 (Marc Mueller)              */
/*                                                                            */
/******************************************************************************/

#include "models2D.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    perturbedDelayedLogisticMap                                 */
/* Derived from:  models2D                                                    */
/* Purpose:       defines the perturbed delayed logistic map                  */
/*                                                                            */
/* Modified:	  Mon Jun 15 10:41:39 METDST 1998 by Marc Mueller             */
/* Last Modified: Mon Jun 15 10:41:39 METDST 1998 (Marc Mueller)              */
/*                                                                            */
/******************************************************************************/

class perturbedDelayedLogisticMap : public models2D {
public:
    void iteration(const qint64&);
};
#endif
