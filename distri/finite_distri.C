#include "finite_distri.h"
#include "error.h"


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       finite_Distri
//
//  Member function:  finite_Distri
//
//  Purpose:          Constructor for classes
//
//
//  Created:          2001-12-28 (Achim Flammenkamp)
//
//  Modified:         2001-12-28 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

finite_Distri::finite_Distri(const uint n, const qreal *pro, const int first) :
prob_Distri(0), no(n), prob(new  qreal[no]),  cumu(new  qreal[no]), start(first)
{
    if (!no)  return;
    for (uint  i=0; i < no ; i++)
    {
        if ((prob[i] = (pro ? pro[i] : 1.0/no)) < 0.0)
           fatalError("finite_Distri::finite_Distri","probability is negative");
        else
           cumu[i] = prob[i] + (i ? cumu[i-1] : 0.0);
    }
    if (!cumu[no-1])
        fatalError("finite_Distri::finite_Distri","all probabilities are zero");
    if (cumu[no-1] != 1.0)
        for (uint  i=0; i < no ; i++)
            prob[i] /= cumu[no-1],
            cumu[i] /= cumu[no-1];
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       finite_Distri
//
//  Member function:  finite_Distri
//
//  Purpose:          Konstruktor for Initialisierung durch finite_Distri
//
//
//  Created:          2001-12-28 (Achim Flammenkamp)
//
//  Modified:         2001-12-28 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

finite_Distri::finite_Distri(const finite_Distri &init) : prob_Distri()
{
   cumu = NULL;
   prob = NULL;
   state.word = NULL;
   *this = init;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       finite_Distri
//
//  Member function:  ~finite_Distri
//
//  Purpose:          Virtuel Destructor for classes
//
//
//  Created:          2001-12-28 (Achim Flammenkamp)
//
//  Modified:         2001-12-28 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline finite_Distri::~finite_Distri()
{
   delete[] state.word;
   delete[] prob;
   delete[] cumu;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       finite_Distri
//
//  Member function:  operator=  
//
//  Purpose:          assignment from finite_Distri object
//
//
//  Created:          2001-12-28 (Achim Flammenkamp)
//
//  Modified:         2001-12-28 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

finite_Distri& finite_Distri::operator=(const finite_Distri & given)
{
   if (this != &given)
   {   state = given.state;
       no = given.no;
       start = given.start;
       for (uint i=0; i < no ; i++)
           prob[i] = given.prob[i],
           cumu[i] = given.cumu[i];
   }
   return  *this;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       finite_Distri
//
//  Member function:  density
//
//  Purpose:          finite probability density function
//
//  Created:          2001-12-28 (Achim Flammenkamp)
//
////////////////////////////////////////////////////////////////////////////////

inline qreal  finite_Distri::density(const int arg) const
{
    if (arg < start)  return  0.0;
    if (arg >= (int)(start+no))  return  0.0;
    return  prob[arg-start];
}

///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       finite_Distri
//
//  Member function:  cumulat
//
//  Purpose:          cumulated finite probability function
//
//  Created:          2001-12-28 (Achim Flammenkamp)
//
////////////////////////////////////////////////////////////////////////////////

inline qreal finite_Distri::cumulat(const int arg) const
{
    if (arg < start)  return  0.0;
    if (arg >= (int)(start+no))  return  1.0;
    return  cumu[arg-start];
}

///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       finite_Distri
//
//  Member function:  inverse
//
//  Purpose:          inverse cumulated probability function
//
//  Created:          2001-12-28 (Achim Flammenkamp)
//
////////////////////////////////////////////////////////////////////////////////

inline qreal finite_Distri::inverse(const qreal arg) const
{
   if (arg < 0.0 || arg > 1.0)
       fatalError("finite_Distri::inverse","parameter out of range");

    return  0.0;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       finite_Distri
//
//  Member function:  random
//
//  Purpose:          random finite distributed value
//
//  Created:          2001-12-28 (Achim Flammenkamp)
//
////////////////////////////////////////////////////////////////////////////////
//
inline int  finite_Distri::random()
{
    fatalError("macrodyn::prob_Distri::random", "NOT_IMPLEMENTED");
    return  0;
}
