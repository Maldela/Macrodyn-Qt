///////////////////////////////////////////////////////////////////////////////
//
// Module name:		floathisto_2d.C
// Contents:		member functions of the class floathisto_2d
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

#include "floathisto2d.h"

///////////////////////////////////////////////////////////////////////////////
//
// member function:	floathisto_2d
// purpose:		constructor
//
// author:		Andreas Starke
// last modified:
//
///////////////////////////////////////////////////////////////////////////////
  
floathisto_2d::floathisto_2d ( qreal x_qMin = 0 , qreal x_max = 1 , int x_res = 100,
                     qreal y_qMin = 0 , qreal y_max = 1 , int y_res = 100 ):
                     x(0,x_qMin,x_max,x_res),
                     y(0,y_qMin,y_max,y_res) {

    hits = new double * [x_res];
    for( int k=0; k<x_res; k++ ) {
      hits[k] = new double[y_res];
    }
    reset();
  };

///////////////////////////////////////////////////////////////////////////////
//
// member function:	~floathisto_2d
// purpose:		destructor
//
// author:		Andreas Starke
// last modified:
//
///////////////////////////////////////////////////////////////////////////////

              
floathisto_2d::~floathisto_2d (void) { 
    for( int k=0; k<get_x_res(); k++ ) {
      delete [] hits[k];
    }
    delete [] hits;
  };

///////////////////////////////////////////////////////////////////////////////
//
// member function:	reset
// purpose:		reset values of histogramm
//
// author:		Andreas Starke
// last modified:
//
///////////////////////////////////////////////////////////////////////////////

            
void floathisto_2d::reset ( void ) {
    for( int k=0; k<get_x_res(); k++ ) {
      for( int l=0; l<get_y_res(); l++ ) {
        hits[k][l]=0;
      }
    }
  };

///////////////////////////////////////////////////////////////////////////////
//
// member function:	inc
// purpose:		
//
// author:		Andreas Starke
// last modified:
//
///////////////////////////////////////////////////////////////////////////////


void floathisto_2d::set_to ( qreal x_val, qreal y_val, double value ) {
//    if( x.is_in_range(x_val) &&  y.is_in_range(y_val) ) {
      x.set_val(x_val);
      y.set_val(y_val);
      hits[x.get_dval()][y.get_dval()]=value;
//    }
  };
         
///////////////////////////////////////////////////////////////////////////////
//
// member function:	operator()
// purpose:		overload operator()
//
// author:		Andreas Starke
// last modified:
//
///////////////////////////////////////////////////////////////////////////////

	 
double floathisto_2d::operator () ( int x_cell, int y_cell ) const {
    if( x.is_in_range(x_cell) && y.is_in_range(y_cell)) {
      return hits[x_cell][y_cell];
    } else {
      return -1;
    }
  };
  
double floathisto_2d::operator () ( qreal x_value, qreal y_value ) {
    if( x.is_in_range(x_value) && y.is_in_range(y_value)) {
    	x.set_val(x_value);
	y.set_val(y_value);
      return hits[x.get_dval()][y.get_dval()];
    } else {
      return -1;
    }
  };
        
///////////////////////////////////////////////////////////////////////////////
//
// member function:	get_x_res
// purpose:		return the set x resolution
//
// author:		Andreas Starke
// last modified:
//
///////////////////////////////////////////////////////////////////////////////
	
	
int floathisto_2d::get_x_res ( void ) const {
    return x.get_res() ; 
  };

///////////////////////////////////////////////////////////////////////////////
//
// member function:	get_y_res
// purpose:		return the set y resolution
//
// author:		Andreas Starke
// last modified:
//
///////////////////////////////////////////////////////////////////////////////


int floathisto_2d::get_y_res ( void ) const {
    return y.get_res() ;
  };
  
///////////////////////////////////////////////////////////////////////////////
//
// member function:	get_x_d_var
// purpose:		return discretisation for x
//
// author:		Andreas Starke
// last modified:
//
///////////////////////////////////////////////////////////////////////////////


void floathisto_2d::get_x_d_var ( d_var * to_get ) const {
    (*to_get) = x;
  };
  
///////////////////////////////////////////////////////////////////////////////
//
// member function:	get_y_d_var
// purpose:		return discretisation for y
//
// author:		Andreas Starke
// last modified:
//
///////////////////////////////////////////////////////////////////////////////
  
  
void floathisto_2d::get_y_d_var ( d_var * to_get ) const { 
    (*to_get) = y;
  };
  
///////////////////////////////////////////////////////////////////////////////
//
// member function:	get_max_hits
// purpose:		
//
// author:		Andreas Starke
// last modified:
//
///////////////////////////////////////////////////////////////////////////////


double floathisto_2d::get_max ( void ) const {
    int k,l;
    double histqMax=0;
    for( k=0; k<get_x_res(); k++) {
      for( l=0; l<get_y_res(); l++) {
        histqMax = qMax(histqMax,hits[k][l]);
      }
    }
    return histqMax;
  };

double floathisto_2d::get_qMin ( void ) const {
    int k,l;
    double histqMin=0;
    for( k=0; k<get_x_res(); k++) {
      for( l=0; l<get_y_res(); l++) {
        histqMin = qMin(histqMin,hits[k][l]);
      }
    }
    return histqMin;
  };
  
// eof
