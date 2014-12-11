///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/histo.h,v 1.1 2000/08/31 16:23:09 mmeyer Exp $
//
// Module name:		histo.h
// Contents:		Class definition of the classes histo_1d and histo_2d
//			counting a sample over a discrete space
//
// Author:		Uli Middelberg
// Last modified:	Wed Jun 18 16:48:21 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#ifndef HISTO_INCLUDED
#define HISTO_INCLUDED

#include "d_var.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		histo_1d
// Derived from:	
// Purpose:		counting a 1d sample over a discrete space
//
// Author:		Uli Middelberg
// Last modified:	Wed Jun 18 16:56:31 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////
class histo_1d {

protected:

    d_var   x;		// discretesation for x
    int   * hits;	// counter for the hits
    int     no_hits;	// counter for the total number of hits

public:

    histo_1d ( qreal x_qMin, qreal x_max, int x_res );
              
   ~histo_1d (void);
             
    void reset ( void );
    void inc ( qreal val );
         
    int  operator() ( int cell ) const;        
    int  get_x_res ( void ) const;
    void get_x_d_var ( d_var * to_get ) const;
    int  get_max_hits ( void ) const;
    int  get_no_hits ( void ) const;

}; // of class histo_1d

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		histo_2d
// Derived from:	
// Purpose:		counting a 2d sample over a discrete space
//
// Author:		Uli Middelberg
// Last modified:	Wed Jun 18 16:56:31 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class histo_2d {

protected:

    d_var   x;		// discretesation for x
    d_var   y;		// discretesation for y
    int * * hits;	// counter for the hits
    int     no_hits;	// counter for the total number of hits

public:

    histo_2d ( qreal x_qMin, qreal x_max, int x_res,
               qreal y_qMin, qreal y_max, int y_res );

   ~histo_2d (void);
             
    void reset ( void );
    void inc ( qreal x_val, qreal y_val );
    int operator () ( int x_cell, int y_cell ) const;    
    int get_x_res ( void ) const;
    int get_y_res ( void ) const;
    void get_x_d_var ( d_var * to_get ) const;
    void get_y_d_var ( d_var * to_get ) const; 
    int get_max_hits ( void ) const;
    int get_no_hits ( void ) const;
}; // of class histo_2d

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

class histo_3d {

protected:

    d_var   x;		// discretesation for x
    d_var   y;		// discretesation for y
    d_var   z;		// discretesation for z
    int * * * hits;	// counter for the hits
    int     no_hits;	// counter for the total number of hits

public:

    histo_3d ( qreal x_qMin, qreal x_max, int x_res,
               qreal y_qMin, qreal y_max, int y_res,
	       qreal z_qMin, qreal z_max, int z_res );

   ~histo_3d (void);
             
    void reset ( void );
    void inc ( qreal x_val, qreal y_val, qreal z_val );
    int operator () ( int x_cell, int y_cell, int z_cell ) const;    
    int get_x_res ( void ) const;
    int get_y_res ( void ) const;
    int get_z_res ( void ) const;
    void get_x_d_var ( d_var * to_get ) const;
    void get_y_d_var ( d_var * to_get ) const;
    void get_z_d_var ( d_var * to_get ) const; 
    int get_max_hits ( void ) const;
    int get_no_hits ( void ) const;
}; // of class histo_3d

#endif

// eof
