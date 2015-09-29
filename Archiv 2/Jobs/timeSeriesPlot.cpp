///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/timeSeriesPlot.C,v 1.1 2000/08/31 16:27:12 mmeyer Exp mmeyer $
//
// Module name:		macrodyn.C
// Contents:		member functions of class timeSeriesPlot and m_timeSeriesPlot
//
// Author:		Uli Middelberg
// Last modified:	Thu Jun  3 14:50:18 CEST 1999  Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////


#include "timeSeriesPlot.h"

//
// initialising static member pointsize
//
int timeSeriesPlot::pointsize = 1;

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		timeSeriesPlot
// member function:	timeSeriesPlot
// Purpose:		constructor
//
// Author:		Uli Middelberg
// Last modified:	Sun May 30 23:21:35 CEST 1999 
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
timeSeriesPlot::timeSeriesPlot(baseModel* const bMod, const QString& label,
                   MacrodynGraphicsItem* const graph,
                   const QString& fileName,int const mp_num,char** const mp_ptr)
          :timeSeriesJob(bMod,label,graph)
{
    log() << "constructing time series plot..."<<"\n";

    multiplotAdr = NULL;
    qreal xmin, xmax;

    QString location =QDir::currentPath();

    if(!fileName.isEmpty())
        outFile.setFileName(fileName);
    else
        outFile.setFileName(location+"/timeSeries.asc");

    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) qDebug() << "Failed to open:" << outFile.errorString();

    graph->get_axis(0, &xmin, &xmax);
    limit = qint64( xmin );

    if (mp_ptr) {
      multiplot_num=mp_num;

      multiplotAdr=new qreal*[multiplot_num];
      if( !(multiplotAdr) ) {
    log() << "macrodyn::timeSeriesPlot::timeSeriesPlot  \
        Can't create array for *multiplot[]";
	exit(-1);
	}

      multiplotOld=new qreal[multiplot_num];
      if( !(multiplotAdr) ) {
    log() << "macrodyn::timeSeriesPlot::timeSeriesPlot  \
        Can't create array for multiplot[]";
	exit(-1);
	}
      QList<QString> list;
      for(int j=0;j<multiplot_num;j++) {
        multiplotAdr[j]=model->setLabels(mp_ptr[j]);
        log()<<"multiplotAdr[j] = "<<*multiplotAdr[j];
        list << QString(mp_ptr[j]);
      }

      if( screenGraphics ) {
         screenGraphics->draw_mp_names(list);
      }

    }
    //multiplotAdr = NULL;
    log() << "finished";
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		timeSeriesPlot
// member function:	timeSeriesPlot
// Purpose:		destructor
//
// Author:		Marc Mueller
// Last modified:	Sun May 30 20:01:35 CEST 1999 
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
timeSeriesPlot::~timeSeriesPlot() {
    if (multiplotAdr) {
    delete [] multiplotAdr;
    delete [] multiplotOld;
    }
}
///////////////////////////////////////////////////////////////////////////////
//
// Class name:		timeSeriesPlot
// member function:	simulation
// Purpose:		plot a time series of a certain variable
//
// Author:		Uli Middelberg
// Last modified:	Thu Jun  3 14:50:18 CEST 1999  Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

void timeSeriesPlot::simulation()
{
    log() << "starting time series simulation...";
    qint64 t;
    qreal oldX, oldY;
    int j;
    model->initialize();
    t = 0;
    oldX = t;
    saveSeries(t);
    oldY = timeSeriesqreal[t];
    if (multiplotAdr)
    {
        for(j=0;j<multiplot_num;j++)
            multiplotOld[j] = *multiplotAdr[j];
    }
    for(t=1;t<length+1;t++)
    {
        model->iteration(t);
        if( t >= limit )
        {
            saveSeries(t);
            if( screenGraphics )
            {
                if (multiplotAdr)
                {
                    //log()<<"multiplot_num = "<<multiplot_num;
                    for(j=0;j<multiplot_num;j++)
                    {
                        screenGraphics->setLine(oldX,multiplotOld[j],(double)t,*multiplotAdr[j],j);
                        screenGraphics->setBigPoint(double(t),*multiplotAdr[j],j,pointsize);
                    }
                }
                else {
                screenGraphics->setLine(oldX,oldY,(double)t,timeSeriesqreal[t],40);
                screenGraphics->setBigPoint((double)t,timeSeriesqreal[t],40,pointsize);
                }
            }
            QTextStream(&outFile) << t << "\t" << timeSeriesqreal[t]<<"\n";
        }
        oldX = t;
        oldY = timeSeriesqreal[t];
        if (multiplotAdr)
        {
           for(j=0;j<multiplot_num;j++)
               multiplotOld[j] = *multiplotAdr[j];
        }
    }
    log() << "finished...\n";
}


muTimeSeriesPlot::muTimeSeriesPlot(baseModel* const bMod, const QString& label,
                   MacrodynGraphicsItem* const graph,
                   const QString& fileName,int const mp_num,char** const mp_ptr, QString multiPlotLabel, qreal* ts_init_values)
          :timeSeriesJob(bMod,label,graph)
{
    log() << "constructing time series plot..."<<"\n";

    multiplotAdr = NULL;
    qreal xmin, xmax;

    QString location =QDir::currentPath();

    if(!fileName.isEmpty())
        outFile.setFileName(fileName);
    else
        outFile.setFileName(location+"/timeSeries.asc");

    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) qDebug() << "Failed to open:" << outFile.errorString();

    graph->get_axis(0, &xmin, &xmax);
    limit = qint64( xmin );

    if (mp_ptr) {
      multiplot_num=mp_num;

      multiplotAdr=new qreal*[multiplot_num];
      if( !(multiplotAdr) ) {
    log() << "macrodyn::timeSeriesPlot::timeSeriesPlot  \
        Can't create array for *multiplot[]";
    exit(-1);
    }

      multiplotOld=new qreal[multiplot_num];
      if( !(multiplotAdr) ) {
    log() << "macrodyn::timeSeriesPlot::timeSeriesPlot  \
        Can't create array for multiplot[]";
    exit(-1);
    }
      QList<QString> list;
      for(int j=0;j<multiplot_num;j++) {
        multiplotAdr[j]=model->setLabels(mp_ptr[j]);
        list << QString(mp_ptr[j]);
      }

      if( screenGraphics ) {
         screenGraphics->draw_mp_names(list);
      }

    }
    multiplotAdr = NULL;

    zParam = model->setLabels(multiPlotLabel);	// get the pointer to the second
                    // bifurcation parameter
    multiplot_values = ts_init_values;

    log() << "finished";
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		timeSeriesPlot
// member function:	timeSeriesPlot
// Purpose:		destructor
//
// Author:		Marc Mueller
// Last modified:	Sun May 30 20:01:35 CEST 1999
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
muTimeSeriesPlot::~muTimeSeriesPlot() {
    if (multiplotAdr) {
    delete [] multiplotAdr;
    delete [] multiplotOld;
    }
}
///////////////////////////////////////////////////////////////////////////////
//
// Class name:		timeSeriesPlot
// member function:	simulation
// Purpose:		plot a time series of a certain variable
//
// Author:		Uli Middelberg
// Last modified:	Thu Jun  3 14:50:18 CEST 1999  Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

void muTimeSeriesPlot::simulation()
{
    log() << "starting time series simulation...";
    qint64 t;
    qreal oldX, oldY;
    int j;


    for(qint64 i=1; i<=multiplot_values[0]; i++) {
        *zParam = multiplot_values[i];
        log()<<"*zParam = "<<*zParam;

        model->initialize();
        t = 0;
        oldX = t;
        saveSeries(t);
        oldY = timeSeriesqreal[t];
        if (multiplotAdr)
        {
            for(j=0;j<multiplot_num;j++)
                multiplotOld[j] = *multiplotAdr[j];
        }
        for(t=1;t<length+1;t++)
        {
            model->iteration(t);
            if( t >= limit )
            {
                saveSeries(t);
                if( screenGraphics )
                {
                    if (multiplotAdr)
                    {
                        for(j=0;j<multiplot_num;j++)
                        {
                            screenGraphics->setLine(oldX,multiplotOld[j],(double)t,*multiplotAdr[j],i-1);
                            screenGraphics->setBigPoint(double(t),*multiplotAdr[j],i-1,1);
                        }
                    }
                    screenGraphics->setLine(oldX,oldY,(double)t,timeSeriesqreal[t],i-1);
                    screenGraphics->setBigPoint((double)t,timeSeriesqreal[t],i-1,1);
                }
                QTextStream(&outFile) << t << "\t" << timeSeriesqreal[t]<<"\n";
            }
            oldX = t;
            oldY = timeSeriesqreal[t];
            if (multiplotAdr)
            {
               for(j=0;j<multiplot_num;j++)
                   multiplotOld[j] = *multiplotAdr[j];
            }
        }
    }


    log() << "finished...\n";
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		m_timeSeriesPlot
// member function:	m_timeSeriesPlot
// Purpose:		constructor
//
// Author:		Uli Middelberg
// Last modified:	Fri Nov  7 14:53:01 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

m_timeSeriesPlot::m_timeSeriesPlot(qreal * ivalues, baseModel* const bMod,
                   const QString& label,
                       MacrodynGraphicsItem* const graph,
			           char* ) 
                                  :timeSeriesPlot(bMod,label,graph)
{
  n_i_vals = (int) ivalues[0];	// the first entry denotes the number of values
  ivalues++;
  i_vals = new qreal [n_i_vals];
  memcpy (i_vals, ivalues, n_i_vals * sizeof(qreal) );
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		m_timeSeriesPlot
// member function:	~m_timeSeriesPlot
// Purpose:		distructor
//
// Author:		Uli Middelberg
// Last modified:	Fri Nov  7 14:53:01 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

m_timeSeriesPlot::~m_timeSeriesPlot(void)
{
  delete [] i_vals;
}
///////////////////////////////////////////////////////////////////////////////
//
// Class name:		timeSeriesPlot
// member function:	simulation
// Purpose:		plot a time series of a certain variable
//
// Author:		Uli Middelberg
// Last modified:	Fri Nov  7 14:53:01 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void m_timeSeriesPlot::simulation()
{
    qint64 t;
    qreal oldX, oldY;
    int  k;
    QTextStream stream(&outFile);
    log()<<"n_i_vals"<<n_i_vals<<"\n";
    for( k=0; k<n_i_vals; k++) {
        model->initialize();                // initialize the model
        srand(1234);
        *modelVar = i_vals[k];		// pick the next initial value
        oldX=0;
        oldY=*modelVar;
        stream << oldX << "\t" << oldY;
        for(t=1;t<length+1;t++) {
            model->iteration(t);          // compute the orbit
            if( t >= limit )  {
                saveSeries(t);
                if( screenGraphics ) {
                    screenGraphics->setLine(oldX,oldY,(double)t,timeSeriesqreal[t],k);
                    screenGraphics->setBigPoint((double)t,timeSeriesqreal[t],k,pointsize);
                }
                stream << t << "\t" << timeSeriesqreal[t];
                oldX=t;
                oldY=timeSeriesqreal[t];
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		m_timeSeriesPlot_p
// member function:	m_timeSeriesPlot_p
// Purpose:		constructor
//
// Author:		Marc Mueller (org.:Uli Middelberg)
// Last modified:	Fri May 28 12:32:41 METDST 1999
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

m_timeSeriesPlot_v::m_timeSeriesPlot_v(const QString& labelm,
				   qreal * ivalues, baseModel* const bMod,
                   const QString& label,
                       MacrodynGraphicsItem* const graph,
			           char* ) 
                                  :timeSeriesPlot(bMod,label,graph)
{
  n_i_vals = (int) ivalues[0];	// the first entry denotes the number of values
  ivalues++;
  i_vals = new qreal [n_i_vals];
  memcpy (i_vals, ivalues, n_i_vals * sizeof(qreal) );

  modelVar2=model->setLabels(labelm);

}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		m_timeSeriesPlot
// member function:	~m_timeSeriesPlot
// Purpose:		distructor
//
// Author:		Uli Middelberg
// Last modified:	Fri May 28 12:32:41 METDST 1999
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

m_timeSeriesPlot_v::~m_timeSeriesPlot_v(void)
{
  delete [] i_vals;
}
///////////////////////////////////////////////////////////////////////////////
//
// Class name:		timeSeriesPlot
// member function:	simulation
// Purpose:		plot a time series of a certain variable
//			with one changed parameter for each plot
//
// Author:		Marc Mueller (org.:Uli Middelberg)
// Last modified:	Fri May 28 12:32:41 METDST 1999
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

void m_timeSeriesPlot_v::simulation()
{
    qint64 t;
    qreal oldX, oldY;
    int  k;
    QTextStream stream(&outFile);

  for( k=0; k<n_i_vals; k++) {
    *modelVar2 = i_vals[k];		// pick the next initial value
    log() << "i_vals[k] = "<<i_vals[k]<< "\n";
    model->initialize();                // initialize the model
    oldX=0;
    oldY=*modelVar;
    stream << oldX << "\t" << oldY;
    for(t=1;t<length+1;t++) {
	model->iteration(t);          // compute the orbit
	if( t >= limit )  {
	    saveSeries(t);                  
              if( screenGraphics ) {
            screenGraphics->setLine(oldX,oldY,(double)t,timeSeriesqreal[t],k);
        screenGraphics->setBigPoint((double)t,timeSeriesqreal[t],k,pointsize);
          }
        stream << t << "\t" << timeSeriesqreal[t];
	    oldX=t;
	    oldY=timeSeriesqreal[t];
	}
    }
  }
}

//eof
