// File:	nParameterAnalysis.C
// Contents:	member functions of class nParameterAnalysis
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:25:09 MEZ 1996
// Changes:	

#include	"nParameterAnalysis.h"
#include	"../logger.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      nParameterAnalysis                                        */
/* Member function: nParameterAnalysis                                        */
/* Purpose:         constructor                                               */
/* Last modified:   07.03.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

nParameterAnalysis::nParameterAnalysis(baseModel* const bMod,
				       const xyRange& axes, 
				       const xyRange& stateSpaceLim, 
				       const xyRange& xDef, 
				       const xyRange& yDef, 
                       MacrodynGraphicsItem* const graph, int maxLag, double autocorrelationBound)
    :parameterSpace(bMod,axes,stateSpaceLim,graph),
     effectiveX(xDef),effectiveY(yDef)
{
    log()<<"effectiveX.dimension = "<<effectiveX.dimension;
    log()<<"xVars.length() = "<<xVars.length();
    xVars.clear();
    for(int i=0;i<effectiveX.dimension;i++) {
    xVars << model->setLabels(effectiveX.label[i]);
                                          // get pointer to the model var.
    //log()<<"effectiveX.label[i] = "<<effectiveX.label[i];
    //log()<<"xVars.length() = "<<xVars.length();
//	if( !xVars[i] )
//	    fatalError("nParameterAnalysis::nParameterAnalysis",
//		       "illegal x parameter specified");
    }
    yVars.clear();
    for(int i=0;i<effectiveY.dimension;i++) {
    yVars << model->setLabels(effectiveY.label[i]);
                                          // get pointer to the model var.
	if( !yVars[i] )
	    fatalError("nParameterAnalysis::nParameterAnalysis",
		       "illegal y parameter specified");
    }
    m_maxLag = maxLag;
    m_autocorrelationBound = autocorrelationBound;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      nParameterAnalysis                                        */
/* Member function: ~nParameterAnalysis                                       */
/* Purpose:         destructor                                                */
/* Last modified:   03.03.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

nParameterAnalysis::~nParameterAnalysis()
{
}

/******************************************************************************/
/*                                                                            */
/* Class name:      nParameterAnalysis                                        */
/* Member function: setXParams                                                */
/* Purpose:         linear update of the x-parameters                         */
/* Last modified:   03.03.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void nParameterAnalysis::setXParams(const qreal& newX)
{
    static qreal divisor=xmax-xmin;
    for(short i=0;i<effectiveX.dimension;i++){
    *xVars[i]=effectiveX.min[i]+(newX-xmin)/divisor*
        (effectiveX.max[i]-effectiveX.min[i]);
    //log()<<"xVars[i] = "<<xVars[i];
    }
}
/******************************************************************************/
/*                                                                            */
/* Class name:      nParameterAnalysis                                        */
/* Member function: setYParams                                                */
/* Purpose:         linear update of the x-parameters                         */
/* Last modified:   03.03.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void nParameterAnalysis::setYParams(const qreal& newY)
{
    static qreal divisor=ymax-ymin;

    for(short i=0;i<effectiveY.dimension;i++){
    *yVars[i]=effectiveY.min[i]+(newY-ymin)/divisor*
        (effectiveY.max[i]-effectiveY.min[i]);

    //log()<<"yVars[i] = "<<yVars[i];
    }
}

/******************************************************************************/
/*                                                                            */
/* Class name:      nParameterAnalysis                                        */
/* Member function: simulation                                                */
/* Purpose:         computes a map showing the orbits dependence on parameters*/
/*                  2 parameters vary, for each parameter combination the     */
/*                  resulting orbit is analysed on periodic behaviour;        */
/*                  the corresponding point in the parameter space is colored */
/*                  depending on the order of the (poss.) periodic behaviour  */
/* Last modified:   07.03.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void nParameterAnalysis::simulation()
{
    qint64 t;				// time
    int order=0;			// order of cycle
    uint tDiv=(uint)(length/10);  // analysis of the simulation results
					// is done every tDiv periods until
					// length is reached or a cycle has
					// been detected
    qreal x=0;
    qreal y=0;
    log()<<"xmin"<<xmin;
    log()<<"xmax"<<xmax;
    log()<<"stepX"<<stepX;
    log()<<"ymin"<<ymin;
    log()<<"ymax"<<ymax;
    log()<<"stepY"<<stepY;
    for(x=xmin;x<=xmax; x+=stepX) {
	setXParams(x);
    for(y=ymin;y<=ymax;y+=stepY) {
	    setYParams(y);
//        log()<<"x = "<<x;
//        log()<<"y = \n"<<y;
        model->initialize();
        for(t=0;t<length;t++)
        {
            model->iteration(t+1);
            if( t>=limit ) {
                for(int varNr = 0; varNr < stateVars.length(); ++varNr)
                    ts_data[varNr][t-limit]=*stateVars[varNr];
            }
            if( t > limit ) {
                if( hash->storePoint(stateVars))
                {
                    hash->resetHashTable();
                    order=-1;    // out of domain, leave it blue
                    break;
                }
                if( !(t % tDiv) || (t==(length-1)) )
                {
                    if ( (order=hash->orderOfCycle()) )
                    {
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

        qreal acfMax=0;
        if(order==0){
            // computing empirical mean slow but more accurate
            for(int varNr=0; varNr<stateVars.length(); varNr++){
                mean_x[varNr]=0;
                for( int j=0; j<(length-limit); j++ ){
                    mean_x[varNr] += ts_data[varNr][j] / (length-limit);
                }
            }
            //log()<<(length-limit-1000)<<"  "<<ts_data[(length-limit)-1000];
            //log()<<"mean_x[0] = "<<mean_x[0];
            //log()<<"mean_x[1] = "<<mean_x[1];
            // computing covariance for lag zero
            qreal acv_0=0;
            for(int varNr=0; varNr<stateVars.length(); varNr++){
                for( int j=0; j<(length-limit); j++){
                    acv_0 += (ts_data[varNr][j]-mean_x[varNr])*(ts_data[varNr][j]-mean_x[varNr])/(length-limit);
                }
            }
            // computing autocorrelations for all lags ymin tqMaxmax
            qreal acv;
            qreal acf;
            for( int i=1;i<m_maxLag;i++ ){
                acv = 0;
                for(int varNr=0; varNr<stateVars.length(); varNr++){
                    for( int j=0; j<(length-limit)-i; j++ ){
                        acv += (ts_data[varNr][j]-mean_x[varNr])*(ts_data[varNr][j+i]-mean_x[varNr])/(length-limit-i);
                    }
                }

                //log() << "acv = " << acv;
                //log() << "acv_0 = " << acv_0;

                acf = acv / acv_0;
                if(acf>acfMax)
                    acfMax=acf;
                //log() << "lag " << i << ":\t" << acf;
            }
            //log() << "acfMax = " << acfMax;
        }

        if( screenGraphics )
            if(fabsf(acfMax-1)<(1.0-m_autocorrelationBound))
                screenGraphics->setRect(x,y,stepX,stepY,QColor(255,255,0));
            else
                screenGraphics->setRect(x,y,stepX,stepY,colorFromOrder(/*((*xParam-xmin)/(xmax-xmin))*34*/order));
            //screenGraphics->setRect(*xParam,*yParam,stepX,stepY,QColor((1234*(order+1))%255,(12345*(order+1))%255,(123456*(order+1))%255));
	}
    }
}


//eof
