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
    qint64 colChange=length/6;
    qreal hx=(xmax-xmin+0.001)/940000000.0;
    qreal hy=(ymax-ymin+0.001)/860000000.0;
    QList<qreal *> stateVars;

    stateVars << xParam;
    stateVars << yParam;

    model->initialize();
    *xParam=xmin+( ((int) ((*xParam-xmin)/hx) ) + 0.5)*hx;
    *yParam=ymin+( ((int) ((*yParam-ymin)/hy) ) + 0.5)*hy;

    for(qint64 t=0;t<length;t++) {
	model->iteration(t+1);
    *xParam=xmin+( ((int) ((*xParam-xmin)/hx) ) + 0.5)*hx;
    *yParam=ymin+( ((int) ((*yParam-ymin)/hy) ) + 0.5)*hy;
	if( t >= limit && inRange(*xParam,*yParam) ) {
        hash->storePoint(stateVars);
	    if( screenGraphics ) 
		screenGraphics->setPoint(*xParam,*yParam,
                     (int)((t/colChange)+1));
	}
    }
    log() << "Number of hits: " << hash->numberOfHits();
    log() << "Number of buckets: " << hash->numberOfBuckets();
    log() << "Number of collisions: " << hash->numberOfCollisions();
    log() << "Order of cycle: " << hash->orderOfCycle();
}

