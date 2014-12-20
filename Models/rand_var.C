///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/rand_var.C,v 1.1 2000/08/31 16:09:28 mmeyer Exp $
//
// Module name:		rand_var.C
// Contents:		random number generator for macrodyn
//
// Author:		Uli Middelberg
// Last modified:	Wed Feb  5 17:53:31 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#include "../Random/ranlib.h"
#include "../eval_expr.h"
#include "../get_expr.h"

#include "rand_var.h"
#include <time.h>
//default seed:
//static const qint64 sd1 = 2036452194;
//static const qint64 sd2 = 1194132260;

//alternative seed1 (omega_1)

//static const qint64 sd1 = 2016478191;
//static const qint64 sd2 = 2112932563;

//alternative seed2 (omega_2)
//static const qint64 sd1 = 1816478191;
//static const qint64 sd2 = 1542932563;

//alternative seed3 (omega_3)
//static const qint64 sd1 = 1616478091;
//static const qint64 sd2 = 1342962563;

//alternative seed4 (omega_4)
//static const qint64 sd1 = 1716478091;
//static const qint64 sd2 = 1842962563;

//alternative seed5 (omega_5)
//static const qint64 sd1 = 1006478091;
//static const qint64 sd2 = 1358942563;

//degenerated seed for Jan's model
//static const qint64 sd1 = 40007730;
//static const qint64 sd2 = 11111111;

//alternative seed? (consecutive different seeds, process time dependened)

//#define sd1 rand()
//#define sd2 rand()
//#define INIT_RANDOM_TIME

///////////////////////////////////////////////////////////////////////////////
static double ranf_d ( void )		// a wrapper for the ranlib function
{
  return (double) ranf();
}

extern "C" { void inrgcm(void); }

static base_gen_t identify ( const QString& gen )
{
    if (gen == "drand48")
    {
      return drand48;
    }
    if (gen == "ranf")
    {
//      (void) ignlgi(); 		// initialize the generator
      #ifdef INIT_RANDOM_TIME
      srand( time(NULL) );
      #endif
//      setall(sd1,sd2);		// set the default seed values
      return ranf_d;
    }
    log() << "Generator couldn't be identified: "<< gen;
    return NULL;
}


///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rand_var
// Member function:	rand_var
// Purpose:		constructor
//
// Author:		Uli Middelberg
// Last modified:	Mon Jan 13 15:48:01 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

rand_var::rand_var ( baseModel * model, const QString& gen, const QString& zvar_expr )
{
    int n=0;
    QByteArray qba = zvar_expr.toLatin1();
    const char *pos  = qba.data();
    QByteArray qba2 = zvar_expr.toLatin1();
    char *expr = qba2.data();
    const char * tpos, * tmark;
    QByteArray qba3 = zvar_expr.toLatin1();
    char *token  = qba3.data();

    double sum = 0.0;

   base_gen = identify(gen);
  
  n_events = zvar_expr.count(';');
  prob = new double[n_events];
  range_min = new double[n_events];
  range_max = new double[n_events];
  while ( n < n_events && (pos = get_expr(pos,expr,';') ) ){	// the event
    tpos = expr;
    tpos = get_expr(tpos,token,'[');			// the probability
    prob[n] = eval_expr(model, token);

    tpos = get_expr(tpos,token,",:");			// the qMin value
    tmark = tpos-1;
    range_min[n] = eval_expr(model, token);
 
    tpos = get_expr(tpos,token,']');			// the qMax value
    range_max[n] = eval_expr(model, token);

    switch (*tmark) {			// postprocessing

      case ':' :			// in case of `:' , the qMax value
        range_min[n]-=range_max[n];	// defines the radius of the interval
        range_max[n]+=range_max[n]+range_min[n];	// and the qMin value
      break;				// defines the midpoint
    
      case ',' :		// ensure that the 
        double m_in = qMin(range_min[n],range_max[n]);
        double m_ax = qMax(range_min[n],range_max[n]);
        range_min[n] = m_in;	// lower value is in range_min
        range_max[n] = m_ax;	// higher value is in range_max
      break;
    }
    
    n++;
  }
  
  for( n=0; n<n_events; n++ ) { 	// now we do a rescale / normalization
    sum += prob[n];			// if the sum of the
  }
  if( sum != 1.0 ) {			// probabilities is not equal to 1
    for( n=0; n<n_events; n++ ) {
      prob[n] /= sum;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rand_var
// Member function:	rand_var
// Purpose:		constructor for one event
//
// Author:		24.01.1999 by Marc Mueller
// Last modified:	24.01.1999 by Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
rand_var::rand_var(const char * gen,const qreal& probab,const qreal& min,const qreal& max) {

    base_gen = identify(gen);
	prob = new double[1];
    range_min = new double[1];
    range_max = new double[1];

	prob[0] = probab;
    range_min[0] = qMin(min, max);
    range_max[0] = qMax(min, max);
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rand_var
// Member function:	~rand_var
// Purpose:		destructor
//
// Author:		Uli Middelberg
// Last modified:	Mon Jan 13 15:48:01 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

rand_var::~rand_var ( void )
{
  delete [] prob;
  delete [] range_min;
  delete [] range_max;
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rand_var
// Member function:	dice
// Purpose:		computes the next qrealisation of the random variable
//
// Author:		Uli Middelberg
// Last modified:	Mon Jan 13 15:48:01 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

double rand_var::dice ( void )
{   double rnd;
    int    n=0;

  rnd = ( * base_gen) ();			// 0 < rnd < 1
  while ( n < n_events && rnd > prob[n] ) {	// find the right event
    rnd -= prob[n];
    n++;
  }
  if ( range_min[n] == range_max[n] ) {
    return (range_min[n]);
  } else {
    return ( (rnd / prob[n] * (range_max[n] - range_min[n]) ) + range_min[n] );
  }
}

// eof
