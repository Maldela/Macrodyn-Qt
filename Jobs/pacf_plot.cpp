// File:	pacf_plot.C
// Contents:	member functions of class pacf_plot
// Author:	Andreas Starke
// Creation:	
// Modified:	
// Changes:	

#include	"pacf_plot.h"
#include 	"../logger.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      pacf_plot                                                  */
/* Member function: pacf_plot                                                  */
/* Purpose:         constructor                                               */
/* Last modified:   			                                      */
/*                                                                            */
/******************************************************************************/

pacf_plot::pacf_plot(baseModel* const bMod, const xyRange& axes, 
         MacrodynGraphicsItem* const graph, qint64 lag_1, qint64 lag_2)
          :Job(bMod,graph), l_min(lag_1), l_max(lag_2)
{
    xmax = axes.max.at(0);
    xmin = axes.min.at(0);
    ymin = axes.min.at(1);
    ymax = axes.max.at(1);
    length=qint64(xmax);			// last iteration to be analysed
    limit=qint64(xmin);				// first iteration to be analysed
    how_many = qint64(xmax-xmin);
	ts_data = new qreal[how_many];
	if ( !ts_data ) fatalError("pacf_plot::pacf_plot","can't create data array!");
	mean_x = 0;
	acv_0 = 0;
    yParam = model->setLabels( axes.label[1] );
	if ( !yParam ) fatalError("pacf_plot::pacf_plot","can't find ylabel!");
    v_correlations = new matrix_neu( l_max+2, 1 );
	if ( !v_correlations ) fatalError("pacf_plot::pacf_plot","can't allocate correlation vector!");
    v_YW_estimators = new matrix_neu( l_max+2, 1 );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      pacf_plot                                                 */
/* Member function: 		                                              */
/* Purpose:         				                              */
/* Last modified:   				                              */
/*                                                                            */
/******************************************************************************/

void pacf_plot::drawBox(qreal lo_x, qreal lo_y, qreal ru_x, qreal ru_y, int color)
{
    qreal draw_step = double((l_max - l_min))/double(500);
	if ( screenGraphics ){
		for (qreal dummy = lo_x; dummy<= ru_x; dummy+=draw_step){
			screenGraphics->setLine(dummy, ru_y, dummy, lo_y, color);
		}
    }
}


/******************************************************************************/
/*                                                                            */
/* Class name:      pacf_plot                                                 */
/* Member function: simulation                                                */
/* Purpose:         computes and draws a correlogram                          */
/* Last modified:   				                              */
/*                                                                            */
/******************************************************************************/

void pacf_plot::simulation()
{
    qint64 t,i,j,l;
	qreal low_bound, high_bound;
	// computing boundaries
	low_bound = -(2*sqrt(double(1)/double(how_many)));
	high_bound = 2*sqrt(double(1)/double(how_many));
	model->initialize();
    if( screenGraphics )
    {
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
		ts_data[t-limit]=*yParam;
	}
	// computing empirical mean slow but more accurate
	for( j=0; j<how_many; j++ ){
		mean_x += ts_data[j] / how_many;
	}
	// computing covariance for lag zero
	for( j=0; j<how_many; j++){
		acv_0 += (ts_data[j]-mean_x)*(ts_data[j]-mean_x)/how_many;
	}
    // computing autocorrelations for all lags ymin tqMaxmax
	qreal acv;
	qreal acf;
    for( i=0;i<=l_max+1;i++ ){
		acv = 0;
		for( j=0; j<how_many-i; j++ ){
			acv += (ts_data[j]-mean_x)*(ts_data[j+i]-mean_x)/how_many;
		}
		acf = acv / acv_0;
		(*v_correlations)(i,0) = acf;		
/*		if( screenGraphics )
			screenGraphics->setLine(i,0,i,acf,9);
		if( printDev )
			printDev->setLine(i,0,i,acf,9);*/
	}
//	log() << "vector of correlations: " << (*v_correlations) 
	// drawing zero pacf value

	// looping in for partial correlations
//	int k;
	
	// alter Ansatz funktioniert zwar ist aber sehr langsam...
/*	for ( l=1;l<=l_max;l++){
        v_int_correlations = new matrix_neu( l, 1 );
        if ( !v_int_correlations ) fatalError("pacf_plot::pacf_plot","can't allocate int correlation vector!");
		m_correlations = new matrix_neu( l, l );
		if ( !m_correlations ) fatalError("pacf_plot::pacf_plot","can't allocate correlation matrix!");	
		m_inv_correlations = new matrix_neu( l,l );
		if ( !m_inv_correlations ) fatalError("pacf_plot::pacf_plot","can't allocate inverse correlation matrix!");		
		v_YW_estimators = new matrix_neu( l, 1 );
		if ( !v_YW_estimators ) fatalError("pacf_plot::pacf_plot","can't allocate Yule-Walker vector!");	
	
		for ( i=0;i<l;i++){
			for ( j=0;j<l;j++ ){
				k = i - j;
				if ( k < 0 ) k = -k;
				(*m_correlations)(i,j) = (*v_correlations)(k,0);
			}
		}
	
//		log() << (*m_correlations);
		
		// solving for Yule-Walker
		(*m_inv_correlations) = m_correlations->inverse();
		for ( i=1;i<=l;i++)
            (*v_int_correlations)(i-1,0) = (*v_correlations)(i,0);
        (*v_YW_estimators) = (*m_inv_correlations) * (*v_int_correlations);
	
//		log() << (*m_inv_correlations);
//		log() << (*v_int_correlations);
        log() << (*v_YW_estimators);
		
        log() << l << ": " << (*v_YW_estimators)(l-1,0) 
		// drawing value
		if ( screenGraphics )
			screenGraphics->setLine(l,0,l,(*v_YW_estimators)(l-1,0),9);
		
        delete v_int_correlations;
		delete m_correlations;
		delete m_inv_correlations;
		delete v_YW_estimators;
	}*/
	/*
	// neuer Ansatz, kommt ohne Inversion der Matrizen aus... ist aber auch sehr langsam
	
	qreal det1, det2, coef;
	
    for ( l=l_min;Lmaxmax;l++){
		m_kappa1 = new matrix_neu(l+1,l+1);
		m_kappa2 = new matrix_neu(l+1,l+1);
		// arranging kappa1
		for ( i=0; i<=l; i++){
			for ( j=0; j<=l; j++) {
				k = i-j;
				if ( k < 0 ) k=-k;
				(*m_kappa1)(i,j)=(*v_correlations)(k,0);
			}
		}
		// arranging kappa2
		for ( i=0; i<=l; i++){
			for ( j=0; j<=l; j++){
				k = 1+j-i;
				if ( k < 0 ) k=-k;
				(*m_kappa2)(i,j)=(*v_correlations)(k,0);
			}
		}
		
//		log() << "kappa1:" << "\n" << (*m_kappa1);
//		log() << "kappa2:" << "\n" << (*m_kappa2);
		
        det1 = m_kappa1->deterqMinant();
        det2 = m_kappa2->deterqMinant();
		
		coef = pow( double(-1), double(l)) * det2 / det1;
		
//		log() << "value: " << coef 
		
		if ( screenGraphics )
			screenGraphics->setLine(l+1,0,l+1,coef,9);
		if ( printDev )
			printDev->setLine(l+1,0,l+1,coef,9);
		
		delete m_kappa1;
		delete m_kappa2;
	}
	*/
	// neuer Ansatz bootstrapping - sollte schnell sein
	qreal Q_p;
	qreal delta;
	(*v_YW_estimators)(0,0) = 1;
	(*v_YW_estimators)(1,0) = (*v_correlations)(1,0);
	Q_p = 1 - (((*v_correlations)(1,0))*((*v_correlations)(1,0)));

	drawBox(-0.4,1,0.4,0,9);
	drawBox(0.6,(*v_YW_estimators)(1,0),1.4,0,9);
    log() << "vector of partial correlations:";
    log() << "lag " << 0 << ":\t" << (*v_YW_estimators)(0,0);
    log() << "lag " << 1 << ":\t" << (*v_YW_estimators)(1,0);
	
	matrix_neu* dummy_YW;
    for ( l=2; l<=l_max; l++){
		delta = (*v_correlations)(l,0);
		for ( i=1; i<l; i++){
			delta -= ((*v_YW_estimators)(i,0)) * ((*v_correlations)(l-i,0));
		}
		(*v_YW_estimators)(l,0) = delta / Q_p;
		dummy_YW = new matrix_neu(l,1);
		for ( i=1; i<l; i++){
			(*dummy_YW)(i,0) = ((*v_YW_estimators)(i,0)) - (((*v_YW_estimators)(l,0)) * ((*v_YW_estimators)(l-i,0))); 
		}
		for ( i=1; i<l; i++){
			(*v_YW_estimators)(i,0) = (*dummy_YW)(i,0);
		}
		Q_p = Q_p*(1-(((*v_YW_estimators)(l,0))*((*v_YW_estimators)(l,0))));
		drawBox(l-0.4, (*v_YW_estimators)(l,0), l+0.4, 0, 9);

        log() << "lag " << l << ":\t" << (*v_YW_estimators)(l,0);
		delete dummy_YW;
	}
	
	delete v_YW_estimators;
	delete []ts_data;
	delete v_correlations;
}
