#include "uniform_distri.h"
#include "pareto_distri.h"
#include <math.h>
#include "logger.h"


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       pareto_Distri
//
//  Member function:  pareto_Distri
//
//  Purpose:          default KOnstruktor
//
//
//  Created:          2001-12-23 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

pareto_Distri::pareto_Distri(const qreal lam, const qreal sca )
: prob_Distri(0), lambda(lam), scale(sca)
{
    if (scale <= 0.0)
        fatalError("pareto_Distri::pareto_Distri","scale must be positive");
    if (lambda <= 0.0)
        fatalError("pareto_Distri::pareto_Distri","lambda must be positive");
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       pareto_Distri
//
//  Member function:  density
//
//  Purpose:          Pareto probability density function on R
//
//
//  Created:          2001-12-23 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline qreal pareto_Distri::density(const qreal arg) const
{
    return  arg>=scale ? lambda/scale * pow(scale/arg,lambda+1.0) : 0.0 ;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       pareto_Distri
//
//  Member function:  cumulat
//
//  Purpose:          cumulative probability function for Pareto density on R
//
//
//  Created:          2001-12-23 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline qreal pareto_Distri::cumulat(const qreal arg) const
{
    return  arg<scale ? 0.0 : 1.0 - pow(scale/arg,lambda);
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       pareto_Distri
//
//  Member function:  inverse
//
//  Purpose:          inverse cumulative probability function for Pareto
//                    distribution
//                    Argument must be in ]0,1[
//
//
//  Created:          2001-12-27 (Achim Flammenkamp)
//
//  Modified:         2001-12-23 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline qreal pareto_Distri::inverse(const qreal arg) const
{
    if (arg < 0.0 || arg >= 1.0)
        fatalError("pareto_Distri::inverse","parameter out of range");
    return  scale/pow(1.0-arg,1.0/lambda);
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       pareto_Distri
//
//  Member function:  random
//
//  Purpose:          random value in R with pareto probability distribution
//
//
//  Created:          2001-12-23 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

qreal pareto_Distri::random()
{
    return  random(lambda,scale);
}

qreal pareto_Distri::random(const qreal lam, const qreal sca)
{
    if (sca <= 0.0)
        fatalError("pareto_Distri::random","scale must be positive");
    if (lam <= 0.0)
        fatalError("pareto_Distri::random","lambda must be positive");
    static uniform_Distri  uni_distribution;
    uni_distribution.swap_state(state);
    qreal  arg = uni_distribution.random();
    uni_distribution.swap_state(state);

    return  sca/pow(1.0-arg,1.0/lam);
}
