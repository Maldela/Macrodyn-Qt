// File:	parameterSpace.C
// Contents:	member functions of class parameterSpace
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:25:09 MEZ 1996
// Changes:	

#include	"parameterSpace.h"
#include	"../logger.h"

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
    log()<<"stepX = "<<stepX;
    log()<<"stepY = "<<stepY;
//    stepY=(ymax-ymin)/50.0;
//    stepX=(xmax-xmin)/50.0;
    stateVars.clear();
    log()<<"stateSpace.dimension = "<<stateSpace.dimension;
    for (int i=0;i<stateSpace.dimension;i++) {
    stateVars << model->setLabels(stateSpace.label[i]);
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
    for(*xParam=xmin; *xParam<=xmax; *xParam+=stepX)
    {
        for(*yParam=ymin;*yParam<=ymax;*yParam+=stepY)
        {
            model->initialize();
            for(t=0;t<length;t++)
            {
                model->iteration(t+1);
                if( t > limit ) {
                    if( hash->storePoint(stateVars) )
                    {
                        hash->resetHashTable();
                        order=-1;    // out of domain, leave it blue
        //			    log() << *(stateVars[0])
                        break;
                    }
                    if( !(t % tDiv) || (t==(length-1)) )
                    {
                        if ( (order=hash->orderOfCycle()) )
                        {
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
                screenGraphics->setRect(*xParam,*yParam,stepX,stepY,colorFromOrder(order));
                //screenGraphics->setRect(*xParam,*yParam,stepX,stepY,QColor((1234*(order+1))%255,(12345*(order+1))%255,(123456*(order+1))%255));
        }
    }
}

QColor parameterSpace::colorFromOrder(int order) {
    switch (order) {
    case -1: return QColor(255,255,255);
    case 1: return QColor(185,82,159);
    case 2: return QColor(111,204,221);
    case 3: return QColor(210,185,210);
    case 4: return QColor(105,189,69);
    case 5: return QColor(237,32,36);
    case 6: return QColor(245,172,189);
    case 7: return QColor(161,157,157);
    case 8: return QColor(57,83,164);
    case 9: return QColor(244,121,33);
    case 10: return QColor(243,219,171);
    case 11: return QColor(131,197,99);
    case 12: return QColor(233,54,137);
    case 13: return QColor(114,142,199);
    case 14: return QColor(242,100,66);
    case 15: return QColor(36,56,59);
    case 16: return QColor(40,65,29);
    case 17: return QColor(184,59,143);
    case 18: return QColor(239,60,36);
    case 19: return QColor(72,194,217);
    case 20: return QColor(48,103,84);
    case 21: return QColor(71,99,160);
    case 22: return QColor(129,22,67);
    case 23: return QColor(0,0,0);
    case 24: return QColor(216,175,121);
    case 25: return QColor(122,40,124);
    case 26: return QColor(68,182,184);
    case 27: return QColor(113,143,201);
    case 28: return QColor(209,159,42);
    case 29: return QColor(31,31,82);
    case 30: return QColor(151,27,21);
    }
    return QColor(246,235,20);
}
    
//eof
