#ifndef CHi_Square_Distri_H
#define CHi_Square_Distri_H

///////////////////////////////////////////////////////////////////////////////
//
// Module name:  chi_square_Distri
//
// Contents:     Class definition of the Chi^2 distribution on R^+
//
//
// Created:      2001-12-23 (Achim Flammenkamp)
//
// Modified:     2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////


#include "prob_distri.h"

class chi_square_Distri : public prob_Distri {
    public:
        chi_square_Distri(const unsigned free);
                                           // constructor
        qreal density(const qreal) const;    // probability density function
//      qreal cumulat(const qreal) const;    // cumulative probability function
//      qreal inverse(const qreal) const;    // inverse cumulative prob. function
        qreal random();                     // random prob. distributed variable
        qreal random(const unsigned);       // random prob. distributed variable
    private:
        unsigned  n;
};
#endif
