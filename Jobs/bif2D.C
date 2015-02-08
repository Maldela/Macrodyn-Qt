// File:	bif2D.C
// Contents:	member functions of class bif2D
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:16:47 MEZ 1996
// Changes:	

#include	"bif2D.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      bif2D                                                     */
/* Member function: bif2D                                                     */
/* Purpose:         constructor                                               */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

bif2D::bif2D(baseModel* const bMod, const xyRange& axes, 
         MacrodynGraphicsItem* const graph)
          :geometricJob(bMod,axes,graph)
{
    length=model->getLength();
    limit=(qint64)(0.2*length);			// 20% are thrown away
    stepX=(xmax-xmin) / (axes.res[0]-1);
}
/******************************************************************************/
/*                                                                            */
/* Class name:      bif2D                                                     */
/* Member function: setStepX                                                  */
/* Purpose:         set a new step size for the x axis                        */
/* Last modified:   16.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void bif2D::setStepX(const qreal& toSet)
{
    stepX=toSet;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      bif2D                                                     */
/* Member function: simulation                                                */
/* Purpose:         computes a 2D bifurcation diagram                         */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void bif2D::simulation()
{
    qint64 t;
    qreal dummy_x;

//    for(*xParam=xmin;*xParam<=xmax; *xParam+=stepX) {
    log()<<"stepX = "<<stepX;
    for (dummy_x=xmin; dummy_x<=xmax; dummy_x+=stepX)
    {
    	*xParam=dummy_x;
        model->initialize();
        for(t=0;t<length;t++)
        {
            model->iteration(t+1);
            if( t >= limit )
            {
                // only points inside the state space
                // section under consideration are
                // analysed
                if( screenGraphics )
                    screenGraphics->setPoint(dummy_x,*yParam,9);

            }
        }
    }
}
