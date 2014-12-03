///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/l_attractor.h,v 1.1 2000/08/31 16:23:55 mmeyer Exp $
//
// Module name:		d_attractor.h
// Contents:		Class definition of the class l_attractor
//			display the orbit with connected lines
//
// Author:		Uli Middelberg
// Last modified:	Thu Oct 30 16:43:33 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#ifndef L_ATTRACTOR_INCLUDED
#define L_ATTRACTOR_INCLUDED

#include	"attractor.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		l_attractor
// Derived from:	attractor 
// Purpose:		display the orbit with connected lines
//
// Author:		Uli Middelberg
// Last modified:	Thu Oct 30 16:42:04 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class l_attractor : public attractor {
  public:
  	int pointsize;
  
  
    l_attractor(baseModel* const, const xyRange&, MacrodynGraphicsItem* const=NULL,
	      printer* const=NULL, int psize=1);
     void simulation();
};

#endif

//eof
