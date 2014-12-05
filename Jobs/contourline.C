///////////////////////////////////////////////////////////////////////////////
//
//
// Module name:		contourline.C
// Contents:		Class definition of the class contourline
//
// Author:		Michael Meyer
// Last modified:	Tue July 07 12:35:00 CEST 1999 (Michael Meyer)
//
///////////////////////////////////////////////////////////////////////////////

#include	"contourline.h"
#include	"../error.h"

///////////////////////////////////////////////////////////////////////////////
//
// member function:	contourline
// Purpose:		constructor
//
// Author:		Michael Meyer
// Last modified:	06.08.2003 astarke
//
///////////////////////////////////////////////////////////////////////////////

contourline::contourline(baseModel* const cMod,const xyRange& axes,
		         const xyRange& xDef, const xyRange& yDef,
             MacrodynGraphicsItem* const graph)
            :geometricJob(cMod,axes,graph),effectiveX(xDef),effectiveY(yDef),
	               h(axes.min[0],axes.max[0],axes.res[0],
	               axes.min[1],axes.max[1],axes.res[1])
{
    int i;
//    limit= length / 10;			// 10% are thrown away
    limit=0;
    zLabel = axes.label[2];
    zParam=model->setLabels(zLabel);
    if( !zParam )
	fatalError("contourline::contourline  Can not find z label ",zLabel);
    stepX=(xmax-xmin) / (axes.res[0]-1);
    stepY=(ymax-ymin) / (axes.res[1]-1);
    
// Bundle-Support
	xVars=new qreal* [effectiveX.dimension];
	if ( !xVars )
		fatalError("contourline::contourline","can't create vector of x-variables");
	for (i=0;i<effectiveX.dimension;i++) {
        xVars[i]=model->setLabels(effectiveX.label[i]); // get bundle vars
		if ( !xVars[i] )
			fatalError("contourline::contourline","unknown x parameter specified");
	}
	
	yVars=new qreal* [effectiveY.dimension];
	if ( !yVars)
		fatalError("contourline::contourline","can't create vector of y-variables");
	for (i=0;i<effectiveY.dimension;i++) {
        yVars[i]=model->setLabels(effectiveY.label[i]); // get bundle vars
		if ( !yVars[i] )
			fatalError("contourline::contourline","unknown y parameter specified");
	}
	
	if ( axes.min[2]==axes.max[2] ) {
		min=0;
		max=0;
	} else {
		min=axes.min[2];
		max=axes.max[2];
	}

    outFile.setFileName("data3D_contour.dat");
    outFile.open(QFile::WriteOnly);
}

///////////////////////////////////////////////////////////////////////////////
//
// member function:	~contourline
// purpose:		destructor
// 
// author:		Andreas Starke
// last modified:	
//
///////////////////////////////////////////////////////////////////////////////

contourline::~contourline()
{
	if ( xVars )
		delete xVars;
	if ( yVars )
		delete yVars;
}

///////////////////////////////////////////////////////////////////////////////
//
// member function:	setXParams
// purpose:		linear update of x-parameters
// 
// author:		Andreas Starke
// last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void contourline::setXParams(const qreal& newX)
{
	static qreal divisor=xmax-xmin;
    for(int i=0;i<effectiveX.dimension;i++)
		*xVars[i]=effectiveX.min[i]+(newX-xmin)/divisor*
			(effectiveX.max[i]-effectiveX.min[i]);
}

///////////////////////////////////////////////////////////////////////////////
//
// member function:	setYParams
// purpose:		linear update of y-parameters
// 
// author:		Andreas Starke
// last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void contourline::setYParams(const qreal& newY)
{
	static qreal divisor=ymax-ymin;
    for(int i=0;i<effectiveY.dimension;i++)
		*yVars[i]=effectiveY.min[i]+(newY-ymin)/divisor*
			(effectiveY.max[i]-effectiveY.min[i]);
}

                         
///////////////////////////////////////////////////////////////////////////////
//
// member function:	simulation
// Purpose:		plot contourline
//
// Author:		Michael Meyer
// Last modified:	06.08.2003 astarke
//
///////////////////////////////////////////////////////////////////////////////

void contourline::simulation()
{
  qreal dummy_x;
  qreal dummy_y;
  qint64 t;
  int col;
  int cmax=34;

  qreal fortschritt=0;
  qreal schritt;
  
  schritt=100.0/((xmax-xmin)/stepX);
  log() << "using value after		: "<< length<<" iterations\n";
  log() << "for variable			: "<< zLabel<<"\n";

	if (min!=max) {
	for(dummy_x=xmin;dummy_x<=xmax; dummy_x+=stepX) {       
		setXParams(dummy_x);
		fortschritt+=schritt;
        log() << "\r" << int(fortschritt) << " % " << "\n";
	     for(dummy_y=ymin;dummy_y<=ymax;dummy_y+=stepY) {
		setYParams(dummy_y);

		model->initialize();

		for (t=0; t<length ; t++)
		model->iteration(t+1);

		h.set_to(dummy_x,dummy_y,*zParam);
		}
	}
	} else {
	setXParams(xmin);
	setYParams(ymin);
	model->initialize();
	for (t=0; t<length; t++)
		model->iteration(t+1);
	min=max=*zParam;
	
	for(dummy_x=xmin;dummy_x<=xmax; dummy_x+=stepX) {       
		setXParams(dummy_x);
		fortschritt+=schritt;
        log() << "\r" << int(fortschritt) << " % " << "\n";
	     for(dummy_y=ymin;dummy_y<=ymax;dummy_y+=stepY) {
		setYParams(dummy_y);

		model->initialize();

		for (t=0; t<length ; t++)
		model->iteration(t+1);

		if(*zParam<min) min=*zParam;
		if(*zParam>max) max=*zParam;
		h.set_to(dummy_x,dummy_y,*zParam);
		}
	}	
	}
	
	screenGraphics->clear_window();
	screenGraphics->set_axis(2,min,max);
	screenGraphics->drawAxis();

    qreal scala=(max-min)/double(cmax); // 34 colors for complete range of values
    qreal dummy_value;
    QDataStream stream(&outFile);

    for(dummy_x=xmin;dummy_x<=xmax; dummy_x+=stepX) {
		for(dummy_y=ymin;dummy_y<=ymax;dummy_y+=stepY) {
		dummy_value=h(dummy_x,dummy_y);
/*		if ((min<0)&&(max>0))
			col=17-int((dummy_value/scala));
		if ((min>=0)&&(max>0))*/
			col=34-int(floor((dummy_value-min)/scala));
/*		if ((min<0)&&(max<=0))
			col=1-int((dummy_value/scala));
		if ((min==max)&&(max==0))
			col=18;*/
		if (col>34) col=34;
		if (col<1) col=1;
		if( screenGraphics ) 
            screenGraphics->setPoint(dummy_x,dummy_y,col);
        stream << dummy_x << "\t" << dummy_y << "\t" << dummy_value << "\n";
       }
       stream << "\n";
    }
    log() << "\nmin=" << min << "\t max=" << max << "\n";
	outFile.close();
}

//eof

		
		
		
		
		
		
