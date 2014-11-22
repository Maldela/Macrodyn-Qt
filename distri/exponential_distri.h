#ifndef ExponenetialDistri_H
#define ExponenetialDistri_H

///////////////////////////////////////////////////////////////////////////////
//
// Module name:  exponential_Distri
//
// Contents:     Class definition of the exponential distribution on R
//
//
// Created:      2001-12-23 (Achim Flammenkamp)
//
// Modified:     2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////


#include "prob_distri.h"

class exponential_Distri : public prob_Distri {
    public:
        exponential_Distri(const qreal sca=1.0, const qreal lef =0.0);
                                           // constructor
        qreal density(const qreal) const;    // probability density function
        qreal cumulat(const qreal) const;    // cumulative probability function
        qreal inverse(const qreal) const;    // inverse cumulative prob. function
        qreal random();                     // random prob. distributed variable
        qreal random(const qreal, const qreal); // random prob. distributed variable
    private:
        qreal  scale, left;
};
#endif
