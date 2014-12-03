// File:	bif2D.h
// Contents:	definition of class bif2D
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:16:47 MEZ 1996
// Changes:	

#ifndef BIF2D_H
#define BIF2D_H
#include	"geometricJob.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    bif2D                                                       */
/* Derived from:  geometric job                                               */
/* Purpose:       computes a 2D bifurcation diagram (one parameter varies,    */
/*                the simulation results are projected on one state variable) */
/* Author:        Markus Lohmann                                              */
/* Last modified: 11.10.1994                                                  */
/*                                                                            */
/******************************************************************************/

class bif2D : public geometricJob {
  protected:
    qreal stepX;                      // stepsize on the x-axis
    
  public:
    bif2D(baseModel* const, const xyRange&, MacrodynGraphicsItem* const=NULL);
    void setStepX(const qreal&);
    void simulation();
};

#endif
//eof
