#ifndef RANDOM_BITS_H
#define RANDOM_BITS_H

#include "machine.h"

///////////////////////////////////////////////////////////////////////////////
//                                                                            
// Module name:  randomBits
// 
// Contents:     Class definition of randomBits
//
//                                                                           
// Created:      2001-12-08 (Achim Flammenkamp)
//
// Modiefied:    2001-12-15 
//
//                                                                            
////////////////////////////////////////////////////////////////////////////////


typedef struct { unsigned  availible_bits;
                 unsigned  size;
                 unsigned  *word; } random_state;

class randomBits {
public: 
    static unsigned  generator_state_size() ;   // # needed internal variables
    randomBits();                               // constructor
    ~randomBits();                              // destructor
    void random_bits(unsigned, unsigned *, unsigned offset = 0);
                                                // return random bit sequence
    unsigned  search_next_1_bit(const bool);    // search next 1 bit
    random_state & attach_state(random_state&); // attach new generator state
    static random_state & default_state(random_state* =0 );// return default state
private:
    random_state * state;                       // pointer to random-state
    void discrete_random_state_generator();     // discrete random generator
    static unsigned twist (unsigned);           // local twister of next bits
    static unsigned  generated_bits();          // # generated random bits
};

#endif
