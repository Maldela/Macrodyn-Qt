// File:	basinTwoCycles.h
// Contents:	definition of class basinTwoCycles
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:16:47 MEZ 1996
// Changes:	

#ifndef BASINTWOCYCLES_H
#define BASINTWOCYCLES_H
#include	"basin.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    basinTwoCycles                                              */
/* Derived from:  basin                                                       */
/* Purpose:       Computes the basin of attraction for two given coexisting   */
/*                cycles                                                      */
/* Author:        Markus Lohmann                                              */
/* Last modified: 11.10.1994                                                  */
/*                                                                            */
/******************************************************************************/

class basinTwoCycles : public basin {
  protected:
    int period2;                       // period of the second cycle
    qreal **cycle2;                   // matrix to store the second cycle
  public:
    basinTwoCycles(baseModel* const, const xyRange&, const QString&, const QString&,
           MacrodynGraphicsItem* const=NULL);
    ~basinTwoCycles();
    void simulation();
};

#endif
//eof    
