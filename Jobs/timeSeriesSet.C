///////////////////////////////////////////////////////////////////////////////
//
// Module name:		macrodyn.C
// Contents:		member functions of class timeSeriesSet
//
// Author:		Andreas Starke
// Last modified: 
//
///////////////////////////////////////////////////////////////////////////////

#include "timeSeriesSet.h"
#include "../error.h"

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		timeSeriesSet
// member function:	timeSeriesSet
// Purpose:		constructor
//
// Author:		Uli Middelberg
// Last modified:	Sun May 30 23:21:35 CEST 1999 
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
timeSeriesSet::timeSeriesSet(baseModel* const bMod, const xyRange& axes, 
                const QString& fileName, bool surf, qint64 res)
          :geometricJob(bMod,axes,NULL), xmin(axes.min[0])
          , xmax(axes.max[0]), ymin(axes.min[1]), ymax(axes.max[1]),
                zmin(res), zmax(axes.max[2]), surface(surf)
{
    log() << "constructing time series set...";

    xParam = model->setLabels(axes.label[0]);
	if( !xParam )
		fatalError("timeSeriesSet::timeSeriesSet  Can not find x label",axes.label[0]);
    yParam = model->setLabels(axes.label[1]);
	if( !yParam )
		fatalError("timeSeriesSet::timeSeriesSet  Can not find y label",axes.label[1]);

    if(!fileName.isEmpty())
        outFile.setFileName(fileName);
    else
        outFile.setFileName("data3D_timeseries.dat");
    if (outFile.open(QFile::WriteOnly))
    {
        log() << "Opened file" << outFile.fileName();
    }
    else
    {
        log() << "Failed to open file" << outFile.fileName();
    }

    stepX=(xmax-xmin) / ymax;
    limit=qint64(ymin);		// definiert den ersten Beobachtungszeitpunkt
    log() << zmin << " " ;
    log() << "finished\n" << "\n";
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		timeSeriesSet
// member function:	setStepX
// Purpose:		set Parameter Resolution of calculation
//
// Author:		Andreas Starke
// Last modified: 
//
///////////////////////////////////////////////////////////////////////////////

void timeSeriesSet::setStepX(const qreal& toSet)
{
	stepX = toSet;
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		timeSeriesSet
// member function:	simulation
// Purpose:		save data of multiple time series (somewhat comparable
// 			to a bifurcation)
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void timeSeriesSet::simulation()
{
    log() << "time series set simulation...";
	double fortschritt = 0;
    double schritt = double(100/((xmax-xmin)/stepX));
    qreal oldy, oldx;
    qint64 t;
	model->initialize();
    QTextStream stream(&outFile);
	if ( surface==true ){
        for(*xParam=xmin; *xParam<=xmax; *xParam+=stepX) {
            fortschritt+=schritt;
            log() << "\r" << fortschritt << "Prozent...";
			model->initialize();
			for(t=1;t<=length;t++) {
				model->iteration(t);
                if( (t >= limit) && ( t % zmin == 0 ) )  {
                    stream << *xParam << "\t" << t << "\t" << *yParam;
			        }
			}
            stream << "\n";
		}
	} else {
        for(*xParam=xmin; *xParam<=xmax; *xParam+=stepX) {
            fortschritt+=schritt;
            log() << "\r" << fortschritt << "Prozent...";
			model->iteration(1);
			model->initialize();
			for(t=1;t<=length;t++) {
				oldx=*xParam;
				oldy=*yParam;	
				model->iteration(t);
				if( t >= limit )  {
                    stream << oldx << "\t" << t-1 << "\t" << oldy;
                    stream << *xParam << "\t" << t << "\t" << *yParam;
                    stream << "\n" << "\n";
			        }
			}
		}	
	}
    stream.flush();
	outFile.close();
    log() << "done\n";
}
