// File:	timeSeriesJob.h
// Contents:	definition of class timeSeriesJob
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:16:47 MEZ 1996
// Changes:	

#ifndef TIMESERIESJOB_H
#define TIMESERIESJOB_H
#include	"job.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    timeSeriesJob                                               */
/* Derived from:  job                                                         */
/* Purpose:       Virtual base class for jobs that analyse a time series      */
/* Author:        Markus Lohmann                                              */
/* Last modified: 11.10.1994                                                  */
/*                                                                            */
/******************************************************************************/

class timeSeriesJob : public job {
  protected:
    qreal *timeSeriesqreal;            // time series to be analysed
    qreal *modelVar;                  // variable that has to be stored
    void saveSeries(const qint64&);      // saves the simualtion results
  public:
    timeSeriesJob(baseModel* const, char* const, MacrodynGraphicsItem* const=NULL);
    virtual ~timeSeriesJob();
    virtual void simulation()=0;
};

#endif
//eof
