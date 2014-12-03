///////////////////////////////////////////////////////////////////////////////
//
//
// Module name:		distribution.h
// Contents:		Class definition of the class distribution
//
// Author:		    Michael Meyer
// Last modified:	Mon Sep 18 14:32:41 METDST 2000 (Michael Meyer)
//
///////////////////////////////////////////////////////////////////////////////

#ifndef DISTRIBUTION_INCLUDED
#define DISTRIBUTION_INCLUDED

#include	"geometricJob.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		distribution
// Derived from:	geometric job 
// Purpose:		    plot distribution /******P(x>=y)=1-P(x<y)*****/
//
// Author:		    Michael Meyer
// Last modified:	Mon Sep 18 14:33:04 METDST 2000 (Michael Meyer)
//
///////////////////////////////////////////////////////////////////////////////
class distribution: public geometricJob {
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
    distribution(int, baseModel* const, xyRange&, MacrodynGraphicsItem* const=NULL, printer* const=NULL);
    void simulation();
};
#endif
//eof
