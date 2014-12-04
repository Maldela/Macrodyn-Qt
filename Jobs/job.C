// File:	job.C
// Contents:	member functions of class job
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:25:09 MEZ 1996
// Changes:	

#include	"job.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      job                                                       */
/* Member function: job                                                       */
/* Purpose:         constructor                                               */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

job::job(baseModel * const bMod, MacrodynGraphicsItem *graph)
    : model(bMod),screenGraphics(graph)
{
    length=model->getLength();
    limit=0;
}
job::~job()
{
}
/******************************************************************************/
/*                                                                            */
/* Class name:      job                                                       */
/* Member function: job                                                       */
/* Purpose:         constructor                                               */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

job::job(baseModel * const bMod, const qint64& len, const qint64& lim,
      MacrodynGraphicsItem *graph)
    : length(len),limit(lim), model(bMod), screenGraphics(graph)
{
}

void job::setStepX(const qreal&)
{
}

//eof
