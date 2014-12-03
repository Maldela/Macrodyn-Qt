////////////////////////////////////////////////////////////////////////////////
// File:	bifnParam.C						      //
// Contents:	member functions of class bifnParam			      //
// Author:	Michael Meyer						      //
// Creation:								      //
// Modified:	Mon Apr 10 12:42:40 METDST 2000				      //
// Changes:	Mon Apr 10 12:42:43 METDST 2000				      //
////////////////////////////////////////////////////////////////////////////////

#include	"bifnParam.h"
#include	"../error.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      bifnParam                                                 */
/* Member function: bifnParam                                                 */
/* Purpose:         constructor                                               */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/

bifnParam::bifnParam(baseModel* const bMod,
					const xyRange& axes, 
//			        const xyRange& stateSpaceLim,  
			        const xyRange& xDef,  
                    MacrodynGraphicsItem* const graph,
			        printer* const outDev)

//    :parameterSpace(bMod,axes,stateSpaceLim,outDev),
    :bif2D(bMod,axes,graph,outDev),

    effectiveX(xDef)
{

    length=model->getLength();
    limit=(long)(0.99*length);	

    xVars=new qreal* [effectiveX.dimension];
    if( !xVars )
	fatalError("bifnParam::bifnParam",
		   "Can't create vector of x-variables");
    for(short i=0;i<effectiveX.dimension;i++) {
	xVars[i]=model->setLabels(effectiveX.label[i]);
                                          // get pointer to the model var.
	if( !xVars[i] )
	    fatalError("bifnParam::bifnParam",
		       "illegal x parameter specified");
    stepX=(xmax-xmin) / (axes.res[0]-1);

    }
}

/******************************************************************************/
/*                                                                            */
/* Class name:      bifnParam                                                 */
/* Member function: ~bifnParam                                                */
/* Purpose:         destructor                                                */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/

bifnParam::~bifnParam()
{
    if( xVars )
	delete xVars;
}

/******************************************************************************/
/*                                                                            */
/* Class name:       bifnParam                                                */
/* Member function:  setXParams                                               */
/* Purpose:          linear update of the x-parameters                        */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/

void bifnParam::setXParams(const qreal& newX)
{
    static qreal divisor=xmax-xmin;
    for(short i=0;i<effectiveX.dimension;i++)
	*xVars[i]=effectiveX.min[i]+(newX-xmin)/divisor*
	    (effectiveX.max[i]-effectiveX.min[i]);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      bifnParam                                                 */
/* Member function: simulation                                                */
/* Purpose:         computes a bifurcation dependence on n parameters         */
/*  									      */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/

void bifnParam::simulation()
{
    long t;				// time

    qreal x=0;

    for(x=xmin;x<=xmax; x+=stepX) {
	setXParams(x);
	    model->initialize();
	    for(t=0;t<length;t++) {
		model->iteration(t+1);
		//}
	    if( t >= limit && inRange(x,*yParam) ) {
                                       // only points inside the state space
				       // section under consideration are
				       // analysed

	    if( screenGraphics ) 
		screenGraphics->setPoint(x,*yParam,9);
	    if( printDev )
		printDev->setBits(x,*yParam,9);
	 }
      }
   }
}
//eof
