// File:	geometricJob.C
// Contents:	member functions of class geometricJob
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	98/08/21 (Marc Mueller) 

#include	"geometricJob.h"
#include	"../error.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      geometricJob                                              */
/* Member function: geometricJob                                              */
/* Purpose:         constructor                                               */
/* Modified:        19.10.1994 (Markus Lohmann)                               */
/* Last modified:   98/08/21 (Marc Mueller)                                   */
/*                                                                            */
/******************************************************************************/

geometricJob::geometricJob(baseModel* const bMod,const xyRange& axes,
                MacrodynGraphicsItem* const graph, printer* const outDev)
          :job(bMod,graph,outDev), xmin(axes.min[0]), xmax(axes.max[0]),
	   ymin(axes.min[1]),ymax(axes.max[1])
{
//    unsigned entries = (unsigned) (length < 103409 ? (length/2+1) : 103409);
// changed to entries mod 4 = 3 , A.Starke
    unsigned entries = (unsigned) (length < 350351 ? (length/2+1) : 350351);
    unsigned resX;
    unsigned resY;

    if( outDev ) {
	resX=5400;
	resY=3600;
    }
    else {
	resX=axes.res[0];
	resY=axes.res[1];
    }
    
    strcpy(xLabel,axes.label[0]);
    xParam=model->setLabels(xLabel);
    if( !xParam )
	fatalError("geometricJob::geometricJob  Can not find x label ",xLabel);

    strcpy(yLabel,axes.label[1]);
    yParam=model->setLabels(yLabel);
    if( !yParam )
	fatalError("geometricJob::geometricJob  Can not find y label ",yLabel);

    hash = new hashTable(entries,axes);
    if( !hash )
	fatalError("geometricJob::geometricJob","Can't allocate hash table");
}

/******************************************************************************/
/*                                                                            */
/* Class name:      geometricJob                                              */
/* Member function: ~geometricJob                                             */
/* Purpose:         destructor                                                */
/* Modified:        11.10.1994 (Markus Lohmann)                               */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

geometricJob::~geometricJob()
{
    if( hash )
	delete hash;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      geometricJob                                              */
/* Member function: inRange                                                   */
/* Purpose:         determine wether the simulation results lay in the section*/
/*                  under consideration or not                                */
/* Modified:        11.10.1994 (Markus Lohmann)                               */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

unsigned geometricJob::inRange(const qreal&x,const qreal& y)
{
    return( ((x >= xmin)&&(x <= xmax)&&(y >= ymin)&&(y <= ymax)) ? 1:0 );
}

