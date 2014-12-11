///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/d_var.h,v 1.1 2000/08/31 16:22:06 mmeyer Exp $
//
// Module name:		d_var.h
// Contents:		Class definition of the class d_var
//			implementing a discretesation for a qreal variable
//
// Author:		Uli Middelberg
// Last modified:	Wed Jun 18 15:37:53 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#ifndef D_VAR_INCLUDED
#define D_VAR_INCLUDED

#include "../sim.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		d_var
// Derived from:	
// Purpose:		defines a qreal variable with a discrete integer
// 			representation
//
// Author:		Uli Middelberg
// Last modified:	Thu Jun  5 11:16:41 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class d_var {

protected:

    qreal x_qreal;	// the actual value of x
    qreal x_real_min;	// bounds for the discrete
    qreal x_real_max;	// representation of x
    int  dflt_res;	// default resolution
    qreal dflt_step_sz;	// default step size

    void dom_error ( qreal x );	// print an error message, but do not exit
public:
    
    d_var ( qreal x_r, qreal x_min,	// constructor with default values
            qreal x_max, int res );

    int  is_in_range ( qreal x ) const;	// returns 1, if x is between qMin and qMax, 0 otherwise
    int  is_in_range ( int x_descr ) const;	// returns 1, if x_descr is between 0 and dflt_res, 0 otherwise

    void set_val( qreal x );		// sets a new qreal value
    void set_val( int x_descr );	// sets a new qreal value with respect to the currrent discretesation
    void set_val( int x_descr, int resolution );	// sets a new qreal value with respect to a new discretesation
    void set_res( int resolution );	// sets a new discretesation
    qreal get_val( void ) const;		// returns a qreal with the current value
    int  get_dval( void ) const;	// returns an int with the discrete current value
    int  get_dval( int resolution );	// returns an int with the discrete current value with respect to a new discretesation  
    int  get_res( void ) const;		// returns the current resolution for the discretesation
}; // of class d_var

#endif

// eof
