// File:	simpleCellMapping.C
// Contents:	member functions of class simpleCellMapping
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:25:09 MEZ 1996
// Changes:	

#include	"simpleCellMapping.h"
#include	"../logger.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      simpleCellMapping                                         */
/* Member function: simpleCellMapping                                         */
/* Purpose:         constructor                                               */
/* Last modified:   29.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

simpleCellMapping::simpleCellMapping(baseModel* const bMod,const xyRange& axes,
                     MacrodynGraphicsItem* const graph)
          :geometricJob(bMod,axes,graph)
{
    int modelDim;
    
    h[0]=axes.res[0];
    h[1]=axes.res[1];
    K=h[0]*h[1];
    Gr=new int[K+1];
    P =new int[K+1];
    St=new int[K+1];
    if( !Gr || !P || !St )
	fatalError("simpleCellMapping::simpleCellMapping",
		   "Can't create vectors");
    for(uint i=0;i<=K;i++)
	Gr[i]=P[i]=St[i]=0;
    lastGroup=0;
    model->sendStateSpace(modelDim,&stateVars);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      simpleCellMapping                                         */
/* Member function: ~simpleCellMapping                                        */
/* Purpose:         destructor                                                */
/* Last modified:   29.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

simpleCellMapping::~simpleCellMapping()
{
    if( Gr )
	delete Gr;
    if( P )
	delete P;
    if( St )
    delete St;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      simpleCellMapping                                         */
/* Member function: diskrete2Cont                                             */
/* Purpose:         transformation of a diskrete cell to the corresponding    */
/*		    lower left corner of that cell in the state space         */
/* Last modified:   30.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void simpleCellMapping::discrete2Cont(const uint& startCell,qreal& x,
					  qreal& y)
{
    uint cellMod=startCell % h[0];
    uint cellDiv=startCell / h[0];

    if( !cellMod ) {
	cellMod=h[0];
	cellDiv--;
    }
    
    x=xmin+(cellMod-1)*(xmax-xmin)/h[0];
    y=ymin+cellDiv*(ymax-ymin)/h[1];
}

/******************************************************************************/
/*                                                                            */
/* Class name:      simpleCellMapping                                         */
/* Member function: iteration                                                 */
/* Purpose:         iteration of the cell mapping                             */
/* Last modified:   30.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

uint simpleCellMapping::iteration(const uint& startCell)
{
    uint cellX;
    uint cellY;
    
    if( !startCell )
	return 0;		// iteration(0)=0
    discrete2Cont(startCell,*xParam,*yParam);

    model->initialize();
    model->iteration(1);

    if( !inRange(*stateVars[0],*stateVars[1]) )
	return 0;

    cellX=(uint)((*stateVars[0]-xmin)/(xmax-xmin)*h[0]) + 1;
    cellY=(uint)((*stateVars[1]-ymin)/(ymax-ymin)*h[1]);

    return ( h[0]*cellY + cellX );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      simpleCellMapping                                         */
/* Member function: virginCell                                                */
/* Purpose:         returns the id (cell number)  of the next virgin cell     */
/* Last modified:   29.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

uint simpleCellMapping::virginCell()
{
    static uint lastCell=1;
    uint j=lastCell;
    
    while( (j<=K) && Gr[j] )
	j++;
    if( j<=K )
	lastCell=j;
    else
	return 0;
    return lastCell;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      simpleCellMapping                                         */
/* Member function: oldGroup                                                  */
/* Purpose:         A cell (or a path) is found that lies in the basin of     */
/*                  attraction of an already known cycle; all cells of that   */
/*                  path are assigned the group number of that cycle          */
/* Last modified:   30.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void simpleCellMapping::oldGroup(uint& startCell,uint& endCell,
                 uint& steps)
{
    uint group=Gr[endCell];
    uint period=P[endCell];
    uint stepNumber=St[endCell];
    uint actualStep=0;

    while(startCell != endCell) {
	Gr[startCell]=group;
	P[startCell]=period;
	St[startCell]=stepNumber+steps-actualStep;
	actualStep++;
	startCell=iteration(startCell);
    }
}
    
/******************************************************************************/
/*                                                                            */
/* Class name:      simpleCellMapping                                         */
/* Member function: newGroup                                                  */
/* Purpose:         A new cycle is found; all cells beqint64ing to that cycle   */
/*                  are assigned a new group number                           */
/* Last modified:   30.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void simpleCellMapping::newGroup(uint& startCell,uint& endCell,
                 uint& steps)
{
    uint group;
    uint period;
    uint actualStep=0;
    uint firstHit;
    uint runCell=startCell;
    
    while(runCell != endCell) { // find the beginning of the new cycle
	actualStep++;
	runCell=iteration(runCell);	
    }
    firstHit=actualStep;
    period=P[endCell]=steps-actualStep;
    group=Gr[endCell]=++lastGroup;

    runCell=iteration(runCell);

    while(runCell != endCell) { // assign the characteristics to the
	P[runCell]=period;	  // periodic cells
	Gr[runCell]=group;
	runCell=iteration(runCell);
    }
    
    runCell=startCell;
    actualStep=0;
    while(runCell != endCell) { // assign the characteristics to the
	P[runCell]=period;	  // cells that lie on the actual path and
	Gr[runCell]=group;	  // therefore in the basin of attraction
	St[runCell]=firstHit-actualStep; // of the new detected cycle
	runCell=iteration(runCell);
	actualStep++;
    }	
}

/******************************************************************************/
/*                                                                            */
/* Class name:      simpleCellMapping                                         */
/* Member function: simulation                                                */
/* Purpose:         Performs a global analysis of a simple cell mapping       */
/*                  (k-cycles and their basins of attraction)                 */
/* Last modified:   29.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void simpleCellMapping::simulation()
{
    uint cellProc;
    uint cellProcBackup;
    uint imageCell;
    uint step=0;
    uint finished;
    qreal x,y;
    
    log() << K << " cells at all";

    model->initialize();
   
    while( (cellProc=virginCell()) ) {
	cellProcBackup=cellProc;
	Gr[cellProc]=-1;
	step=0;
	finished=0;
	while( !finished ) {
	    imageCell=iteration(cellProc);
	    step++;
	    if( !Gr[imageCell] ) {
		Gr[imageCell]=-1;
		cellProc=imageCell;
		continue;
	    }
	    finished=1;
	    if( Gr[imageCell] > 0)
		oldGroup(cellProcBackup,imageCell,step);
	    else
		newGroup(cellProcBackup,imageCell,step);
	}
    }
    uint *groups=new uint[lastGroup+1];

    log() << "group[0]=" << Gr[0];
    for(uint i=0;i<=K;i++)
       	if( Gr[i] != 0 ) {
	    discrete2Cont(i,x,y);
        screenGraphics->setPoint(x,y,Gr[i]+1);
	    if( !groups[Gr[i]] ){
		groups[Gr[i]]=1;
        log() << "Group " << Gr[i] << " with period " << P[i];
	    }
	}
    log() << lastGroup << " groups at all";
    delete groups;
}

//eof
