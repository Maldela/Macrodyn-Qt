// File:	cyclogram_3d.h
// Contents:	definition of class cyclogram_3d
// Author:	Andreas Starke
// Creation:	
// Modified:	
// Changes:	

#ifndef CYCLOGRAM_3D_H
#define CYCLOGRAM_3D_H
#include	"bif3D.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    cyclogram_3d                                              */
/* Derived from:  bif2D                                                       */
/* Purpose:       Computes a map showing the orbits dependence on parameters; */
/*                2 parameters vary, for each parameter combination the       */
/*                resulting orbit is analysed on periodic behaviour;          */
/*                the corresponding point in the parameter space is colored   */
/*                depending on the order of the (possibly) periodic behaviour */
/* Author:        Andreas Starke                                              */
/* Last modified:                                                  */
/*                                                                            */
/******************************************************************************/

class cyclogram_3d : public bif3D_2par {
  protected:
    xyRange stateSpace;
    qreal stepY;                      // stepsize for the second parameter
    qreal stepX;
    qreal stepZ;
    long int resolution_x;
    long int resolution_y;
    long int resolution_z;
    long int dx,dy,dz;
    
    const qreal **stateVars;          // pointer to the state variables
    
    
    
  public:
    cyclogram_3d(baseModel* const, const xyRange&, const xyRange&,
           MacrodynGraphicsItem* const=NULL,printer* const=NULL, const long=1,
		   const long=1, const long=1);
    virtual ~cyclogram_3d();
    void simulation();
};    

#endif
//eof
