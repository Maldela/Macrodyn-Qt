///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/d_attractor.h,v 1.1 2000/08/31 16:21:28 mmeyer Exp $
//
// Module name:		d_attractor.h
// Contents:		Class definition of the class d_attractor
//			compute the approximation of the limit distribution
//
// Author:		Uli Middelberg
// Last modified:	Wed Oct 29 15:30:30 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#ifndef D_ATTRACTOR_INCLUDED
#define D_ATTRACTOR_INCLUDED

#include	"geometricJob.h"
#include	"histo.h"


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		d_attractor
// Derived from:	geometric job 
// Purpose:		compute the approximation of the limit distribution
//
// Author:		Uli Middelberg
// Last modified:	Thu Jun 19 17:02:59 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class d_attractor : public geometricJob {
  protected:
    ofstream outFile;
    histo_2d	h;
    qreal 	stepX;	// stepsize on the x-axis
    qreal 	stepY;	// stepsize on the y-axis
  public:
    d_attractor(baseModel* const, const xyRange&, MacrodynGraphicsItem* const=NULL);
    void simulation();
};

#endif

//eof
