///////////////////////////////////////////////////////////////////////////////
//
// $Header: density.C,v 1.1 2000/08/31 16:22:20 mmeyer Exp $
//
// Module name:		density.C
// Contents:		member functions of classes density_1d and density_1d_2p
//			making a density plot with respect to a state variable
//
// Author:		Uli Middelberg
// Last modified:	Fri Jul 11 18:18:14 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#include	<unistd.h>
#include	"../error.h"
#include	"density.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		density_1d
// Member function:	density_1d
// Purpose:		constructor
//
// Author:		Uli Middelberg
// Last modified:	Fri Jul 11 18:18:08 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

density_1d::density_1d(baseModel* const bMod, xyRange & axes, 
                   MacrodynGraphicsItem* const graph, printer* const outDev)
                      :geometricJob(bMod,axes,graph,outDev),
                       h(axes.min[0],axes.max[0],axes.res[0])
{
    stepX=(xmax-xmin) / (1.0*axes.res[0]);
    stepY=(ymax-ymin) / (1.0*axes.res[1]);
    n_axes = & axes;
    length=model->getLength();
    limit=(long)(0.2*length);			// 20% are thrown away
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		density_1d
// Member function:	density_1d
// Purpose:		simulation
//			plot a histogram with shows the relative number of
//			hits with respect to one state variable
//
// Author:		Uli Middelberg
// Last modified:	Fri Jul 11 18:18:22 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void density_1d::simulation()
{
    long t;
    int  k;
    int  color = 9;	// color used for the density plot
    qreal d;
    qreal dy;

  model->initialize();		// run only one simulation with a fixed
  for(t=0;t<length;t++) {	// parameter set
    model->iteration(t+1);
    if( t >= limit  ) {
      h.inc(*xParam);		// store the hit in the histogram
    }
  }

  strcpy(n_axes->label[1],"den");	// relabel the y-axis
  if( screenGraphics ) {
    screenGraphics->reset(*n_axes);
  }
  if( printDev ){
    printDev->setLimits(*n_axes);
  }
  
  for( *xParam=xmin, k=0 ;*xParam<=xmax; *xParam+=stepX, k++) {
    d=((double) h(k))/h.get_no_hits();
    for( dy=0; dy<d; dy+=stepY ) {	// draw a vertical line with points
      if( screenGraphics ) {
        screenGraphics->setPoint(*xParam,dy,color);
      }
      if( printDev ) {
        printDev->setBits(*xParam,dy,color);
      }
    }
    if( screenGraphics ) {		// draw the end point
      screenGraphics->setPoint(*xParam,d,color);
    }
    if( printDev ) {
      printDev->setBits(*xParam,d,color);
    }
  }
  cout << endl;				// this is for external usage
  for( k=0 ; k< h.get_x_res() ; k++) {
    d=((double) h(k))/h.get_no_hits();
    cout << "\t" << d;
  }
  cout << endl;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		density_1d_1p
// Member function:	density_1d_1p
// Purpose:		constructor
//
// Author:		Uli Middelberg
// Last modified:	Fri Jul 11 18:18:08 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

density_1d_1p::density_1d_1p(baseModel* const bMod, xyRange & axes, 
                         MacrodynGraphicsItem* const graph, printer* const outDev)
                      :density_1d(bMod,axes,graph,outDev)
{
    zmin=axes.min[2];			// save the bounderies for the second
    zmax=axes.max[2];			// bifurcation parameter
    strcpy(zLabel,axes.label[2]);
    zParam=model->setLabels(zLabel);	// get the pointer to the second
					// bifurcation parameter
    if( !zParam )
	error("density_1d_1p::density_1d_1p: Undefined label %s",zLabel);
    stepZ=(zmax-zmin)/axes.res[2];
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		density_1d_1p
// Member function:	density_1d_1p
// Purpose:		simulation
//			plot a sequence of histograms with shows the number of
//			relative hits with respect to one state variable
//			this job is available for the graphics device only
//
// Author:		Uli Middelberg
// Last modified:	Fri Jul 11 18:18:22 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void density_1d_1p::simulation()
{
    long t;
    int  k;
    int  color = 9;				// color used for the density plot
    qreal d;
    qreal dy;

  strcpy(n_axes->label[1],"den");		// relabel the y-axis
  if( screenGraphics ) {
    screenGraphics->reset(*n_axes);
  }
  
  for(*zParam=zmin;*zParam<=zmax; *zParam+=stepZ) {
    model->initialize();
    h.reset();
    for(t=0;t<length;t++) {
      model->iteration(t+1);
      if( t >= limit  ) {
        h.inc(*xParam);
      }
    }
    for( *xParam=xmin, k=0 ;*xParam<=xmax; *xParam+=stepX, k++) {
      screenGraphics->clearColumn(*xParam);	// clear the column of the old picture
      d=((double) h(k))/h.get_no_hits();
      for( dy=0; dy<d; dy+=stepY ) {		// draw a vertical line with points
        if( screenGraphics ) {
          screenGraphics->setPoint(*xParam,dy,color);
        }
      }
      if( screenGraphics ) {			// draw the end point
        screenGraphics->setPoint(*xParam,d,color);
        screenGraphics->flushGraph();
      }
    }
  sleep(2);  					// there is still a problem with X
  }
}

// eof
