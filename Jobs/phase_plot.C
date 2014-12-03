///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/phase_plot.C,v 1.1 2000/08/31 16:24:50 mmeyer Exp $
//
// Module name:		phase_plot.C
// Contents:		Class definition of the class phase_plot
//			plot x_t vs x_t+k
//
// Author:		Uli Middelberg
// Last modified:	Mon May 11 13:57:37 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////


#include	"phase_plot.h"

///////////////////////////////////////////////////////////////////////////////
//
// member function:	phase_plot
// Purpose:		constructor
//
// Author:		Uli Middelberg
// Last modified:	Mon Nov  3 16:59:16 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

phase_plot::phase_plot(long lag, baseModel* const bMod,const xyRange& axes,
              MacrodynGraphicsItem* const graph, int psize)
          :geometricJob(bMod,axes,graph)
{
    limit= length / 10;			// 10% are thrown away
//	limit = 0;
    lag_size = lag;
    pointsize = psize;
}                                      


///////////////////////////////////////////////////////////////////////////////
//
// member function:	simulation
// Purpose:		plot x_t vs x_t+k
//
// Author:		Uli Middelberg
// Last modified:	Mon Nov  3 17:00:26 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void phase_plot::simulation()
{
    long colChange=length/6;           // for better viewing there are 6 colors
				       // used to draw the attractor

    qreal * fifo;		// queue to store last lag_size values of *xParam
    long   fpos;		// the current position in the queue
        
    model->initialize();

    fifo = new qreal [lag_size] ;
    for(fpos=0; fpos < lag_size; fpos++) {
       model->iteration(fpos+1);
       fifo[fpos]=*xParam;
    }

    fpos=0;
    if ( pointsize>1 ){
    for(long t=lag_size;t<length;t++) {
	model->iteration(t+1);
	
	if( t >= limit && inRange(fifo[fpos],*xParam) ) {
//	if(inRange(fifo[fpos],*xParam) ) {
	    if( screenGraphics ) 
		screenGraphics->setBigPoint(fifo[fpos],*xParam,
					 (short)((t/colChange)+1), pointsize);   
                                                     // and draw them
	}
	fifo[fpos] = *xParam;	// update the value in the queue

	fpos++;			// update the the queue
	if( fpos>=lag_size ) {	// update the position counter
	  fpos=0;
	}
    }
    } else {
    for(long t=lag_size;t<length;t++) {
	model->iteration(t+1);
	
	if( t >= limit && inRange(fifo[fpos],*xParam) ) {
//	if(inRange(fifo[fpos],*xParam) ) {
	    if( screenGraphics ) 
		screenGraphics->setPoint(fifo[fpos],*xParam,
					 (short)((t/colChange)+1));   
                                                     // and draw them
	}
	fifo[fpos] = *xParam;	// update the value in the queue

	fpos++;			// update the the queue
	if( fpos>=lag_size ) {	// update the position counter
	  fpos=0;
	}
    }
    }
    delete [] fifo;
}

//eof
