// File:	cyclogram_3d.C
// Contents:	member functions of class cyclogram_3d
// Author:	Andreas Starke
// Creation:	
// Modified:	
// Changes:	

#include	"3d_cyclogram.h"
#include	"../error.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      cyclogram_3d                                            */
/* Member function: cyclogram_3d                                            */
/* Purpose:         constructor                                               */
/* Last modified:                                 */
/*                                                                            */
/******************************************************************************/

cyclogram_3d::cyclogram_3d(baseModel* const bMod, const xyRange& axes, 
			       const xyRange& stateSpaceLim, 
                   MacrodynGraphicsItem* const graph,
                   const qint64 bif3D_dx, const qint64 bif3D_dy,
                   const qint64 bif3D_dz)
               :bif3D_2par(bMod,axes,graph, bif3D_dx, bif3D_dy, bif3D_dz), stateSpace(stateSpaceLim)
{
    hash->resetDomain(stateSpace);        // save bounderies of the state
					  // space section
    stepY=(ymax-ymin)/(axes.res[1]-1);
    stepZ=(zmax-zmin)/(axes.res[2]-1);
    stepX=(xmax-xmin)/(axes.res[0]-1);
    resolution_x = axes.res[0];
    resolution_y = axes.res[1];
    resolution_z = axes.res[2];
    log() << "resx: " << resolution_x << " resy: " << resolution_y << " resz: "
    << resolution_z;
    dx = bif3D_dx;
    dy = bif3D_dy;
    dz = bif3D_dz;
    
    stateVars=new const qreal* [stateSpace.dimension];
    if( !stateVars )
    fatalError("cyclogram_3d::cyclogram_3d",
           "Can't create vector of state variables");
    for(int i=0;i<stateSpace.dimension;i++) {
    stateVars[i]=model->setLabels(stateSpace.label[i]);
                                          // get pointer to the model var.
    if( !stateVars[i] )
        fatalError("cyclogram_3d::cyclogram_3d",
               "illegal state variable specified");
    }
    limit = length / 10;		  // 10% are thrown away
}

/******************************************************************************/
/*                                                                            */
/* Class name:      cyclogram_3d                                            */
/* Member function: ~cyclogram_3d                                           */
/* Purpose:         destructor                                                */
/* Last modified:                                  */
/*                                                                            */
/******************************************************************************/

cyclogram_3d::~cyclogram_3d()
{
    if( stateVars )
    delete stateVars;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      cyclogram_3d                                            */
/* Member function: simulation                                                */
/* Purpose:         computes a map showing the orbits dependence on parameters*/
/*                  3 parameters vary, for each parameter combination the     */
/*                  resulting orbit is analysed on periodic behaviour;        */
/*                  the corresponding point in the parameter space is colored */
/*                  depending on the order of the (poss.) periodic behaviour  */
/* Last modified:                                  */
/*                                                                            */
/******************************************************************************/

void cyclogram_3d::simulation()
{
    qint64 t;				// time
    int order=0;			// order of cycle
    quint64 tDiv=(uint)(length/10);// analysis of the simulation results
					// is done every tDiv periods until
					// length is reached or a cycle has
					// been detected
    qreal dummy_x;
    qreal dummy_y;
    qreal dummy_z;
    qint64 count_x,count_y,count_z;
    char data[resolution_x][resolution_y][resolution_z];
    QFile outFile("cycle_data_3par.dat");
    if (!outFile.open(QFile::WriteOnly))
        exit(-1);
    int jobtag_dummy = 26;
    
    outFile.write((char*)&jobtag_dummy, 4);
    
    outFile.write((char*)&resolution_x, 4);
    outFile.write((char*)&resolution_y, 4);
    outFile.write((char*)&resolution_z, 4);
    outFile.write((char*)&dx, 4);
    outFile.write((char*)&dy, 4);
    outFile.write((char*)&dz, 4);

    outFile.write((char*)&xmin, 8);
    outFile.write((char*)&xmax, 8);
    outFile.write((char*)&ymin, 8);
    outFile.write((char*)&ymax, 8);
    outFile.write((char*)&zmin, 8);
    outFile.write((char*)&zmax, 8);
    
    QTextStream stream(&outFile);

    stream << xLabel;
    stream << yLabel;
    stream << zLabel;
    
    for(dummy_x=xmin, count_x=0;count_x<resolution_x; dummy_x+=stepX,count_x++) {
    for(dummy_y=ymin,count_y=0;count_y<resolution_y;dummy_y+=stepY,count_y++) {
       for(dummy_z=zmin,count_z=0;count_z<resolution_z;dummy_z+=stepZ,count_z++) {
	   	    *xParam=dummy_x;
		    *yParam=dummy_y;
		    *zParam=dummy_z;
		    model->initialize();
		    for(t=0;t<length;t++) {
                model->iteration(t+1);
                if( t > limit ) {
                    if( hash->storePoint(stateVars) ) {
                        hash->resetHashTable();
                        order=-1;    // out of domain, leave it blue
        //			    log() << *(stateVars[0]) 
                        break;
                    }
                    if( !(t % tDiv) || (t==(length-1)) ) {
                        if ( (order=hash->orderOfCycle()) ) {
                            hash->resetHashTable();
                            break;	// a cycle has been detected
                                // so the analysis of this parameterset
                                // can be terqMinated and the hash table
                                // should be initialized for the next
                                // parameterset
                        }
                        else
                        hash->resetHashTable();
                            // a new analysis has to be done
                            // clean the hash table for the new
                            // simulation results
                    }
                }
            }
		    data[count_x][count_y][count_z] = char( order+1 );
		}
	}
//	if ( screenGraphics ) progress( count_x, resolution_x );
    }
    for(count_z=0;count_z<resolution_z;count_z++)
    	for(count_y=0;count_y<resolution_y;count_y++)
		for(count_x=0;count_x<resolution_x;count_x++){
            outFile.write(&data[count_x][count_y][count_z], 1);
		}
    outFile.flush();
    outFile.close();
}
    
//eof
