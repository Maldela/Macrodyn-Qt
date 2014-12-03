///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/timeSeriesPlot_Loop.h,v 1.1 2001/06/22 07:26:25 mhoffman Exp $
//
// Module name:		timeSeriesPlot_Loop.h
// Contents:		definition of classes timeSeriesPlot_Loop ans m_timeSeriesPlot_Loop
//
// Author:		Uli Middelberg
// Last modified:	Fri May 28 12:32:41 METDST 1999
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

#ifndef timeSeriesPlot_Loop_H
#define timeSeriesPlot_Loop_H
#include "timeSeriesJob.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		timeSeriesPlot_Loop
// Derived from:	timeSeriesJob
// Purpose:		plot a time series of a certain variable
//
// Author:		Uli Middelberg
// Last modified:	Mon Nov 10 14:23:53 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class timeSeriesPlot_Loop : public timeSeriesJob {
protected:
    ofstream outFile;
    int multiplot_num;		// number of additional variables to plot
    qreal ** multiplotAdr;	// addresses of additional variables
    qreal * multiplotOld;	// to store old values
    int loop_size;		//
public:
    timeSeriesPlot_Loop(int, baseModel* const, char* const, MacrodynGraphicsItem* const=NULL,
        char* const=NULL, int const=0, char** const=NULL);
    ~timeSeriesPlot_Loop();
    void simulation();
};

/*
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		m_timeSeriesPlot_Loop
// Derived from:	timeSeriesPlot_Loop
// Purpose:		plot multiple time series of a certain variable
//			with respect to multiple initial values
//
// Author:		Uli Middelberg
// Last modified:	Mon Nov 10 14:23:53 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class m_timeSeriesPlot_Loop : public timeSeriesPlot_Loop {
protected:
    int    n_i_vals;	// number of initial values
    qreal * i_vals;	// the vector of initial values

public:
    m_timeSeriesPlot_Loop(qreal * ivalues,
                     baseModel* const, char* const, 
		     printer* const=NULL, char* const=NULL);
    ~m_timeSeriesPlot_Loop( void );
    void simulation();
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		m_timeSeriesPlot_Loop_p
// Derived from:	timeSeriesPlot_Loop
// Purpose:		plot multiple time series of a certain variable
//			with respect to multiple initial values of one
//			other variable
//
// Author:		Marc Mueller (org.:Uli Middelberg)
// Last modified:	Fri May 28 12:32:41 METDST 1999
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

class m_timeSeriesPlot_Loop_v : public timeSeriesPlot_Loop {
protected:
    int    n_i_vals;	// number of initial values
    qreal * i_vals;	// the vector of initial values
    qreal * modelVar2;	// Pointer to be changed variable
public:
    m_timeSeriesPlot_Loop_v( char* const,qreal * ivalues,
                     baseModel* const, char* const, 
		     printer* const=NULL, char* const=NULL);
    ~m_timeSeriesPlot_Loop_v( void );
    void simulation();
};

*/
#endif
//eof
