// File:	bif2Param.h
// Contents:	definition of class bif2Param
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:16:47 MEZ 1996
// Changes:	

#ifndef BIF2PARAM_H
#define BIF2PARAM_H

#include	"bif2D.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    bif2Param                                                   */
/* Derived from:  bif2D                                                       */
/* Purpose:       Computes a two parameter bifurcation diagram                */
/* Author:        Markus Lohmann                                              */
/* Last modified: 11.10.1994                                                  */
/*                                                                            */
/******************************************************************************/

class bif2Param : public bif2D {
  protected:
    qreal zmin;                       // describes the second bifurcation
    qreal zmax;                       // parameter
    QString zLabel;
    qreal stepZ;
    qreal *zParam;
  public:
    bif2Param(baseModel* const, const xyRange&, MacrodynGraphicsItem* const=NULL);
    void simulation();
};

#endif
//eof
