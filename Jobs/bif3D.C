// File:	bif3D.C
// Contents:	member functions of class bif3D
// Author:	Andreas Starke
// Creation:	Mon Mar 08 09:19:04 MEZ 2004
// Modified:	
// Changes:	

#include	"bif3D.h"

void bif3D_1par::progress( qint64 state, qint64 range )
{
    screenGraphics->drawLine( (xmax-xmin)*0.25+xmin, (ymax-ymin)*0.5+ymin,
                              (xmax-xmin)*0.25+xmin+double(state+1)/double(range)*(xmax-xmin)*0.5,
                              (ymax-ymin)*0.5+ymin, 9 );
}


void bif3D_2par::progress( qint64 state, qint64 range )
{
    screenGraphics->drawLine( (xmax-xmin)*0.25+xmin, (ymax-ymin)*0.5+ymin,
                              (xmax-xmin)*0.25+xmin+double(state+1)/double(range)*(xmax-xmin)*0.5,
                              (ymax-ymin)*0.5+ymin, 9 );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      bif3D                                                     */
/* Member function: bif3D                                                     */
/* Purpose:         constructor                                               */
/* Last modified:   			                                      */
/*                                                                            */
/******************************************************************************/

bif3D_2par::bif3D_2par(baseModel* const bMod, const xyRange& axes, 
         MacrodynGraphicsItem* const graph, const qint64 bif3D_dx,
         const qint64 bif3D_dy, const qint64 bif3D_dz)
          :geometry3D(bMod,axes,graph),h(axes.min[1],axes.max[1],
	  axes.res[1])
{
    length=model->getLength();
    limit=(qint64)(0.2*length);			// 20% are thrown away
    resolution_x = axes.res[0];
    resolution_y = axes.res[1];
    resolution_z = axes.res[2];
//    resolution_x = resolution_y = resolution_z =axes.res[2];
    log() << "resx: " << resolution_x << " resy: " << resolution_y
    << " resz: " << resolution_z;
    dx = bif3D_dx;
    dy = bif3D_dy;
    dz = bif3D_dz;
    stepX=(xmax-xmin) / (resolution_x-1);
    stepY=(ymax-ymin) / (resolution_y-1);
    stepZ=(zmax-zmin) / (resolution_z-1);
    if ( screenGraphics ){
    screenGraphics->clear_window();
    screenGraphics->drawString( (xmax-xmin)*0.3+xmin, (ymax-ymin)*0.25+ymin,
    "progress", 9 );
    screenGraphics->drawLine( (xmax-xmin)*0.25+xmin, (ymax-ymin)*0.45+ymin,
    (xmax-xmin)*0.25+xmin, (ymax-ymin)*0.55+ymin, 9);
    screenGraphics->drawLine( (xmax-xmin)*0.75+xmin, (ymax-ymin)*0.45+ymin,
    (xmax-xmin)*0.75+xmin, (ymax-ymin)*0.55+ymin, 9);
    }
}

bif3D_1par::bif3D_1par(baseModel* const bMod, const xyRange& axes, 
         MacrodynGraphicsItem* const graph, const qint64 bif3D_dx,
         const qint64 bif3D_dy, const qint64 bif3D_dz)
          :geometry3D(bMod,axes,graph),h(axes.min[1],axes.max[1],
          axes.res[1],axes.min[2],axes.max[2],axes.res[2])
{
    length=model->getLength();
    limit=(qint64)(0.2*length);			// 20% are thrown away
    resolution_x = axes.res[0];
    resolution_y = axes.res[1];
    resolution_z = axes.res[2];
//    resolution_x = resolution_y = resolution_z =axes.res[2];
    log() << "resx: " << resolution_x << " resy: " << resolution_y
    << " resz: " << resolution_z;
    dx = bif3D_dx;
    dy = bif3D_dy;
    dz = bif3D_dz;
    stepX=(xmax-xmin) / (resolution_x-1);
    stepY=(ymax-ymin) / (resolution_y-1);
    stepZ=(zmax-zmin) / (resolution_z-1);
    if ( screenGraphics )
    {
        screenGraphics->clear_window();
        screenGraphics->drawString( (xmax-xmin)*0.3+xmin, (ymax-ymin)*0.25+ymin,
        "progress", 9 );
        screenGraphics->drawLine( (xmax-xmin)*0.25+xmin, (ymax-ymin)*0.45+ymin,
        (xmax-xmin)*0.25+xmin, (ymax-ymin)*0.55+ymin, 9);
        screenGraphics->drawLine( (xmax-xmin)*0.75+xmin, (ymax-ymin)*0.45+ymin,
        (xmax-xmin)*0.75+xmin, (ymax-ymin)*0.55+ymin, 9);
    }
}
/******************************************************************************/
/*                                                                            */
/* Class name:      bif3D                                                     */
/* Member function: setStepX                                                  */
/* Purpose:         set a new step size for the x axis                        */
/* Last modified:   							      */
/*                                                                            */
/******************************************************************************/

void bif3D_2par::setStepX(const qreal& toSet)
{
    stepX=toSet;
}
void bif3D_1par::setStepX(const qreal& toSet)
{
    stepX=toSet;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      bif3D                                                     */
/* Member function: setStepY                                                  */
/* Purpose:         set a new step size for the y axis                        */
/* Last modified:   							      */
/*                                                                            */
/******************************************************************************/

void bif3D_2par::setStepZ(const qreal& toSet)
{
    stepZ=toSet;
}
void bif3D_1par::setStepZ(const qreal& toSet)
{
    stepZ=toSet;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      bif3D                                                     */
/* Member function: simulation                                                */
/* Purpose:         computes a 3D bifurcation diagram                         */
/* Last modified:   			                                      */
/*                                                                            */
/******************************************************************************/

void bif3D_2par::simulation()
{
    qint64 t;
    qreal dummy_x;
    qreal dummy_y;
    qreal dummy_z;
    // for the histogram
    double h_max;
    qint64 color=0;
    double hitshilf;
    double hitpoint;
    qint64 dummy_a;
    qint64 count_x, count_z;
    char data[resolution_x][resolution_y][resolution_z];
    
    //information
    //log() << "h.x_res: " << h.get_x_res() 

    // initialize the output file writing header for vrend data
    QFile outFile("pseudo_data_2par.dat");
    if (!outFile.open(QFile::WriteOnly)) exit(-1);
    int jobtag_dummy = 25;
    outFile.write((char*)&jobtag_dummy, 4);
    
    outFile.write((char*)&resolution_x, 4);
    outFile.write((char*)&resolution_y, 4);
    outFile.write((char*)&resolution_z, 4);
    outFile.write((char*)&dx, 4);
    outFile.write((char*)&dy, 4);
    outFile.write((char*)&dz, 4);

    outFile.write((char*)&xmin, 8);
    outFile.write((char*)&xmax, 8);
    outFile.write((char*)&ymin, 8);
    outFile.write((char*)&ymax, 8);
    outFile.write((char*)&zmin, 8);
    outFile.write((char*)&zmax, 8);
    
    QTextStream stream(&outFile);

    stream << xLabel;
    stream << yLabel;
    stream << zLabel;
//    log() << zLabel << " for z with " << length_of_label << " characters\n";
    
    h.reset();
    for (dummy_z=zmin, count_z=0; count_z<resolution_z; dummy_z+=stepZ, count_z++) {
    for (dummy_x=xmin, count_x=0; count_x<resolution_x; dummy_x+=stepX, count_x++) {
//    	log() << "x: " << dummy_x << "\tz: " << dummy_z 
    	*xParam=dummy_x;
	*zParam=dummy_z;
	model->initialize();
	for(t=0;t<limit;t++) 
	    model->iteration(t+1);
	for(t=limit;t<length;t++){
	    model->iteration(t+1);
	    if( inRange(dummy_x,*yParam,dummy_z) ) {
                       	// only points inside the state space
			// section under consideration are
			// analysed
			h.inc(*yParam);
	    }
	}
    
    h_max = double (h.get_max_hits());
//    log() << "x: " << dummy_x << "\th_max: " << h_max 
    if ( h.get_max_hits()==0 ) h_max=1;
    for( dummy_y=ymin, dummy_a = 0; dummy_a<h.get_x_res(); dummy_y+=stepY,
    dummy_a++){
    	hitshilf=double(h(dummy_a));
    hitpoint=hitshilf/h_max;
	if (hitpoint>0){
		color=int(ceil(hitpoint*94.0));
	}
	else color=0;
	data[count_x][dummy_a][count_z] = char(color);
    }
    h.reset();
    }
    if ( screenGraphics ) progress( count_z, resolution_z );
    }
    for ( count_z=0;count_z<resolution_z;count_z++)    
    	for ( dummy_a=0; dummy_a<resolution_y; dummy_a++)
		for ( count_x=0;count_x<resolution_x;count_x++){
            QTextStream(&outFile) << data[count_x][dummy_a][count_z];
//			log() << int ( data[count_x][dummy_a][count_z] ) << " ";
		}
    outFile.flush();
    outFile.close();
}

void bif3D_1par::simulation()
{
    qint64 t;
    qreal dummy_x;
    qreal dummy_y;
    qreal dummy_z;
    // for the histogram
    double h_max;
    qint64 color;
    char data[resolution_x][resolution_y][resolution_z];
    double hitshilf;
    double hitpoint;
    qint64 dummy_a;
    qint64 dummy_b;
    qint64 count_x;
    
    // initialize the output file writing header for vrend data
    QFile outFile("pseudo_data_1par.dat");
    if (!outFile.open(QFile::WriteOnly)) exit(-1);
    
    int jobtag_dummy = 24;
    outFile.write((char*)&jobtag_dummy, 4);    
    
    outFile.write((char*)&resolution_x, 4);
    outFile.write((char*)&resolution_y, 4);
    outFile.write((char*)&resolution_z, 4);
    outFile.write((char*)&dx, 4);
    outFile.write((char*)&dy, 4);
    outFile.write((char*)&dz, 4);

    outFile.write((char*)&xmin, 8);
    outFile.write((char*)&xmax, 8);
    outFile.write((char*)&ymin, 8);
    outFile.write((char*)&ymax, 8);
    outFile.write((char*)&zmin, 8);
    outFile.write((char*)&zmax, 8);
    
    QTextStream stream(&outFile);

    stream << xLabel;
    stream << yLabel;
    stream << zLabel;

    h.reset();
    for (dummy_x=xmin, count_x=0; count_x<resolution_x; dummy_x+=stepX, count_x++) {
//    	log() << "x: " << dummy_x << "\tz: " << dummy_z 
    	*xParam=dummy_x;
	model->initialize();
	for(t=0;t<limit;t++) 
	    model->iteration(t+1);
	for(t=limit;t<length;t++){
	    model->iteration(t+1);
	    if( inRange(dummy_x,*yParam,*zParam) ) {
                       	// only points inside the state space
			// section under consideration are
			// analysed
			h.inc(*yParam,*zParam);
	    }
	}
    
    h_max = double (h.get_max_hits());
//    log() << "x: " << dummy_x << "\th_max: " << h_max 
    if ( h_max==0 ) h_max=1;
    for( dummy_z=zmin, dummy_b = 0; dummy_b<h.get_y_res(); dummy_z+=stepZ,
    dummy_b++){
    for( dummy_y=ymin, dummy_a = 0; dummy_a<h.get_x_res(); dummy_y+=stepY,
    dummy_a++){
    	hitshilf=h(dummy_a,dummy_b);
    hitpoint=hitshilf/h_max;
	// progress report to be included...
	if (hitpoint>0){
		color=int(ceil(hitpoint*94));
	}
	else color=0;

	data[count_x][dummy_a][dummy_b] = char(color);
    }
    }
    if ( screenGraphics ) progress( count_x, resolution_x );
    h.reset();
    }
    for ( dummy_b = 0; dummy_b<resolution_z; dummy_b++)
    	for ( dummy_a = 0; dummy_a<resolution_y; dummy_a++)
	    for ( count_x = 0; count_x<resolution_x; count_x++)
            QTextStream(&outFile) << data[count_x][dummy_a][dummy_b];
//			outFile.write((char*)&data[count_x][dummy_a][dummy_b],1);
    outFile.flush();
    outFile.close();
}
