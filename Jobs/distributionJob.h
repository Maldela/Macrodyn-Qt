///////////////////////////////////////////////////////////////////////////////
//
//
// Module name:		distributionJob.h
// Contents:		Class definition of the class distribution
//
// Author:		    Michael Meyer
// Last modified:	Mon Sep 18 14:32:41 METDST 2000 (Michael Meyer)
//
///////////////////////////////////////////////////////////////////////////////

#ifndef DISTRIBUTIONJOB_INCLUDED
#define DISTRIBUTIONJOB_INCLUDED

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
class distributionJob: public geometricJob {
  protected:
    QString xLabel;
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
    distributionJob(int, baseModel* const, xyRange&, MacrodynGraphicsItem* const=NULL);
    void simulation();
};
#endif
//eof
