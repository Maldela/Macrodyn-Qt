// File:	geometry3D.h
// Contents:	definition of class geometry3D
// Author:	Andreas Starke
// Creation:	Mon Mar 08 09:32:04 MEZ 2004
// Modified:	
// Changes:	

#ifndef GEOMETRY3D_H
#define GEOMETRY3D_H
#include	"job.h"
#include	"../hashTab.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    geometry3D                                                  */
/* Derived from:  job                                                         */
/* Purpose:       Virtual base class for all jobs that perform a geometric    */
/*                3D analysis of the model under consideration; a graphical   */
/*                output is the result of that analysis                       */
/* Author:        Andreas Starke                                              */
/* Last modified: 	                                                      */
/*                                                                            */
/******************************************************************************/

class geometry3D : public job {
  protected:
    char xLabel[20];                   // variables that determine the
    char yLabel[20];                   // coordinate system
    char zLabel[20];
    qreal xmin;
    qreal xmax;
    qreal ymin;
    qreal ymax;
    qreal zmin;
    qreal zmax;
    qreal *xParam;                    // pointer to a model variable that
                                       // describes the x-axis
    qreal *yParam;                    // .. the y-axis
    qreal *zParam;
    hashTable *hash;                   // hash table to store simulation
                                       // results
    uint inRange(const qreal&,const qreal&,const qreal&);
                                       // returns wether the simulation results
                                       // are inside the considered section
  public:
    geometry3D(baseModel* const,const xyRange&, MacrodynGraphicsItem* const=NULL);
    virtual ~geometry3D();
    virtual void simulation()=0;
};
#endif
//eof
