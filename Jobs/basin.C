// File:	basin.C
// Contents:	member functions of class basin
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:16:47 MEZ 1996
// Changes:	

#include	"basin.h"
#include	"../error.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      basin                                                     */
/* Member function: basin                                                     */
/* Purpose:         constructor                                               */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

basin::basin(baseModel* const bMod,const xyRange& axes, char* const cycleFile,
         MacrodynGraphicsItem* const graph)
          :geometricJob(bMod,axes,graph)
{
    initCycle(cycleFile,&cycle,period1); // read cycle out of a file
    stepX=(xmax-xmin) / (1.02*axes.res[0]);
    stepY=(ymax-ymin) / (1.02*axes.res[1]);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      basin                                                     */
/* Member function: initCycle                                                 */
/* Purpose:         read cycle out of a file                                  */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void basin::initCycle(char *const name,qreal*** cycle,int& period)
{
    ifstream cycleFile(name);

    if( cycleFile.bad() )
	fatalError("basin::initCycle","Can't open cycle-file");

    cycleFile >> period >> dimension;
    if( !(*cycle = new qreal*[dimension]) )
	fatalError("basin::initCycle","Can't create cycle matrix");
    for(int i=0;i<dimension;i++)
	if( !( (*cycle)[i] = new qreal[period] ) )
	    fatalError("basin::initCycle","Can't create cycle matrix");
    
    for(int j=0;j<period;j++)
    for(int i=0;i<dimension;i++)
	    cycleFile >> (*cycle)[i][j];
}

/******************************************************************************/
/*                                                                            */
/* Class name:      basin                                                     */
/* Member function: isAttracting                                              */
/* Purpose:         determine wether the computed orbit tends to the given    */
/*                  cycle or not                                              */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

uint basin::isAttracting(qreal** lastOrbit,qreal** cycle,const int& period)
{
    qreal eps=0.001;                   // accuracy of "tends to"
    qreal firstComp=lastOrbit[0][0];   // get the first component of the 
				        // orbit's first point
    int i=0;				// 0 <= i < period
    int j=0;			        // 0 <= j < dimension
    int found=0;		        // first point of the orbit and the
				        // cycle match
    int ok=1;		        // all other points match too
    int attempt=0;		        // point under consideration
				        // 0<=attempt< period

    while( !found && i < period) {	// try to find the corresponding point
					// of the cycle
        found= fabs(firstComp-cycle[0][i]) < eps;
        i++;
    }
    if( !found )
        return 0;
    i--;
    while( ok && attempt < period ) {
        if( i == period )
            i=0;
	j=0;
	while( ok && (j<dimension) ) {
	    ok=fabs(lastOrbit[j][attempt]-cycle[j][i]) < eps;
	    j++;
	}
	attempt++;
	i++;
    }  
    return (uint) ok;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      basin                                                     */
/* Member function: setStepX                                                  */
/* Purpose:         set a new step size for the x axis                        */
/* Last modified:   16.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void basin::setStepX(const qreal& toSet)
{
    stepX=toSet;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      basin                                                     */
/* Member function: simulation                                                */
/* Purpose:         computes the basin of attraction for a given cycle        */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void basin::simulation()
{
    uint att1=0;			// orbit tends to cycle or not
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
    
    const qreal **stateVars=NULL;      // pointer to the state vars.

    int dummy;
    
    model->sendStateSpace(dummy,&stateVars);

    if( !(lastOrbit = new qreal*[dimension]) )
	fatalError("basin::simulation","Can't create orbit matrix");
    for(i=0;i<dimension;i++)
	if( !(lastOrbit[i] = new qreal[period1]) )
	    fatalError("basin::simulation","Can't create orbit matrix");
    
    for( *xParam=xmin; *xParam<=xmax ;*xParam += stepX) {
        for( *yParam=ymin; *yParam<=ymax ;*yParam += stepY) {
	    record=0;
            model->initialize();
            for( t=0;t<length;t++ ) {
		model->iteration(t+1);

		if( record ) {
		    for(i=0;i<dimension;i++) 
			lastOrbit[i][record-1]=*(stateVars[i]);
		    if( record == period1 ) {
			record=0;
			att1=isAttracting(lastOrbit,cycle,period1);
			if( att1 ) {
                if( screenGraphics )
                screenGraphics->setPoint(*xParam,*yParam,9);
			    break;
			}
		    }
		    else
			record++;
		}
		else
		    record= ((t+period1) % tDiv) == 0;
					// if record equals 1 the next period1
					// simulation results are stored in the
					// orbit vectors; then the orbit is 
					// checked in the method isAttracting

	    }				// end of for(t=..  
	}
    }
    if( stateVars )
	delete stateVars;
    for(i=0;i<dimension;i++)
	delete lastOrbit[i];
    delete lastOrbit;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      basin                                                     */
/* Member function: basin                                                     */
/* Purpose:         destructor                                                */
/* Last modified:   06.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

basin::~basin()
{
    for(int i=0;i<dimension;i++)
	delete cycle[i];
    delete cycle;
}

//eof
