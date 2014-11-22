#ifndef DiscreteDistri_H
#define DiscreteDistri_H

///////////////////////////////////////////////////////////////////////////////
//
// Module name:  discrete_Distri
//
// Contents:     Class definition of the uniform distribution on an 
//               interval in Z starting with the first parameter and having
//               a width given by the 2nd parameter.
//
//
// Created:      2001-12-14 (Achim Flammenkamp)
//
// Modified:     2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////


#include "prob_distri.h"
#include "random_bits.h"


class discrete_Distri : public prob_Distri {
    public:
        discrete_Distri(const int wid, const int sta =1);  //  constructor
        qreal density( const int) const;    // probability density function
        qreal cumulat( const int) const;    // cumulative probability function
        qreal inverse( const qreal) const;   // inverse cumulative prob. function
        int random();                      // random prob. distributed variable
        int random(const int, const int); // random prob. distributed variable
    private:
        unsigned  width;
        int  start;
};

#endif
