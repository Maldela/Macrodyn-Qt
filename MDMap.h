///////////////////////////////////////////////////////////////////////////////
//
// Module name:		MDMap.h
// Contents:		Class definition of the class MDMap
//			onedimensional (in parameter space) map of model space
//
// Author:		Andreas Starke
// Last modified:				
//
///////////////////////////////////////////////////////////////////////////////

#ifndef MD___MAP_INCLUDED
#define MD___MAP_INCLUDED

#include "logger.h"
#include "PublicModels/baseModel.h"

class md_Map {

public:
    md_Map(double min,double max,int res,int dim,int tlim
        ,const QString& filename, const QString& mothermap);		// constructor new
    md_Map(const QString& filename);					// constructor to load
	~md_Map();						// destructor
	
	void give_attributes();			// attributes to standard output		
	
    qreal* value_field;			// pointer to store values

private:
    double l_min, l_max;		// limits of map
	int resolution;			// resolution of map
	int model_dim;			// dimension of model variables space
	int time_limit;			// upper end of time dimension
    QString name;			// name(filename) of map
    QString mother;			// pointer to name of mother-map
    QStringList daugther;		// pointer to names of daugthers
};

#endif
