// File:	parameterSpace.h
// Contents:	definition of class parameterSpace
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:16:47 MEZ 1996
// Changes:	

#ifndef PARAMETERSPACE_H
#define PARAMETERSPACE_H
#include	"bif2D.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    parameterSpace                                              */
/* Derived from:  bif2D                                                       */
/* Purpose:       Computes a map showing the orbits dependence on parameters; */
/*                2 parameters vary, for each parameter combination the       */
/*                resulting orbit is analysed on periodic behaviour;          */
/*                the corresponding point in the parameter space is colored   */
/*                depending on the order of the (possibly) periodic behaviour */
/* Author:        Markus Lohmann                                              */
/* Last modified: 11.10.1994                                                  */
/*                                                                            */
/******************************************************************************/

class parameterSpace : public bif2D {
  protected:
    qreal** ts_data;
    qreal* mean_x;
    xyRange stateSpace;
    qreal stepY;                      // stepsize for the second parameter
    QList<qreal *> stateVars;         // pointer to the state variables
    int m_maxLag;
    double m_autocorrelationBound;
  public:
    parameterSpace(baseModel* const, const xyRange&, const xyRange&, MacrodynGraphicsItem* const=NULL, int maxLag=200, double autocorrelationBound=0.99);
    virtual ~parameterSpace();
    void simulation();
    QColor colorFromOrder(int order);
};    

#endif
//eof
