///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Jobs/RCS/max_lyapunov_exp.h,v 1.1 2000/08/31 16:24:09 mmeyer Exp $
//
// Module name:		max_lyapunov_exp.h
// Contents:		Class definition of the class max_lyapunov_exp
//			compute the approximation of the maximal
//			lyapunov exponent of a time series
//
// Author:		Uli Middelberg
// Last modified:	Tue Jul  1 19:13:02 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _MAX_LYAPUNOV_EXP_INCLUDED
#define _MAX_LYAPUNOV_EXP_INCLUDED

#include	"d_var.h"
#include	"geometricJob.h"
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		max_lyapunov_exp
// Derived from:	geometric job 
// Purpose:		compute the approximation of the maximal
//			lyapunov exponent of a time series
//
// Author:		Uli Middelberg
// Last modified:	Thu Jun 19 17:02:59 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class max_lyapunov_exp_1d : public geometricJob {
  protected:
    const qreal  eps;		// criterion to be positive or negative
    qreal  sum;	        	// current sum
    qreal  l_exp;		// current l_exp after evaluate
    

    qreal  stepX;

    qreal * x_prime;		// pointer to the derivation
				// is obtained by setLabels("x_prime");
				
    xyRange * n_axes;		// we have to manipulate the xy_range
				// because, the y-axes is always reserved
				// for the lyapunov exponent        
    void  update_sum(void);
    void  evaluate(void);
    void  evaluate( long t );
    
  public:
    max_lyapunov_exp_1d(baseModel* const, xyRange&, MacrodynGraphicsItem* const=NULL,
		       printer* const=NULL);
    virtual ~max_lyapunov_exp_1d();
    void simulation();
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		max_lyapunov_exp_2d
// Derived from:	geometric job 
// Purpose:		compute the approximation of the maximal
//			lyapunov exponent of a time series
//
// Author:		Uli Middelberg
// Last modified:	Thu Jun 19 17:02:59 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class max_lyapunov_exp_2d : public max_lyapunov_exp_1d {
  protected:

    qreal  stepY;
        
    int		get_encoding(void);	// the interval between -2 and 1
    const int	n_colors;		// will be devided into n_color parts
    d_var	* color_code;		// each part is encoded with a different
    					// color
    
  public:
    max_lyapunov_exp_2d(baseModel* const, xyRange&, MacrodynGraphicsItem* const=NULL,
		       printer* const=NULL);
    void simulation(void);
};


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		max_lyapunov_exp_t
// Derived from:	max_lyapunov_exp_1d
// Purpose:		display the approximation of the maximal
//			lyapunov exponent of a time series
//
// Author:		Uli Middelberg
// Last modified:	Tue Jul  1 18:46:52 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class max_lyapunov_exp_t : public max_lyapunov_exp_1d {
  public:
    max_lyapunov_exp_t(baseModel* const, xyRange&, MacrodynGraphicsItem* const=NULL,
		       printer* const=NULL);
    void simulation(void);
};


#endif	// _MAX_LYAPUNOV_EXP_INCLUDED
//eof
