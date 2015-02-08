////////////////////////////////////////////////////////////////////////////////
// File:	d_bifnParam.C						      //
// Contents:	member functions of class d_bifnParam			      //
// Author:	Andreas Starke						      //
// Creation:								      //
// Modified:								      //
// Changes:								      //
////////////////////////////////////////////////////////////////////////////////

#include	"d_bifnParam.h"
#include	"../logger.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      d_bifnParam                                               */
/* Member function: d_bifnParam                                               */
/* Purpose:         constructor                                               */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/

d_bifnParam::d_bifnParam(baseModel* const bMod,
				const xyRange& axes, 
			        const xyRange& xDef,  
                    MacrodynGraphicsItem* const graph)
    :bifnParam(bMod,axes,xDef,graph),
    h(axes.min[1],axes.max[1],axes.res[1])
{
    outFile.setFileName("data3D_bifnParam.dat");
    outFile.open(QFile::WriteOnly);
    stepY=(ymax-ymin) / (axes.res[1]-1);
    resolution_x = axes.res[0];
}

/******************************************************************************/
/*                                                                            */
/* Class name:      d_bifnParam                                               */
/* Member function: ~d_bifnParam                                              */
/* Purpose:         destructor                                                */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/

d_bifnParam::~d_bifnParam()
{
    if( xVars )
	delete xVars;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      d_bifnParam                                               */
/* Member function: simulation                                                */
/* Purpose:         computes a bifurcation dependent on n parameters          */
/*  									      */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/

void d_bifnParam::simulation()
{
    qint64 t;				// time
    int k;
    qreal dy;
    qreal x=0;
    double h_max;
    int color=0;
    
    //initialize output information
    int jobtag_dummy = 35;

    int resolution_y = h.get_x_res();
    log() << "resolution: [" << resolution_x << "," << resolution_y <<"]\n";
    outFile.write((char*)&jobtag_dummy, 4);
    outFile.write((char*)&resolution_x, 4);
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

    for(x=xmin;x<=xmax; x+=stepX) {
	setXParams(x);
	model->initialize();
	for(t=0;t<length;t++) {
		model->iteration(t+1);
	    	if( t >= limit && inRange(x,*yParam) ) {
                                       // only points inside the state space
				       // section under consideration are
				       // analysed
			h.inc(*yParam);
		}
	}
	
    h_max = double (h.get_max_hits());
    log() << "h_max = " << h_max;
    log() << "color step every " << h_max/94 << " hits in cell\n";
    if ( h_max == 0 ) h_max=1;
	
	double dummy2=94;
	double hitshilf;
	double hitpoint;
	
    for ( dy=ymin, k=0; k<h.get_x_res(); dy+=stepY, k++ ){
		hitshilf = h(k);
        hitpoint = hitshilf/h_max;
        if ( hitshilf==h_max )
            log() << "qMaximal hitcounts at: (" << x << " , " << dy <<")\n";
		if ( hitpoint>0 ){
			color = int(ceil(hitpoint*dummy2));
		} else color = 0;
		
	if( screenGraphics ) 
		screenGraphics->setPoint(x,dy,color);

    QTextStream(&outFile) << char( color );
	}
	h.reset();
    }
    outFile.flush();
    outFile.close();
}
//eof
