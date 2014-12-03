///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/d_var.C,v 1.1 2000/08/31 16:22:03 mmeyer Exp $
//
// Module name:		d_var.C
// Contents:		member functions of the class d_var
//			implementing a discretesation for a qreal variable 
//
// Author:		Uli Middelberg
// Last modified:	Wed Jun 18 15:37:53 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "d_var.h"

void d_var::dom_error ( qreal x ) {
    std::cout << "DOMAIN ERROR: "<< x << " is not in [" <<  x_qreal_min << "," <<x_qreal_max << "]" << std::endl;
  };

// constructor with default values
d_var::d_var ( qreal x_r = 0.5 , qreal x_rmin = 0 ,
               qreal x_rmax = 1 , int res = 100 ):
               x_qreal(x_r), x_qreal_min(x_rmin), x_qreal_max(x_rmax) {
    set_res(res);
  };

// returns 1, if x is between min and max, 0 otherwise
int  d_var::is_in_range ( qreal x ) const {
    return ( x >= x_qreal_min && x <= x_qreal_max );
  };

// returns 1, if x_descr is between 0 and dflt_res, 0 otherwise
int  d_var::is_in_range ( int x_descr ) const {	
    return ( x_descr >=0 && x_descr < dflt_res );
  };

// sets a new qreal value
void d_var::set_val( qreal x ) { 
    if( is_in_range(x) ) {
      x_qreal = x;
    } else {
      dom_error(x);
    }
  };
         
// sets a new qreal value with respect to the currrent discretesation
void d_var::set_val( int x_descr ) {
    if( is_in_range(x_descr) ) {
      x_qreal = (x_descr * dflt_step_sz) + (dflt_step_sz * 0.5);
    } else {
      dom_error( (qreal) x_descr);
    }
  };

// sets a new qreal value with respect to a new discretesation
void d_var::set_val( int x_descr, int resolution ) {
  int res = dflt_res;
    set_res(resolution);
    set_val(x_descr);
    set_res(res);
  };

// sets a new discretesation
void d_var::set_res( int resolution ) {
    dflt_res = resolution;
    dflt_step_sz =  (x_qreal_max - x_qreal_min) / dflt_res;
  };
 
// returns a qreal with the current value
qreal d_var::get_val( void ) const { 
    return x_qreal;
  };

// returns an int with the discrete current value
int d_var::get_dval( void ) const { 
    if( x_qreal == x_qreal_max ) {
      return (dflt_res -1);
    } else {
      return (int) ((x_qreal - x_qreal_min) / dflt_step_sz);
    }
  };
         
// returns an int with the discrete current value with respect to a new discretesation
int d_var::get_dval( int resolution ) {
    int res = dflt_res;
    set_res(resolution);
    int ret = get_dval();
    set_res(res);
    return ret;
  } ;

// returns the current resolution for the discretesation
int d_var::get_res( void ) const {
    return dflt_res;
  };

// eof
