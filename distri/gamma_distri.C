#include "uniform_distri.h"
#include "normal_distri.h"
#include "gamma_distri.h"
#include <math.h>
#include "error.h"
const unsigned  l_max = 40;  // maximal l_max/2 iterations in each loop


const double  gam=0.57721566490153286060651209;// Euler-Mac-Laurin'sche constant
//  bern[2k] = Zeta(2k) 2* (2k)! * cos(k*Pi) / (2*Pi)^(2k)
const double  bern[] = { 1.0, -1.0/2.0, 1.0/6.0, 0.0, -1.0/30.0, 0.0, 1.0/42.0,
             0.0, -1.0/30.0, 0.0, 5.0/66.0, 0.0, -691.0/2730.0, 0.0, 7.0/6.0,
             0.0, -3617.0/510.0, 0.0, 43867.0/798.0, 0.0, -174611.0/330.0 };


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       ---
//
//  Function:         log_gamma
//
//  Purpose:          logarithm of the Gamma-function for positive argument
//                    Copyright(C) 1991 Achim Flammenkamp (achim@hrz.uni-bielefeld.de)
//                    URL: http://www.uni-bielefeld.de/~achim/
//                    usage higher functions:  exp(), log()
//
//
//  Created:          1989-12-28 (Achim Flammenkamp)  // exactly 12 years ago :)
//
//  Modified:         2001-12-28 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

double  log_gamma(const double arg)
{
const double arg_limit = 15.0;
const unsigned n_limit = 11;
if (arg <= 0.0)
    fatalError("log_gamma:","DOMAIN-error");
double  y, z, f, x;
unsigned   i, k;
z=1.0;
for (int k=0,x=arg-1.0;x<arg_limit;x+=1.0,k++)
    z *= (x+=1.0);
y = x * log(x) - x + 0.5*log( 2.0*M_PI*x ) - log(z);
for (f=1.0, i=2, z=x; i<n_limit ;i+=2,z*=x*x,f+=2.0)
    y += bern[i]/(z*f*(f+1.0));
return  y;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       ---
//
//  Function:         factorial
//
//  Purpose:          Factorial function
//                    Copyright(C) 1991 Achim Flammenkamp (achim@hrz.uni-bielefeld.de)
//                    URL: http://www.uni-bielefeld.de/~achim/
//                    usage higher functions:  exp(), log(), sin()
//                    x! = Gamma(x+1) = x * Gamma(x)
//                    Gamma(x) * Gamma(1-x) = Pi/sin(Pi*x)
//
//
//  Created:          1989-12-28 (Achim Flammenkamp)
//
//  Modified:         2001-12-28 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

double  factorial(double arg)
{
double  y;
if (arg <= -1.0  &&  floor(arg)==arg)
    fatalError("factorial","SING-error");
if (arg < 0.0)
    return   M_PI*arg/sin(M_PI*arg)/factorial(-arg) ;
if (arg <= 30.0  &&  floor(arg) == arg)
    for (y=1.0;arg>0.0;arg-=1.0)
        y *= arg;
else
    y = exp( log_gamma(arg+1.0) );
return  y;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       gamma_square_Distri
//
//  Member function:  gamma_square_Distri
//
//  Purpose:          default Konstruktor
//
//
//  Created:          2001-12-27 (Achim Flammenkamp)
//
//  Modified:         2001-12-28 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

gamma_Distri::gamma_Distri(const qreal lam, const qreal sca ) :
prob_Distri(0), lambda(lam), scale(sca)
{
    if (lambda <= 0.0)
        fatalError("gamma_Distri::gamma_Distri","lambda must be positive");
    if (scale <= 0.0)
        fatalError("gamma_Distri::gamma_Distri","scale must be positive");
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       gamma_Distri
//
//  Member function:  density
//
//  Purpose:          gamma probability density function on R^+_0
//
//
//  Created:          2001-12-27 (Achim Flammenkamp)
//
//  Modified:         2001-12-28 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline qreal gamma_Distri::density(const qreal arg) const
{
    if (arg < 0.0)  return  0.0;
    return  pow(arg/scale,lambda-1)*exp(-arg/scale)/scale/factorial(lambda-1.0);
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       gamma_Distri
//
//  Member function:  random
//
//  Purpose:          random value in R with Gamma probability distribution
//                    see: D.E. Knuth Seminumerical Algorithm Vol. 2 Chap. 3.4.1
//
//
//  Created:          2001-12-27 (Achim Flammenkamp)
//
//  Modified:         2001-12-28 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

qreal gamma_Distri::random()
{
    return  random(lambda,scale);
}

qreal gamma_Distri::random(const qreal lam, const qreal sca)
{
    if (lam <= 0.0)
        fatalError("gamma_Distri::random","lambda must be positive");
    if (sca <= 0.0)
        fatalError("gamma_Distri::random","scale must be positive");
    qreal  arg;
    static  uniform_Distri  uni = uniform_Distri();
    if (lam < l_max)
    { 
        arg = 1.0;
        qreal  lamb;
        uni.swap_state(state);
        for (lamb=lam;lamb >= 1.0;lamb-=1.0)
            arg *= uni.random();
        uni.swap_state(state);
        arg = -log(arg);
        if (lamb)
        {
            const double e = 2.71828182845904523536;
            uni.swap_state(state);
            qreal  v, q, u;
            qreal const  p = 1.0/(1.0+lamb/e);
            //  average number of iterations: (lamb+e)/e/lamb! < 1.368
            do {
                u=uni.random();
                if (u < p)
                {
                    v = pow(uni.random(),1.0/lamb);
                    q = p*exp(-v);
                }
                else
                {
                    v = 1.0 - log( uni.random() );
                    q = p+(1.0-p)*pow( v, lamb-1.0 );
                }
            } while (u >= q);
            uni.swap_state(state);
            arg += v;
        }
    }
    else
    {
        qreal  m = lam-1, v;
        uni.swap_state(state);
        do {
            do {
                v = tan(M_PI*uni.random());
                arg = sqrt(2*lam-1.0)*v + m;
            } while (arg <= 0.0);
        } while (uni.random() > (1.0+v*v)*pow(arg/m,m)/exp(arg-m));
        uni.swap_state(state);
    }
    
    return  arg*sca;
}
