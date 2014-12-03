///////////////////////////////////////////////////////////////////////////////
//
//
// Module name:		MDMap.C
// Contents:		Class definition of the class MDMap
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

#include "MDMap.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		MDMap
// Member function:	MDMap
// Purpose:		constructor 1 - for new maps to be initialized
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

md_Map::md_Map(double min,double max,int res,int dim,int tlim
	,char* filename,char* mothermap)
{
	l_min=min;
	l_max=max;
	resolution=res;
	model_dim=dim;
	time_limit=tlim;
	mother = mothermap;
	name = filename;
	
	int d = resolution*model_dim*time_limit;
	
	daugther = new char*[resolution];
	if ( !daugther ) {
		log() << "md_Map::md_Map can't allocate memory\n" << "\n";
		exit (-1);
	}
	for (int i=0;i<resolution;i++)
		daugther[i]="NULL";
		
	value_field = new double[d];
	if ( !value_field ) {
		log() << "md_Map::md_Map can't allocate memory\n" << "\n";
		exit (-1);
	} else { for (int j=0;j<d;j++)
			value_field[j]=0;
		 }
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		MDMap
// Member function:	MDMap
// Purpose:		constructor 2 - for old maps to be loaded
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

md_Map::md_Map(char* filename)
{
	std::fstream inFile;
	inFile.open(filename, ios::in|ios::binary);
	if ( inFile.bad() ) {
		log() << "md_Map::md_Map input file bad...\n" << "\n";
		exit (-1);
	}
	
	name=filename;
	
	inFile >> l_min >> l_max;
	inFile >> resolution;
	inFile >> model_dim;
	inFile >> time_limit;
	inFile >> (char*) mother;
	
	daugther = new char* [resolution];
	for (int i=0;i<resolution;i++){
		inFile >> daugther[i];
	}
	
	value_field = new double[long(resolution)*long(model_dim)*time_limit];
	if ( !value_field ) 
		log() << "mdMap::mdMap can't allocate memory\n" << "\n";
/*	for (i=0;i<resolution;i++){
		values[i] = new const double* [model_dim];
		for (int j=0;j<model_dim;j++){
			values[i][j] = new double[time_limit];
			for (long k=0;k<time_limit;k++){
//				inFile >> values[i][j][k];
			}
		}
	}*/
	inFile.close();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		MDMap
// Member function:	~MDMap
// Purpose:		destructor
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

md_Map::~md_Map()
{
	if ( daugther )
		delete [] daugther;
	if ( value_field ) 
		delete [] value_field;	
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		MDMap
// Member function:	give_attributes
// Purpose:		map internals to standard output
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void md_Map::give_attributes()
{
	log() << "l_min: " << l_min << endl;
	log() << "l_max: " << l_max << endl;
	log() << "resolution: " << resolution << endl;
	log() << "model_dim: " << model_dim << endl;
	log() << "time_limit: " << time_limit << endl;
	log() << "name: " << name << endl;
	for (int i=0; i<resolution ; i++)
		if ( daugther[i] ) 
			log() << "daugther["<<i<<"]: "<<daugther[i]<<endl;
	if ( mother )
		log() << "mother: " << mother << endl;
	log() << "values: \n";
    for (int i=0; i<resolution*model_dim*time_limit;i++)
		log() << value_field[i] <<  " ";
	log() << endl;
}
