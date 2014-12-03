// File:	nParameterAnalysis.h
// Contents:	definition of class nParameterAnalysis
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:16:47 MEZ 1996
// Changes:	

#ifndef NPARAMETERANALYSIS_H
#define NPARAMETERANALYSIS_H
#include	"parameterSpace.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    nParameterAnalysis                                          */
/* Derived from:  parameterSpace                                              */
/* Purpose:       Computes a map showing the orbits dependence on parameters; */
/*                k parameters vary on the x axis, l on the y axis, for each  */
/*                parameter combination the resulting orbit is analysed on    */
/*                periodic behaviour;                                         */
/*                the corresponding point in the parameter space is colored   */
/*                depending on the order of the (possibly) periodic behaviour */
/* Author:        Markus Lohmann                                              */
/* Last modified: 07.03.1995                                                  */
/*                                                                            */
/******************************************************************************/

class nParameterAnalysis : public parameterSpace {
  protected:
    xyRange effectiveX;		// effective parameters on the x-axis
    xyRange effectiveY;		// effective parameters on the y-axis
    qreal** xVars;		// model parameters on the x-axis
    qreal** yVars;		// model parameters on the y-axis
    void setXParams(const qreal&); // update parameters on the x-axis
    void setYParams(const qreal&); // update parameters on the y-axis
  public:
    nParameterAnalysis(baseModel* const, const xyRange&, const xyRange&,
		       const xyRange&,const xyRange&,
               MacrodynGraphicsItem* const=NULL,
		       printer* const=NULL);
    virtual ~nParameterAnalysis();
    void simulation();
};    

#endif
//eof
