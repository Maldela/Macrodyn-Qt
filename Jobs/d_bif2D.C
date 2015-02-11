///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/d_bif2D.C,v 1.1 2000/08/31 16:21:44 mmeyer Exp $
//
// Module name:		d_bif2D.C
// Contents:		member functions of class d_bif2D
//			draw a bifurcation diagram with encoded densities
//
// Author:		Uli Middelberg
// Last modified:	Thu Jul 17 17:03:20 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#include	"d_bif2D.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		d_bif2D
// Member function:	d_bif2D
// Purpose:		constructor
//
// Author:		Uli Middelberg
// Last modified:	Thu Jul 17 17:05:43 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

d_bif2D::d_bif2D(baseModel* const bMod, const xyRange& axes, 
         MacrodynGraphicsItem* const graph)
          :geometricJob(bMod,axes,graph),
           h(axes.min[1],axes.max[1],(axes.res[0]>0?axes.res[0]:100.0))
{
    log()<<"axes.res[0] = "<<axes.res[0];
//    if( fileName )
//	outFile.open(fileName,ios::out);
 //   else
    outFile.setFileName("data3D_bifurcation.dat");
    outFile.open(QFile::WriteOnly);

    length=model->getLength();
    limit=(qint64)(0.2*length);			// 20% are thrown away
    stepX=(xmax-xmin) / (axes.res[0]-1);
    stepY=(ymax-ymin) / (axes.res[1]-1);
//    stepX=(xmax-xmin) / 10.0;
//    stepY=(ymax-ymin) / 10.0;
    log() << "stepX = " <<stepX;
    log() << "stepY = " <<stepY;
//    x_res = axes.res[0];
    x_res = 10;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		d_bif2D
// Member function:	setStepX
// Purpose:		set a new step size for the x axis
//
// Author:		Uli Middelberg
// Last modified:	Thu Jul 17 17:05:43 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void d_bif2D::setStepX(const qreal& toSet)
{
    stepX=toSet;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		d_bif2D
// Member function:	simulation
// Purpose:		draw a bifurcation diagram with encoded densities
//
// Author:		Uli Middelberg
// Last modified:	Thu Jul 17 17:05:43 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

int redFromUnit(double x){
    if(x<0.333) return (0.333-x)*3*255;
    if(x>0.666) return (x-0.666)*3*255;
    return 0;
}

int greenFromUnit(double x){
    if(x<0.333) return 255;
    if(x>0.333&&x<0.666) return (0.666-x)*3*255;
    return 0;
}

int blueFromUnit(double x){
    if(x<0.333) return (0.333-x)*3*255;
    if(x>0.333&&x<0.666) return (x-0.333)*3*255;
    return (1.0-x)*3*255;
}

void d_bif2D::simulation()
{
    qint64 t;
    int  k;
    qreal dy;
    qreal dummy_x;
    double  h_max;
    int  color=0;

//  log() << "xparam" << "\t"<< xmin << "\t"<< xmax 
//  log() << "yparam" << "\t"<< ymin << "\t"<< ymax 

//initialize output information
  int jobtag_dummy = 31;
  int resolution_y = h.get_x_res();

  log() << "resolution: [" << x_res << "," << resolution_y <<"]\n";
  outFile.write((char*)&jobtag_dummy, 4);
  outFile.write((char*)&x_res, 4);
  outFile.write((char*)&resolution_y, 4);
  
  outFile.write((char*)&xmin, 8);
  outFile.write((char*)&xmax, 8);
  outFile.write((char*)&ymin, 8);
  outFile.write((char*)&ymax, 8);
  
  int length_of_label = xLabel.size();
  outFile.write((char*)&length_of_label, 4);
  outFile.write((char*)&xLabel, length_of_label);
  length_of_label = yLabel.size();
  outFile.write((char*)&length_of_label, 4);
  outFile.write((char*)&yLabel, length_of_label);

  for(dummy_x=xmin;dummy_x<=xmax; dummy_x+=stepX) {
    *xParam = dummy_x;
    model->initialize();
    for(t=0;t<length;t++) {
      model->iteration(t+1);
      if( t >= limit && inRange(dummy_x,*yParam) ) {
        // only points inside the state space
	// section under consideration are
	// analysed
//          log() << "xParam = " <<*xParam;
//          log() << "yParam = " <<*yParam;
        h.inc(*yParam);
      }
    }
    
    h_max = double (h.get_max_hits());
    log() << "h_max = " << h_max;
    log() << "color step every " << h_max/94 << " hits in cell";
    if( h_max == 0 ) {
      h_max = 1;
    }

	double dummy2=94;
	double hitshilf;
	double hitpoint;
	   
    for( dy=ymin, k=0; k<h.get_x_res(); dy+=stepY, k++) {
    
      // color = h(k) % 32;
/*neu*/
	
	hitshilf=h(k);
    hitpoint = hitshilf/h_max;
    if (hitshilf==h_max)
    log() << "qMaximal hitcounts at: " << dy;
	if(hitpoint>0){ 
		color=int(ceil(hitpoint*dummy2));
//	    outFile << *xParam << "\t" << *yParam << "\t" << hitpoint 
	}
	else color=0;
/*Ende neu*/
// color alt      color = int (floor( ( double (h(k)) / double (h_max) ) * 34));
  
    QTextStream(&outFile) << char( color );
      if( screenGraphics ) {
        screenGraphics->setRect(dummy_x,dy,stepX,stepY,QColor(redFromUnit(hitpoint),greenFromUnit(hitpoint),blueFromUnit(hitpoint)));
      }

    //outFile << *xParam << "\t" << dy << "\t" << hitpoint 
//    outFile << hitpoint << "\t";

//      log() << *xParam << "\t" << dy << "\t" << h(k) 
    }
    h.reset();
//	    outFile 
  }
  outFile.flush();
  outFile.close();
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		d_bif2D_f2
// Member function:	d_bif2D_f2
// Purpose:		constructor
//
// Author:		Uli Middelberg
// Last modified:	Thu Jul 17 17:05:43 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

d_bif2D_f2::d_bif2D_f2(baseModel* const bMod, const xyRange& axes, 
         MacrodynGraphicsItem* const graph)
          :d_bif2D(bMod,axes,graph)
{
  length *= 2;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		d_bif2D_f2
// Member function:	simulation
// Purpose:		draw a bifurcation diagram with encoded densities
//			for model->iteration()^2
//
// Author:		Uli Middelberg
// Last modified:	Wed Jul 23 18:16:42 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void d_bif2D_f2::simulation()
{
    qint64 t;
    int  k;
    qreal dy;
    qreal dummy_x;
    int  h_max;
    int  color;

  log() << "xparam" << "\t"<< xmin << "\t"<< xmax;
  log() << "yparam" << "\t"<< ymin << "\t"<< ymax;

  for(dummy_x=xmin;dummy_x<=xmax; dummy_x+=stepX) {
    *xParam = dummy_x;
    model->initialize();
    for(t=0;t<length;t+=2) {
      model->iteration(t+1);
      model->iteration(t+2);
      if( t >= limit && inRange(dummy_x,*yParam) ) {
        // only points inside the state space
	// section under consideration are
	// analysed
        h.inc(*yParam);
      }
    }
    
    h_max = h.get_max_hits();

    if( h_max == 0 ) {
      h_max = 1;
    }
    
    for( dy=ymin, k=0; k<h.get_x_res(); dy+=stepY, k++) {
    
      // color = h(k) % 32;
      color = int (floor( ( double (h(k)) / double (h_max) ) * 32));
      
      if( screenGraphics ) {
        screenGraphics->setPoint(dummy_x,dy,color);
      }

      log() << dummy_x << "\t" << dy << "\t" << h(k);
    }
    h.reset();
  }
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		d_bif2D_f4
// Member function:	d_bif2D_f4
// Purpose:		constructor
//
// Author:		Uli Middelberg
// Last modified:	Tue Aug 26 14:30:46 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

d_bif2D_f4::d_bif2D_f4(baseModel* const bMod, const xyRange& axes, 
         MacrodynGraphicsItem* const graph)
          :d_bif2D(bMod,axes,graph)
{
  length *=4;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		d_bif2D_f4
// Member function:	simulation
// Purpose:		draw a bifurcation diagram with encoded densities
//			for model->iteration()^4
//
// Author:		Uli Middelberg
// Last modified:	Tue Aug 26 14:31:09 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void d_bif2D_f4::simulation()
{
    qint64 t;
    int  k;
    qreal dy;
    qreal dummy_x;
    int  h_max;
    int  color;

  log() << "xparam" << "\t"<< xmin << "\t"<< xmax;
  log() << "yparam" << "\t"<< ymin << "\t"<< ymax;

  for(dummy_x=xmin;dummy_x<=xmax; dummy_x+=stepX) {
    *xParam = dummy_x;
    model->initialize();
    for(t=0;t<length;t+=4) {
      model->iteration(t+1);
      model->iteration(t+2);
      model->iteration(t+3);
      model->iteration(t+4);
      if( t >= limit && inRange(dummy_x,*yParam) ) {
        // only points inside the state space
	// section under consideration are
	// analysed
        h.inc(*yParam);
      }
    }
    
    h_max = h.get_max_hits();

    if( h_max == 0 ) {
      h_max = 1;
    }
    
    for( dy=ymin, k=0; k<h.get_x_res(); dy+=stepY, k++) {
    
      // color = h(k) % 32;
      color = int (floor( ( double (h(k)) / double (h_max) ) * 32));
      
      if( screenGraphics ) {
        screenGraphics->setPoint(dummy_x,dy,color);
      }

      log() << dummy_x << "\t" << dy << "\t" << h(k);
    }
    h.reset();
  }
}

// eof
