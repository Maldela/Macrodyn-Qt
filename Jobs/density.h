///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/density.h,v 1.1 2000/08/31 16:22:23 mmeyer Exp $
//
// Module name:		density.h
// Contents:		Class definition of classes density_1d and density_1d_2p
//			making a density plot with respect to a state variable
//
// Author:		Uli Middelberg
// Last modified:	Thu Jul 10 17:56:12 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _DENSITY_INCLUDED
#define _DENSITY_INCLUDED

#include	"geometricJob.h"
#include	"histo.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		density_1d
// Derived from:	
// Purpose:		making a density plot with respect to a state variable
//			plot a histogram with shows the relative number of
//			hits with respect to one state variable
//
// Author:		Uli Middelberg
// Last modified:	Thu Jul 10 17:56:12 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class density_1d : public geometricJob {
  protected:
    histo_1d	h;
    qreal        stepX;
    qreal        stepY;
    xyRange     * n_axes;	// we have to manipulate the xy_range
				// because, the y-axes is always reserved
				// for the relative count        
    
  public:
    density_1d(baseModel* const, xyRange &, MacrodynGraphicsItem* const=NULL);
    void simulation();
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		density_1d_1p
// Derived from:	density_1d
// Purpose:		making a series of density plots with respect to a state variable
//			plot a sequence of histograms with shows the number of
//			relative hits with respect to one state variable
//			this job is available for the graphics device only
//
// Author:		Uli Middelberg
// Last modified:	Thu Jul 10 17:56:12 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class density_1d_1p : public density_1d {
  protected:
    qreal zmin;                       // describes the second
    qreal zmax;                       // parameter
    char zLabel[256];
    qreal stepZ;
    qreal *zParam;
    
  public:
    density_1d_1p(baseModel* const, xyRange &, MacrodynGraphicsItem* const=NULL);
    void simulation();
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		density_1d_var
// Derived from:	
// Purpose:		making a density plot with respect to a state variable
//			plot a histogram which shows the relative number of
//			hits with respect to one state variable
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

class density_1d_var : public density_1d_1p {
  protected:    
    qint64 var_res;
    
  public:
    density_1d_var(baseModel* const, xyRange &, MacrodynGraphicsItem* const=NULL);
    void simulation();
    void plot_current_data(qint64 how_many);
    void save_current_picture(qint64 how_many);
};

#endif // _DENSITY_INCLUDED

//eof
