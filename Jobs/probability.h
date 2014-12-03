///////////////////////////////////////////////////////////////////////////////
//
//
// Module name:		probability.h
// Contents:		Class definition of the class probability
//
// Author:		    Michael Meyer
// Last modified:	Mon Sep 18 14:32:41 METDST 2000 (Michael Meyer)
//
///////////////////////////////////////////////////////////////////////////////

#ifndef PROBABILITY_INCLUDED
#define PROBABILITY_INCLUDED

#include	"geometricJob.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		probability
// Derived from:	geometric job 
// Purpose:		    plot probability /******P(x>=y)=1-P(x<y)*****/
//
// Author:		    Michael Meyer
// Last modified:	Mon Sep 18 14:33:04 METDST 2000 (Michael Meyer)
//
///////////////////////////////////////////////////////////////////////////////
class probability: public geometricJob {
  protected:
    char xLabel[20];
    qreal *xParam;
    qreal xmin,xmax;
	qreal xStep;
	double Count[100001];
    double relativeCount;
	double relativeSum;
    xyRange * n_axes;
	int col,i,psize;
	qreal prob;

  public:
    probability(int, baseModel* const, xyRange&, MacrodynGraphicsItem* const=NULL, printer* const=NULL);
    void simulation();
};
#endif
//eof
