#include "uniform_distri.h"
#include "geometric_distri.h"
#include <math.h>
#include "error.h"


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       geometric_Distri
//
//  Member function:  geometric_Distri
//
//  Purpose:          default Kinstruktor
//
//
//  Created:          2001-12-14 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

geometric_Distri::geometric_Distri(const qreal pro ) :
prob_Distri(0), prob(pro)
{
    if (prob <= 0.0 || prob >= 1.0)
        fatalError("geometric_Distri::geometric_Distri","probability is >= 1 or <= 0");
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       geometric_Distri
//
//  Member function:  density
//
//  Purpose:          geometric probability density function on N
//
//
//  Created:          2001-12-14 (Achim Flammenkamp)
//
//  Modified:         2001-12-14 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline qreal geometric_Distri::density( const int arg) const
{
    return  arg <= 0 ? 0.0 : (1.0-prob) * pow(prob,arg-1);
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       geometric_Distri
//
//  Member function:  cumulat
//
//  Purpose:          cumulative geometric probability function on N
//
//
//  Created:          2001-12-14 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline qreal geometric_Distri::cumulat(const int arg) const
{
    return  arg <= 0 ? 0.0 : 1.0 - pow(prob,arg);
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       geometric_Distri
//
//  Member function:  inverse
//
//  Purpose:          inverse cumulative geometric probability function on N
//                    Argument must be in ]0,1[
//
//
//  Created:          2001-12-14 (Achim Flammenkamp)
//
//  Modified:         2001-12-14 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

qreal geometric_Distri::inverse(const qreal arg) const
{
   if (arg < 0.0 || arg >= 1.0)
       fatalError("geometric_Distri::inverse","parameter out of range");

   return  log(1.0-arg)/log(prob) ;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       geometric_Distri
//
//  Member function:  random
//
//  Purpose:          random value geometric distributed on N
//
//
//  Created:          2001-12-14 (Achim Flammenkamp)
//
//  Modified:         2001-12-14 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

int geometric_Distri::random()
{
    return  random(prob);
}

int geometric_Distri::random(const qreal pro)
{
    if (pro <= 0.0 || pro >= 1.0)
        fatalError("geometric_Distri::random","probability is >= 1 or <= 0");
    if (pro == 0.5)
    {
        static randomBits  generator;
        random_state & safe = generator.attach_state(state);
        unsigned dummy=  generator.search_next_1_bit(true);
        generator.attach_state(safe);

        return  int( dummy+1 );
    }
    else
    {
        static uniform_Distri  uni_distribution;
        uni_distribution.swap_state(state);
        qreal  arg = uni_distribution.random();
        uni_distribution.swap_state(state);

        return  int( ceil( log(1.0-arg)/log(pro) ) );
    }
}
