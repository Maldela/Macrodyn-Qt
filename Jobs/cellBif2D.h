// File:	cellBif2D.h
// Contents:	definition of class cellBif2D
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:16:47 MEZ 1996
// Changes:	

#ifndef CELLBIF2D_H
#define CELLBIF2D_H

#include	"bif2D.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    cellBif2D                                                   */
/* Derived from:  bif2D                                                       */
/* Purpose:       computes a 2D bifurcation diagram (one parameter varies,    */
/*                the simulation results a projection to one state variable)  */
/*                using a simple cell mapping                                 */
/* Author:        Markus Lohmann                                              */
/* Last modified: 24.11.1994                                                  */
/*                                                                            */
/******************************************************************************/

class cellBif2D : public bif2D {
  public:
    cellBif2D(baseModel* const, const xyRange&, MacrodynGraphicsItem* const=NULL);
    void simulation();
};

#endif
//eof
