// File:	Job.h
// Contents:	definition of class Job
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:16:47 MEZ 1996
// Changes:	

#ifndef MAPPING_H
#define MAPPING_H
#include "job.h"
#include "../Models/baseModel.h"
#include "../Graphics/macrodyngraphicsitem.h"
#include "../MDMap.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    mapping                                                     */
/* Derived from:  Job                                                         */
/* Purpose:       mapping of model for successive and complete analysis       */
/*                			                                      */
/* Author:        Andreas Starke                                              */
/* Last modified: 12.11.2003                                                  */
/*                                                                            */
/******************************************************************************/

class mapping : public Job
{
  protected:
	md_Map* m;
    double lmin;
    double Lmax;
	int resolution;
	int model_dim;
	int time_limit;
    QString name;
    QString mother;
	
  public:
    mapping(baseModel * const, MacrodynGraphicsItem* const=NULL,
        double qMin=0, double qMax=0, int res=0, int dim=0, int tlim=0,
    const QString& = "", const QString& = "");
    virtual ~mapping();
    
    void simulation();
};
#endif
//eof
