// File:	stateSpaceAnalysis.C
// Contents:	member functions of class stateSpaceAnalysis
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:25:09 MEZ 1996
// Changes:	

#include	"stateSpaceAnalysis.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      stateSpaceAnalysis                                        */
/* Member function: stateSpaceAnalysis                                        */
/* Purpose:         constructor                                               */
/* Last modified:   06.12.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

stateSpaceAnalysis::stateSpaceAnalysis(baseModel* const bMod,
				       const xyRange& axes,
                       MacrodynGraphicsItem* const graph)
          :attractor(bMod,axes,graph)
{
    int modelDim;
    
    model->sendStateSpace(modelDim,&stateVars);
    maxCell=hash->noCells();
    limit =(long)(length *0.66); // first two thirds are thrown away
}                                      

/******************************************************************************/
/*                                                                            */
/* Class name:      stateSpaceAnalysis                                        */
/* Member function: stateSpaceAnalysis                                        */
/* Purpose:         destructor                                                */
/* Last modified:   06.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

stateSpaceAnalysis::~stateSpaceAnalysis()
{
    if( stateVars )
    delete stateVars;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      stateSpaceAnalysis                                        */
/* Member function: simulation                                                */
/* Purpose:         computes a global analysis of the system for a fixed      */
/*                  parameterset                                              */
/* Last modified:   06.12.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void stateSpaceAnalysis::simulation()
{
    unsigned outside;
    qreal cell;
    long t;
    short color;
    qreal *param[2];
  
    param[0]=xParam;
    param[1]=yParam;
      
    for(cell=0;cell<maxCell;cell++) {
	hash->discrete2Cont(cell,param);
	model->initialize();
	outside=0;
	for(t=0;t<length;t++) {
	    model->iteration(t+1);
	    if( t >= limit ) {
		if( hash->storePoint(stateVars) ) {
		    outside=1;
		    color=-1;
		    break;
		}
	    }
	}
	if( !outside ) 
	    color=hash->orderOfCycle();
	
	if( screenGraphics ) 
	    screenGraphics->setPoint(*xParam,*yParam,color+1);   

	hash->resetHashTable();
    }
}

//eof
