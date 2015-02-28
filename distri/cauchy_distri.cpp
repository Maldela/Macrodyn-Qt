#include "uniform_distri.h"
#include "cauchy_distri.h"
#include <math.h>
#include "logger.h"


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       cauchy_Distri
//
//  Member function:  cauchy_Distri
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

cauchy_Distri::cauchy_Distri(const qreal med , const qreal sca ) :
 prob_Distri(0), median(med), scale(sca)
{
    if (scale <= 0.0)
        fatalError("cauchy_Distri::cauchy_Distri","scale must be positive");
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       cauchy_Distri
//
//  Member function:  density
//
//  Purpose:          Cauchy probability density function on R
//
//
//  Created:          2001-12-23 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline qreal cauchy_Distri::density(const qreal arg) const
{
    return  scale/(M_PI*(scale*scale+(arg-median)*(arg-median)));
}

///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       cauchy_Distri
//
//  Member function:  cumulat
//
//  Purpose:          cumulative probability function for Cauchy density on R
//
//
//  Created:          2001-12-23 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline qreal cauchy_Distri::cumulat(const qreal arg) const
{
    return  0.5 + atan((arg-median)/scale)/M_PI;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       cauchy_Distri
//
//  Member function:  inverse
//
//  Purpose:          inverse cumulative probability function for Cauchy
//                    distribution
//                    Argument must be in ]0,1[
//
//
//  Created:          2001-12-23 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline qreal cauchy_Distri::inverse(const qreal arg) const
{
    if (arg <= 0.0 || arg >= 1.0)
        fatalError("cauchy_Distri::inverse","parameter out of range");
    return  median+scale*tan(M_PI*(arg-0.5));
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       cauchy_Distri
//
//  Member function:  random
//
//  Purpose:          random value in R with Cauchy probability distribution
//
//
//  Created:          2001-12-23 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

qreal cauchy_Distri::random()
{
    return  random(median,scale);
}

qreal cauchy_Distri::random(const qreal med, const qreal sca)
{
    if (sca <= 0.0)
        fatalError("cauchy_Distri::random","scale must be positive");
    static uniform_Distri  uni_distribution;
    uni_distribution.swap_state(state);
    qreal  arg = uni_distribution.random();
    uni_distribution.swap_state(state);

    return  med+sca*tan(M_PI*(arg-0.5));
}
