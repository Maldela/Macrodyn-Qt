///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/indicator.h,v 1.1 2000/08/31 16:23:23 mmeyer Exp $
//
// Module name:		indicator.h
// Contents:		Class definition of class indicator
//			draw color encoded iterates of the indicator function
//
// Author:		Uli Middelberg
// Last modified:	Mon Jul 14 19:34:32 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _INDICATOR_INCLUDED
#define _INDICATOR_INCLUDED

#include	"geometricJob.h"
#include	"d_var.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		indicator_2d
// Derived from:	geometricJob
// Purpose:		draw color encoded iterates of the indicator function
//			
// Author:		Uli Middelberg
// Last modified:	Mon Jul 14 19:36:22 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class indicator_2d : public geometricJob {
  protected:
    qreal        stepX;
    qreal        stepY;
    qreal 	zmin;
    qreal 	zmax;
    char 	zLabel[256];
    qreal	stepZ;
    qreal 	* zParam;

    int		get_encoding(void);	// the interval between zmin and zmax
    const int	n_colors;		// will be devided into n_color parts
    d_var	* color_code;		// each part is encoded with a different
    					// color
  public:
    indicator_2d(baseModel* const, xyRange &, MacrodynGraphicsItem* const=NULL);
    void simulation();
};

#endif // _INDICATOR_INCLUDED

//eof
