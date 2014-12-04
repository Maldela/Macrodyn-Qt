// File:	geometry3D.C
// Contents:	member functions of class geometry3D
// Author:	Andreas Starke
// Creation:	Mon Mar 08 09:37:04 MEZ 2004
// Modified:	

#include	"geometry3D.h"
#include	"../error.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      geometry3D                                                */
/* Member function: geometry3D                                                */
/* Purpose:         constructor                                               */
/* Modified:        			                                      */
/* Last modified:   			                                      */
/*                                                                            */
/******************************************************************************/

geometry3D::geometry3D(baseModel* const bMod,const xyRange& axes,
                MacrodynGraphicsItem* const graph)
          :job(bMod,graph), xmin(axes.min[0]), xmax(axes.max[0]),
	  ymin(axes.min[1]), ymax(axes.max[1]), zmin(axes.min[2]),
	  zmax(axes.max[2])
{
    uint entries = (uint) (length < 103409 ? (length/2+1) : 103409);
    uint resX;
    uint resY;

	resX=axes.res[0];
	resY=axes.res[1];
    
    strcpy(xLabel,axes.label[0].toLatin1().data());
    xParam=model->setLabels(xLabel);
    if( !xParam )
	fatalError("geometry3D::geometry3D  Can not find x label ",xLabel);

    strcpy(yLabel,axes.label[1].toLatin1().data());
    yParam=model->setLabels(yLabel);
    if( !yParam )
	fatalError("geometry3D::geometry3D  Can not find y label ",yLabel);

    strcpy(zLabel,axes.label[2].toLatin1().data());
	zParam=model->setLabels(zLabel);
	if ( !zParam )
		fatalError("geometry3D::geometry3D  Can not find z label",
		zLabel);

    hash = new hashTable(entries,axes);
    if( !hash )
	fatalError("geometry3D::geometry3D","Can't allocate hash table");
}

/******************************************************************************/
/*                                                                            */
/* Class name:      geometry3D                                                */
/* Member function: ~geometry3D                                               */
/* Purpose:         destructor                                                */
/* Modified:        			                                      */
/* Last modified:   			                                      */
/*                                                                            */
/******************************************************************************/

geometry3D::~geometry3D()
{
    if( hash )
	delete hash;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      geometry3D                                                */
/* Member function: inRange                                                   */
/* Purpose:         determine wether the simulation results lay in the section*/
/*                  under consideration or not                                */
/* Modified:        			                                      */
/* Last modified:   			                                      */
/*                                                                            */
/******************************************************************************/

uint geometry3D::inRange(const qreal&x,const qreal& y, const qreal& z)
{
    return( ((x >= xmin)&&(x <= xmax)&&(y >= ymin)&&(y <= ymax)&&(z >= zmin)
    &&(z<=zmax)) ? 1:0 );
}

