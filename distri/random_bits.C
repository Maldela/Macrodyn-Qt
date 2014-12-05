#include "random_bits.h"
#include "error.h"

const uint  state_size = 25;


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       randomBits
//
//  Member function:  generator_state_size
//
//  Purpose:          return the number of uint used for state space
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2002-01-02 
// 
//
////////////////////////////////////////////////////////////////////////////////

uint randomBits::generator_state_size()
{
   return  state_size;
}



///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       randomBits
//
//  Member function:  randomBits
//
//  Purpose:          Constructor
//                    state of randomBits is defined, but may be invalid
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2002-01-02 
// 
//
////////////////////////////////////////////////////////////////////////////////

randomBits::randomBits()
{
   state = 0;
}



///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       randomBits
//
//  Member function:  ~randomBits
//
//  Purpose:          Destruct element
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2002-01-02 
// 
//
////////////////////////////////////////////////////////////////////////////////

randomBits::~randomBits()
{
    // everything extern , don't change
}

///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       randomBits
//
//  Member function:  discrete_random_state_generator
//
//  Purpose:          computed next pseudo-random state
//
//  Method:           Twisted Linear Congruence Generator
//                    by M. Matsumoto email: matumoto@math.keio.ac.jp
//                    generates 25 32-bit uint numbers distributed uniformly
//                    on [0,2^32-1]
//                    ACM Transactions on Modelling and Computer Simulation, 
//                    Vol. 4, No. 3, 1994, pages 254-266.
//                    state.new <- 2^(32*M-1) * Magic * state.old  mod  2^800
//
//
//  Created:          2002-01-02 (Achim Flammenkamp)
//
//  Modified:         2002-01-02 
//
////////////////////////////////////////////////////////////////////////////////


void randomBits::discrete_random_state_generator()
{
    static const uint  M = 7, mag01[2]={ 0x0, 0x8ebfd028 };
    uint i;
    for (uint i=0;i<state_size-M;i++)
        state->word[i] = state->word[i+M] ^ (state->word[i] >> 1) ^ mag01[state->word[i]&1];
    for (;i<state_size;i++)
        state->word[i] = state->word[i+M-state_size] ^ (state->word[i] >> 1) ^ mag01[state->word[i]&1];
    state->availible_bits = generated_bits();
}    


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       randomBits
//
//  Member function:  twist
//
//  Purpose:          local twisting of the to deliver 32 bits
//
//
//  Created:          2002-01-02 (Achim Flammenkamp)
//
//  Modified:         2002-01-02 
//
///////////////////////////////////////////////////////////////////////////////

inline uint randomBits::twist (uint y)
{
    uint const  b = 0x2b5b2500, c = 0xdb8b0000;
    y ^= (y <<  7) & b;
    y ^= (y << 15) & c;
    y ^= (y >> 16);
    return  y;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       randomBits
//
//  Member function:  generated_bits
//
//  Purpose:          number of random bits of discrete pseudo random generator
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2002-01-02 
//
///////////////////////////////////////////////////////////////////////////////

inline uint  randomBits::generated_bits()
{
   return  bits_per_word*state_size ;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       randomBits
//
//  Member function:  random_bits
//
//  Purpose:          return a sequence of pseudo random bits
//                    low order bits are firstly defined in the words
//                    only 1-bit positions get set, no other are changed !!
//                    so, maybe  word  clearing is needed before
//                    offset  >=  bits_per_word  is allowed
//
//
//  Created:          2001-12-15 (Achim Flammenkamp)
//
//  Modified:         2002-01-02 
//
///////////////////////////////////////////////////////////////////////////////

void  randomBits::random_bits(uint needed_bits, uint *words,
                              uint offset)
{
   uint  defined_bits= 0;
   while (offset >= bits_per_word)
       offset -= bits_per_word,  words++;
   while (defined_bits < needed_bits)
   {
       if (!state->availible_bits)
           discrete_random_state_generator();
       int  diff = generated_bits() - state->availible_bits;
       uint  i = diff > 0 ? diff / bits_per_word : 0;
       uint  gap_bits = diff > 0 ? diff % bits_per_word : 0;

       for ( ; defined_bits < needed_bits  &&  state->availible_bits ; i++ )
       {
           uint  max_bits =
                              state->availible_bits + gap_bits < bits_per_word ?
                              state->availible_bits : bits_per_word - gap_bits
;
           uint  next_bits =
                               defined_bits + max_bits > needed_bits ?
                               needed_bits - defined_bits  :  max_bits ;
           uint  next = twist(state->word[i]);
           if (gap_bits)
           {   //  remove the lower used bits in state->word[i] before usage
               next >>= gap_bits;
               gap_bits = 0;
           }
           if (next_bits < bits_per_word)
              next &= (1<<next_bits)-1;

           *words |=  next << offset;
           offset += next_bits;
           if (offset >= bits_per_word)
           {  words++;
              offset -= bits_per_word;
              if (offset)
                 *words |=  next >> (bits_per_word-offset);
           }
           defined_bits += next_bits;
           state->availible_bits -= next_bits;
       }
   }
   return;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       randomBits
//
//  Member function:  search_next_1_bit
//
//  Purpose:          return the number of skiped 0 bits until next 1 bit
//                    only if  skiped is false
//                    garanties that the first bit from random_bits() will be 1
//
//
//  Created:          2001-12-15 (Achim Flammenkamp)
//
//  Modified:         2002-01-02 
//
///////////////////////////////////////////////////////////////////////////////

uint  randomBits::search_next_1_bit(bool const  skip)
{
   uint  zero_bits = 0;
   while (true)
   {
       if (!state->availible_bits)
           discrete_random_state_generator();
       int  diff = generated_bits() - state->availible_bits;
       uint  i = diff > 0 ? diff / bits_per_word : 0;
       uint  gap_bits = diff > 0 ? diff % bits_per_word : 0;

       for ( ; state->availible_bits ; i++ )
       {
           uint  max_bits =
                              state->availible_bits + gap_bits < bits_per_word ?
                              state->availible_bits : bits_per_word - gap_bits ;
           uint  next = twist(state->word[i]);
           if (gap_bits)
           {   //  remove the lower used bits in state->word[i] before usage
               next >>= gap_bits;
               gap_bits = 0;
           }
           if (max_bits < bits_per_word)
               next &= (1U<<max_bits)-1;
           for  (; next ; next >>= 1 )
               if (next&1)
               {   state->availible_bits -= skip;
                   return  zero_bits;
               }
               else
               {
                   state->availible_bits --;
                   zero_bits ++;
               }
           state->availible_bits -= max_bits;
           zero_bits += max_bits;
       }
   }
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       randomBits
//
//  Member function:  attach_state
//
//  Purpose:          attach the argument as current random generator state
//                    returns a reference to the old random generator state
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2002-01-02 
//
///////////////////////////////////////////////////////////////////////////////

random_state &  randomBits::attach_state(random_state &new_state)
{
  random_state &dummy = *state;
  state = &new_state;
  return  dummy;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       randomBits
//
//  Member function:  default_state
//
//  Purpose:          returns a new created default random_state
//
//
//  Created:          2002-01-02 (Achim Flammenkamp)
//
//  Modified:         2002-01-02 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

random_state & randomBits::default_state(random_state *new_state )
{
    static quint64 x[state_size]={  /* initial seeds, not all = 0 */
           0x95f24dab, 0x0b685215, 0xe76ccae7, 0xaf3ec239, 0x715fad23,
           0x24a590ad, 0x69e4b5ef, 0xbf456141, 0x96bc1b7b, 0xa7bdf825,
           0xc1de75b7, 0x8858a9c9, 0x2da87693, 0xb657f9dd, 0xffdc8a9f,
           0x8121da71, 0x8b823ecb, 0x885d05f5, 0x4e20cd47, 0x5a9ad5d9,
           0x512c0c03, 0xea857ccd, 0x4cc1d30f, 0x8891a8a1, 0xa6b7aadb
    };
    if (!new_state)
    {   new_state = new random_state;
        new_state->size = 0;
    }
    if (!new_state->size)
    {   new_state->size = state_size;
        new_state->word = new uint[state_size];
        new_state->availible_bits = 0;
    }
    for (uint i=0;i<state_size;i++)
        new_state->word[i] = x[i];
    new_state->availible_bits = generated_bits();
    return  *new_state;
}
