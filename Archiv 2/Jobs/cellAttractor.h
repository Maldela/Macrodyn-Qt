// File:	cellAttractor.h
// Contents:	definition of class cellAttractor
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:16:47 MEZ 1996
// Changes:	

#ifndef CELLATTRACTOR_H
#define CELLATTRACTOR_H
#include	"geometricJob.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    cellAttractor                                               */
/* Derived from:  geometric job                                               */
/* Purpose:       computes the orbit of a discreete dynamical system that has */
/*                a discreete state space using a fixed parameterset          */
/* Author:        Markus Lohmann                                              */
/* Last modified: 11.10.1994                                                  */
/*                                                                            */
/******************************************************************************/

class cellAttractor : public geometricJob {
  public:
    cellAttractor(baseModel* const, const xyRange&, MacrodynGraphicsItem* const=NULL);
    void simulation();
};

#endif
//eof
