///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/max_lyapunov_exp.C,v 1.1 2000/08/31 16:24:07 mmeyer Exp $
//
// Module name:		max_lyapunov_exp.C
// Contents:		member functions of the class max_lyapunov_exp
//			compute the approximation of the maximal
//			lyapunov exponent of a time series
//
// Author:		Uli Middelberg
// Last modified:	Tue Jun 24 14:28:39 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#include	"max_lyapunov_exp.h"
#include	"math.h"
#include	"../logger.h"

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		max_lyapunov_exp_1d
// member function:	max_lyapunov_exp_1d
// Purpose:		constructor
//
// Author:		Uli Middelberg
// Last modified:	Fri Jun 27 18:21:41 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

max_lyapunov_exp_1d::max_lyapunov_exp_1d(baseModel* const bMod,
				       xyRange& axes,
                       MacrodynGraphicsItem* const graph)
          :geometricJob(bMod,axes,graph), eps(0.0001)
{
    stepX=(xmax-xmin) / (1.00*axes.res[0]); // 1.02 -> 1.00
    n_axes = & axes;

    x_prime = bMod->setLabels("x_prime");	// get a pointer to x_prime
    if( x_prime == NULL ) {
      error("This model does not know the parameter \"x_prime\""
            " which is necessary for the computation of the"
            " lyapunov exponent.");
    }
    limit = 0;		// there is no limit
}                                      

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		max_lyapunov_exp_1d
// member function:	~max_lyapunov_exp_1d
// Purpose:		destructor
//
// Author:		Uli Middelberg
// Last modified:	Tue Jun 24 15:37:50 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

max_lyapunov_exp_1d::~max_lyapunov_exp_1d()
{
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		max_lyapunov_exp_2d
// member function:	simulation
// Purpose:		compute the approximation of the maximal
//			lyapunov exponent of a time series
//
// Author:		Uli Middelberg
// Last modified:	Tue Jun 24 15:37:50 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void max_lyapunov_exp_1d::simulation()
{
    qint64 t;
    int color = 6;	// color used for the exponents
    qreal zero=0;	// variable used to draw the zero line
    int z_color = 30;	// color used for the zero line
    qreal l_min=0;
    qreal l_max=0;
    int  l_len = (int) ceil((xmax-xmin)/stepX)+2;
    qreal * l_vals = new double[l_len];
    int    l = 0;

    
  for( *xParam=xmin; *xParam<=xmax ;*xParam += stepX) {
    model->initialize();
    sum=0;
    for( t=0;t<length;t++ ) {
      model->iteration(t+1);
      if( t >= limit ) {
        update_sum();
      }
    }
    evaluate();
    l_min = qMin(l_min,l_exp);
    l_max = qMax(l_max,l_exp);
    l_vals[l++]=l_exp;
  }
  l_min=floor(l_min);
  l_max=ceil(l_max);
//  n_axes->min[1]=l_min;
//  n_axes->max[1]=l_max;
  n_axes->label[1] = "lyap";

  if( screenGraphics ) {
    screenGraphics->reset(*n_axes);
  }


  for( *xParam=xmin, l=0; *xParam<=xmax ;*xParam += stepX, l++) {
    if( screenGraphics ) {
      screenGraphics->setPoint(*xParam,l_vals[l],color);
      screenGraphics->setPoint(*xParam,zero,z_color);
    }
  }
  delete [] l_vals;
}

///////////////////////////////////////////////////////////////////////////////

void max_lyapunov_exp_1d::update_sum( void ) {
  sum += log(fabs(*x_prime));
}

void max_lyapunov_exp_1d::evaluate( void ) {
  l_exp = (1.0 / (length)) * sum;
}

void max_lyapunov_exp_1d::evaluate( qint64 t ) {
  l_exp = (1.0 / (t)) * sum;
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		max_lyapunov_exp_2d
// member function:	max_lyapunov_exp_2d
// Purpose:		constructor
//
// Author:		Uli Middelberg
// Last modified:	Tue Jun 24 15:37:50 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

max_lyapunov_exp_2d::max_lyapunov_exp_2d(baseModel* const bMod,
				       xyRange& axes,
                       MacrodynGraphicsItem* const graph)
        :max_lyapunov_exp_1d(bMod,axes,graph),
		n_colors(30)
{
    stepY=(ymax-ymin) / (1.00*axes.res[1]); // 1.02 -> 1.00
    color_code = new d_var (-2, -2, 1, n_colors);
    limit = 1;		// this limit is used for the correct analysis of
}                       // convergence of the lyapunov exponent


///////////////////////////////////////////////////////////////////////////////
//
// Class name:		max_lyapunov_exp_2d
// member function:	simulation
// Purpose:		compute the approximation of the maximal
//			lyapunov exponent of a time series
//
// Author:		Uli Middelberg
// Last modified:	Tue Jun 24 15:37:50 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void max_lyapunov_exp_2d::simulation()
{
    qint64  t;
    int color;
    const qreal l_exp_eps=0.000001;
    qreal  l_exp_old;
    qreal  l_exp_diff = 0;
  log() << "\nData-file for conversion to xpm-file";
  log() << "Format: y x z";
  log() << "xparam" << "\t"<< xmin << "\t"<< xmax << "\t" << (xmax-xmin)/stepX;
  log() << "yparam" << "\t"<< ymin << "\t"<< ymax << "\t" << (ymax-ymin)/stepY;
  log() << "\nSTART DATA NOW:";
 
 for( *yParam=ymax; *yParam>=(ymin-stepY/2) ;*yParam -= stepY) {
    if (*yParam < ymin)
      *yParam = ymin;
    for( *xParam=xmin; *xParam<=(xmax+stepX/2) ;*xParam += stepX) {
      if (*xParam>xmax)
        *xParam=xmax;
      model->initialize();
      sum=0;
      l_exp_diff = 1 + l_exp_eps;
      l_exp = 1000000;			// this value should be high enough
      					// to prevent stopping after the first
      					// iteration (l_exp_old would be undefined)
      for( t=0;(t<length) && ( l_exp_diff > l_exp_eps ) ; t++ ) {
        model->iteration(t+1);
        if( t >= limit ) {
          update_sum();
          l_exp_old = l_exp;
          evaluate(t);
          l_exp_diff = fabs(l_exp_old - l_exp);
        }
      }
      color=get_encoding(); 
      if( screenGraphics ) {
        screenGraphics->setPoint(*xParam,*yParam,color);
      }
      log() << *yParam << "\t" << *xParam << "\t" << l_exp <<  "\n";
    }  
  }
   log() << "STOP DATA NOW\n";
}

int max_lyapunov_exp_2d::get_encoding( void ) {
  if( l_exp > 1  ) {
    return 31;
  }
  if( l_exp < -2 ) {
    return 0;
  }
  // now, l_exp is between -2 and 1 and we have n_colors colors left
  color_code->set_val(l_exp);		// the d_var color_code translates
  return color_code->get_dval();	// l_exp into a color code
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		max_lyapunov_exp_t
// member function:	max_lyapunov_exp_t
// Purpose:		constructor
//
// Author:		Uli Middelberg
// Last modified:	Tue Jun 24 15:37:50 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

max_lyapunov_exp_t::max_lyapunov_exp_t(baseModel* const bMod,
				       xyRange& axes,
                       MacrodynGraphicsItem* const graph)
          :max_lyapunov_exp_1d(bMod,axes,graph)
{
}                                      

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		max_lyapunov_exp_t
// member function:	simulation
// Purpose:		compute the approximation of the maximal
//			lyapunov exponent of a time series
//
// Author:		Uli Middelberg
// Last modified:	Tue Jun 24 15:37:50 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////
void max_lyapunov_exp_t::simulation()
{
    qint64 t;
    int color = 6;	// color used for the exponents
    qreal zero=0;	// variable used to draw the zero line
    int z_color = 30;	// color used for the zero line
    qreal l_min=0;
    qreal l_max=0;
    qint64 l_len = length;
    qreal * l_vals = new double[l_len];
//    int    l = 0;
    qreal   t_qreal;

    
  model->initialize();
  sum=0;
  for( t=0;t<length;t++ ) {
    model->iteration(t+1);
    if( t >= limit ) {
      update_sum();
      evaluate(t);
      l_min = qMin(l_min,l_exp);
      l_max = qMax(l_max,l_exp);
      l_vals[t]=l_exp;
    }
  }

  l_min=floor(l_min);
  l_max=ceil(l_max);
  
  n_axes->label[0] = "t";
  
//  n_axes->min[1]=l_min;
//  n_axes->max[1]=l_max;
  n_axes->label[1] = "lyap";

  if( screenGraphics ) {
    screenGraphics->reset(*n_axes);
  }

  for( t=0;t<length;t++ ) {
    t_qreal=t;
    if( screenGraphics ) {
      screenGraphics->setPoint(t_qreal,l_vals[t],color);
      screenGraphics->setPoint(t_qreal,zero,z_color);
    }
  }
  delete [] l_vals;
}

//eof
