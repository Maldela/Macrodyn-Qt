///////////////////////////////////////////////////////////////////////////////
//
// $Header: 		distribution.h
//
// Module name:		distribution.h
// Contents:		
//
// Author:			Mathias Hoffmann & Michael Meyer
// Last modified:	Wed Sep 27 14:58:59 METDST 2000
// By:				Mathias Hoffmann & Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _DISTRIBUTION_INCLUDED
#define _DISTRIBUTION_INCLUDED

#include "baseModel.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		distribution
// Derived from:	
// Purpose:			
//
// Author:			Mathias Hoffmann & Michael Meyer
// Last modified:	22.03.05
// By:				Andreas Starke
//
///////////////////////////////////////////////////////////////////////////////


class distribution {
	protected:
		time_t t;			
	public:
		distribution();
		~distribution();
		void newseed();
		void setseed(unsigned int val);
		real rectangular();
		real triangular(const real, const real, const real);
		real st_normal();
		real chi_square(const int);
		real st_cauchy();
		real student_t(const int);
		real exponential(const real);
		real pareto(const real, const real);
};
#endif	// _DISTRIBUTION_INCLUDED
// eof


