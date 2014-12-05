///////////////////////////////////////////////////////////////////////////////
//
//
// Module name:		color_plot.C
// Contents:		Class definition of the class color_plot
//
// Author:		Marc Mueller
// Last modified:	mhoffman Mon Sep 11 13:13:08 METDST 2000
//
///////////////////////////////////////////////////////////////////////////////

#include	"color_plot.h"
#include	"../error.h"

///////////////////////////////////////////////////////////////////////////////
//
// member function:	color_plot
// Purpose:		constructor
//
// Author:		Marc Mueller
// Last modified:	Tue Jun 15 12:35:00 CEST 1999 (Marc Mueller)
//
///////////////////////////////////////////////////////////////////////////////

color_plot::color_plot(int p_size, baseModel* const cMod,const xyRange& axes,
          MacrodynGraphicsItem* const graph)
          :geometricJob(cMod,axes,graph)
{
    limit= length / 10;			// 10% are thrown away
//    limit=0;
    zLabel = axes.label[2];
    zParam=model->setLabels(zLabel);
    if( !zParam )
	fatalError("color_plot::color_plot  Can not find z label ",zLabel);

    zmax=axes.max[2];
    zmin=axes.min[2];

	psize = p_size;

}                                      


///////////////////////////////////////////////////////////////////////////////
//
// member function:	simulation
// Purpose:		plot colors where the points have variable size
//
// Author:		Marc Mueller
// Last modified:	mhoffman Mon Sep 11 13:11:43 METDST 2000
//
///////////////////////////////////////////////////////////////////////////////

void color_plot::simulation()
{
    qint64 t;
	int col,i;
	int cmax=17;
	qreal min=zmax;
	qreal max=zmin;	
    qreal intv=(zmax-zmin)/cmax;

if((zmax <= 0) && (zmin < 0)){
    model->initialize();
	for(t=0;t<limit;t++)
		model->iteration(t+1);	
	for(t=limit;t<length;t++) {
		model->iteration(t+1);
		if(*zParam<min) min=*zParam;
		if(*zParam>max) max=*zParam;
        col=cmax+1;        
if(*zParam<zmax)
	{
        for(i=1;i<cmax;i+=1)
          {
           if(*zParam < zmax-i*intv)
           {
           col=cmax+1+i;
           } 
          } 
	}
		if( screenGraphics ) 
            screenGraphics->setBigPoint(*xParam,*yParam,col,psize);
     }	
   
   log() << "min=" << min << "\t max=" << max << "\n";

 //TODO if( screenGraphics ) screenGraphics->draw_color_count();
}
if((zmax > 0) && (zmin < 0)){
    qreal smin=zmin/cmax; // 17 colors under Zero 
    qreal smax=zmax/cmax; // 17 colors under Zero 
      
	model->initialize();

	for(t=0;t<limit;t++)
		model->iteration(t+1);	
	for(t=limit;t<length;t++) {
		model->iteration(t+1);
		if(*zParam<min) min=*zParam;
		if(*zParam>max) max=*zParam;

		if(*zParam>0) {
			col=cmax;
			for(i=1;i<cmax;i+=1) {
				if(*zParam > i*smax) {
					col=cmax-i;
				}
			}
		}
		else	{
			col=cmax+1;
			for(i=1;i<cmax;i+=1) {
				if(*zParam < i*smin) {
					col=cmax+1+i;
				}
			}
		}
		if( screenGraphics ) 
            screenGraphics->setBigPoint(*xParam,*yParam,col,psize);
	}
 log() << "min=" << min << "\t max=" << max << "\n";
//TODO if( screenGraphics ) screenGraphics->draw_color_count();
}
if(zmax>0 && zmin >= 0)
{	
model->initialize();
	for(t=0;t<limit;t++)
		model->iteration(t+1);	
	for(t=limit;t<length;t++) {
		model->iteration(t+1);
		if(*zParam<min) min=*zParam;
		if(*zParam>max) max=*zParam;
        col=cmax;        
if(*zParam>zmin){
        for(i=1;i<cmax;i+=1)
          {
           if(*zParam > i*intv+zmin)
           {
           col=cmax-i;
           }
          }
}		if( screenGraphics ) 
            screenGraphics->setBigPoint(*xParam,*yParam,col,psize);
     }	
   
   log() << "min=" << min << "\t max=" << max << "\n";

//TODO if( screenGraphics ) screenGraphics->draw_color_count();
	
}
}
//eof
