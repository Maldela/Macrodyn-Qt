///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/rand_var.h,v 1.1 2000/08/31 16:09:31 mmeyer Exp $
//
// Module name:		rand_var.h
// Contents:		random number generator for macrodyn
//
// Author:		Uli Middelberg
// Last modified:	Tue Jan 21 15:27:34 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _RAND_VAR_INCLUDED
#define _RAND_VAR_INCLUDED

#include "baseModel.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rand_var
// Derived from:	-
// Purpose:		random number generator for RDS
//
// Author:		Uli Middelberg
// Last modified:	Mon Jan 13 15:48:01 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

typedef double (* base_gen_t ) (void);

class rand_var
{
  protected:
    base_gen_t	base_gen;		// Basic Random Number Generator
    int n_events;			// number of events
    double * prob;			// the probability for each event
    double * range_min;			// the left and
    double * range_max;			// the right interval bound
    
  public:
    rand_var ( baseModel* model,const QString& gen,const QString& zvar_expr);// constructor
    rand_var ( const char* gen,const qreal& probab,const qreal& qMin,const qreal& qMax);
    ~rand_var ( void );			// distructor

    double dice (void);			// returns the next dice
};

#endif	// _RAND_VAR_INCLUDED
// eof
