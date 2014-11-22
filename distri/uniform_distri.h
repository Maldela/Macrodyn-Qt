#ifndef UniformDistri_H
#define UniformDistri_H

///////////////////////////////////////////////////////////////////////////////
//
// Module name:  uniform_Distri
//
// Contents:     Class definition of the uniform distribution on an open
//               interval in R centered around the first parameter with 
//               width given by the 2nd parameter.   default interval:  ]0,1[
//
//
// Created:      2001-12-08 (Achim Flammenkamp)
//
// Modified:     2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////


#include "prob_distri.h"
#include "random_bits.h"

class uniform_Distri : public prob_Distri {
    public:
        uniform_Distri(const qreal cen = 0.5, const qreal wid = 1.0 ); // constructor
        qreal density(const qreal) const;    // probability density function
        qreal cumulat(const qreal) const;    // cumulative probability function
        qreal inverse(const qreal) const;    // inverse cumulative prob. function
        qreal random();                     // random prob. distributed variable
        qreal random(const qreal, const qreal); // random prob. distributed variable
    private:
        qreal  center, width;

};
#endif
