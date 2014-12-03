///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/histo.C,v 1.1 2000/08/31 16:23:07 mmeyer Exp $
//
// Module name:		histo.C
// Contents:		member functions of the classes histo_1d and histo_2d
//			counting a 1d sample over a discrete space
//
// Author:		Uli Middelberg
// Last modified:	Wed Jun 18 15:37:53 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#include "histo.h"

histo_1d::histo_1d ( qreal x_min = 0 , qreal x_max = 1 , int x_res = 100 ):
                     x(0,x_min,x_max,x_res) {
    hits = new int[x_res];
    reset();
  };
              
histo_1d::~histo_1d (void) {
    delete [] hits ;
  };
             
void histo_1d::reset ( void ) {
    for (int k=0; k<get_x_res(); k++) {
      hits[k]=0;
    }
    no_hits=0;
  };

void histo_1d::inc ( qreal val ) {
    if( x.is_in_range(val) ) {
      x.set_val(val);
      hits[x.get_dval()]++;
      no_hits++;
    }
  };
         
int histo_1d::operator() ( int cell ) const {
    if( x.is_in_range(cell) ) {
      return hits[cell];
    } else {
      return -1;
    }
  };
        
int histo_1d::get_x_res ( void ) const { 
    return x.get_res() ;
  };

void histo_1d::get_x_d_var ( d_var * to_get ) const {
    (*to_get) = x;
  };

int histo_1d::get_max_hits ( void ) const {
    int k, h_max(0);
    for( k=0; k<get_x_res(); k++) {
      h_max=MAX(h_max,hits[k]);
    }
    return h_max;
  };

int histo_1d::get_no_hits ( void ) const {
    return no_hits;
  };
  
histo_2d::histo_2d ( qreal x_min = 0 , qreal x_max = 1 , int x_res = 100,
                     qreal y_min = 0 , qreal y_max = 1 , int y_res = 100 ):
                     x(0,x_min,x_max,x_res),
                     y(0,y_min,y_max,y_res) {

    hits = new int * [x_res];
    for( int k=0; k<x_res; k++ ) {
      hits[k] = new int[y_res];
    }
    reset();
  };
              
histo_2d::~histo_2d (void) { 
    for( int k=0; k<get_x_res(); k++ ) {
      delete [] hits[k];
    }
    delete [] hits;
  };
             
void histo_2d::reset ( void ) {
    for( int k=0; k<get_x_res(); k++ ) {
      for( int l=0; l<get_y_res(); l++ ) {
        hits[k][l]=0;
      }
    }
    no_hits=0;
  };

void histo_2d::inc ( qreal x_val, qreal y_val ) {
    if( x.is_in_range(x_val) &&  y.is_in_range(y_val) ) {
      x.set_val(x_val);
      y.set_val(y_val);
      hits[x.get_dval()][y.get_dval()]++;
      no_hits++;
    }
  };
         
int histo_2d::operator () ( int x_cell, int y_cell ) const {
    if( x.is_in_range(x_cell) && y.is_in_range(y_cell)) {
      return hits[x_cell][y_cell];
    } else {
      return -1;
    }
  };
        
int histo_2d::get_x_res ( void ) const {
    return x.get_res() ; 
  };

int histo_2d::get_y_res ( void ) const {
    return y.get_res() ;
  };

void histo_2d::get_x_d_var ( d_var * to_get ) const {
    (*to_get) = x;
  };
  
void histo_2d::get_y_d_var ( d_var * to_get ) const { 
    (*to_get) = y;
  };

int histo_2d::get_max_hits ( void ) const {
    int k,l, h_max(0);
    for( k=0; k<get_x_res(); k++) {
      for( l=0; l<get_y_res(); l++) {
        h_max=MAX(h_max,hits[k][l]);
      }
    }
    return h_max;
  };

int histo_2d::get_no_hits ( void ) const {
    return no_hits;
  };


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		histo_3d
// Derived from:	
// Purpose:		counting a 3d sample over a discrete space
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

histo_3d::histo_3d ( qreal x_min = 0 , qreal x_max = 1 , int x_res = 100,
                     qreal y_min = 0 , qreal y_max = 1 , int y_res = 100,
		     qreal z_min = 0 , qreal z_max = 1 , int z_res = 100 ):
                     x(0,x_min,x_max,x_res),
                     y(0,y_min,y_max,y_res),
		     z(0,z_min,z_max,z_res) 
{
	hits = new int ** [x_res];
	for( int k=0; k<x_res; k++ ) {
		hits[k] = new int * [y_res];
		for( int l=0; l<y_res; l++ ) {
    			hits[k][l] = new int[z_res];
	    	}
	}
    	reset();
};
              
histo_3d::~histo_3d (void) 
{ 
	for( int k=0; k<get_x_res(); k++ ) {
		for ( int l=0; l<get_y_res(); l++ ){
			delete [] hits[k][l];
		}
		delete [] hits[k];
	}
	delete [] hits;
};
             
void histo_3d::reset ( void ) {
	for( int k=0; k<get_x_res(); k++ ) {
		for( int l=0; l<get_y_res(); l++ ) {
			for( int m=0; m<get_z_res(); m++ ){
				hits[k][l][m]=0;
			}
		}
	}
	no_hits=0;
};

void histo_3d::inc ( qreal x_val, qreal y_val, qreal z_val ) 
{
	if( x.is_in_range(x_val) &&  y.is_in_range(y_val) && z.is_in_range(z_val) ) {
		x.set_val(x_val);
		y.set_val(y_val);
		z.set_val(z_val);
		hits[x.get_dval()][y.get_dval()][z.get_dval()]++;
		no_hits++;
	}
};

         
int histo_3d::operator () ( int x_cell, int y_cell, int z_cell ) const 
{
	if( x.is_in_range(x_cell) && y.is_in_range(y_cell) && z.is_in_range(z_cell) ) {
		return hits[x_cell][y_cell][z_cell];
	} else {
		return -1;
	}
};
        
int histo_3d::get_x_res ( void ) const 
{
	return x.get_res() ; 
};

int histo_3d::get_y_res ( void ) const 
{
    	return y.get_res() ;
};

int histo_3d::get_z_res ( void ) const 
{
    	return z.get_res() ;
};

void histo_3d::get_x_d_var ( d_var * to_get ) const 
{
	(*to_get) = x;
};
  
void histo_3d::get_y_d_var ( d_var * to_get ) const 
{ 
	(*to_get) = y;
};

void histo_3d::get_z_d_var ( d_var * to_get ) const 
{ 
	(*to_get) = z;
};

int histo_3d::get_max_hits ( void ) const 
{
	int k,l,m, h_max(0);
	for( k=0; k<get_x_res(); k++) {
		for( l=0; l<get_y_res(); l++) {
      			for ( m=0; m<get_z_res(); m++) {
			        h_max=MAX(h_max,hits[k][l][m]);
			}
      		}
    	}
    	return h_max;
};

int histo_3d::get_no_hits ( void ) const 
{
	return no_hits;
};


// eof
