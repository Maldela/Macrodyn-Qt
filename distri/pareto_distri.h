#ifndef ParetoDistri_H
#define ParetoDistri_H

///////////////////////////////////////////////////////////////////////////////
//
// Module name:  pareto_distri
//
// Contents:     Class definition of the Pareto distribution on R
//
//
// Created:      2001-12-23 (Achim Flammenkamp)
//
// Modified:     2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////


#include "prob_distri.h"

class pareto_Distri : public prob_Distri {
    public:
        pareto_Distri(const qreal lam, const qreal sca = 1.0);   // constructor
        qreal density(const qreal) const;   // probability density function
        qreal cumulat(const qreal) const;   // cumulative probability function
        qreal inverse(const qreal) const;   // inverse cumulative prob. function
        qreal random();                    // random prob. distributed variable
        qreal random(const qreal, const qreal); // random prob. distributed variable
    private:
        qreal  lambda, scale;
};

#endif
