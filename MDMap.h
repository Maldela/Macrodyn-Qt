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

#include "error.h"
#include "PublicModels/baseModel.h"

class md_Map {

public:
	md_Map(double min,double max,int res,int dim,int tlim
		,char* filename,char* mothermap);		// constructor new
	md_Map(char* filename);					// constructor to load
	~md_Map();						// destructor
	
	void give_attributes();			// attributes to standard output		
	
    qreal* value_field;			// pointer to store values

private:
	double l_min, l_max;		// limits of map
	int resolution;			// resolution of map
	int model_dim;			// dimension of model variables space
	int time_limit;			// upper end of time dimension
	char* name;			// name(filename) of map
	char* mother;			// pointer to name of mother-map
	char** daugther;		// pointer to names of daugthers		
};

#endif
