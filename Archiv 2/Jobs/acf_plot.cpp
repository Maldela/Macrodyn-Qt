// File:	acf_plot.C
// Contents:	member functions of class acf_plot
// Author:	Andreas Starke
// Creation:	
// Modified:	
// Changes:	

#include	"acf_plot.h"
#include 	"../logger.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      acf_plot                                                  */
/* Member function: acf_plot                                                  */
/* Purpose:         constructor                                               */
/* Last modified:   			                                      */
/*                                                                            */
/******************************************************************************/

acf_plot::acf_plot(baseModel* const bMod, const xyRange& axes, const xyRange& stateSpaceLim,
         MacrodynGraphicsItem* const graph, qint64 lag_1, qint64 lag_2)
          :Job(bMod,graph), l_min(lag_1), l_max(lag_2), stateSpace(stateSpaceLim)
{
    xmax = axes.max[0];
    xmin = axes.min[0];
    ymin = axes.min[1];
    ymax = axes.max[1];
    length=qint64(xmax);			// last iteration to be analysed
    limit=qint64(xmin);				// first iteration to be analysed
    how_many = qint64(xmax-xmin);
//	ts_data = new qreal[how_many];
//	if ( !ts_data ) fatalError("acf_plot::acf_plot","can't create data array!");
//	mean_x = 0;
//	acv_0 = 0;

    stateVars.clear();
    log()<<"stateSpace.dimension = "<<stateSpace.dimension;
    for (int i=0;i<stateSpace.dimension;i++) {
    stateVars << model->setLabels(stateSpace.label[i]);
    // get pointer to the model var.
    if( !stateVars[i] )
    fatalError("parameterSpace::parameterSpace",
    "illegal state variable specified");
    }

    ts_data = new qreal*[stateVars.length()];
    for(int i = 0; i < stateVars.length(); ++i)
        ts_data[i] = new qreal[length-limit];
    mean_x = new qreal[stateVars.length()];
    log()<<"stateVars.length() = "<<stateVars.length();

    yParam = model->setLabels( axes.label[1] );
	if ( !yParam ) fatalError("acf_plot::acf_plot","can't find ylabel!");
}

/******************************************************************************/
/*                                                                            */
/* Class name:      acf_plot                                                 */
/* Member function: 		                                              */
/* Purpose:         				                              */
/* Last modified:   				                              */
/*                                                                            */
/******************************************************************************/

void acf_plot::drawBox(qreal lo_x, qreal lo_y, qreal ru_x, qreal ru_y, int color)
{
    qreal draw_step = double(l_max - l_min)/double(500);
    if ( screenGraphics ){
        for (qreal dummy = lo_x; dummy<= ru_x; dummy+=draw_step){
            screenGraphics->setLine(dummy, ru_y, dummy, lo_y, color);
        }
    }
}


/******************************************************************************/
/*                                                                            */
/* Class name:      acf_plot                                                  */
/* Member function: simulation                                                */
/* Purpose:         computes and draws a correlogram                          */
/* Last modified:   				                              */
/*                                                                            */
/******************************************************************************/

void acf_plot::simulation()
{
    qint64 t,i,j;
	qreal low_bound, high_bound;
	model->initialize();
	// drawing zeroline and confidential bands for
	// white noise... 95%
	low_bound = double(-1/how_many) - (1.96 / sqrt(double(how_many)));
	high_bound = double(-1/how_many) + (1.96 / sqrt(double(how_many)));
    if( screenGraphics ){
        screenGraphics->setLine(l_min,0,l_max,0,24);
        screenGraphics->setLine(l_min,low_bound,l_max,low_bound,6);
        screenGraphics->setLine(l_min,high_bound,l_max,high_bound,6);
    }


    for(t=0;t<limit;t++) {
        model->iteration(t+1);
    }
    // collecting data
    for(t=limit;t<length;t++){
        model->iteration(t+1);
        for(int varNr = 0; varNr < stateVars.length(); ++varNr){
            ts_data[varNr][t-limit]=*stateVars[varNr];
        }
    }

    log()<<"ts_data[0][length-limit-1] = "<<ts_data[0][length-limit-1];
    // computing empirical mean slow but more accurate
    for(int varNr=0; varNr<stateVars.length(); varNr++){
        mean_x[varNr]=0;
        for( int j=0; j<(length-limit); j++ ){
            mean_x[varNr] += ts_data[varNr][j] / (length-limit);
        }
    }
    //log()<<(length-limit-1000)<<"  "<<ts_data[(length-limit)-1000];
    log()<<"mean_x[0] = "<<mean_x[0];
    log()<<"mean_x[1] = "<<mean_x[1];
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
    for(int i=l_min;i<l_max;i++ ){
        acv = 0;
        for(int varNr=0; varNr<stateVars.length(); varNr++){
            for( int j=0; j<(length-limit)-i; j++ ){
                acv += (ts_data[varNr][j]-mean_x[varNr])*(ts_data[varNr][j+i]-mean_x[varNr])/(length-limit-i);
            }
        }

        //log() << "acv = " << acv;
        //log() << "acv_0 = " << acv_0;

        acf = acv / acv_0;

        drawBox(i-0.4,acf,i+0.4,0,9);
        //log() << "lag " << i << ":\t" << acf;
    }






//	for(t=0;t<limit;t++) {
//	    model->iteration(t+1);
//	}
//	// collecting data
//	for(t=limit;t<length;t++){
//		model->iteration(t+1);
//		ts_data[t-limit]=*yParam;
//	}
//	// computing empirical mean slow but more accurate
//	for( j=0; j<how_many; j++ ){
//		mean_x += ts_data[j] / how_many;
//	}
//	// computing covariance for lag zero
//	for( j=0; j<how_many; j++){
//		acv_0 += (ts_data[j]-mean_x)*(ts_data[j]-mean_x)/how_many;
//	}
//    // computing autocorrelations for all lags ymin tqMaxmax
//	qreal acv;
//	qreal acf;
//    log() << "vector of correlations:\n";
//    for( i=l_min;i<l_max;i++ ){
//		acv = 0;
//		for( j=0; j<how_many-i; j++ ){
//            acv += (ts_data[j]-mean_x)*(ts_data[j+i]-mean_x)/(how_many-i);
//		}
//		acf = acv / acv_0;
//		drawBox(i-0.4,acf,i+0.4,0,9);
//        //log() << "lag " << i << ":\t" << acf;
//	}
    delete []ts_data;
}
