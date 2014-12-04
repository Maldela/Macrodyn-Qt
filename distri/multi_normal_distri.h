#ifndef MultiNormalDistri_H
#define MultiNormalDistri_H

///////////////////////////////////////////////////////////////////////////////
//
// Module name:  multi_normal_Distri
//
// Contents:     Class definition of the multivariate normal distribution on R^n
//
//
// Created:      2001-12-29 (Achim Flammenkamp)
//
// Modified:     2001-12-30 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////


#include "prob_distri.h"

class multi_normal_Distri : public prob_Distri {
    public:
        multi_normal_Distri(const uint d, const qreal *med =0, const qreal *covar =0);  // constructor
        multi_normal_Distri(const multi_normal_Distri&); // copy-constructor
        ~multi_normal_Distri(); // destructor
        multi_normal_Distri& operator=(const multi_normal_Distri&);
                                         // assign-operator
        qreal density(const qreal*) const; // probability density function
        qreal cumulat(const qreal*) const; // cumulative probability function
        qreal inverse(const qreal*) const; // inverse cumulative prob. function
        qreal* random();                  // random prob. distributed variable
//      qreal* random(const uint, const qreal *, const qreal *);            
                                         // random prob. distributed variable
    private:
        uint  dim;
        qreal  *median, *lower_left, *inver, sqrt_det;
        void compute_lowerleft_inverse_and_sqrt_of_determinate();
};

#endif
