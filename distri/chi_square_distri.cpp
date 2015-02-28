#include "uniform_distri.h"
#include "normal_distri.h"
#include "chi_square_distri.h"
#include <math.h>
#include "logger.h"
const uint  n_max = 32;  // qMaximal n_max/2 iterations in each loop


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       chi_square_Distri
//
//  Member function:  chi_square_Distri
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

chi_square_Distri::chi_square_Distri(const uint free) :
prob_Distri(0), n(free)
{
    if (!n)
        fatalError("chi_square_Distri::chi_square_Distri","n must be positive");
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       chi_square_Distri
//
//  Member function:  density
//
//  Purpose:          normalized normal probability density function on R^+_0
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

qreal chi_square_Distri::density(const qreal arg) const
{
    if (arg < 0.0)  return  0.0;
    qreal tmp = pow(sqrt(arg),n-2)*exp(-arg*0.5),  v;
    if (n >= n_max)
    {  qreal  m=qreal(0.5*n-1.0), m2 = 1.0/m/m;
       v = 2*sqrt(2.5066282746310005*m)*pow(2*m,m)*
           exp(-m*(1-m2/12*(1-m2/30*(1-m2/3.5*(1-3*m2/4*(1-99/140.0*m2))))));
    }    
    else
    {  v = (n&1) ? 2.5066282746310005024 : 2.0;
       for (int  i=n-2; i > 0 ; i-=2 )
          v *= i;
    }
    return  tmp/v;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       chi_square_Distri
//
//  Member function:  random
//
//  Purpose:          random value in R with Chi^2 probability distribution
//                    see: D.E. Knuth SeqMinumerical Algorithm Vol. 2 Chap. 3.4.1
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

qreal chi_square_Distri::random()
{
    return  random(n);
}

qreal chi_square_Distri::random(const uint free)
{
    if (!free)
        fatalError("chi_square_Distri::random","number of free must be positive");
    qreal  arg;
    static  normal_Distri  norm = normal_Distri();
    static  uniform_Distri  uni = uniform_Distri();   // never 0.0
    if (free < n_max)
    { 
        arg = 1.0;
        uni.swap_state(state);
        for (int  i=free; i >= 2 ; i-=2 )
            arg *= uni.random();
        uni.swap_state(state);
        arg = -log(arg);   // Sum of free/2 exponential distributed values
        if (free&1)
        {
            norm.swap_state(state);
            qreal  v = norm.random();
            norm.swap_state(state);
            arg += v*v*0.5;
        }
    }
    else
    {
        qreal  m = 0.5*free-1, v;
        uni.swap_state(state);
        do {
            do {
                v = tan(M_PI*uni.random());
                arg = sqrt(free-1.0)*v + m;
            } while (arg <= 0.0);
        } while (uni.random() > (1.0+v*v)*pow(arg/m,m)/exp(sqrt(free-1.0)*v));
        uni.swap_state(state);
    }
    
    return  arg;
}
