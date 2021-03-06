///////////////////////////////////////////////////////////////////////////////
//
// Module name:		floathisto2d.h
// Contents:		Class definition of the class floathisto_2d
//			saving space of variable values used to
//			colorate a picture after (total) computation of 
//			value range
//
// Author:		Andreas Starke
// Last modified:				
//
///////////////////////////////////////////////////////////////////////////////

#ifndef FLOATHISTO_INCLUDED
#define FLOATHISTO_INCLUDED

#include "d_var.h"

class floathisto_2d {

protected:

    d_var   x;			// discretesation for x
    d_var   y;			// discretesation for y
    double * * hits;		// counter for the hits

public:

    floathisto_2d ( qreal x_qMin = 0 , qreal x_max = 1 , int x_res = 100,
                    qreal y_qMin = 0 , qreal y_max = 1 , int y_res = 100 );

   ~floathisto_2d (void);
             
    void reset ( void );
    void set_to ( qreal x_val, qreal y_val, double value );
    double operator () ( int x_cell, int y_cell ) const;
    double operator () ( qreal x_value, qreal y_value) ;    
    int get_x_res ( void ) const;
    int get_y_res ( void ) const;
    void get_x_d_var ( d_var * to_get ) const;
    void get_y_d_var ( d_var * to_get ) const; 
    double get_max ( void ) const;
    double get_qMin ( void ) const;
}; 

#endif

// eof
