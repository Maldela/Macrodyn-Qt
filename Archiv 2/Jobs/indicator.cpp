///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/indicator.C,v 1.1 2000/08/31 16:23:20 mmeyer Exp $
//
// Module name:		indicator.C
// Contents:		member functions of class indicator
//			making a density plot with respect to a state variable
//
// Author:		Uli Middelberg
// Last modified:	Fri Jul 11 18:18:14 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#include	<unistd.h>
#include	"../logger.h"
#include	"indicator.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		indicator_2d
// Member function:	indicator_2d
// Purpose:		constructor
//
// Author:		Uli Middelberg
// Last modified:	Mon Jul 14 19:46:38 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

indicator_2d::indicator_2d(baseModel* const bMod, xyRange & axes, 
                   MacrodynGraphicsItem* const graph)
                      :geometricJob(bMod,axes,graph),
                       n_colors(30)
{
    stepX=(xmax-xmin) / (1.0*axes.res[0]);
    stepY=(ymax-ymin) / (1.0*axes.res[1]);
    zmin=axes.min[2];	// save the bounderies for the range
    zmax=axes.max[2];	// of encoding of the indicator function
    zLabel = axes.label[2];
    zParam=model->setLabels(zLabel);	// get the pointer to the indicator value
    if( !zParam )
	error("indicator::indicator: Undefined label %s",zLabel);

    limit=(qint64)(0.0*length);		// 0% are thrown away
    
    color_code = new d_var (zmin, zmin, zmax, n_colors);
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		indicator_2d
// Member function:	simulation
//			draw color encoded iterates of the indicator function
//
// Author:		Uli Middelberg
// Last modified:	Fri Jul 11 18:18:22 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void indicator_2d::simulation()
{
    qint64 t;
    qint64 T;
    int color;
    qreal rx, ry;
  
  for( T=length;T<=length;T++ ) {
    for( rx=xmin; rx<=xmax; rx += stepX) {
      for( ry=ymin; ry<=ymax; ry += stepY) {
        model->initialize();
        *xParam=rx; *yParam=ry;
          for( t=0;t<T;t++ ) {
            model->iteration(t+1);
          }
        //color=get_encoding();
        double unitColor = (*zParam-zmin)/(zmax-zmin);
        QColor color = QColor(redFromUnit(unitColor),greenFromUnit(unitColor),blueFromUnit(unitColor));
        if( screenGraphics ) {
            screenGraphics->setRect(rx,ry,stepX,stepY,colorFromOrder((int)(30*unitColor)));
            //screenGraphics->setPoint(rx,ry,color);
        }
      }
    }
  }
}


int indicator_2d::get_encoding( void ) {
  if( *zParam > zmax  ) {
    return 31;
  }
  if( *zParam < zmin ) {
    return 0;
  } // now, *zParam is between zmin and zmax and we have n_colos colors left
  color_code->set_val(*zParam);		// the d_var color_code translates
  return color_code->get_dval();	// *zParam into a color code
}

QColor indicator_2d::colorFromOrder(int order) {
    switch (order) {
    case -1: return QColor(0,0,0);
    case 0: return QColor(255,255,255);
    case 1: return QColor(255,0,255);
    case 2: return QColor(0,255,255);
    case 3: return QColor(211,186,211);
    case 4: return QColor(0,255,0);
    case 5: return QColor(255,0,0);
    case 6: return QColor(251,174,190);
    case 7: return QColor(162,158,158);
    case 8: return QColor(0,0,255);
    case 9: return QColor(251,121,20);
    case 10: return QColor(243,219,170);
    case 11: return QColor(121,219,101);
    case 12: return QColor(235,52,138);
    case 13: return QColor(113,142,207);
    case 14: return QColor(247,101,65);
    case 15: return QColor(37,56,60);
    case 16: return QColor(37,65,23);
    case 17: return QColor(185,59,143);
    case 18: return QColor(255,56,23);
    case 19: return QColor(67,198,219);
    case 20: return QColor(48,103,84);
    case 21: return QColor(72,99,160);
    case 22: return QColor(129,5,65);
    case 23: return QColor(0,0,0);
    case 24: return QColor(216,175,121);
    case 25: return QColor(125,27,126);
    case 26: return QColor(67,183,186);
    case 27: return QColor(102,152,255);
    case 28: return QColor(212,160,23);
    case 29: return QColor(21,27,84);
    case 30: return QColor(152,5,23);
    }
    return QColor(152,5,23);
}

// eof
