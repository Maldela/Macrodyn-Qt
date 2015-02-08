#include "uniform_distri.h"
#include "exponential_distri.h"
#include <math.h>
#include "logger.h"


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       exponential_Distri
//
//  Member function:  density
//
//  Purpose:          default Konstruktor
//
//
//  Created:          2001-12-23 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

exponential_Distri::exponential_Distri(const qreal sca, const qreal lef ):
prob_Distri(0) , scale(sca), left(lef)
{
    if (scale <= 0.0)
        fatalError("exponential_Distri::exponential_Distri","parameter scale must be positive");
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       exponential_Distri
//
//  Member function:  density
//
//  Purpose:          exponential probability density function on R
//
//
//  Created:          2001-12-23 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline qreal exponential_Distri::density(const qreal arg) const
{
    return  arg<left ? 0.0 : 1.0/scale * exp(-(arg-left)/scale);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       exponential_Distri
//
//  Member function:  cumulat
//
//  Purpose:          cumulative probability function for Exponential density on R
//
//
//  Created:          2001-12-23 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline qreal exponential_Distri::cumulat(const qreal arg) const
{
    return  arg<left ? 0.0 : 1.0 - exp(-(arg-left)/scale);
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       exponential_Distri
//
//  Member function:  inverse
//
//  Purpose:          inverse cumulative probability function for exponential
//                    distribution
//                    Argument must be in [0,1[
//
//
//  Created:          2001-12-23 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline qreal exponential_Distri::inverse(const qreal arg) const
{ 
    if (arg < 0.0 || arg >= 1.0)
        fatalError("exponential_Distri::inverse","parameter out of range");
    return  left-scale/log(1.0-arg);
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       exponential_Distri
//
//  Member function:  random
//
//  Purpose:          random value in R with exponential probability distribution
//
//
//  Created:          2001-12-23 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

qreal exponential_Distri::random()
{
    return  random(scale,left);
}

qreal exponential_Distri::random(const qreal sca, const qreal lef)
{
    if (sca <= 0.0)
        fatalError("exponential_Distri::random","parameter scale must be positive");
    static uniform_Distri  uni_distribution;
    uni_distribution.swap_state(state);
    qreal  arg = uni_distribution.random();
    uni_distribution.swap_state(state);

    return  lef-sca/log(1.0-arg);
}
