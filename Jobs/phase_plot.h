///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/phase_plot.h,v 1.1 2000/08/31 16:24:56 mmeyer Exp $
//
// Module name:		phase_plot.h
// Contents:		Class definition of the class phase plot
//			plot x_t vs x_t+k
//
// Author:		Uli Middelberg
// Last modified:	Mon Nov  3 16:54:14 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#ifndef PHASE_PLOT_INCLUDED
#define PHASE_PLOT_INCLUDED

#include	"geometricJob.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		phase_plot
// Derived from:	geometric job 
// Purpose:		plot x_t vs x_t+k
//
// Author:		Uli Middelberg
// Last modified:	Mon Nov  3 16:55:12 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class phase_plot : public geometricJob {
  protected:
    long lag_size;	// the size of the lag (k)
    int pointsize;

  public:
    phase_plot(long, baseModel* const, const xyRange&, MacrodynGraphicsItem* const=NULL,
	      printer* const=NULL, int psize=1);
    void simulation();
};

#endif
//eof
