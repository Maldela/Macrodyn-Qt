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
         MacrodynGraphicsItem* const graph, printer* const outDev)
          :geometricJob(bMod,axes,graph,outDev),
           h(axes.min[1],axes.max[1],axes.res[1])
{
//    if( fileName )
//	outFile.open(fileName,ios::out);
 //   else
	outFile.open("data3D_bifurcation.dat",ios::out);

    length=model->getLength();
    limit=(long)(0.2*length);			// 20% are thrown away
    stepX=(xmax-xmin) / (axes.res[0]-1);
    stepY=(ymax-ymin) / (axes.res[1]-1);
    x_res = axes.res[0];
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

void d_bif2D::simulation()
{
    long t;
    int  k;
    qreal dy;
    qreal dummy_x;
    double  h_max;
    int  color=0;

//  cout << "xparam" << "\t"<< xmin << "\t"<< xmax << endl;
//  cout << "yparam" << "\t"<< ymin << "\t"<< ymax << endl; 

//initialize output information
  int jobtag_dummy = 31;
  int resolution_y = h.get_x_res();

  cout << "resolution: [" << x_res << "," << resolution_y <<"]\n";
  outFile.write((char*)&jobtag_dummy, 4);
  outFile.write((char*)&x_res, 4);
  outFile.write((char*)&resolution_y, 4);
  
  outFile.write((char*)&xmin, 8);
  outFile.write((char*)&xmax, 8);
  outFile.write((char*)&ymin, 8);
  outFile.write((char*)&ymax, 8);
  
  int length_of_label = strlen(xLabel);
  outFile.write((char*)&length_of_label, 4);
  outFile.write((char*)&xLabel, length_of_label);
  length_of_label = strlen(yLabel);
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
        h.inc(*yParam);
      }
    }
    
    h_max = double (h.get_max_hits());
	cout << "h_max = " << h_max << endl;
	cout << "color step every " << h_max/94 << " hits in cell" << endl;
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
	cout << "maximal hitcounts at: " << dy << endl;
	if(hitpoint>0){ 
		color=int(ceil(hitpoint*dummy2));
//	    outFile << *xParam << "\t" << *yParam << "\t" << hitpoint << endl; 		
	}
	else color=0;
/*Ende neu*/
// color alt      color = int (floor( ( double (h(k)) / double (h_max) ) * 34));
  
  	outFile << char( color );
      if( screenGraphics ) {
        screenGraphics->setPoint(dummy_x,dy,color); 
      }
      if( printDev ) {
        printDev->setBits(dummy_x,dy,color);
      }

    //outFile << *xParam << "\t" << dy << "\t" << hitpoint << endl; 
//    outFile << hitpoint << "\t";

//      cout << *xParam << "\t" << dy << "\t" << h(k) << endl;
    }
    h.reset();
//	    outFile << endl;
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
         MacrodynGraphicsItem* const graph, printer* const outDev)
          :d_bif2D(bMod,axes,graph,outDev)
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
    long t;
    int  k;
    qreal dy;
    qreal dummy_x;
    int  h_max;
    int  color;

  cout << "xparam" << "\t"<< xmin << "\t"<< xmax << endl;
  cout << "yparam" << "\t"<< ymin << "\t"<< ymax << endl; 

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
      if( printDev ) {
        printDev->setBits(dummy_x,dy,color);
      }

      cout << dummy_x << "\t" << dy << "\t" << h(k) << endl;
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
         MacrodynGraphicsItem* const graph, printer* const outDev)
          :d_bif2D(bMod,axes,graph,outDev)
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
    long t;
    int  k;
    qreal dy;
    qreal dummy_x;
    int  h_max;
    int  color;

  cout << "xparam" << "\t"<< xmin << "\t"<< xmax << endl;
  cout << "yparam" << "\t"<< ymin << "\t"<< ymax << endl; 

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
      if( printDev ) {
        printDev->setBits(dummy_x,dy,color);
      }

      cout << dummy_x << "\t" << dy << "\t" << h(k) << endl;
    }
    h.reset();
  }
}

// eof
