// File:	job.h
// Contents:	definition of class job
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:16:47 MEZ 1996
// Changes:	

#ifndef JOB_H
#define JOB_H
#include "../Models/baseModel.h"
#include "../Graphics/macrodyngraphicsitem.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    job                                                         */
/* Derived from:  -                                                           */
/* Purpose:       Virtual base class for all jobs; contains the pure virtual  */
/*                member function simulation                                  */
/* Author:        Markus Lohmann                                              */
/* Last modified: 11.10.1994                                                  */
/*                                                                            */
/******************************************************************************/

class job {
  protected:
    qint64 length;                       // length of simulation
    qint64 limit;                        // for limit <t<= length the simulations
                                       // output is examined
    baseModel* const model;            // model to analysed
    MacrodynGraphicsItem* screenGraphics;    // the output of the simulation can be
                                       // drawn on the screen ..
  public:
    job(baseModel * const, MacrodynGraphicsItem * = NULL);
    job(baseModel * const, const qint64&, const qint64&, MacrodynGraphicsItem * = NULL);
    virtual ~job()=0;
    virtual void setStepX(const qreal&);
    virtual void simulation()=0;
};
#endif
//eof
