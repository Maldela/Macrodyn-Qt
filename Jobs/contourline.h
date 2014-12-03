///////////////////////////////////////////////////////////////////////////////
//
//
// Module name:		contourline.h
// Contents:		Class definition of the class contourline
//
// Author:		Michael Meyer
// Last modified:	Tue July 06 12:35:00 CEST 1999 (Michael Meyer)
//
///////////////////////////////////////////////////////////////////////////////

#ifndef CONTOURLINE_INCLUDED
#define CONTOURLINE_INCLUDED

#include	"geometricJob.h"
#include 	"floathisto2d.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		contourline
// Derived from:	geometric job 
// Purpose:		plot contourline
//
// Author:		Michael Meyer
// Last modified:	Tue July 06 12:35:00 CEST 1999 (Michael Meyer)
//
///////////////////////////////////////////////////////////////////////////////
class contourline : public geometricJob {
  protected:
    char zLabel[20];
    qreal *zParam;

    qreal stepX;
    qreal stepY;
    qreal min;
    qreal max;
    xyRange effectiveX;			// effective parameters on x-axis
    xyRange effectiveY;			// effective parameters on y-axis
    qreal** xVars;			// model parameters on x-axis
    qreal** yVars;			// model parameters on y-axis
    void setXParams(const qreal&);	// update x-parameters
    void setYParams(const qreal&);	// update y-parameters
    floathisto_2d h;
    ofstream outFile;
    	
  public:
    contourline(baseModel* const, const xyRange&, const xyRange&, const xyRange&,
        MacrodynGraphicsItem* const=NULL);
	
	~contourline();
	
    void simulation();
};

#endif
//eof
