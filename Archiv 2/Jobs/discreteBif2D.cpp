// File:	discreteBif2D.C
// Contents:	member functions of class discreteBif2D
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:25:09 MEZ 1996
// Changes:	

#include	"discreteBif2D.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      discreteBif2D                                             */
/* Member function: discreteBif2D                                             */
/* Purpose:         constructor                                               */
/* Last modified:   27.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

discreteBif2D::discreteBif2D(baseModel* const bMod, const xyRange& axes, 
                 MacrodynGraphicsItem* const graph)
    :geometricJob(bMod,axes,graph)
{
    length=model->getLength();
    limit=(qint64)(0.66*length);			// 66% are thrown away
    stepX=(int)((xmax-xmin) / (axes.res[0]-1));
}

/******************************************************************************/
/*                                                                            */
/* Class name:      discreteBif2D                                             */
/* Member function: simulation                                                */
/* Purpose:         computes a 2D bifurcation diagram useing a discrete       */
/*                  parameter                                                 */
/* Last modified:   27.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void discreteBif2D::simulation()
{
    qint64 t;

    int *xInt=(int*)xParam;
    qreal nextX;
    
    for(*xInt=(int)xmin;*xInt<=(int)xmax; *xInt+=stepX) {
    model->initialize();
//	model->printParamset();
    for(t=0;t<length;t++) {
        model->iteration(t+1);
        if( t >= limit && inRange((qreal)*xInt,*yParam) ) {
                                       // only points inside the state space
                       // section under consideration are
                       // analysed
            //log()<<"außen *xInt = "<<*xInt;
        if( screenGraphics ) {
            //log()<<"innen *xInt = "<<*xInt;
            nextX=*xInt+stepX;
            if( nextX > xmax )
            nextX=xmax;
            screenGraphics->setLine((qreal)*xInt,*yParam,
                                     nextX,*yParam,6);
	    
	
        }
        }
    }
    }
}
    
//eof
