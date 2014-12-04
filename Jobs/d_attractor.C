///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/d_attractor.C,v 1.1 2000/08/31 16:21:25 mmeyer Exp $
//
// Module name:		d_attractor.C
// Contents:		Class definition of the class d_attractor
//			compute the approximation of the limit distribution
//
// Author:		Uli Middelberg
// Last modified:	Wed Oct 29 15:30:36 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#include	"d_attractor.h"

///////////////////////////////////////////////////////////////////////////////
//
// member function:	d_attractor
// Purpose:		constructor
//
// Author:		Uli Middelberg
// Last modified:	Thu Jun 19 16:56:29 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

d_attractor::d_attractor(baseModel* const bMod,const xyRange& axes,
                  MacrodynGraphicsItem* const graph)
            :geometricJob(bMod,axes,graph),
             h(axes.min[0],axes.max[0],axes.res[0],
               axes.min[1],axes.max[1],axes.res[1])
{
//    if( fileName )
//	outFile.open(fileName,ios::out);
 //   else
	outFile.open("data3D_attraktor.dat",ios::out);

    limit= length / 10;			// 10% are thrown away
    stepX=(xmax-xmin) / (axes.res[0]-1);
    stepY=(ymax-ymin) / (axes.res[1]-1);
}                                      

///////////////////////////////////////////////////////////////////////////////
//
// member function:	simulation
// Purpose:		compute the approximation of the limit distribution
//
// Author:		Uli Middelberg
// Last modified:	Thu Jun 19 16:56:29 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void d_attractor::simulation()
{
    qint64 t;
    double h_max;
    int  k, l;
    int  color;
    qreal dx, dy;

  model->initialize();
  h.reset();
  
  //initialize output information
  int jobtag_dummy = 61;
  int resolution_x = h.get_x_res();
  int resolution_y = h.get_y_res();
  log() << "resolution: [" << resolution_x << "," << resolution_y <<"]\n";
  outFile.write((char*)&jobtag_dummy, 4);
  outFile.write((char*)&resolution_x, 4);
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
  
  for(t=0;t<length;t++) {
    model->iteration(t+1);
    if( t >= limit && inRange(*xParam,*yParam) ) {
      h.inc(*xParam, *yParam);
    }
  }

  h_max = double (h.get_max_hits());
  log() << "h_max = " << h_max << "\n";
  log() << "color step every " << h_max/94 << " hits in cell" << "\n";
  if( h_max == 0 ) {
    h_max = 1;
  }

	double dummy2=94.0;	
	double hitshilf;
	double hitpoint;

  for( dx=xmin, k=0; k<h.get_x_res(); dx+=stepX, k++) {
    for( dy=ymin, l=0; l<h.get_y_res(); dy+=stepY, l++) {
//      color = int (floor( ( double (h(k,l)) / double (h_max) ) * 32));

/*neu*/
	
	hitshilf=h(k,l);
	hitpoint = hitshilf / h_max;
	if (hitshilf==h_max)
    log() << "maximal hitcounts at: "<< dx << " , " << dy << "\n";

	if(hitpoint>0){
/*		for(int i=0;i<=dummy2;i++){
			if(hitpoint>=i*intervall){
				color=i+1;
			}
		}*/
		color=int (ceil (hitpoint*dummy2));
//	    outFile << dx << "\t" << dy << "\t" << hitpoint << "\n";
	} else color=0;
	
    if ( color>94 ) log() << "warning: color > 94\n";
     //   outFile << dx << "\t" << dy << "\t" << hitpoint << "\n";
	outFile << char( color );
/*Ende neu*/



     if( screenGraphics ) {
        screenGraphics->setPoint(dx,dy,color);
      }
    }
    //outFile << "\n";
  }
  outFile.flush();
  outFile.close();  
}

// eof
