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
#include "../Printer/printer.h"

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
    long length;                       // length of simulation
    long limit;                        // for limit <t<= length the simulations
                                       // output is examined
    baseModel* const model;            // model to analysed
    MacrodynGraphicsItem* const screenGraphics;    // the output of the simulation can be
                                       // drawn on the screen ..
    printer* const printDev;           // .. or printed into a PCL- or PS-File
  public:
    job(baseModel * const, QObject * = NULL, printer * const = NULL);
    job(baseModel * const, const long&, const long&, QObject * = NULL, printer * const = NULL);
    virtual ~job()=0;
    virtual void setStepX(const qreal&);
    virtual void simulation()=0;
};
#endif
//eof
