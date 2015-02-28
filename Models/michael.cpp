///////////////////////////////////////////////////////////////////////////////
//
// $Header:michael.C,v 1.18 99/03/15 10:52:16 mmeyer Exp $
//
// Module name:		michael.C
// Author:		Michael Meyer
// Last modified:	Fri Mar 12 14:39:03 MET 1999
// By:			
//
///////////////////////////////////////////////////////////////////////////////

#include "michael.h"
#include "../logger.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Member functions:	pf_cd, pf_cd_prime
// Purpose:		production function and its derivative: fraction
//
// Author:		Uli Middelberg
// Last modified:	Mon May 11 12:26:37 METDST 1998
// By:			
//
///////////////////////////////////////////////////////////////////////////////


static qreal pf_cd ( qreal k, qreal z, qreal b, qreal r )
{ return exp( r *log( z/b * exp( b * log(k) ) ) );
  //
  // expression:	z/b * e^(b * log(k) )
  // constraints:	z > 0, 0<b<=1
}

static qreal pf_cd_prime ( qreal k, qreal z, qreal b, qreal r )
{ return ( z/k * exp( b * log(k) ) ) * (r * exp( (r-1) *log( z/b * exp( b * log(k) ) ) ) );
  //
  // expression:	z/k * e^(b * log(k) )
  // constraints:	z > 0, 0<b<=1
}


//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//****                                                                     ****
//****                    g r o w t h _ m o d e l_m                        ****
//****                                                                     ****
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		michael
// Member function:	pf_init
// Purpose:		maps normalized (A,B,C,D) -> (a,b,c,alpha) and sets pf, pf_prime
//
// Author:		Uli Middelberg
// Last modified:	Mon May 11 12:44:16 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
void michael::pf_init ( void )
{
   b=B; 
   pf=pf_cd; pf_prime=pf_cd_prime;       
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		michael
// Member function:	sf_y
// Purpose:		returns the savings rate for entire economy
//
// Author:		Uli Middelberg
// Last modified:	Mon Mar 10 14:44:08 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////
qreal michael::sf_y ( qreal ) {
 return syr;
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		michael
// Member function:	michael
// Purpose:		constructor
//
// Author:		Michael Meyer
// Last modified:	Mon Mar 15 14:09:17 MET 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

michael::michael() : baseModel(1)
{
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		michael
// Member function:	michael
// Purpose:		destructor
//
// Author:		Michael Meyer & Stefan Lüke
// Last modified:	Mon Mar 10 14:09:17 MET 1999
// By:			Michael Meyer 
//
///////////////////////////////////////////////////////////////////////////////

michael::~michael(void) {
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		michael
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		Michael Meyer
// Last modified:	Mon Mar  15 14:27:09 MET 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void michael::iteration(const qint64& )
{ 
  qreal k_n;             // value of k for the next period
  qreal z_n;		// value of z for the next period

  y = (*pf)(k,z,b,r);
//  ren = (*pf_prime)(k,z,b,r);
  w = (1-r*b)*(*pf)(k,z,b,r);

  sav_y = (1- gamma)*(1-tau)*w;
  k_n = sav_y;
  k = k_n;
  z_n = tau * w + (1-rho) * z;
  z = z_n;

}
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		michael
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Uli Middelberg
// Last modified:	Mon Mar 10 15:43:17 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void michael::initialize()
{
  k = k_0;
  z = z_0;
  pf_init();
}
 

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		michael
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Uli Middelberg
// Last modified:	98/10/15
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

qreal* michael::setLabels(const QString& label)
{
    if (label == "K") return(&K);
    if (label == "L") return(&L);
    if (label == "k") return(&k);
    if (label == "k_0") return(&k_0);
    if (label == "z_0") return(&z_0);
    if (label == "z") return(&z);
    if (label == "y") return(&y);
    if (label == "w") return(&w);
    if (label == "r") return(&r);
    if (label == "b") return(&b);
    if (label == "syr") return(&syr);
    if (label == "n") return(&n);
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		michael
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		Uli Middelberg
// Last modified:	Mon Mar 10 15:43:28 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void michael::sendStateSpace(int &quantity,QList<qreal *> *stateSpace)
{
    stateSpace->clear();
    quantity = dimension;
    *stateSpace << &k;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		michael
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Michael Meyer 
// Last modified:	Mon May 11 13:07:57 METDST 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void michael::loadParamset(QTextStream& inFile)
{
    inFile >> k_0;
    inFile >> z_0;    
    inFile >> r;
    inFile >> tau;
    inFile >> rho;
    inFile >> gamma;
    inFile >> B;
     
    
    inFile >> n;
      
    inFile >> length;
    initialize();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		michael
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		Uli Middelberg
// Last modified:	Mon May 11 13:11:41 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

void michael::saveParamset(QTextStream& outFile)
{
    outFile << k_0 << "\t";
    outFile << z_0 << "\t";
    outFile << r << "\t";
    outFile << tau << "\t";
    outFile << rho << "\t";
    outFile << gamma << "\t";
    outFile << B << "\t";
    
    outFile << n << "\t";
   

    outFile << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		michael
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		Uli Middelberg
// Last modified:	Mon May 11 13:12:05 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

void michael::printParamset()
{
    log() << k_0;
    log() << z_0;
    log() << r;
    log() << tau;
    log() << rho;
    log() << gamma;
    log() << B;
    
    log() << n;

    log() << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		growth_model
// Member functions:	sendModelVar, sendParameters, receiveParameters
// Purpose:		for compatibity reasons only
//
// Author:		Uli Middelberg
// Last modified:	Tue Mar 11 16:10:38 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////
qreal* michael::sendModelVar(void)
{ error("macrodyn::michael::sendModelVar is not implemented");
  return NULL;
}
void michael::sendParameters(int& ,qreal** )
{ error("macrodyn::michael::sendParameters is not implemented");
}
void michael::receiveParameters(const QList<qreal>&)
{ error("macrodyn::michael::receiveParameters is not implemented");
}

