// File:	bif3D.h
// Contents:	definition of class bif3D
// Author:	Andreas Starke
// Creation:	Mon Mar 08 09:16:00 MEZ 2004
// Modified:	
// Changes:	

#ifndef BIF3D_H
#define BIF3D_H
#include	"geometry3D.h"
#include	"histo.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    bif3D                                                       */
/* Derived from:  geometric job                                               */
/* Purpose:       computes a 3D bifurcation diagram (one parameter varies,    */
/*                the simulation results are projected on two state variables)*/
/* Author:        Andreas Starke                                              */
/* Last modified: 	                                                      */
/*                                                                            */
/******************************************************************************/

class bif3D_2par : public geometry3D {
  protected:
    qreal stepX;                      // stepsize on the x-axis
    qreal stepY;				// stepsize on the y-axis
    qreal stepZ;				// stepsize on the z-axis
    long int resolution_x;
    long int resolution_y;
    long int resolution_z;
    long int dx,dy,dz;
    histo_1d	h;
    void progress(long int, long int);
    
  public:
    bif3D_2par(baseModel* const, const xyRange&, MacrodynGraphicsItem* const=NULL,
      printer* const=NULL, const long=1, const long=1, const long=1);
    void setStepX(const qreal&);
    void setStepZ(const qreal&);
    void simulation();
};

class bif3D_1par : public geometry3D {
  protected:
    qreal stepX;                      // stepsize on the x-axis
    qreal stepY;				// stepsize on the y-axis
    qreal stepZ;				// stepsize on the z-axis
    long int resolution_x;
    long int resolution_y;
    long int resolution_z;
    long int dx,dy,dz;
    histo_2d	h;
    void progress(long int, long int);
    
  public:
    bif3D_1par(baseModel* const, const xyRange&, MacrodynGraphicsItem* const=NULL,
      printer* const=NULL, const long=1, const long=1, const long=1);
    void setStepX(const qreal&);
    void setStepZ(const qreal&);
    void simulation();
};
#endif
//eof
