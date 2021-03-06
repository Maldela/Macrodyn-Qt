// File:	acf_plot.h
// Contents:	definition of class acf_plot
// Author:	Andreas Starke
// Creation:	15.03.2004
// Modified:	
// Changes:	

#ifndef ACFPLOT_H
#define ACFPLOT_H
#include	"job.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    acf_plot                                                    */
/* Derived from:  geometric Job                                               */
/* Purpose:       computes and draws a 2D acf diagram 			      */
/*                							      */
/* Author:        Andreas Starke                                              */
/* Last modified: 	                                                      */
/*                                                                            */
/******************************************************************************/

class acf_plot : public Job {
  protected:
    qreal** ts_data;
    qreal* mean_x;
    xyRange stateSpace;
    QList<qreal *> stateVars;         // pointer to the state variables

    qint64 how_many,l_min,l_max;
    qreal xmax,xmin;
    qreal ymax,ymin;
    qreal* yParam;
    qreal acv_0;
    void drawBox(qreal lo_x, qreal lo_y, qreal ru_x, qreal ru_y, int color);
    
  public:
    acf_plot(baseModel* const, const xyRange&, const xyRange&, MacrodynGraphicsItem* const=NULL,
       qint64 lag_1 = 0, qint64 lag_2 = 20);
    void simulation();
};

#endif
//eof
