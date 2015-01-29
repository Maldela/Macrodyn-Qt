#include "uniform_distri.h"
#include <math.h>
#include "logger.h"


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       uniform_Distri
//
//  Member function:  uniform_Distri
//
//  Purpose:          default Konstruktor
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

uniform_Distri::uniform_Distri(const qreal cen , const qreal wid )
: prob_Distri(0), center(cen), width(wid)
{
    if (width <= 0.0)
        fatalError("uniform_Distri::uniform_Distri","width must be positive");
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       uniform_Distri
//
//  Member function:  density
//
//  Purpose:          normalized uniform probability density function on an 
//                    open interval in R.  It is 0 if outside the interval
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline qreal uniform_Distri::density(const qreal arg) const
{
   return  arg <= center-0.5*width || arg >= center+0.5*width ? 0.0 : 1.0/width;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       uniform_Distri
//
//  Member function:  cumulat
//
//  Purpose:          cumulative probability function for uniform density on
//                    normalized uniform probability density function on an 
//                    open interval in R.
//                    0  for argument <= 0.0   and   1.0 for argument >= 1.0
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline qreal uniform_Distri::cumulat(const qreal arg) const
{
    if (arg <= center-0.5*width)  return 0.0;
    if (arg >= center+0.5*width)  return 1.0;
    return  0.5 + (arg-center)/width;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       uniform_Distri
//
//  Member function:  inverse
//
//  Purpose:          inverse cumulative probability function for uniform
//                    density on ]0,1[
//                    Argument must be in [0,1]
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline qreal uniform_Distri::inverse(const qreal arg) const
{
    if (arg < 0.0 || arg > 1.0)
        fatalError("uniform_Distri::inverse","1. parameter out of range");
    return  width*(arg-0.5)+center;
}



///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       uniform_Distri
//
//  Member function:  random
//
//  Purpose:          random value in ]0,1[ with uniform probability density 
//                    perfect use of the availible bits of the base generator
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////


qreal uniform_Distri::random()
{
    return  random(center,width);
}

qreal uniform_Distri::random(const qreal cen, const qreal wid)
{
    if (wid <= 0.0)
        fatalError("uniform_Distri::random","width must be positive");
    const uint   mantissa_words = (mantissa_bits + bits_per_word - 1)
                                     / bits_per_word;
    static uint  *words = new uint [mantissa_words];

    for (uint  i=0;i < mantissa_words ;i++)
        words[i] = 0;
    static randomBits  generator;
    random_state & safe = generator.attach_state(state);
    uint  skiped_bits = generator.search_next_1_bit(false);
    generator.random_bits(mantissa_bits,words,0);
    generator.attach_state(safe);
    if (!(words[0]&1))
        fatalError("uniform_Distri::random","first bit not set");
    if (!(words[0]&~(~0U>>1)))  //  (int)words[0] >= 0)  // highest bit not set
        words[0] &= ~1U, words[0] |= ~(~0U>>1);  // exchange highest with lowest
    double arg = 0.0;
    for (uint  defined_bits=0, i=0; i < mantissa_words ;i++)
    {   uint  next_bits = defined_bits + bits_per_word > mantissa_bits ?
                              mantissa_bits - defined_bits : bits_per_word ;
        arg= ldexp(arg,next_bits);
        arg += double(words[i]);
        defined_bits += next_bits;
    }
    arg= ldexp(arg, -int(mantissa_bits+skiped_bits));
    return  wid*(arg-0.5)+cen;
}
