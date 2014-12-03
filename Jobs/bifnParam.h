// File:	bifnParam.h
// Contents:	definition of class nParameterBifurcation
// Author:	Michael Meyer
// Creation:	Mon Apr 10 12:29:17 METDST 2000
// Modified:	Mon Apr 10 12:29:21 METDST 2000
// Changes:	

#ifndef BIFNPARAM_H
#define BIFNPARAM_H
//#include "parameterSpace.h"
#include "bif2D.h"
/******************************************************************************/
/*                                                                            */
/* Class name:    Bif_n_Parameter                                             */
/* Derived from:  parameterSpace                                              */
/* Purpose:       compute a bifurcation for an                                */
/*                x-bundle for n parameters  				      */
/*                                                                            */
/* Author:        Michael Meyer	                                              */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/

//class bifnParam : public parameterSpace {
class bifnParam : public bif2D {
	
  protected:
    xyRange effectiveX;		// effective parameters on the x-axis
    qreal stepX;                      // stepsize for the first parameter
    qreal** xVars;		// model parameters on the x-axis
    void setXParams(const qreal&); // update parameters on the x-axis
  public:
//    bifnParam(baseModel* const, const xyRange&,
     bifnParam(baseModel* const, 
		       const xyRange&,const xyRange&,
               MacrodynGraphicsItem* const=NULL,printer* const=NULL);
    virtual ~bifnParam();
    void simulation();
};    

#endif
//eof
