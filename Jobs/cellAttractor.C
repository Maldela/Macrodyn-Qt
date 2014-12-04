// File:	cellAttractor.C
// Contents:	member functions of class cellattractor
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:25:09 MEZ 1996
// Changes:	

#include	"cellAttractor.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      cellAttractor                                             */
/* Member function: cellAttractor                                             */
/* Purpose:         constructor                                               */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

cellAttractor::cellAttractor(baseModel* const bMod,const xyRange& axes,
             MacrodynGraphicsItem* const graph)
             :geometricJob(bMod,axes,graph)
{
    limit= length / 10;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      cellAttractor                                             */
/* Member function: simulation                                                */
/* Purpose:         compute the orbit of a discrete system and a discrete     */
/*                  state space                                               */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void cellAttractor::simulation()
{
    long colChange=length/6;
    qreal hx=(xmax-xmin+0.001)/940000000.0;
    qreal hy=(ymax-ymin+0.001)/860000000.0;
    const qreal *state[2];

    state[0]=xParam;
    state[1]=yParam;

    model->initialize();
    *xParam=xmin+( ((int) ((*xParam-xmin)/hx) ) + 0.5)*hx;
    *yParam=ymin+( ((int) ((*yParam-ymin)/hy) ) + 0.5)*hy;

    for(long t=0;t<length;t++) {
	model->iteration(t+1);
	*xParam=xmin+( ((int) ((*xParam-xmin)/hx) ) + 0.5)*hx;
	*yParam=ymin+( ((int) ((*yParam-ymin)/hy) ) + 0.5)*hy;
	if( t >= limit && inRange(*xParam,*yParam) ) {
	    hash->storePoint(state);
	    if( screenGraphics ) 
		screenGraphics->setPoint(*xParam,*yParam,
                     (short)((t/colChange)+1));
	}
    }
    cout << "Number of hits: " << hash->numberOfHits() << "\n";
    cout << "Number of buckets: " << hash->numberOfBuckets() << "\n";
    cout << "Number of collisions: " << hash->numberOfCollisions() << "\n";
    cout << "Order of cycle: " << hash->orderOfCycle() << "\n";
}

