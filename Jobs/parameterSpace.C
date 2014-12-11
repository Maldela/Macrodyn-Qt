// File:	parameterSpace.C
// Contents:	member functions of class parameterSpace
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:25:09 MEZ 1996
// Changes:	

#include	"parameterSpace.h"
#include	"../error.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      parameterSpace                                            */
/* Member function: parameterSpace                                            */
/* Purpose:         constructor                                               */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

parameterSpace::parameterSpace(baseModel* const bMod, const xyRange& axes, 
                   const xyRange& stateSpaceLim,
                   MacrodynGraphicsItem* const graph)
               :bif2D(bMod,axes,graph), stateSpace(stateSpaceLim)
{
    hash->resetDomain(stateSpace);        // save bounderies of the state
					  // space section
    stepY=(ymax-ymin)/(axes.res[1]-1);
    stateVars=new const qreal* [stateSpace.dimension];
    if( !stateVars )
    fatalError("parameterSpace::parameterSpace",
           "Can't create vector of state variables");
    for(int i=0;i<stateSpace.dimension;i++) {
    stateVars[i]=model->setLabels(stateSpace.label[i]);
                                          // get pointer to the model var.
	if( !stateVars[i] )
	    fatalError("parameterSpace::parameterSpace",
		       "illegal state variable specified");
    }
    limit = length / 10;		  // 10% are thrown away
}

/******************************************************************************/
/*                                                                            */
/* Class name:      parameterSpace                                            */
/* Member function: ~parameterSpace                                           */
/* Purpose:         destructor                                                */
/* Last modified:   15.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

parameterSpace::~parameterSpace()
{
    if( stateVars )
    delete stateVars;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      parameterSpace                                            */
/* Member function: simulation                                                */
/* Purpose:         computes a map showing the orbits dependence on parameters*/
/*                  2 parameters vary, for each parameter combination the     */
/*                  resulting orbit is analysed on periodic behaviour;        */
/*                  the corresponding point in the parameter space is colored */
/*                  depending on the order of the (poss.) periodic behaviour  */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void parameterSpace::simulation()
{
    qint64 t;				// time
    int order=0;			// order of cycle
    quint64 tDiv=(uint)(length/10);// analysis of the simulation results
					// is done every tDiv periods until
					// length is reached or a cycle has
					// been detected

    for(*xParam=xmin;*xParam<=xmax; *xParam+=stepX) {
    for(*yParam=ymin;*yParam<=ymax;*yParam+=stepY) {
	    model->initialize();
	    for(t=0;t<length;t++) {
            model->iteration(t+1);
            if( t > limit ) {
                if( hash->storePoint(stateVars) ) {
                    hash->resetHashTable();
                    order=-1;    // out of domain, leave it blue
    //			    log() << *(stateVars[0]) 
                    break;
                }
                if( !(t % tDiv) || (t==(length-1)) ) {
                    if ( (order=hash->orderOfCycle()) ) {
                        hash->resetHashTable();
                        break;	// a cycle has been detected
                            // so the analysis of this parameterset
                            // can be terqMinated and the hash table
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
        screenGraphics->setPoint(*xParam,*yParam,order+1);
	}
    }
}
    
//eof
