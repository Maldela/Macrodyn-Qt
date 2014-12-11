// File:	stateSpaceAnalysis.h
// Contents:	definition of class stateSpaceAnalysis
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:16:47 MEZ 1996
// Changes:	

#ifndef STATESPACEANALYSIS_H
#define STATESPACEANALYSIS_H
#include	"attractor.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    stateSpaceAnalysis                                          */
/* Derived from:  attractor                                                   */
/* Purpose:       computes a global analysis of the system for a fixed        */
/*                parameterset                                                */
/* Author:        Markus Lohmann                                              */
/* Last modified: 06.12.1994                                                  */
/*                                                                            */
/******************************************************************************/

class stateSpaceAnalysis: public attractor {
  protected:
    qreal qMaxCell;		        // number of cells in the state Space
    const qreal **stateVars;

  public:
    stateSpaceAnalysis(baseModel* const, const xyRange&, MacrodynGraphicsItem* const=NULL);
    virtual ~stateSpaceAnalysis();
    void simulation();
};

#endif
//eof
