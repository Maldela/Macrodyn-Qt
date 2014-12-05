///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/timeSeriesPlot_Loop.C,v 1.4 2001/08/14 13:17:08 mhoffman Exp $
//
// Module name:		macrodyn.C
// Contents:		member functions of class timeSeriesPlot_Loop and m_timeSeriesPlot_Loop
//
// Author:		Uli Middelberg
// Last modified:	Thu Jun  3 14:50:18 CEST 1999  Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////


#include "timeSeriesPlot_Loop.h"

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		timeSeriesPlot_Loop
// member function:	timeSeriesPlot_Loop
// Purpose:		constructor
//
// Author:		Uli Middelberg
// Last modified:	Sun May 30 23:21:35 CEST 1999
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
timeSeriesPlot_Loop::timeSeriesPlot_Loop(int l_size, baseModel* const bMod,char* const label,
                   MacrodynGraphicsItem* const graph,
			       char* const fileName,int const mp_num,char** const mp_ptr)
          :timeSeriesJob(bMod,label,graph)
{
   loop_size = l_size;
/*    if( fileName )
	outFile.open(fileName,ios::out);
    else
	outFile.setFileName("timeSeries.asc",ios::out);
    limit=0;

    if (mp_ptr) {
      multiplot_num=mp_num;

      multiplotAdr=new qreal*[multiplot_num];
      if( !(multiplotAdr) ) {
		cerr << "macrodyn::timeSeriesPlot_Loop::timeSeriesPlot_Loop  \
		Can't create array for *multiplot[]"<< "\n";
		exit(-1);
	  }

      multiplotOld=new qreal[multiplot_num];
      if( !(multiplotAdr) ) {
		cerr << "macrodyn::timeSeriesPlot_Loop::timeSeriesPlot_Loop  \
		Can't create array for multiplot[]"<< "\n";
		exit(-1);
	  }

      for(int j=0;j<multiplot_num;j++) {
        multiplotAdr[j]=model->setLabels(mp_ptr[j]);
      }

      if( screenGraphics ) {
         screenGraphics->draw_mp_names(multiplot_num,mp_ptr);
      }
    }
*/
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		timeSeriesPlot_Loop
// member function:	timeSeriesPlot_Loop
// Purpose:		destructor
//
// Author:		Marc Mueller
// Last modified:	Sun May 30 20:01:35 CEST 1999
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
timeSeriesPlot_Loop::~timeSeriesPlot_Loop() {
}
///////////////////////////////////////////////////////////////////////////////
//
// Class name:		timeSeriesPlot_Loop
// member function:	simulation
// Purpose:		
//
// Author:		
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void timeSeriesPlot_Loop::simulation()
{
    qint64 t;
	double temp;
    double mean_error, sum;

	sum = 0;
	for (int i=0; i<loop_size; i++){
	    model->initialize();
    	t=0;
    	for(t=1;t<=length;t++) {
			model->iteration(t);
	    }
//	    log() << "\nmodelVar (i=" << i << ") = " << *modelVar;
		temp = *modelVar;
		temp = ((temp>0) ? temp : (-temp));			
		sum += temp;				
	}
	mean_error = sum / qreal(loop_size);	
	log() << "\nmean_error = " << mean_error << "\n";
}


//eof








/*	if( t >= limit )  {
	    	saveSeries(t);
     			if( screenGraphics ) {
                	if (multiplotAdr) {
                  		for(j=0;j<multiplot_num;j++)
                     		screenGraphics->drawLine(oldX,multiplotOld[j],(double)t,*multiplotAdr[j],9+j);	
               		}
	        		screenGraphics->drawLine(oldX,oldY,(double)t,timeSeriesqreal[t],6);
	    		}
	      		if( printDev ) {
        			if (multiplotAdr) {
          				for(j=0;j<multiplot_num;j++)
	             			printDev->drawLine(oldX,multiplotOld[j],(double)t,*multiplotAdr[j],9+j);
                	}
	        		printDev->drawLine(oldX,oldY,(double)t,timeSeriesqreal[t],6);
	      		}
	    		outFile << t << "\t" << timeSeriesqreal[t] << "\n";
        }
        oldX=t;
        oldY=timeSeriesqreal[t];	
        if (multiplotAdr) {
        	for(j=0;j<multiplot_num;j++)
         		multiplotOld[j]=*multiplotAdr[j];	
		}
	*/	
