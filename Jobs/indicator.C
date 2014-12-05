///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/indicator.C,v 1.1 2000/08/31 16:23:20 mmeyer Exp $
//
// Module name:		indicator.C
// Contents:		member functions of class indicator
//			making a density plot with respect to a state variable
//
// Author:		Uli Middelberg
// Last modified:	Fri Jul 11 18:18:14 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#include	<unistd.h>
#include	"../error.h"
#include	"indicator.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		indicator_2d
// Member function:	indicator_2d
// Purpose:		constructor
//
// Author:		Uli Middelberg
// Last modified:	Mon Jul 14 19:46:38 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

indicator_2d::indicator_2d(baseModel* const bMod, xyRange & axes, 
                   MacrodynGraphicsItem* const graph)
                      :geometricJob(bMod,axes,graph),
                       n_colors(30)
{
    stepX=(xmax-xmin) / (1.0*axes.res[0]);
    stepY=(ymax-ymin) / (1.0*axes.res[1]);
    zmin=axes.min[2];	// save the bounderies for the range
    zmax=axes.max[2];	// of encoding of the indicator function
    zLabel = axes.label[2];
    zParam=model->setLabels(zLabel);	// get the pointer to the indicator value
    if( !zParam )
	error("indicator::indicator: Undefined label %s",zLabel);

    limit=(qint64)(0.0*length);		// 0% are thrown away
    
    color_code = new d_var (zmin, zmin, zmax, n_colors);
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		indicator_2d
// Member function:	simulation
//			draw color encoded iterates of the indicator function
//
// Author:		Uli Middelberg
// Last modified:	Fri Jul 11 18:18:22 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void indicator_2d::simulation()
{
    qint64 t;
    qint64 T;
    int color;
    qreal rx, ry;
  
  for( T=length;T<=length;T++ ) {
    for( rx=xmin; rx<=xmax; rx += stepX) {
      for( ry=ymin; ry<=ymax; ry += stepY) {
        model->initialize();
        *xParam=rx; *yParam=ry;
          for( t=0;t<T;t++ ) {
            model->iteration(t+1);
          }
        color=get_encoding();
        if( screenGraphics ) {
          screenGraphics->setPoint(rx,ry,color);
        }
      }
    }
  }
}


int indicator_2d::get_encoding( void ) {
  if( *zParam > zmax  ) {
    return 31;
  }
  if( *zParam < zmin ) {
    return 0;
  } // now, *zParam is between zmin and zmax and we have n_colos colors left
  color_code->set_val(*zParam);		// the d_var color_code translates
  return color_code->get_dval();	// *zParam into a color code
}

// eof
