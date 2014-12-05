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
timeSeriesPlot::timeSeriesPlot(baseModel* const bMod,char* const label,
                   MacrodynGraphicsItem* const graph,
                   const QString& fileName,int const mp_num,char** const mp_ptr)
          :timeSeriesJob(bMod,label,graph)
{
    log() << "constructing time series plot..."<<"\n";
	qreal xmin, xmax;
    if(!fileName.isEmpty())
        outFile.setFileName(fileName);
    else
        outFile.setFileName("timeSeries.asc");
    outFile.open(QFile::WriteOnly);
    graph->get_axis(0, &xmin, &xmax);
    limit = qint64( xmin );

    if (mp_ptr) {
      multiplot_num=mp_num;

      multiplotAdr=new qreal*[multiplot_num];
      if( !(multiplotAdr) ) {
    log() << "macrodyn::timeSeriesPlot::timeSeriesPlot  \
        Can't create array for *multiplot[]"<< "\n" << "\n";
	exit(-1);
	}

      multiplotOld=new qreal[multiplot_num];
      if( !(multiplotAdr) ) {
    log() << "macrodyn::timeSeriesPlot::timeSeriesPlot  \
        Can't create array for multiplot[]"<< "\n" << "\n";
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
    log() << "finished\n" << "\n";
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
    t=0;
    oldX=t;
    saveSeries(t);
    oldY=timeSeriesqreal[t];	
    if (multiplotAdr) {
        for(j=0;j<multiplot_num;j++)
            multiplotOld[j]=*multiplotAdr[j];	    
    }
    for(t=1;t<length+1;t++) {
	model->iteration(t);
	if( t >= limit )  {
	    saveSeries(t);
              if( screenGraphics ) {
                if (multiplotAdr) {
                  for(j=0;j<multiplot_num;j++){ 
                     screenGraphics->drawLine(oldX,multiplotOld[j],(double)t,*multiplotAdr[j],41+j);
		     screenGraphics->setBigPoint(double(t),*multiplotAdr[j],41+j,pointsize);
		  }
               }
	        screenGraphics->drawLine(oldX,oldY,(double)t,timeSeriesqreal[t],40);
		screenGraphics->setBigPoint((double)t,timeSeriesqreal[t],40,pointsize);

	      }
        QDataStream(&outFile) << t << "\t" << timeSeriesqreal[t] << "\n";
        }
        oldX=t;
        oldY=timeSeriesqreal[t];	
        if (multiplotAdr) {
           for(j=0;j<multiplot_num;j++)
               multiplotOld[j]=*multiplotAdr[j];	    
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
				   char* const label,
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
    QDataStream stream(&outFile);

  for( k=0; k<n_i_vals; k++) {
    model->initialize();                // initialize the model
    *modelVar = i_vals[k];		// pick the next initial value
    oldX=0;
    oldY=*modelVar;
    stream << oldX << "\t" << oldY << "\n";
    for(t=1;t<length+1;t++) {
	model->iteration(t);          // compute the orbit
	if( t >= limit )  {
	    saveSeries(t);                  
              if( screenGraphics ) {
	        screenGraphics->drawLine(oldX,oldY,(double)t,timeSeriesqreal[t],k+6);
		screenGraphics->setBigPoint((double)t,timeSeriesqreal[t],k+6,pointsize);
          }
//	    }
        stream << t << "\t" << timeSeriesqreal[t] << "\n";
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

m_timeSeriesPlot_v::m_timeSeriesPlot_v(char* const labelm,
				   qreal * ivalues, baseModel* const bMod,
				   char* const label,
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
    QDataStream stream(&outFile);

  for( k=0; k<n_i_vals; k++) {
    *modelVar2 = i_vals[k];		// pick the next initial value
    model->initialize();                // initialize the model
    oldX=0;
    oldY=*modelVar;
    stream << oldX << "\t" << oldY << "\n";
    for(t=1;t<length+1;t++) {
	model->iteration(t);          // compute the orbit
	if( t >= limit )  {
	    saveSeries(t);                  
              if( screenGraphics ) {
	        screenGraphics->drawLine(oldX,oldY,(double)t,timeSeriesqreal[t],k+6);
		screenGraphics->setBigPoint((double)t,timeSeriesqreal[t],k+6,pointsize);
          }
        stream << t << "\t" << timeSeriesqreal[t] << "\n";
	    oldX=t;
	    oldY=timeSeriesqreal[t];
	}
    }
  }
}

//eof
