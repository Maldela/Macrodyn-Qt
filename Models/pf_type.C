
///////////////////////////////////////////////////////////////////////////////
//
// $Header: pf_type.C,v 1.13 99/02/23 14:24:43 mmeyer Exp $
//
// Module name:		pf_type.C
// Contents:		Class definition of the class rSolow
//			
//
// Author:		Michael Meyer & Stefan L�ke
// Last modified:	Mon May 11 12:40:17 METDST 1998
// By:			
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
//
// Functions:		pf_ces, pf_ces_prime
// Purpose:		production function and its derivative: CES
//
// Author:		Uli Middelberg
// Last modified:	Mon Mar 10 14:44:08 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#include "pf_type.h"


//static qreal pf_ces(qreal k, qreal a, qreal b, qreal c, qreal)
//{
//    return ( exp ( log ( a + b * exp(log(k)*c) ) * 1/c) );
//  //
//  // expression:	[ a + b * k^c ] ^ {1/c}
//  // constraints:	c <= 1, c # 0
//}

//static qreal pf_ces_prime(qreal k, qreal a, qreal b, qreal c, qreal)
//{
//    return ( (1/c) * exp ( log ( a + b * exp(log(k)*c) ) * (1/c - 1) ) * \
//                             b * c * exp(log(k)*(c-1)) );
//  //
//  // expression:	(1/c) [ a + b * k^c ] ^ (1/c-1) * [ b * c * k^(c-1) ]
//  // constraints:	c <= 1, c # 0
//}

///////////////////////////////////////////////////////////////////////////////
//
//
// Functions:		pf_ces2, pf_ces2_prime
// Purpose:		production function and its derivative: CES
//
// Author:		Uli Middelberg
// Last modified:	Fri Nov 14 15:01:36 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////
//static qreal pf_ces2 ( qreal k, qreal a, qreal b, qreal c, qreal )
//{ return ( c==0 ? a * exp(log(k)*b) : exp ( log ( a * ( (1-b) + b * exp(log(k)*c) ) ) * 1/c) );
//  //
//  // expression:	a * [ (1-b) + b * k^c ] ^ {1/c}		c != 0
//  //			a * k ^ b				c == 0
//  // constraints:	c <= 1
//}

//static qreal pf_ces2_prime ( qreal k, qreal a, qreal b, qreal c, qreal )
//{ return ( c==0 ? b * a * exp(log(k)*(b-1)) : \
//                (1/c) * exp(log( a * ((1-b) + b * exp(log(k)*c)) ) * (1/c - 1) ) * \
//                      a * b * c * exp(log(k)*(c-1)) );
//  //
//  // expression:	(1/c) * [ a * [ (1-b) + b * k^c ] ] ^ (1/c-1) * [ a * b * c * k^(c-1) ] 	c!=0
//  // 			b * a * k ^ (b-1)			c == 0
//  // constraints:	c <= 1
//}

///////////////////////////////////////////////////////////////////////////////
//
//
// Functions:		pf_leo, pf_leo_prime
// Purpose:		production function and its derivative: leo
//
// Author:		Uli Middelberg
// Last modified:	Mon Mar 10 14:44:08 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////
//static qreal pf_leo ( qreal k, qreal a, qreal b, qreal c, qreal )
//{ return ( a / b * ( 1 + c - exp (-b * k) ) );
//  //
//  // expression:	a/b * ( 1 - e^(-b * k) )
//  // constraints:	a,b >= 0
//}

//static qreal pf_leo_prime ( qreal k, qreal a, qreal b, qreal , qreal )
//{ return ( a * exp (-b * k) );
//  //
//  // expression:	a * e^(-b * k)
//  // constraints:	a,b >= 0
//}

///////////////////////////////////////////////////////////////////////////////
//
//
// Functions:		pf_cd, pf_cd_prime
// Purpose:		production function and its derivative: cobb-douglas
//
// Author:		Uli Middelberg
// Last modified:	Fri Nov 14 14:58:49 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////
//static qreal pf_cd ( qreal k, qreal a, qreal b, qreal , qreal )
//{ return ( a/b * exp( b * log(k) ) );
//  //
//  // expression:	a/b * e^(b * log(k) )
//  // constraints:	a > 0, 0<b<=1
//}

//static qreal pf_cd_prime ( qreal k, qreal a, qreal b, qreal , qreal )
//{ return ( a/k * exp( b * log(k) ) );
//  //
//  // expression:	a/k * e^(b * log(k) )
//  // constraints:	a > 0, 0<b<=1
//}

///////////////////////////////////////////////////////////////////////////////
//
//
// Member functions:	pf_leontiev, pf_leontiev_prime
// Purpose:		production function and its derivative: fraction
//
// Author:		Uli Middelberg
// Last modified:	Mon May 11 12:26:37 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
//static qreal pf_leontiev ( qreal k, qreal a, qreal b, qreal c, qreal )
//{ return ( a*k<b ? a*k+c : b+c );
//  //
//  // expression:	qMin{a*k,b}+c
//}

//static qreal pf_leontiev_prime ( qreal k, qreal a, qreal b, qreal , qreal )
//{ return ( k<=b/a ? a : 0);
//  //
//  // expression:	a	k<=b/a
//  //			0	k> b/a
//}

///////////////////////////////////////////////////////////////////////////////
//
//
// Member functions:	pf_app, pf_app_prime
// Purpose:		production function and its derivative: fraction
//
// Author:		Leo Kaas
// Last modified:	Mon May 11 12:44:43 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
//static qreal pf_app ( qreal k, qreal a, qreal b, qreal c, qreal alpha )
//{
// return (a*k+alpha*a*(log((1+exp(-b/(alpha*a)))/(1+exp((1/alpha)*(k-b/a)))))+c);
//}

//static qreal pf_app_prime ( qreal k, qreal a, qreal b, qreal , qreal alpha )
//{
// return ( a/( 1+exp( (1/alpha)*(k-b/a) ) ) );
//}
