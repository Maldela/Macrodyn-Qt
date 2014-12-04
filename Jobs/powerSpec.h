// File:	powerSpec.h
// Contents:	definition of class powerSpec
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:16:47 MEZ 1996
// Changes:	

#ifndef POWERSPEC_H
#define POWERSPEC_H
#include	"timeSeriesJob.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    powerSpec                                                   */
/* Derived from:  timeSeriesJob                                               */
/* Purpose:       Computes the power spectrum of a time series                */
/* Author:        Markus Lohmann                                              */
/* Last modified: 11.10.1994                                                  */
/*                                                                            */
/******************************************************************************/

class powerSpec : public timeSeriesJob {
  protected:
    qreal squareAndSum(const qreal&, const qreal&);
    void FFTSwap(qreal&,qreal&);
    void FFTSolve(const int&);
    void powerSpecCalc(const qreal&);
    qreal parzen(const qint64&);
    qreal hanning(const qint64&);
    qreal welch(const qint64&);
    qreal hamming(const qint64&);
    qreal exactBlackman(const qint64&);
    void windowData(qreal*);

    qreal* timeSeriesImag;
    const qreal Pi;
    const window win;
  public:
    powerSpec(baseModel* const,const window&, char* const, MacrodynGraphicsItem* const=NULL);
    virtual ~powerSpec();
    void simulation();
};

#endif
//eof
