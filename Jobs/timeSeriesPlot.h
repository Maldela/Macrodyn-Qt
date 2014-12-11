///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/timeSeriesPlot.h,v 1.1 2000/08/31 16:27:16 mmeyer Exp $
//
// Module name:		timeSeriesPlot.h
// Contents:		definition of classes timeSeriesPlot ans m_timeSeriesPlot
//
// Author:		Uli Middelberg
// Last modified:	Fri May 28 12:32:41 METDST 1999
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

#ifndef TIMESERIESPLOT_H
#define TIMESERIESPLOT_H
#include "timeSeriesJob.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		timeSeriesPlot
// Derived from:	timeSeriesJob
// Purpose:		plot a time series of a certain variable
//
// Author:		Uli Middelberg
// Last modified:	Mon Nov 10 14:23:53 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class timeSeriesPlot : public timeSeriesJob {
protected:
    QFile outFile;
    int multiplot_num;		// number of additional variables to plot
    qreal ** multiplotAdr;	// addresses of additional variables
    qreal * multiplotOld;	// to store old values
    
public:
	static int pointsize;
    
public:
    timeSeriesPlot(baseModel* const, const QString&, MacrodynGraphicsItem* const=NULL,
        const QString& = "", int const=0, char** const=NULL);
    ~timeSeriesPlot();
    void simulation();
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		m_timeSeriesPlot
// Derived from:	timeSeriesPlot
// Purpose:		plot multiple time series of a certain variable
//			with respect to multiple initial values
//
// Author:		Uli Middelberg
// Last modified:	Mon Nov 10 14:23:53 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class m_timeSeriesPlot : public timeSeriesPlot {
protected:
    int    n_i_vals;	// number of initial values
    qreal * i_vals;	// the vector of initial values
    
public:
    m_timeSeriesPlot(qreal * ivalues, baseModel* const, const QString&, MacrodynGraphicsItem* const=NULL,
             char* const=NULL);
    ~m_timeSeriesPlot( void );
    void simulation();
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		m_timeSeriesPlot_p
// Derived from:	timeSeriesPlot
// Purpose:		plot multiple time series of a certain variable
//			with respect to multiple initial values of one
//			other variable
//
// Author:		Marc Mueller (org.:Uli Middelberg)
// Last modified:	Fri May 28 12:32:41 METDST 1999
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

class m_timeSeriesPlot_v : public timeSeriesPlot {
protected:
    int    n_i_vals;	// number of initial values
    qreal * i_vals;	// the vector of initial values
    qreal * modelVar2;	// Pointer to be changed variable
    
public:
    m_timeSeriesPlot_v(const QString& ,qreal * ivalues,
                     baseModel* const, const QString&, MacrodynGraphicsItem* const=NULL,
             char* const=NULL);
    ~m_timeSeriesPlot_v( void );
    void simulation();
};

#endif
//eof
