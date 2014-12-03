///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/d_bif2D.h,v 1.1 2000/08/31 16:21:47 mmeyer Exp $
//
// Module name:		d_bif2D.h
// Contents:		Class definition of class d_bif2D d_bif2D_f2
//			draw a bifurcation diagram with encoded densities
//
// Author:		Uli Middelberg
// Last modified:	Wed Jul 23 18:13:41 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#ifndef D_BIF2D_INCLUDED
#define D_BIF2D_INCLUDED

#include	"geometricJob.h"
#include	"histo.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		d_bif2D
// Derived from:	geometricJob
// Purpose:		draw a bifurcation diagram with encoded densities
//			
// Author:		Uli Middelberg
// Last modified:	Thu Jul 17 17:00:05 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class d_bif2D : public geometricJob {
  protected:
    ofstream outFile;
    histo_1d	h;
    qreal 	stepX;	// stepsize on the x-axis
    qreal 	stepY;	// stepsize on the y-axis
    int		x_res;
    
  public:
    d_bif2D(baseModel* const, const xyRange&, MacrodynGraphicsItem* const=NULL,
	  printer* const=NULL);
    void setStepX(const qreal&);
    void simulation();
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		d_bif2D_f2
// Derived from:	d_bif2D
// Purpose:		draw a bifurcation diagram with encoded densities
//			for model->iteration()^2
//			
// Author:		Uli Middelberg
// Last modified:	Thu Jul 17 17:00:05 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class d_bif2D_f2 : public d_bif2D {
  public:
    d_bif2D_f2(baseModel* const, const xyRange&, MacrodynGraphicsItem* const=NULL,
	  printer* const=NULL);
    void simulation();
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		d_bif2D_f4
// Derived from:	d_bif2D
// Purpose:		draw a bifurcation diagram with encoded densities
//			for model->iteration()^4
//			
// Author:		Uli Middelberg
// Last modified:	Tue Aug 26 14:29:57 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class d_bif2D_f4 : public d_bif2D {
  public:
    d_bif2D_f4(baseModel* const, const xyRange&, MacrodynGraphicsItem* const=NULL,
	  printer* const=NULL);
    void simulation();
};

#endif	// D_BIF2D_INCLUDED

//eof
