// File:	parameterSpace.C
// Contents:	member functions of class parameterSpace
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:25:09 MEZ 1996
// Changes:	

#include	"parameterSpace.h"
#include	"../logger.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      parameterSpace                                            */
/* Member function: parameterSpace                                            */
/* Purpose:         constructor                                               */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

parameterSpace::parameterSpace(baseModel* const bMod, const xyRange& axes, 
                   const xyRange& stateSpaceLim,
                   MacrodynGraphicsItem* const graph, int maxLag, double autocorrelationBound)
               :bif2D(bMod,axes,graph), stateSpace(stateSpaceLim)
{
    hash->resetDomain(stateSpace);        // save bounderies of the state
					  // space section
    stepY=(ymax-ymin)/(axes.res[1]-1);
    log()<<"stepX = "<<stepX;
    log()<<"stepY = "<<stepY;
//    stepY=(ymax-ymin)/50.0;
//    stepX=(xmax-xmin)/50.0;
    stateVars.clear();
    log()<<"stateSpace.dimension = "<<stateSpace.dimension;
    for (int i=0;i<stateSpace.dimension;i++) {
    stateVars << model->setLabels(stateSpace.label[i]);
                                          // get pointer to the model var.
	if( !stateVars[i] )
	    fatalError("parameterSpace::parameterSpace",
		       "illegal state variable specified");
    }
    limit = length / 10;		  // 10% are thrown away
    ts_data = new qreal*[stateVars.length()];
    for(int i = 0; i < stateVars.length(); ++i)
        ts_data[i] = new qreal[length-limit];
    mean_x = new qreal[stateVars.length()];
    log()<<"stateVars.length() = "<<stateVars.length();
    m_maxLag = maxLag;
    m_autocorrelationBound = autocorrelationBound;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      parameterSpace                                            */
/* Member function: ~parameterSpace                                           */
/* Purpose:         destructor                                                */
/* Last modified:   15.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

parameterSpace::~parameterSpace()
{
}

/******************************************************************************/
/*                                                                            */
/* Class name:      parameterSpace                                            */
/* Member function: simulation                                                */
/* Purpose:         computes a map showing the orbits dependence on parameters*/
/*                  2 parameters vary, for each parameter combination the     */
/*                  resulting orbit is analysed on periodic behaviour;        */
/*                  the corresponding point in the parameter space is colored */
/*                  depending on the order of the (poss.) periodic behaviour  */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void parameterSpace::simulation()
{
    qint64 t;				// time
    int order=0;			// order of cycle
    quint64 tDiv=(uint)(length/10);// analysis of the simulation results
					// is done every tDiv periods until
					// length is reached or a cycle has
                    // been detected

    for(*xParam=xmin; *xParam<=xmax; *xParam+=stepX)
    {
        for(*yParam=ymin;*yParam<=ymax;*yParam+=stepY)
        {
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
                    screenGraphics->setRect(*xParam,*yParam,stepX,stepY,QColor(255,255,0));
                else
                    screenGraphics->setRect(*xParam,*yParam,stepX,stepY,colorFromOrder(/*((*xParam-xmin)/(xmax-xmin))*34*/order));
                //screenGraphics->setRect(*xParam,*yParam,stepX,stepY,QColor((1234*(order+1))%255,(12345*(order+1))%255,(123456*(order+1))%255));
        }
    }
}

QColor parameterSpace::colorFromOrder(int order) {
    switch (order) {
    case -1: return QColor(0,0,0);
    case 0: return QColor(255,255,255);
    case 1: return QColor(255,0,255);
    case 2: return QColor(0,255,255);
    case 3: return QColor(211,186,211);
    case 4: return QColor(0,255,0);
    case 5: return QColor(255,0,0);
    case 6: return QColor(251,174,190);
    case 7: return QColor(162,158,158);
    case 8: return QColor(0,0,255);
    case 9: return QColor(251,121,20);
    case 10: return QColor(243,219,170);
    case 11: return QColor(121,219,101);
    case 12: return QColor(235,52,138);
    case 13: return QColor(113,142,207);
    case 14: return QColor(247,101,65);
    case 15: return QColor(37,56,60);
    case 16: return QColor(37,65,23);
    case 17: return QColor(185,59,143);
    case 18: return QColor(255,56,23);
    case 19: return QColor(67,198,219);
    case 20: return QColor(48,103,84);
    case 21: return QColor(72,99,160);
    case 22: return QColor(129,5,65);
    case 23: return QColor(255,40,91);
    case 24: return QColor(216,175,121);
    case 25: return QColor(125,27,126);
    case 26: return QColor(67,183,186);
    case 27: return QColor(102,152,255);
    case 28: return QColor(212,160,23);
    case 29: return QColor(21,27,84);
    case 30: return QColor(152,5,23);
    }
    return QColor(152,5,23);
}
    
//eof
