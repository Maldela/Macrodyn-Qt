#ifndef PROB_DISTRI_H
#define PROB_DISTRI_H

#include "machine.h"
#include "random_bits.h"  //  typedef struct { ... } random_state
                          //  generator_state_size()  for  constructor


///////////////////////////////////////////////////////////////////////////////
//                                                                            
// Module name:  prob_Distri                                                 
// 
// Contents:     Class definition of the abstract class prob_distribution
//
//               Every other distribution to be qrealized must be derived from
//               this abstract class.
//
//                                                                           
// Created:      2001-12-08 (Achim Flammenkamp)
//
// Modified:     2001-12-27 (Achim Flammenkamp)
//
//                                                                            
////////////////////////////////////////////////////////////////////////////////


class prob_Distri
{
public:
    prob_Distri() {}
    uint state_size() const;            // # needed internal variables
    prob_Distri(const uint );        // constructor
    virtual ~prob_Distri();                 // destructor
    prob_Distri(const prob_Distri&);        // copy-constructor
    virtual prob_Distri& operator=(const prob_Distri&);   // assign-operator

    virtual qreal density(const qreal) const; // probability density function
    virtual qreal cumulat(const qreal) const; // cumulative probability function
    virtual qreal inverse(const qreal) const; // inverse cumulative function
//  virtual qreal random();                  // random distributed variable
//  virtual qreal random(...);               // random distributed variable
    virtual void seed(const uint * , const uint ); // set seed value for distribution
    void save_state(random_state&);         // save generator state
    virtual void swap_state(random_state&); // swap generator state for distri
protected:
    random_state  state;                    // internal state
};


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       prob_Distri
//
//  Member function:  ~prob_Distri
//
//  Purpose:          Virtuel Destructor for classes
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2001-12-09 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline prob_Distri::~prob_Distri()
{
   delete[] state.word;
}

#endif
