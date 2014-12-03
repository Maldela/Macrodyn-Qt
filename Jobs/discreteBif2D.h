// File:	discreteBif2D.h
// Contents:	definition of class discreteBif2D
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:25:09 MEZ 1996
// Changes:	

#ifndef DISCRETEBIF2D_H
#define DISCRETEBIF2D_H
#include	"geometricJob.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    discreteBif2D                                               */
/* Derived from:  geometric job                                               */
/* Purpose:       computes a 2D bifurcation diagram for a discrete variable,  */
/*                the simulation results are projected on one state variable  */
/* Author:        Markus Lohmann                                              */
/* Last modified: 27.02.1995                                                  */
/*                                                                            */
/******************************************************************************/

class discreteBif2D : public geometricJob {
  protected:
    short stepX;                      // stepsize on the x-axis
    
  public:
    discreteBif2D(baseModel* const, const xyRange&, MacrodynGraphicsItem* const=NULL);
    void simulation();
};

#endif
//eof
