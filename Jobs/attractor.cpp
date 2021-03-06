// File:	attractor.C
// Contents:	member functions of class attractor
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:25:09 MEZ 1996
// Changes:	27.02.1997 (Marc Mueller)

#include	"attractor.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      attractor                                                 */
/* Member function: attractor                                                 */
/* Purpose:         constructor                                               */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

attractor::attractor(baseModel* const bMod,const xyRange& axes,
             MacrodynGraphicsItem* const graph)
          :geometricJob(bMod,axes,graph)
{
    limit= length / 10;			// 10% are thrown away
}                                      


/******************************************************************************/
/*                                                                            */
/* Class name:      attractor                                                 */
/* Member function: simulation                                                */
/* Purpose:         compute the orbit for a given parameterset                */
/* Modified:        11.10.1994 (Markus Lohmann)                               */
/* Last modified:   27.02.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/

void attractor::simulation()
{
    qint64 colChange=length/6;           // for better viewing there are 6 colors
				       // used to draw the attractor
    const qreal *state[2];

    state[0]=xParam;
    state[1]=yParam;
    model->initialize();

    for(qint64 t=0;t<length;t++)
    {
        model->iteration(t+1);
        if( t >= limit && inRange(*xParam,*yParam) )
        {
    //	    hash->storePoint(state);     // save the results
            if( screenGraphics )
                //screenGraphics->setPoint(*xParam,*yParam,(int)((t/colChange)+1));
                screenGraphics->setRectangularBigPoint(*xParam,*yParam,(int)((t/colChange)+1),2);
                                                         // and draw them
        }
    }
				       // look if the orbit shows a periodic
                                       // behaviour
/***************
    log() << "Number of hits: " << hash->numberOfHits() 
    log() << "Number of buckets: " << hash->numberOfBuckets() 
    log() << "Number of collisions: " << hash->numberOfCollisions() 
    log() << "Order of cycle: " << hash->orderOfCycle() 
    *******************************/
}






m_attractor::m_attractor(baseModel* const bMod,const xyRange& axes,
             MacrodynGraphicsItem* const graph, QString multiPlotLabel, qreal* ts_init_values)
          :geometricJob(bMod,axes,graph)
{
    limit= length / 10;			// 10% are thrown away


    zParam = model->setLabels(multiPlotLabel);	// get the pointer to the second
                    // bifurcation parameter
    multiplot_values = ts_init_values;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      attractor                                                 */
/* Member function: simulation                                                */
/* Purpose:         compute the orbit for a given parameterset                */
/* Modified:        11.10.1994 (Markus Lohmann)                               */
/* Last modified:   27.02.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/

void m_attractor::simulation()
{
    qint64 colChange=length/6;           // for better viewing there are 6 colors
                       // used to draw the attractor
    const qreal *state[2];

    state[0]=xParam;
    state[1]=yParam;

    for(qint64 i=1; i<=multiplot_values[0]; i++) {
        *zParam = multiplot_values[i];
        log()<<*zParam;
        model->initialize();

        for(qint64 t=0;t<length;t++)
        {
            model->iteration(t+1);
            if( t >= limit && inRange(*xParam,*yParam) )
            {
        //	    hash->storePoint(state);     // save the results
                if( screenGraphics )
                    //screenGraphics->setPoint(*xParam,*yParam,(int)((t/colChange)+1));
                    screenGraphics->setRectangularBigPoint(*xParam,*yParam,i,2);
                                                             // and draw them
            }
        }
    }


}

//eof
