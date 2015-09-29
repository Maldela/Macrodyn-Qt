// File:	mapping.C
// Contents:	member functions of class mapping
// Author:	Andreas Starke
// Creation:	12.11.2003
// Modified:	
// Changes:	

#include	"mapping.h"
#include 	"../logger.h"
#include 	"../MDMap.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      mapping                                                   */
/* Member function: mapping                                                   */
/* Purpose:         constructor                                               */
/* Last modified:   12.11.2003 (Andreas Starke)                               */
/*                                                                            */
/******************************************************************************/

mapping::mapping(baseModel* const bMod, MacrodynGraphicsItem* const graph, double min, double max, int res, int dim, int tlim,
    const QString& filename, const QString& mothermap)
    :Job(bMod,graph)
{
    m = new md_Map(min,max,res,dim,tlim,filename,mothermap);
	if ( !m )
		fatalError("mapping::mapping","can't allocate map");
    lmin = min;
    Lmax = max;
	resolution=res;
	model_dim=dim;
	time_limit=tlim;
	name=filename;
	mother=mothermap;
}


mapping::~mapping()
{
	delete m;
}

void mapping::simulation()
{
    log() << "mapping initialized...\n";
	int dummy;
    QList<qreal *> stateVars;
	m->give_attributes();
	
    for (int i=0; i<resolution; i++)
    {
		model->initialize();
        log() << "model length:" << model->getLength();
		for (int j=0; j<time_limit; j++) {
			model->iteration(j+1);
            model->sendStateSpace(dummy, &stateVars);
            for (int k=0; k<model_dim; k++)
            {
            //	log() << i*time_limit*model_dim+j*model_dim+k << " " 
            //	log() << (states)[0] << " " << (states)[1] << "\n" 
            //	log() << ((*states)[0]) << " " << ((*states)[1]) << "\n" 
                m->value_field[i*time_limit*model_dim+j*model_dim+k] = *stateVars[k]; //Weird
			}
		}
	}
	m->give_attributes();
}

//eof
