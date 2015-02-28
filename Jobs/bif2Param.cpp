// File:	bif2Param.C
// Contents:	member functions of class bif2Param
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:25:09 MEZ 1996
// Changes:	

#include	"bif2Param.h"
#include	"../logger.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      bif2Param                                                 */
/* Member function: bif2Param                                                 */
/* Purpose:         constructor                                               */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

bif2Param::bif2Param(baseModel* const bMod, const xyRange& axes,
              MacrodynGraphicsItem* const graph)
          :bif2D(bMod,axes,graph)
{
    zmin = axes.min[2];			// save the bounderies for the second
    zmax = axes.max[2];			// bifurcation parameter
    zLabel = axes.label[2];
    zParam = model->setLabels(zLabel);	// get the pointer to the second
					// bifurcation parameter
    if( !zParam )
	fatalError("bif2Param::bif2Param","Undefined label");
    stepZ=(zmax-zmin)/axes.res[2];
}

/******************************************************************************/
/*                                                                            */
/* Class name:      bif2Param                                                 */
/* Member function: simulation                                                */
/* Purpose:         computes 2D bifurcation diagrams where 2 parameters vary  */
/*                  in a nested loop; the resulting pictures are shown one    */
/*                  after the other                                           */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void bif2Param::simulation()
{
    qint64 t;
    qreal dummy_x;
    qreal dummy_z;

    for(dummy_z=zmin;dummy_z<=zmax; dummy_z+=stepZ)
    {
        for(dummy_x=xmin;dummy_x<=xmax; dummy_x+=stepX)
        {
            *xParam = dummy_x;
            *zParam = dummy_z;
            model->initialize();
            screenGraphics->clearColumn(dummy_x);   // clear column of the
//                                                    // old picture
//          log()<<"1";
          for(t=0;t<length;t++)
          {
//              log()<<"2";
                model->iteration(t+1);
//              log()<<"3";
                if( t > limit && inRange(dummy_x,*yParam) )
              {
                    if( screenGraphics )
                        screenGraphics->setPoint(dummy_x,*yParam,9);

                }
            }
        }
    }
}

//eof
