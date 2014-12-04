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
        void setseed(uint val);
		qreal rectangular();
		qreal triangular(const qreal, const qreal, const qreal);
		qreal st_normal();
		qreal chi_square(const int);
		qreal st_cauchy();
		qreal student_t(const int);
		qreal exponential(const qreal);
		qreal pareto(const qreal, const qreal);
};
#endif	// _DISTRIBUTION_INCLUDED
// eof


