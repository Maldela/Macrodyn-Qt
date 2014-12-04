// File:	gemetricJob.h
// Contents:	definition of class geometricJob
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:16:47 MEZ 1996
// Changes:	

#ifndef GEOMETRICJOB_H
#define GEOMETRICJOB_H
#include	"job.h"
#include	"../hashTab.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    geometricJob                                                */
/* Derived from:  job                                                         */
/* Purpose:       Virtual base class for all jobs that perform a geometric    */
/*                analysis of the model under consideration; a graphical      */
/*                output is the result of that analysis                       */
/* Author:        Markus Lohmann                                              */
/* Last modified: 11.10.1994                                                  */
/*                                                                            */
/******************************************************************************/

class geometricJob : public job
{
  protected:
    char xLabel[20];                   // variables that determine the
    char yLabel[20];                   // coordinate system
    qreal xmin;
    qreal xmax;
    qreal ymin;
    qreal ymax;
    qreal *xParam;                    // pointer to a model variable that
                                       // describes the x-axis
    qreal *yParam;                    // .. the y-axis
    hashTable *hash;                   // hash table to store simulation
                                       // results
    uint inRange(const qreal&,const qreal&);
                                       // returns wether the simulation results
                                       // are inside the considered section
  public:
    geometricJob(baseModel* const, const xyRange&, MacrodynGraphicsItem* const=NULL);
    virtual ~geometricJob();
    virtual void simulation()=0;
};
#endif
//eof
