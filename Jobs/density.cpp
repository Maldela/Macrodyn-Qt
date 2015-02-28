///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/density.C,v 1.2 2000/10/18 14:43:31 mmeyer Exp $
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
#include	"../logger.h"
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
                   MacrodynGraphicsItem* const graph)
                      :geometricJob(bMod,axes,graph),
                       h(axes.min[0],axes.max[0],(axes.res[0]>0?axes.res[0]:100.0))
{
    stepX=(xmax-xmin) / (1.0*(axes.res[0]>0?axes.res[0]:100.0));
    stepY=(ymax-ymin) / (1.0*(axes.res[1]>0?axes.res[1]:100.0));
    n_axes = & axes;
    length=model->getLength();
    limit=(qint64)(0.2*length);			// 20% are thrown away
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
    qint64 t;
    int  k;
    qreal d;
//    qreal dy;

  model->initialize();		// run only one simulation with a fixed
  for(t=0;t<length;t++) {	// parameter set
    model->iteration(t+1);
    if( t >= limit  ) {
      h.inc(*xParam);		// store the hit in the histogram
    }
  }

  n_axes->label[1] = "den";	// relabel the y-axis
  if( screenGraphics )
    screenGraphics->setXYRange(*n_axes);

  for( *xParam=xmin, k=0 ;*xParam<=xmax; *xParam+=stepX, k++) {
    // d=((double) h(k))/h.get_no_hits();

    // Introducing new density calculation for qreal probability
    d= ((double) h(k))/double(length - limit);
    d= d/stepX;

    screenGraphics->setRect(*xParam,0,stepX*1.2,d,QColor(0,0,150));
//    for( dy=0; dy<d; dy+=stepY ) {	// draw a vertical line with points
//      if( screenGraphics ) {
//        //log() << *xParam << "   " << dy << "   " << color;
//        screenGraphics->setBigPoint(*xParam,dy,color,5);
//      }
//    }
//    if( screenGraphics ) {		// draw the end point
//      screenGraphics->setBigPoint(*xParam,d,color,5);
//    }

  }
  //log() << "class width = " << stepX;
 // log() << "possible number of hits = " << length - limit;
  for( k=0 ; k< h.get_x_res() ; k++) {
  // d=((double) h(k))/h.get_no_hits();
    d=((double) h(k))/double(length - limit);
    d=d/stepX;
//    log() << "\t" << d;
  }
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
                         MacrodynGraphicsItem* const graph)
                      :density_1d(bMod,axes,graph)
{
    zmin=axes.min[2];			// save the bounderies for the second
    zmax=axes.max[2];			// bifurcation parameter
    zLabel = axes.label[2];
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
    qint64 t;
    int  k;
    int  color = 9;				// color used for the density plot
    qreal d;
    qreal dy;

  n_axes->label[1] = "den";		// relabel the y-axis
  if( screenGraphics ) {
    screenGraphics->setXYRange(*n_axes);
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
      //d=((double) h(k))/h.get_no_hits();
        d=((double) h(k))/double(length - limit);
	d=d/stepX;
      for( dy=0; dy<d; dy+=stepY ) {		// draw a vertical line with points
        if( screenGraphics ) {
          screenGraphics->setPoint(*xParam,dy,color);
        }
      }
      if( screenGraphics ) {			// draw the end point
        screenGraphics->setPoint(*xParam,d,color);
      }
    }
  sleep(2);  					// there is still a problem with X
  }
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		density_1d_var
// Member function:	density_1d_var
// Purpose:		constructor
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

density_1d_var::density_1d_var(baseModel* const bMod, xyRange & axes, 
                   MacrodynGraphicsItem* const graph)
                      :density_1d_1p(bMod,axes,graph)
{
	var_res = axes.res[2]+1;
    log() << "no. of runs: " << var_res << "\tT: " << length;
    log() << "z: " << zLabel << "\tzmin: " << zmin << "\tzmax: " << zmax;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		density_1d_var
// Member function:	simulation
// Purpose:		
//			plot a histogram with shows the relative number of
//			hits with respect to one state variable
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void density_1d_var::simulation()
{
    qint64 t;
    qint64 how_many;
	float percent = 0.0;
    n_axes->label[1] = "den";	// relabel the y-axis

	// looping in for varying zParam
    for ( *zParam=zmin, how_many=1; *zParam<=zmax; *zParam+=stepZ, how_many++ ){
        model->initialize();		// run one simulation with a fixed
		for(t=0;t<length;t++) {		// zParameter to T
			model->iteration(t+1);
		}
		h.inc(*xParam);		// store the hit in the histogram
		if ( screenGraphics )
			if ( how_many>= 100 ){
            if ( how_many%(qint64(var_res/100))==0 ){
				plot_current_data(how_many);
				percent+=1;
//                log() << "percent: " << percent;
			}
			}
	}
	// make final picture and file
	if ( screenGraphics ) plot_current_data(how_many);

//    log() << "class width = " << stepX;
//    log() << "possible number of hits = " << var_res;
	int k;
	double d;
	double d_cum = 0;
	for( k=0 ; k< h.get_x_res() ; k++) {
		// d=((double) h(k))/h.get_no_hits();
		d=((double) h(k))/double(var_res);
		d_cum+=d;
		d=d/stepX;		
//        log() << "F(" << double(k)*stepX+xmin << ")= " << d_cum << "\t"
//        << "f()= " << d;
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		density_1d_var
// Member function:	plot_current_data
// Purpose:		plot intermediate results
//			plot a histogram which shows the relative number of
//			hits with respect to one state variable
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void density_1d_var::plot_current_data(qint64 how_many)
{
	int  k;
	qreal dummyx;
	int  color = 9;	// color used for the density plot
	qreal d;
	qreal y_plot;
	
    y_plot = qreal(h.get_max_hits()) / qreal(how_many);
// plotting pseudo density
//	y_plot = y_plot/stepX*1.01;
// plotting relative occurence
    y_plot *= 1.01;

    n_axes->max[1] = y_plot;
	
    screenGraphics->setXYRange(*n_axes);
	
    for( dummyx=xmin, k=0 ;dummyx<=xmax; dummyx+=stepX, k++) {
        // d=((double) h(k))/h.get_no_hits();
		
		// Introducing new density calculation for qreal probability
		d= ((double) h(k))/double(how_many);
// plotting pseudo density
//		d= d/stepX;
		screenGraphics->setLine(dummyx,0,dummyx,d,color);
  	}
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		density_1d_var
// Member function:	save_current_picture
// Purpose:		save results in xpm file
//			plot a histogram which shows the relative number of
//			hits with respect to one state variable
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void density_1d_var::save_current_picture(qint64 how_many)
{
	int  k;
	qreal dummyx;
//	int  color = 9;	// color used for the density plot
	qreal d;
	qreal y_plot;
	
    y_plot = double(h.get_max_hits()) / double(how_many);
// plotting pseudo density
//	y_plot = y_plot/stepX*1.01;
// plotting qrealtive occurence
	y_plot = y_plot*1.01;
	
    n_axes->max[1] = y_plot;

    for( dummyx=xmin, k=0 ; dummyx<=xmax; dummyx+=stepX, k++) {
        // d=((double) h(k))/h.get_no_hits();
		
		// Introducing new density calculation for qreal probability
		d= ((double) h(k))/double(how_many);
// plotting pseudo density
		// d= d/stepX;
  	}
}

// eof
