///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/d_attractor.h,v 1.1 2000/08/31 16:21:28 mmeyer Exp $
//
// Module name:		3d_attractor.h
// Contents:		Class definition of the class d_attractor
//			compute the approximation of the limit distribution
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

#ifndef ATTRACTOR_3D_INCLUDED
#define ATTRACTOR_3D_INCLUDED

#include	"geometry3D.h"
#include	"histo.h"


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		3d_attractor
// Derived from:	geometric job 
// Purpose:		compute the approximation of the limit distribution
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

class attractor_3d : public geometry3D {
  protected:
    ofstream outFile;
    histo_3d	h;
    qreal 	stepX;	// stepsize on the x-axis
    qreal 	stepY;	// stepsize on the y-axis
    qreal	stepZ;	// stepsize on the z-axis
  public:
    attractor_3d(baseModel* const, const xyRange&, MacrodynGraphicsItem* const=NULL,
          printer* const=NULL);
    void simulation();
};

#endif

//eof
