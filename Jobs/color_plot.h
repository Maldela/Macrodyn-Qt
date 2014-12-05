///////////////////////////////////////////////////////////////////////////////
//
//
// Module name:		color_plot.h
// Contents:		Class definition of the class color_plot
//
// Author:		Marc Mueller
// Last modified:	mhoffman Mon Sep 11 13:15:48 METDST 2000
//
///////////////////////////////////////////////////////////////////////////////

#ifndef color_plot_INCLUDED
#define color_plot_INCLUDED

#include	"geometricJob.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		color_plot
// Derived from:	geometric job 
// Purpose:		plot color
//
// Author:		Marc Mueller
// Last modified:	mhoffman Mon Sep 11 13:15:39 METDST 2000
//
///////////////////////////////////////////////////////////////////////////////

class color_plot : public geometricJob {
  protected:
    QString zLabel;
    qreal *zParam;
    qreal zmin,zmax;
	int psize;

  public:
    color_plot(int, baseModel* const, const xyRange&, MacrodynGraphicsItem* const=NULL);
    void simulation();
};

#endif
//eof
