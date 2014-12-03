// File:	job.h
// Contents:	definition of class job
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
/* Derived from:  job                                                         */
/* Purpose:       mapping of model for successive and complete analysis       */
/*                			                                      */
/* Author:        Andreas Starke                                              */
/* Last modified: 12.11.2003                                                  */
/*                                                                            */
/******************************************************************************/

class mapping : public job
{
  protected:
	md_Map* m;
	double lmin;
	double lmax;
	int resolution;
	int model_dim;
	int time_limit;
	char* name;
	char* mother;
	
  public:
    mapping(baseModel * const, MacrodynGraphicsItem* const=NULL,
    	double min=0, double max=0, int res=0, int dim=0, int tlim=0, char*
	filename=NULL, char* mothermap=NULL);
    virtual ~mapping();
    
    void simulation();
};
#endif
//eof
