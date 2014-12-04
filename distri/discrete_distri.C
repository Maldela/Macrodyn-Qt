#include "discrete_distri.h"
#include <math.h>
#include "error.h"
//  #include <iostream>  // debugging


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       discrete_Distri
//
//  Member function:  density
//
//  Purpose:          default Konstruktor
//
//
//  Created:          2001-12-14 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

discrete_Distri::discrete_Distri(const int wid, const int sta ) :
 prob_Distri(0), width(wid), start(sta)
{
    if (int(width) <= 0)
        fatalError("cauchy_Distri::discrete_Distri","width must be positive");
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       discrete_Distri
//
//  Member function:  density
//
//  Purpose:          discrete uniform probability density function on a finite
//                    interval in Z
//
//
//  Created:          2001-12-14 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline qreal discrete_Distri::density(const int arg) const
{
    if ( arg < start || arg > start+int(width))
        return  0.0;
    else
        return  1.0 / qreal(width);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       discrete_Distri
//
//  Member function:  cumulat
//
//  Purpose:          cumulative probability function for discrete uniform
//                    distribution on a finite interval in Z
//
//
//  Created:          2001-12-14 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline qreal discrete_Distri::cumulat(const int arg) const
{
    if ( arg < start )
        return  0.0;
    else if  (arg > start+int(width))
        return  1.0;
    else
        return  qreal(arg-start+1) / qreal(width);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       discrete_Distri
//
//  Member function:  inverse
//
//  Purpose:          inverse cumulative probability function for discrete
//                    uniform distribution on a finite interval in Z
//                    Argument must be in ]0,1[
//
//
//  Created:          2001-12-14 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline qreal discrete_Distri::inverse(const qreal arg) const
{
   if (arg < 0.0 || arg > 1.0)
       fatalError("discrete_Distri::inverse","parameter out of range");

   return  qreal(start-1) + width*arg;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       discrete_Distri
//
//  Member function:  random
//
//  Purpose:          random value uniform distributed on a finite interval in Z
//
//
//  Created:          2001-12-14 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

int discrete_Distri::random()
{
    return  random(width,start);
}

int discrete_Distri::random(const int wid, const int sta)
{
    if (wid <= 0)
        fatalError("cauchy_Distri::random","width must be positive");
    uint  bits=0;
    for (uint  i=1; i < uint(wid)  &&  i; i <<= 1)
        bits ++;
    uint  arg;
    static randomBits  generator;
    random_state & safe = generator.attach_state(state);
    //  for given  width  we have  2^bits/width < 2  tries on average
    //  and on random width  we have < 2*ln(2) < 1.3863  tries on average
    do {
       arg= 0;
       generator.random_bits(bits,&arg); 
    } while (arg >= uint(wid)); 
    generator.attach_state(safe);
    return  sta + int(arg);
}
