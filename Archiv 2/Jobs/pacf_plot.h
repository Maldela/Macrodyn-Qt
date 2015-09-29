// File:	pacf_plot.h
// Contents:	definition of class pacf_plot
// Author:	Andreas Starke
// Creation:	15.03.2004
// Modified:	
// Changes:	

#ifndef PACFPLOT_H
#define PACFPLOT_H
#include	"job.h"
#include 	"../matrix_neu.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    pacf_plot                                                    */
/* Derived from:  geometric Job                                               */
/* Purpose:       computes and draws a 2D acf diagram 			      */
/*                							      */
/* Author:        Andreas Starke                                              */
/* Last modified: 	                                                      */
/*                                                                            */
/******************************************************************************/

class pacf_plot : public Job {
  protected:
    qreal* ts_data;
    qint64 how_many,l_min,l_max;
    qreal xmax,xmin;
    qreal ymax,ymin;
    qreal* yParam;
    qreal mean_x;
    qreal acv_0;
    matrix_neu* v_correlations;
// matrix_neu* v_int_correlations;
// matrix_neu* m_correlations;    
// matrix_neu* m_inv_correlations;
// matrix_neu* m_kappa1;
// matrix_neu* m_kappa2;
    matrix_neu* v_YW_estimators;
    void drawBox(qreal lo_x, qreal lo_y, qreal ru_x, qreal ru_y, int color);
    
  public:
    pacf_plot(baseModel* const, const xyRange&, MacrodynGraphicsItem* const=NULL,
      qint64 lag_1 = 0, qint64 lag_2 = 20);
    void simulation();
};

#endif
//eof
