// File:	timeSeriesJob.C
// Contents:	member functions of class timeSeriesJob
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// last modified:   Thu Jun  3 14:45:49 CEST 1999 Marc Mueller

#include	"timeSeriesJob.h"
#include	"../logger.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      timeSeriesJob                                             */
/* Member function: timeSeriesJob                                             */
/* Purpose:         constructor                                               */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

timeSeriesJob::timeSeriesJob(baseModel* const bMod, const QString& label,
                 MacrodynGraphicsItem* const graph)
          :Job(bMod,graph)
{
    timeSeriesqreal=new qreal[length+1];
    if( !timeSeriesqreal )
    fatalError("timeSeriesJob::timeSeriesJob",
		   "Can't allocate enough memory");
    modelVar=model->setLabels(label);
    if( !modelVar )
    fatalError("timeSeriesJob::timeSeriesJob",
		   "Null pointer as model var");
}

/******************************************************************************/
/*                                                                            */
/* Class name:      timeSeriesJob                                             */
/* Member function: ~timeSeriesJob                                            */
/* Purpose:         destructor                                                */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

timeSeriesJob::~timeSeriesJob()
{
    delete timeSeriesqreal;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      timeSeriesJob                                             */
/* Member function: saveSeries                                                */
/* Purpose:         save simulation results (modelVar)                        */
/* Modified:        11.10.1994 (Markus Lohmann)                               */
/* last modified:   Thu Jun  3 14:45:49 CEST 1999 Marc Mueller                */
/*                                                                            */
/******************************************************************************/

void timeSeriesJob::saveSeries(const qint64& t)
{
    if( (t>=0) && (t<=length) )
	timeSeriesqreal[t]=*modelVar;
}

//eof
