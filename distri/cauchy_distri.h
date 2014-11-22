#ifndef CauchyDistri_H
#define CauchyDistri_H

///////////////////////////////////////////////////////////////////////////////
//
// Module name:  cauchy_Distri
//
// Contents:     Class definition of the Cauchy distribution on R
//
//
// Created:      2001-12-23 (Achim Flammenkamp)
//
// Modified:     2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////


#include "prob_distri.h"

class cauchy_Distri : public prob_Distri {
    public:
        cauchy_Distri(const qreal med =0.0, const qreal sca =1.0);
                                           // constructor
        qreal density(const qreal) const;    // probability density function
        qreal cumulat(const qreal) const;    // cumulative probability function
        qreal inverse(const qreal) const;    // inverse cumulative prob. function
        qreal random();                     // random prob. distributed variable
        qreal random(const qreal, const qreal); // random prob. distributed variable
    private:
        qreal  median, scale;
};
#endif
