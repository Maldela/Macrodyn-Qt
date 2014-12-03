// File:	attractor.h
// Contents:	definition of class attractor
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:16:47 MEZ 1996
// Changes:	

#ifndef ATTRACTOR_H
#define ATTRACTOR_H
#include	"geometricJob.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    attractor                                                   */
/* Derived from:  geometric job                                               */
/* Purpose:       computes the orbit using a fixed parameterset of the model  */
/* Author:        Markus Lohmann                                              */
/* Last modified: 11.10.1994                                                  */
/*                                                                            */
/******************************************************************************/

class attractor : public geometricJob {
  public:
    attractor(baseModel* const, const xyRange&, MacrodynGraphicsItem* const=NULL);
    void simulation();
};

#endif
//eof
