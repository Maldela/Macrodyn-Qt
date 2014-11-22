#ifndef NormalDistri_H
#define NormalDistri_H

///////////////////////////////////////////////////////////////////////////////
//
// Module name:  normal_Distri
//
// Contents:     Class definition of the normal distribution on R
//
//
// Created:      2001-12-08 (Achim Flammenkamp)
//
// Modified:     2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////


#include "prob_distri.h"

class normal_Distri : public prob_Distri {
    public:
        normal_Distri(const qreal med =0.0, const qreal var =1.0);  // constructor
        qreal density(const qreal) const; // probability density function
        qreal cumulat(const qreal) const; // cumulative probability function
        qreal inverse(const qreal) const; // inverse cumulative prob. function
        qreal random();                  // random prob. distributed variable
        qreal random(const qreal, const qreal); // random prob. distributed variable
    private:
        qreal  median, varia;
};

qreal ierfc(const qreal);         // inverse error function complement

#endif
