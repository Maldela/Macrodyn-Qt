#ifndef GammaDistri_H
#define GammaDistri_H

///////////////////////////////////////////////////////////////////////////////
//
// Module name:  gamma_Distri
//
// Contents:     Class definition of the Gamma distribution on R^+
//
//
// Created:      2001-12-28 (Achim Flammenkamp)
//
// Modified:     2001-12-28 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////


#include "prob_distri.h"

class gamma_Distri : public prob_Distri {
    public:
        gamma_Distri(const qreal lam = 1.0, const qreal sca = 1.0);
                                           // constructor
        qreal density(const qreal) const;    // probability density function
//      qreal cumulat(const qreal) const;    // cumulative probability function
//      qreal inverse(const qreal) const;    // inverse cumulative prob. function
        qreal random();                     // random prob. distributed variable
        qreal random(const qreal, const qreal); // random prob. distributed variable
    private:
        qreal  lambda, scale;
};

double  log_gamma(const double x);         // ln(Gamma(x))-Function   x > 0
double  factorial(double x);               // x!   x not a negative integer

#endif
