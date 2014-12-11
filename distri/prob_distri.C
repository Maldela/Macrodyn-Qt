#include "prob_distri.h"
#include "error.h"
#include <algorithm>   // qMin

const char * NOT_IMPLEMENTED = " is not implemented\n";

///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       prob_Distri
//
//  Member function:  state_size
//
//  Purpose:          return the number of uint used for state space
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2001-12-23 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline uint prob_Distri::state_size()  const
{
   return  state.size;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       prob_Distri
//
//  Member function:  prob_Distri
//
//  Purpose:          Constructor for classes
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2001-12-15 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

prob_Distri::prob_Distri(const uint size)
{
   state.size = size + randomBits::generator_state_size();
   state.word = new uint [state.size];
   randomBits::default_state(&state);
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       prob_Distri
//
//  Member function:  prob_Distri
//
//  Purpose:          Konstruktor for Initialisierung durch prob_Distri
//
//
//  Created:          2001-12-23 (Achim Flammenkamp)
//
//  Modified:         2001-12-23 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

prob_Distri::prob_Distri(const prob_Distri & init)
{
   state.word = 0;
   this->operator=(init);
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       prob_Distri
//
//  Member function:  operator=  
//
//  Purpose:          assignment from prob_Distri object
//
//
//  Created:          2001-12-23 (Achim Flammenkamp)
//
//  Modified:         2001-12-23 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

prob_Distri& prob_Distri::operator=(const prob_Distri & given)
{
   if (this != &given)
   {   state.size = given.state_size();
       delete[] state.word;
       if ((state.word = given.state.word) ? new uint [state.size] : 0)
           for (uint i=0; i < state.size ; i++)
               state.word[i] = given.state.word[i];
       state.availible_bits = given.state.availible_bits;
   }
   return  *this;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       prob_Distri
//
//  Member function:  density
//
//  Purpose:          not implemented, probability density function
//
//  Created:          2001-12-27 (Achim Flammenkamp)
//
////////////////////////////////////////////////////////////////////////////////

inline qreal  prob_Distri::density(const qreal) const
{
    fatalError("macrodyn::prob_Distri::density",NOT_IMPLEMENTED);
    return  0.0;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       prob_Distri
//
//  Member function:  cumulat
//
//  Purpose:          not implemented, cumulated probability function
//
//  Created:          2001-12-27 (Achim Flammenkamp)
//
////////////////////////////////////////////////////////////////////////////////

inline qreal  prob_Distri::cumulat(const qreal) const
{
    fatalError("macrodyn::prob_Distri::cumulat",NOT_IMPLEMENTED);
    return  0.0;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       prob_Distri
//
//  Member function:  inverse
//
//  Purpose:          not implemented, inverse cumulated probability function
//
//  Created:          2001-12-27 (Achim Flammenkamp)
//
////////////////////////////////////////////////////////////////////////////////

inline qreal  prob_Distri::inverse(const qreal) const
{
    fatalError("macrodyn::prob_Distri::inverse",NOT_IMPLEMENTED);
    return  0.0;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       prob_Distri
//
//  Member function:  random
//
//  Purpose:          not implemented, random density distributed value
//
//  Created:          2001-12-27 (Achim Flammenkamp)
//
////////////////////////////////////////////////////////////////////////////////
//
//inline qreal  prob_Distri::random()
//{
//    fatalError("macrodyn::prob_Distri::random",NOT_IMPLEMENTED);
//    return  0.0;
//}

///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       prob_Distri
//
//  Member function:  seed
//
//  Purpose:          seed for random distribution  (if 0-pointer, random seed)
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2001-12-15 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

#include <time.h>
#include <sys/times.h>

void  prob_Distri::seed(const uint *array = 0, const uint no = 1)
{
    uint  count = qMin(no,randomBits::generator_state_size());
    if (array)
    {
        for (uint  i=0; i < count; i++)
            state.word[i]= array[i];
        state.availible_bits = count * bits_per_word;
    }
    else
    {
        state.availible_bits = 0;
// number of seconds since 1970-01-01
        if (count > 0)
        {
            state.word[0] = uint( time(NULL) );
            state.availible_bits = bits_per_word;
        }
        if (count > 1)
        {   struct tms  dummy;
// number of TICKS since boot
            state.word[1] = uint( times(&dummy) );
            for (uint  i= state.word[1]; i; i>>=1)
                state.availible_bits += 1;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       prob_Distri
//
//  Member function:  save_state
//
//  Purpose:          save the current discrete pseudo random generator state
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2001-12-15
//
///////////////////////////////////////////////////////////////////////////////

void  prob_Distri::save_state(random_state &new_state)
{
   if (new_state.size < state.size)
       return;
   for (uint  i=0; i < state.size; i++)
       new_state.word[i] = state.word[i];
   new_state.availible_bits = state.availible_bits;
   new_state.size = state.size;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       prob_Distri
//
//  Member function:  swap_state
//
//  Purpose:          swap generator state
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2001-12-15 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

void  prob_Distri::swap_state(random_state  &new_state)
{
   uint  dummy;
   if (new_state.size != state.size)
       return;
   for (uint  i=0; i < state.size; i++)
   {
       dummy =state.word[i];
       state.word[i]= new_state.word[i];
       new_state.word[i]=  dummy;
   }
   dummy = state.availible_bits;
   state.availible_bits = new_state.availible_bits;
   new_state.availible_bits = dummy;
}
