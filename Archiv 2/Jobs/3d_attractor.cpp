///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/attractor_3d.C,v 1.1 2000/08/31 16:21:25 mmeyer Exp $
//
// Module name:		attractor_3d.C
// Contents:		Class definition of the class attractor_3d
//			compute the approximation of the limit distribution
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

#include	"3d_attractor.h"

///////////////////////////////////////////////////////////////////////////////
//
// member function:	attractor_3d
// Purpose:		constructor
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

attractor_3d::attractor_3d(baseModel* const bMod,const xyRange& axes,
                 MacrodynGraphicsItem* const graph)
            :geometry3D(bMod,axes,graph),
             h(axes.min[0],axes.max[0],axes.res[0],
               axes.min[1],axes.max[1],axes.res[1],
               axes.min[2],axes.max[2],axes.res[2])
{
//    if( fileName )
//	outFile.open(fileName,ios::out);
 //   else
    outFile.setFileName("data3D_3d_attraktor.dat");
    outFile.open(QFile::WriteOnly);

    limit= length / 10;			// 10% are thrown away
    stepX=(xmax-xmin) / (axes.res[0]-1);
    stepY=(ymax-ymin) / (axes.res[1]-1);
    stepZ=(zmax-zmin) / (axes.res[2]-1);
}                                      

///////////////////////////////////////////////////////////////////////////////
//
// member function:	simulation
// Purpose:		compute the approximation of the limit distribution
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void attractor_3d::simulation()
{
    qint64 t;
    double h_max;
    int  k, l, m;
    int  color;
    qreal dx, dy, dz;
    qint64 dummy=1;
    
    model->initialize();
    h.reset();
  
  //initialize output information
  int jobtag_dummy = 63;
  int resolution_x = h.get_x_res();
  int resolution_y = h.get_y_res();
  int resolution_z = h.get_z_res();
  log() << "resolution: [" << resolution_x << "," << resolution_y << "," << resolution_z <<"]\n";
  outFile.write((char*)&jobtag_dummy, 4);
  outFile.write((char*)&resolution_x, 4);
  outFile.write((char*)&resolution_y, 4);
  outFile.write((char*)&resolution_z, 4);
  
  outFile.write((char*)&dummy, 4);		// dummyx
  outFile.write((char*)&dummy, 4);		// dummyy
  outFile.write((char*)&dummy, 4);		// dummyz
  
  outFile.write((char*)&xmin, 8);
  outFile.write((char*)&xmax, 8);
  outFile.write((char*)&ymin, 8);
  outFile.write((char*)&ymax, 8);
  outFile.write((char*)&zmin, 8);
  outFile.write((char*)&zmax, 8);
  
  log() << "xmin: " << xmin << "\txmax: " << xmax;
  log() << "ymin: " << ymin << "\tymax: " << ymax;
  log() << "zmin: " << zmin << "\tzmax: " << zmax;
  
  QTextStream stream(&outFile);

  stream << xLabel;
  stream << yLabel;
  stream << zLabel;


  
  for(t=0;t<length;t++) {
    model->iteration(t+1);
    if( t >= limit && inRange(*xParam,*yParam,*zParam) ) {
      h.inc(*xParam, *yParam, *zParam);
    }
  }

  h_max = double (h.get_max_hits());
  log() << "h_max = " << h_max;
  log() << "color step every " << h_max/94 << " hits in cell";
  if( h_max == 0 ) {
    h_max = 1;
  }

	double dummy2=94.0;	
	double hitshilf;
	double hitpoint;

  for( dz=zmin, m=0; m<h.get_z_res(); dz+=stepZ, m++) {
    for( dy=ymin, l=0; l<h.get_y_res(); dy+=stepY, l++) {
      for( dx=xmin, k=0; k<h.get_x_res(); dx+=stepX, k++) {
//      color = int (floor( ( double (h(k,l)) / double (h_max) ) * 32));

/*neu*/
	
	hitshilf=h(k,l,m);
    hitpoint = hitshilf / h_max;
    if (hitshilf==h_max)
    log() << "qMaximal hitcounts at: "<< dx << " , " << dy << " , " << dz;

	if(hitpoint>0){
/*		for(int i=0;i<=dummy2;i++){
			if(hitpoint>=i*intervall){
				color=i+1;
			}
		}*/
		color=int (ceil (hitpoint*dummy2));
//	    outFile << dx << "\t" << dy << "\t" << hitpoint 
	} else color=0;
	
    if ( color>94 ) log() << "warning: color > 94\n";
     //   outFile << dx << "\t" << dy << "\t" << hitpoint 
    QTextStream(&outFile) << color;
/*Ende neu*/



//     if( screenGraphics ) {
//        screenGraphics->setPoint(dx,dy,color);
//      }

    }
    //outFile 
  }
  }
  outFile.flush();
  outFile.close();  
}

// eof
