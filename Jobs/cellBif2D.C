// File:	cellBif2D.C
// Contents:	member functions of class cellBif2D
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:25:09 MEZ 1996
// Changes:	

#include	"cellBif2D.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      cellBif2D                                                 */
/* Member function: cellBif2D                                                 */
/* Purpose:         constructor                                               */
/* Last modified:   24.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

cellBif2D::cellBif2D(baseModel* const bMod, const xyRange& axes, 
          MacrodynGraphicsItem* const graph)
          :bif2D(bMod,axes,graph)
{
}

/******************************************************************************/
/*                                                                            */
/* Class name:      cellBif2D                                                 */
/* Member function: simulation                                                */
/* Purpose:         computes a 2D bifurcation diagram using a simple cell     */
/*                  mapping                                                   */
/* Last modified:   24.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void cellBif2D::simulation()
{
    long t;
    qreal hx=(xmax-xmin+0.0001)/10000.0;
    qreal hy=(ymax-ymin+0.0001)/10000.0;

    for(*xParam=xmin;*xParam<=xmax; *xParam+=stepX) {
	model->initialize();
	*xParam=xmin+( ((int) ((*xParam-xmin)/hx) ) + 0.5)*hx;
	*yParam=ymin+( ((int) ((*yParam-ymin)/hy) ) + 0.5)*hy;

	for(t=0;t<length;t++) {
	    model->iteration(t+1);
	    *xParam=xmin+( ((int) ((*xParam-xmin)/hx) ) + 0.5)*hx;
	    *yParam=ymin+( ((int) ((*yParam-ymin)/hy) ) + 0.5)*hy;
	    if( t >= limit && inRange(*xParam,*yParam) ) {
                                       // only points inside the state space
				       // section under consideration are
				       // analysed
		if( screenGraphics )
            screenGraphics->setPoint(*xParam,*yParam,9);
	    }
	}
    }
}

//eof
