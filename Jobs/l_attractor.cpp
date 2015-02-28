///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/l_attractor.C,v 1.1 2000/08/31 16:23:52 mmeyer Exp $
//
// Module name:		l_attractor.C
// Contents:		Class definition of the class l_attractor
//			display the orbit with connected lines
//
// Author:		Uli Middelberg
// Last modified:	Thu Oct 30 16:44:49 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#include	"l_attractor.h"

///////////////////////////////////////////////////////////////////////////////
//
// member function:	l_attractor
// Purpose:		constructor
//
// Author:		Uli Middelberg
// Last modified:	Fri Nov  7 14:53:01 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

l_attractor::l_attractor(baseModel* const bMod,const xyRange& axes,
                  MacrodynGraphicsItem* const graph, int psize)
                        : attractor(bMod,axes,graph)
{
	pointsize = psize;
}                                      

///////////////////////////////////////////////////////////////////////////////
//
// member function:	simulation
// Purpose:		display the orbit with connected lines
//
// Author:		Uli Middelberg
// Last modified:	Thu Oct 30 16:45:21 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void l_attractor::simulation()
{
    qint64 colChange=length/6;           // for better viewing there are 6 colors
				       // used to draw the attractor
    qreal old_x = 0;
    qreal old_y = 0;

    model->initialize();

    for(qint64 t=0;t<length;t++) {
        old_x = *xParam;
	old_y = *yParam;
	model->iteration(t+1);
	if( t >= limit /*&& inRange(*xParam,*yParam)*/ ) {
	    if( screenGraphics ){
		screenGraphics->setLine(old_x,old_y,*xParam,*yParam,
                    (int)((t/colChange)));
		if ( pointsize > 1 )
		screenGraphics->setBigPoint(old_x,old_y,
        (int)((t/colChange)+1),pointsize);
	    }
	                                                 // and draw them
	}
    }
}

// eof
