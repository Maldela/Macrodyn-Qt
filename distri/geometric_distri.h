#ifndef GeometricDistri_H
#define GeometricDistri_H

///////////////////////////////////////////////////////////////////////////////
//
// Module name:  geometric_Distri
//
// Contents:     Class definition of the geometric distribution on N_0
//               first parameter give the probabilty parameter
//
//
// Created:      2001-12-14 (Achim Flammenkamp)
//
// Modified:     2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////


#include "prob_distri.h"


class geometric_Distri : public prob_Distri
{
    public:
        geometric_Distri(const qreal pro=0.5);  // constructor
        qreal density(const qreal) const;   // probability density function
        qreal cumulat(const qreal) const;   // cumulative probability function
        qreal inverse(const qreal) const;  // inverse cumulative prob. function
        int random( );                     // random prob. distributed variable
        int random(const qreal);            // random prob. distributed variable
    private:
        qreal  prob;

};
#endif
