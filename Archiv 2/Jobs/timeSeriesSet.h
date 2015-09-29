///////////////////////////////////////////////////////////////////////////////
//
// Module name:		timeSeriesSet.h
// Contents:		definition of class timeSeriesSet
//
// Author:		Andreas Starke
// last modified:
//
///////////////////////////////////////////////////////////////////////////////

#ifndef TIMESERIESSET_H
#define TIMESERIESSET_H
#include "geometricJob.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		timeSeriesSet
// Derived from:	timeSeriesJob
// Purpose:		create multiple time series data file
//
// Author:		Andreas Starke
// last modified:
//
///////////////////////////////////////////////////////////////////////////////

class timeSeriesSet : public geometricJob {

protected:
	QFile outFile;
	qreal stepX;
	QString xLabel;
    qreal xmin, xmax;
    qreal ymin, ymax;
    qint64 zmin;
    qreal zmax;
	qreal* xParam;
	qreal* yParam;
	bool surface;
	
public:
	timeSeriesSet(baseModel* const, const xyRange& axes, 
             const QString& filename, bool surf, qint64 res);
	void setStepX(const qreal&);
	void simulation();
};

#endif
//eof
