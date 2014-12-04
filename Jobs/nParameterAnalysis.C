// File:	nParameterAnalysis.C
// Contents:	member functions of class nParameterAnalysis
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:25:09 MEZ 1996
// Changes:	

#include	"nParameterAnalysis.h"
#include	"../error.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      nParameterAnalysis                                        */
/* Member function: nParameterAnalysis                                        */
/* Purpose:         constructor                                               */
/* Last modified:   07.03.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

nParameterAnalysis::nParameterAnalysis(baseModel* const bMod,
				       const xyRange& axes, 
				       const xyRange& stateSpaceLim, 
				       const xyRange& xDef, 
				       const xyRange& yDef, 
                       MacrodynGraphicsItem* const graph)
    :parameterSpace(bMod,axes,stateSpaceLim,graph),
     effectiveX(xDef),effectiveY(yDef)
{
    for(int i=0;i<effectiveX.dimension;i++) {
    xVars[i]=*model->setLabels(effectiveX.label[i].toLatin1().data());
                                          // get pointer to the model var.
	if( !xVars[i] )
	    fatalError("nParameterAnalysis::nParameterAnalysis",
		       "illegal x parameter specified");
    }

    for(int i=0;i<effectiveY.dimension;i++) {
    yVars[i]=*model->setLabels(effectiveY.label[i].toLatin1().data());
                                          // get pointer to the model var.
	if( !yVars[i] )
	    fatalError("nParameterAnalysis::nParameterAnalysis",
		       "illegal y parameter specified");
    }
}

/******************************************************************************/
/*                                                                            */
/* Class name:      nParameterAnalysis                                        */
/* Member function: ~nParameterAnalysis                                       */
/* Purpose:         destructor                                                */
/* Last modified:   03.03.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

nParameterAnalysis::~nParameterAnalysis()
{
}

/******************************************************************************/
/*                                                                            */
/* Class name:      nParameterAnalysis                                        */
/* Member function: setXParams                                                */
/* Purpose:         linear update of the x-parameters                         */
/* Last modified:   03.03.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void nParameterAnalysis::setXParams(const qreal& newX)
{
    static qreal divisor=xmax-xmin;
    for(int i=0;i<effectiveX.dimension;i++)
    xVars[i]=effectiveX.min[i]+(newX-xmin)/divisor*
	    (effectiveX.max[i]-effectiveX.min[i]);
}
/******************************************************************************/
/*                                                                            */
/* Class name:      nParameterAnalysis                                        */
/* Member function: setXParams                                                */
/* Purpose:         linear update of the x-parameters                         */
/* Last modified:   03.03.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void nParameterAnalysis::setYParams(const qreal& newY)
{
    static qreal divisor=ymax-ymin;

    for(int i=0;i<effectiveY.dimension;i++)
    yVars[i]=effectiveY.min[i]+(newY-ymin)/divisor*
	    (effectiveY.max[i]-effectiveY.min[i]);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      nParameterAnalysis                                        */
/* Member function: simulation                                                */
/* Purpose:         computes a map showing the orbits dependence on parameters*/
/*                  2 parameters vary, for each parameter combination the     */
/*                  resulting orbit is analysed on periodic behaviour;        */
/*                  the corresponding point in the parameter space is colored */
/*                  depending on the order of the (poss.) periodic behaviour  */
/* Last modified:   07.03.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void nParameterAnalysis::simulation()
{
    qint64 t;				// time
    int order=0;			// order of cycle
    uint tDiv=(uint)(length/10);  // analysis of the simulation results
					// is done every tDiv periods until
					// length is reached or a cycle has
					// been detected
    qreal x=0;
    qreal y=0;
    for(x=xmin;x<=xmax; x+=stepX) {
	setXParams(x);
	for(y=ymin;y<=ymax;y+=stepY) {
	    setYParams(y);
	    model->initialize();
	    for(t=0;t<length;t++) {
            model->iteration(t+1);
            if( t > limit ) {
                if( hash->storePoint(stateVars) ) {
                    hash->resetHashTable();
                    order=-1;    // out of domain, leave it blue
                    break;
                }
                if( !(t % tDiv) || (t==(length-1)) ) {
                    if ( (order=hash->orderOfCycle()) ) {
                        hash->resetHashTable();
                        break;	// a cycle has been detected
                            // so the analysis of this parameterset
                            // can be terminated and the hash table
                            // should be initialized for the next
                            // parameterset
                    }
                    else
                    hash->resetHashTable();
                        // a new analysis has to be done
                        // clean the hash table for the new
                        // simulation results
                }
            }
	    }
	    if( screenGraphics ) 
        screenGraphics->setPoint(x,y,order+1);
	}
    }
}

//eof
