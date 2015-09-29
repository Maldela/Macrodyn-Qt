///////////////////////////////////////////////////////////////////////////////
//
//
// Module name:		distributionJob.C
// Contents:		Class definition of the class distribution
//
// Author:		    Michael Meyer
// Last modified:	Mon Sep 18 14:28:05 METDST 2000(Michael Meyer)
//
///////////////////////////////////////////////////////////////////////////////

#include	"distributionJob.h"
#include	"../logger.h"

///////////////////////////////////////////////////////////////////////////////
//
// member function:	distribution
// Purpose:		    constructor
//
// Author:		    Michael Meyer
// Last modified:	Mon Sep 18 14:27:52 METDST 2000 (Michael Meyer)
//
///////////////////////////////////////////////////////////////////////////////

distributionJob::distributionJob(int p_size, baseModel* const cMod, xyRange& axes,
                 MacrodynGraphicsItem* const graph) : geometricJob(cMod,axes,graph)
{
    xLabel = axes.label[0];
    xParam=model->setLabels(xLabel);
    if( !xParam )
	fatalError("distribution::distribution  Can not find z label ",xLabel);
    xmax=axes.max[0];
    xmin=axes.min[0];
    relativeCount=0;
	relativeSum=0;
    n_axes = & axes;
	psize = p_size;

    for(i=0;i<100001;i++)
    	Count[i]=0;
}                            
///////////////////////////////////////////////////////////////////////////////
//
// member function:	simulation
// Purpose:		    plot distribution   /******P(x>=y)=1-P(x<y)*****/
//
// Author:		    Michael Meyer
// Last modified:	Mon Sep 18 14:28:15 METDST 2000 (Michael Meyer)
//
///////////////////////////////////////////////////////////////////////////////

void distributionJob::simulation()
{
    qint64 t;
    int cqMax=100000;

    qreal old_x = 0;
    qreal old_y = 0;

    qreal intervall=(xmax-xmin)/cqMax;
//    double intervall=pow(cqMax,-1);

    log() << "int=" << intervall;

	model->initialize();

	for(t=0;t<length;t++) 
    {
        model->iteration(t+1);
        col=9;
        for(i=1;i<cqMax;i+=1)
        {
            if(*xParam <= ((i*intervall)+xmin))
            {
                Count[i]+=1;
                break;
            }
        }
    }
    n_axes->label[1] = "F()";	// relabel the y-axis
    if( screenGraphics )
        screenGraphics->setXYRange(*n_axes);

    for(i=0;i<=100000;i++)
    {
        old_x = relativeSum;
        relativeCount=Count[i] / double (length);
        relativeSum+=relativeCount;
        old_y = relativeSum;
        old_xStep=xStep;
        xStep=(i*intervall)+xmin;
        old_prob=prob;
        prob=relativeSum;

//	    if( screenGraphics )
//			screenGraphics->setPoint(xStep,relativeSum,col);
        if( screenGraphics && i>0)
            screenGraphics->setLine(old_xStep,old_prob,xStep,prob,col);


/*	    if( screenGraphics )
        screenGraphics->setLine(old_x,old_y,xstep,relativeSum,col);

                                                     // and draw them
        if( printDev )
        printDev->setLine(old_x,old_y,xStep,relativeSum,col);
*/
    }
}



