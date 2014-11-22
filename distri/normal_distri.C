#include "uniform_distri.h"
#include "normal_distri.h"
#include <math.h>
#include "error.h"


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       ---
//
//  Function:         ierfc
//
//  Purpose:          inverse of error function complement by rational Chebychev
//                    Copyright(C) 1996 Takuya Ooura (ooura@mmm.t.u-tokyo.ac.jp)
//                    URL: http://momonga.t.u-tokyo.ac.jp/~ooura/
//                    usage higher functions:  1 x exp(), 2 x log(), 1 x sqrt()
//                    Phi(x) + Phi(-x) = 1
//                    Erf(x) = 1 - Erfc(x)
//                    Phi(x) = 1/2 Erfc(-x/sqrt(2))         x <= 0
//                    Phi(x) = 1/2 + 1/2 Erf(x/sqrt(2))     x >= 0
//
//
//  Created:          2001-12-15 (Achim Flammenkamp)
//
//  Modified:         2001-12-15 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

qreal ierfc(const qreal arg)
{
    const double IERFC_LIM = 27;
    if (arg > 1.0)
        return  -ierfc(2.0-arg);
    if (arg <= 0.0)
        fatalError("normal_Distri::ierfc","argument out of range");
    if (arg < 1e-300)
        return  IERFC_LIM ;
    double u = 0.916461398268964 - log(arg);
    double x = sqrt(u);
    double s = (log(x) + 0.488826640273108) / u;
    double t = 1 / (x + 0.231729200323405);
    x *= (1 - s * (s * 0.124610454613712 + 0.5));
    x -=  ((((-0.0728846765585675 * t + 0.269999308670029) * t +
             0.150689047360223) * t + 0.116065025341614) * t +
           0.499999303439796) * t;
    t = 3.97886080735226 / (x + 3.97886080735226);
    u = t - 0.5;
    s = (((((((((0.00112648096188977922 * u +
            1.05739299623423047e-4) * u - 0.00351287146129100025) * u -
            7.71708358954120939e-4) * u + 0.00685649426074558612) * u +
            0.00339721910367775861) * u - 0.011274916933250487) * u -
            0.0118598117047771104) * u + 0.0142961988697898018) * u +
            0.0346494207789099922) * u + 0.00220995927012179067;
    s = ((((((((((((s * u - 0.0743424357241784861) * u -
            0.105872177941595488) * u + 0.0147297938331485121) * u +
            0.316847638520135944) * u + 0.713657635868730364) * u +
            1.05375024970847138) * u + 1.21448730779995237) * u +
            1.16374581931560831) * u + 0.956464974744799006) * u +
            0.686265948274097816) * u + 0.434397492331430115) * u +
            0.244044510593190935) * t -
            arg * exp(x * x - 0.120782237635245222);
    x += s * (x * s + 1);
    return  x;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       normal_Distri
//
//  Member function:  normal_Distri
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

normal_Distri::normal_Distri(const qreal med , const qreal var ) :
prob_Distri(0), median(med), varia(var)
{
    if (varia <= 0.0)
        fatalError("normal_Distri::normal_Distri","parameter variance must be positive");
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       normal_Distri
//
//  Member function:  density
//
//  Purpose:          normalized normal probability density function on R
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline qreal normal_Distri::density(const qreal arg) const
{
    qreal  tmp = (arg-median)/varia;
    return  0.39894228040143267794/varia* exp(-0.5*tmp*tmp); // 1/sqrt(2.0*M_PI)
}

///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       normal_Distri
//
//  Member function:  cumulat
//
//  Purpose:          cumulative probability function for normal density on R
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline qreal normal_Distri::cumulat(const qreal arg) const
{
    qreal  tmp = 0.70710678118654752440*(arg-median)/varia;   //  sqrt(0.5)
    return  tmp > 0.0 ? 0.5+0.5*erf(tmp) : 0.5*erfc(-tmp) ;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       normal_Distri
//
//  Member function:  inverse
//
//  Purpose:          inverse cumulative probability function for normal
//                    distribution
//                    Argument must be in ]0,1[
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2001-12-09 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

#ifndef  OLD_INVERS_PHI
inline qreal normal_Distri::inverse(const qreal arg) const
{
    if (arg <= 0.0 || arg >= 1.0)
        fatalError("normal_Distri::inverse","parameter out of range");
    return  median - varia*1.41421356237309504880*ierfc(2.0*arg);  // sqrt(2.0)
}
#else
qreal normal_Distri::inverse(const qreal arg) const
{
    if (arg <= 0.0 || arg >= 1.0)
        fatalError("normal_Distri::inverse","parameter out of range");
    if (arg > 0.5)
        return  -normal_Distri::inverse(1.0-arg) ;
    const qreal  C1 = 2.50662827463100050241;  // sqrt(2*PI)
    qreal  y, z;
    if (arg <= 0.025)
        y= -sqrt(-log( arg*arg*(-4.0*M_PI*log(arg))));
    else
    {
        const unsigned  N=10;
        const qreal taylor[N]={1,1,7,127,4369,243649, 20036983.,
                              2280356863.,343141433761.,65967241200001.};
        qreal  t = taylor[N-1];
        y= C1*(arg-0.5);
        z= y*y;
        //  Taylorapproximation --- better would be Chebychev
        for (unsigned  i=N-1;i>0;i--)
            t =  taylor[i-1]+t*z/(2*i)/(2*i+1);
        y *= t;
        //   y *= (1.0+z/6.0*(1.0+z/20.0*(7.0+z/42.0*(127+z/72.0*4369.0))));
    }
    do {   // Newton-Raphson Iteration
        z = y;
        y-= (cumulat(y)-arg)*C1*exp(0.5*y*y);
    } while (y-z < z*qreal_PRECISION);   //   y,z < 0
    return  median+varia*y;
}
#endif


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       normal_Distri
//
//  Member function:  random
//
//  Purpose:          random value in R with normal probability distribution
//                    Ratio method for normal deviates
//                    A.J. Kinderman & J.F. Monahan 1976
//                    see: D.E. Knuth Seminumerical Algorithm Vol. 2 Chap. 3.4.1
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

qreal normal_Distri::random()
{
    return  random(median,varia);
}

qreal normal_Distri::random(const qreal med, const qreal var)
{
    if (var <= 0.0)
        fatalError("normal_Distri::random","parameter variance must be positive");
    qreal const  C1 = 1.71552776992141359296;   // sqrt(8/E)
    qreal const  C2 = 5.13610166675096593629;   // 4*E^0.25
    qreal const  C3 = 1.036961042583566030287;  // 4/E^1.35
    static uniform_Distri  uni_distribution;
    qreal arg, val;
    //  following loop will be called 1.369 times on the average
    do {   
       uni_distribution.swap_state(state);
       arg = uni_distribution.random() - 0.5;
       val = uni_distribution.random();
       uni_distribution.swap_state(state);
       arg *= C1/val;
    } while (  arg*arg > 5-C2*val  &&
              (arg*arg >= C3/val+1.4  ||  arg*arg > -4.0*log(val)) );
    return  med+var*arg;
}
