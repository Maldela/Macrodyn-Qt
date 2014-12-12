// File:	basinTwoCycles.C
// Contents:	member functions of class basinTwoCycles
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:16:47 MEZ 1996
// Changes:	

#include	"basinTwoCycles.h"
#include	"../error.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      basinTwoCycles                                            */
/* Member function: basinTwoCycles                                            */
/* Purpose:         constructor                                               */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

basinTwoCycles::basinTwoCycles(baseModel* const bMod,const xyRange& axes,
                   const QString& name1, const QString& name2,
                   MacrodynGraphicsItem* const graph)
          :basin(bMod,axes,name1,graph)
{
    initCycle(name2,&cycle2,period2);         // read the second cycle out of a
					      // file
}

/******************************************************************************/
/*                                                                            */
/* Class name:      basinTwoCycles                                            */
/* Member function: ~basinTwoCycles                                           */
/* Purpose:         destructor                                                */
/* Last modified:   06.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

basinTwoCycles::~basinTwoCycles()
{
    for(int i=0;i<dimension;i++)
	delete cycle2[i];
    delete cycle2;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      basinTwoCycles                                            */
/* Member function: simulation                                                */
/* Purpose:         computes the basins of attraction for two given           */
/*                  coexisting cycles                                         */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void basinTwoCycles::simulation()
{
    uint att1=0;			// orbit tends to cycle1 or not
    uint att2=0;			// orbit tends to cycle2 or not
    qreal **lastOrbit;		        // matrix to store the actual orbit
    int i=0;			        // 0 <= i < dimension
    qint64 t;				// time
    int record=0;			// index of the orbit vectors that
					// shows where the actual simulation
					// results are stored
					// 0 <= record <= period1
    qint64 tDiv=length/6;			// analysis of the simulation results
					// is done every tDiv periods until
					// length is reached or the orbit
					// tends to the cycle
    QList<qreal *> stateVars;      // pointer to the state vars.

    int period= period1 > period2 ? period1: period2; // qMax(period1,period2);

					// the algorithm is done in the same
					// way as in the method 
					// basin::simulation except that the
					// points that tend to cycle1 are 
					// marked in color one whereas the
					// points that tend to cycle2 are
					// marked in color 2
    int dummy;
    qint64 iter=0;
    model->sendStateSpace(dummy,&stateVars);
    if( !(lastOrbit = new qreal*[dimension]) )
	fatalError("basinTwoCycles::simulation","Can't create orbit matrix");
    for(i=0;i<dimension;i++)
	if( !(lastOrbit[i] = new qreal[period]) )
	    fatalError("basinTwoCycles::simulation",
		       "Can't create orbit matrix");

    for( *xParam=xmin; *xParam<=xmax ;*xParam += stepX)
    {
        for( *yParam=ymin; *yParam<=ymax ;*yParam += stepY)
        {
	    record=0;
            model->initialize();
            for( t=0;t<length;t++ )
            {
		model->iteration(t+1);
		iter++;
		if( record ) {
		    for(i=0;i<dimension;i++) 
			lastOrbit[i][record-1]=*(stateVars[i]);
		    if( record == period ) {
			record=0;
			att1=isAttracting(lastOrbit,cycle,period1);
			att2=isAttracting(lastOrbit,cycle2,period2);
            if( (att1 || att2) && (screenGraphics) ) {
                if( att1 ) {
                if( screenGraphics )
                    screenGraphics->setPoint(*xParam,*yParam,1);
                }
                else{
                if( screenGraphics )
                    screenGraphics->setPoint(*xParam,*yParam,6);
                }
                break;
            }
            }
		    else
			record++;
		}
		else
		    record= ((t+period) % tDiv) == 0;
	    }                               // end of for(t=..  
	}
    }
    for(i=0;i<dimension;i++)
	delete lastOrbit[i];
    delete lastOrbit;
}

//eof
