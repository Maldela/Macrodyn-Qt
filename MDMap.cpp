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

md_Map::md_Map(double qMin,double qMax,int res,int dim,int tlim
    ,const QString& filename, const QString& mothermap)
{
    l_min = qMin;
    l_max = qMax;
	resolution=res;
	model_dim=dim;
	time_limit=tlim;
	mother = mothermap;
	name = filename;
	
	int d = resolution*model_dim*time_limit;
	
	for (int i=0;i<resolution;i++)
        daugther << "";
		
	value_field = new double[d];
	if ( !value_field ) {
        log() << "md_Map::md_Map can't allocate memory";
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

md_Map::md_Map(const QString& filename)
{
    QFile inFile(filename);

    if (!inFile.open(QFile::ReadOnly)) {
        log() << "md_Map::md_Map input file bad...";
		exit (-1);
	}
	
    QTextStream stream(&inFile);
	name=filename;
	
    stream >> l_min >> l_max;
    stream >> resolution;
    stream >> model_dim;
    stream >> time_limit;
    stream >> mother;
	
	for (int i=0;i<resolution;i++){
        stream >> daugther[i];
	}
	
    value_field = new double[qint64(resolution)*qint64(model_dim)*time_limit];
	if ( !value_field ) 
        log() << "mdMap::mdMap can't allocate memory";
/*	for (i=0;i<resolution;i++){
		values[i] = new const double* [model_dim];
		for (int j=0;j<model_dim;j++){
			values[i][j] = new double[time_limit];
            for (qint64 k=0;k<time_limit;k++){
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
    log() << "l_min: " << l_min;
    log() << "l_max: " << l_max;
    log() << "resolution: " << resolution;
    log() << "model_dim: " << model_dim;
    log() << "time_limit: " << time_limit;
    log() << "name: " << name;
	for (int i=0; i<resolution ; i++)
        if ( !daugther[i].isEmpty() )
            log() << "daugther["<<i<<"]: "<<daugther[i];
    if ( !mother.isEmpty() )
        log() << "mother: " << mother;
    log() << "values:";
    for (int i=0; i<resolution*model_dim*time_limit;i++)
		log() << value_field[i] <<  " ";
}
