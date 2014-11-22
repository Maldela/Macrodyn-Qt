#ifndef FiniteDistri_H
#define FiniteDistri_H

///////////////////////////////////////////////////////////////////////////////
//
// Module name:  finite_Distri
//
// Contents:     Class definition of the finite distribution on Z
//
//
// Created:      2001-12-28 (Achim Flammenkamp)
//
// Modified:     2001-12-28 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////


#include "prob_distri.h"

class finite_Distri : public prob_Distri {
    public:
        finite_Distri(const unsigned n, const qreal *prob, const int first=0);
                                           // constructor
        finite_Distri(const finite_Distri &);
                                           // copy-constructor
        ~finite_Distri();
                                           // destructor
        finite_Distri & operator=(const finite_Distri &);
                                           // assign-operator
        qreal density(const int) const;     // probability density function
        qreal cumulat(const int) const;     // cumulative probability function
        qreal inverse(const qreal) const;    // inverse cumulative prob. function
        int random();                      // random prob. distributed variable
        int random(const unsigned, const qreal*, const int);
                                           // random prob. distributed variable
    private:
        unsigned  no;
        qreal  *prob, *cumu;
        int  start;
};
#endif
