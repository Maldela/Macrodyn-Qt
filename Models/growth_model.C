 ///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/growth_model.C,v 1.3 2002/01/08 09:04:32 mmeyer Exp mmeyer $
//
// Module name:		growth_model.C
// Contents:		Member functions of the classes
//                       1. growth_model
//                       2. rgrowth_model
//                       3. depreciate
//                       4. pasinetti
//
// Author:		Uli Middelberg
// Last modified:	Tue Dec  8 14:39:03 MET 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "growth_model.h"

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
static qreal pf_ces ( qreal k, qreal a, qreal b, qreal c, qreal )
{ return ( exp ( log ( a + b * exp(log(k)*c) ) * 1/c) );
  //
  // expression:	[ a + b * k^c ] ^ {1/c}
  // constraints:	c <= 1, c # 0	
}

static qreal pf_ces_prime ( qreal k, qreal a, qreal b, qreal c, qreal )
{ return ( (1/c) * exp ( log ( a + b * exp(log(k)*c) ) * (1/c - 1) ) * \
                             b * c * exp(log(k)*(c-1)) );
  //
  // expression:	(1/c) [ a + b * k^c ] ^ (1/c-1) * [ b * c * k^(c-1) ]
  // constraints:	c <= 1, c # 0
}
static qreal pf_ces_second_prime ( qreal k, qreal a, qreal b, qreal c, qreal )
{ return ( (1/c-1) * exp ( log ( a + b * exp(log(k)*c) ) * (1/c - 2) ) * \
			( a + b*c * exp(log(k)*(c-1)) )*b*exp(log(k)*(c-1))+ \
			( exp ( log ( a + b * exp(log(k)*c) ) * (1/c-1)) )* \
            b *(c-1) * exp(log(k)*(c-2)) );
  //
  // expression:	(1/c) [ a + b * k^c ] ^ (1/c-2) * [ b * c * (c-1)k^(c-2) ]
  // constraints:	c <= 1, c # 0
}
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
static qreal pf_ces2 ( qreal k, qreal a, qreal b, qreal c, qreal )
{ return ( c==0 ? a * exp(log(k)*b) : exp ( log ( a * ( (1-b) + b * exp(log(k)*c) ) ) * 1/c) );
  //
  // expression:	a * [ (1-b) + b * k^c ] ^ {1/c}		c != 0
  //			a * k ^ b				c == 0
  // constraints:	c <= 1	
}

static qreal pf_ces2_prime ( qreal k, qreal a, qreal b, qreal c, qreal )
{ return ( c==0 ? b * a * exp(log(k)*(b-1)) : \
                (1/c) * exp(log( a * ((1-b) + b * exp(log(k)*c)) ) * (1/c - 1) ) * \
                      a * b * c * exp(log(k)*(c-1)) );
  //
  // expression:	(1/c) * [ a * [ (1-b) + b * k^c ] ] ^ (1/c-1) * [ a * b * c * k^(c-1) ] 	c!=0
  // 			b * a * k ^ (b-1)			c == 0
  // constraints:	c <= 1
}

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
static qreal pf_leo ( qreal k, qreal a, qreal b, qreal , qreal alpha )
{ return ( a / b * ( 1 - exp (-b * k) +alpha ) );
  //
  // expression:	a/b * ( 1 - e^(-b * k) )
  // constraints:	a,b >= 0
}

static qreal pf_leo_prime ( qreal k, qreal a, qreal b, qreal , qreal )
{ return ( a * exp (-b * k) );
  //
  // expression:	a * e^(-b * k)
  // constraints:	a,b >= 0
}
static qreal pf_leo_second_prime ( qreal k, qreal a, qreal b, qreal , qreal )
{ return ( -b * a * exp (-b * k) );
  //
  // expression:	-b * a * e^(-b * k)
  // constraints:	a,b >= 0
}

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
static qreal pf_cd ( qreal k, qreal a, qreal b, qreal , qreal )
{ return ( a/b * exp( b * log(k) ) );
  //
  // expression:	a/b * e^(b * log(k) )
  // constraints:	a > 0, 0<b<=1
}

static qreal pf_cd_prime ( qreal k, qreal a, qreal b, qreal , qreal )
{ return ( a/k * exp( b * log(k) ) );
  //
  // expression:	a/k * e^(b * log(k) )
  // constraints:	a > 0, 0<b<=1
}

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
static qreal pf_leontiev ( qreal k, qreal a, qreal b, qreal , qreal alpha)
{ return ( a*k<b ? a*k+alpha : b+alpha );
  //
  // expression:	min{a*k,b}+c
}

static qreal pf_leontiev_prime ( qreal k, qreal a, qreal b, qreal , qreal )
{ return ( k<=b/a ? a : 0);
  //
  // expression:	a	k<=b/a
  //			0	k> b/a
}

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
static qreal pf_app ( qreal k, qreal a, qreal b, qreal c, qreal alpha )
{
 return (a*k+c*a*(log((1+exp(-b/(c*a)))/(1+exp((1/c)*(k-b/a)))))+alpha);
}

static qreal pf_app_prime ( qreal k, qreal a, qreal b, qreal c, qreal )
{
 return ( a/( 1+exp( (1/c)*(k-b/a) ) ) );
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Member functions:	pf_quad, pf_quad_prime
// Purpose:		production function and its derivative: fraction
//
// Author:		Michael Meyer
// Last modified:	Wed Mar 15 08:50:08 MET 2000
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////
static qreal pf_quad ( qreal k, qreal a, qreal b, qreal c, qreal alpha)
{
 return ((k<=exp(log(1/a)*(1/(b-1)))) ? c*(k-(a/b*exp(log(k)*b)))+alpha : 
 
                        c*(exp(log(1/a)*(1/(b-1)))-(a/b*exp(log(exp(log(1/a)*(1/(b-1))))*b)))+alpha);
}

static qreal pf_quad_prime ( qreal k, qreal a, qreal b, qreal c , qreal  )
{
 return ((k<=exp(log(1/a)*(1/(b-1)))) ? c*(1-(a*exp(log(k)*(b-1)))) : 0);
}

static qreal pf_quad_second_prime ( qreal k, qreal a, qreal b, qreal c , qreal  )
{
 return ((k<=exp(log(1/a)*(1/(b-1)))) ? -c*((b-1)*(a*exp(log(k)*(b-2)))) : 0);
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Member functions:	pf_log, pf_log_prime
// Purpose:		production function and its derivative: fraction
//
// Author:		Michael Meyer
// Last modified:	Wed Mar 15 08:50:08 MET 2000
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////
static qreal pf_log ( qreal k, qreal a, qreal b, qreal , qreal alpha)
{
 return a/b*log(exp(log(k)*b)+1+alpha);                        
}

static qreal pf_log_prime ( qreal k, qreal a, qreal b, qreal , qreal alpha  )
{
 return a*exp(log(k)*(b-1))/(exp(log(k)*b)+1+alpha);
} 
///////////////////////////////////////////////////////////////////////////////
//
//
// Member functions:	pf_gcd, pf_gcd_prime
// Purpose:		production function and its derivative: fraction
//
// Author:		Michael Meyer
// Last modified:	Wed Mar 15 08:50:08 MET 2000
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////
static qreal pf_gcd ( qreal k, qreal a, qreal b, qreal c, qreal alpha)
{
 return a/b*(exp(log(k+exp(log(c)*(1/b)))*b)-c)+alpha;                        
}

static qreal pf_gcd_prime ( qreal k, qreal a, qreal b, qreal c, qreal )
{
 return a/b*exp(log(k+exp(log(c)*(1/c)))*(b-1));
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Member functions:	pf_quad, pf_quad_prime
// Purpose:				production function and its derivative: fraction
//
// Author:				Michael Meyer
// Last modified:		Wed Mar 15 08:50:08 MET 2000
// By:					Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////
static qreal pf_michael ( qreal k, qreal a, qreal b, qreal c, qreal alpha)
{
	return ((k<=exp(log(1/a)*(1/(b-1)))) ? alpha*(c*(k-(a/b*exp(log(k)*b))))+
											(1-alpha)*(a*k+c*a*(log((1+exp(-b/(c*a)))/(1+exp((1/c)*(k-b/a)))))) :
 
                       alpha*(c*(exp(log(1/a)*(1/(b-1)))-(a/b*exp(log(exp(log(1/a)*(1/(b-1))))*b))))+
				(1-alpha)*(a*k+c*a*(log((1+exp(-b/(c*a)))/(1+exp((1/c)*(k-b/a)))))));
}

static qreal pf_michael_prime ( qreal k, qreal a, qreal b, qreal c, qreal alpha  )
{
 return ((k<=exp(log(1/a)*(1/(b-1)))) ? (alpha*c*(1-(a*exp(log(k)*(b-1)))))
					+((1-alpha)*( a/( 1+exp( (1/c)*(k-b/a) ) ) )) : (1-alpha)*( a/( 1+exp( (1/c)*(k-b/a) ) ) ) );
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//****                                                                     ****
//****                    g r o w t h _ m o d e l                          ****
//****                                                                     ****
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		growth_model
// Member function:	pf_init
// Purpose:		maps normalized (A,B,C,D) -> (a,b,c,alpha) and sets pf, pf_prime
//
// Author:		Uli Middelberg
// Last modified:	Mon May 11 12:44:16 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
void growth_model::pf_init ( void )
{
  a=A; b=B; c=C; alpha=D;
  switch ( int(pf_type)) {
    case 0 :
      pf=pf_ces; pf_prime=pf_ces_prime; pf_second_prime=pf_ces_second_prime;
    break;
    case 1 :
      pf=pf_leo; pf_prime=pf_leo_prime; pf_second_prime=pf_leo_second_prime;
    break;
    case 2 :
      pf=pf_cd; pf_prime=pf_cd_prime;
    break;
    case 3 :
      pf=pf_leontiev; pf_prime=pf_leontiev_prime;
    break;
    case 4 :
      pf=pf_ces2; pf_prime=pf_ces2_prime;
    break;
    case 5 :
      pf=pf_app; pf_prime=pf_app_prime;
    break;
    case 6 :
      pf=pf_quad; pf_prime=pf_quad_prime; pf_second_prime=pf_quad_second_prime;
    break;
    case 7 :
      pf=pf_log; pf_prime=pf_log_prime;
    break;
    case 8 :
      pf=pf_gcd; pf_prime=pf_gcd_prime;
    break;
    case 9 :
      pf=pf_michael; pf_prime=pf_michael_prime;
    break;

    default :
      error("macrodyn::growth_model::pf_init: pf type %d not yet implemented : ", QString::number(pf_type));
  }
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		growth_model
// Member function:	sf_y
// Purpose:		returns the savings rate for entire economy
//
// Author:		Uli Middelberg
// Last modified:	Mon Mar 10 14:44:08 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////
qreal growth_model::sf_y ( qreal ) {
 return syr;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		growth_model
// Member function:	sf_w
// Purpose:		returns the savings rate for the worker w.r.t the wage
//
// Author:		Uli Middelberg
// Last modified:	Mon Mar 10 14:44:08 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////
qreal growth_model::sf_w ( qreal ) {
 return swr; }

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		growth_model
// Member function:	sf_r
// Purpose:		returns the savings rate for shareholder w.r.t the rent
//
// Author:		Uli Middelberg
// Last modified:	Mon Mar 10 14:44:08 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////
qreal growth_model::sf_r ( qreal ) {
 return srr; }


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		growth_model
// Member function:	growth_model
// Purpose:		constructor
//
// Author:		Uli Middelberg
// Last modified:	Mon Mar 10 14:09:17 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

growth_model::growth_model() : baseModel(1)
{
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		growth_model
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		Uli Middelberg
// Last modified:	Tue Dec  8 14:27:09 MET 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

void growth_model::iteration(const long& )
{ 
  qreal k_n;		// value of k for the next period
//  y = (*pf)(k,a,b,c,alpha);
//  w = (*pf)(k,a,b,c,alpha) - k * (*pf_prime)(k,a,b,c,alpha);
//  r = (*pf_prime)(k,a,b,c,alpha);

  y = (*pf)(k,a,b,c,alpha);
  r = (*pf_prime)(k,a,b,c,alpha);
  w = y - k * r;

  sav_y = sf_y(y) * y;
  sav_w = sf_w(w) * w;
  sav_r = k * sf_r(r) * r;
  k_n = 1 / (1+n) * ( (1-delta_p) * k + sav_w + sav_r + sav_y);
  k = k_n;
  
// How about the qreal model?!?!
//  qreal K_old = K;
  K = (1-delta_p)*K+ sav_w *L + sav_r * L + sav_y * L;
  L *= 1+n;
  ln_K = log(K);
  ln_L = log(L);
  d_K_kL = K - k_fix*L;
//log() << a << " " << b << " " << c << " " << alpha << " : ";
//log() << syr << " " << swr << " " << srr << " : ";
//log() << sav_y << " " << sav_w << " " << sav_r << " " << k_n << "\n";
}
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		growth_model
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Uli Middelberg
// Last modified:	Mon Mar 10 15:43:17 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void growth_model::initialize()
{
  k = k_0;
  K = k_0;
  L = 1;
  k_fix = pow( a*syr/(b*(delta_p+n)),(1.0/(1.0-b)));
  d_K_kL = K - k_fix*L;
  pf_init();
}
 

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		growth_model
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Uli Middelberg
// Last modified:	98/10/15
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

qreal* growth_model::setLabels(char* label)
{
    if( !strcmp(label,"K") ) return(&K);
    if( !strcmp(label,"L") ) return(&L);
    if( !strcmp(label,"ln_K") ) return(&ln_K);
    if( !strcmp(label,"d_K_kL") ) return(&d_K_kL);    
    if( !strcmp(label,"ln_L") ) return(&ln_L);    
    if( !strcmp(label,"k") ) return(&k);
    if( !strcmp(label,"k_fix") ) return(&k_fix);    
    if( !strcmp(label,"k_0") ) return(&k_0);
    if( !strcmp(label,"y") ) return(&y);
    if( !strcmp(label,"w") ) return(&w);
    if( !strcmp(label,"r") ) return(&r);
    if( !strcmp(label,"pf_type") ) return(&pf_type);
    if( !strcmp(label,"A") ) return(&A);
    if( !strcmp(label,"B") ) return(&B);
    if( !strcmp(label,"C") ) return(&C);
    if( !strcmp(label,"D") ) return(&D);
    if( !strcmp(label,"alpha") ) return(&D);
    if( !strcmp(label,"a") ) return(&a);
    if( !strcmp(label,"b") ) return(&b);
    if( !strcmp(label,"c") ) return(&c);
    if( !strcmp(label,"d") ) return(&alpha);
    if( !strcmp(label,"syr") ) return(&syr);
    if( !strcmp(label,"swr") ) return(&swr);
    if( !strcmp(label,"srr") ) return(&srr);
    if( !strcmp(label,"sav_y") ) return(&sav_y);
    if( !strcmp(label,"sav_w") ) return(&sav_w);
    if( !strcmp(label,"sav_r") ) return(&sav_r);
    if( !strcmp(label,"n") ) return(&n);
    if( !strcmp(label,"delta_p") ) return(&delta_p);
    if( !strcmp(label,"xBundle") ) return(&xBundle);

    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		growth_model
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		Uli Middelberg
// Last modified:	Mon Mar 10 15:43:28 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void growth_model::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("growth_model::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&k;
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		growth_model
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Uli Middelberg
// Last modified:	Mon May 11 13:07:57 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

void growth_model::loadParamset(ifstream& inFile)
{
    inFile >> k_0 ;
    inFile >> pf_type;
    inFile >> A;
    inFile >> B;
    inFile >> C;
    inFile >> D;    
    inFile >> syr;
    inFile >> swr;
    inFile >> srr;
    inFile >> n;
    inFile >> delta_p;
  
    inFile >> length;
    initialize();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		growth_model
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		Uli Middelberg
// Last modified:	Mon May 11 13:11:41 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

void growth_model::saveParamset(ofstream& outFile)
{
    outFile << k_0 << "\t";
    outFile << pf_type << "\t";
    outFile << A << "\t";
    outFile << B << "\t";
    outFile << C << "\t";
    outFile << D << "\t";
    outFile << syr << "\t";
    outFile << swr << "\t";
    outFile << srr << "\t";
    outFile << n << "\t";
    outFile << delta_p << "\t";

    outFile << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		growth_model
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		Uli Middelberg
// Last modified:	Mon May 11 13:12:05 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

void growth_model::printParamset()
{
    log() << k_0 << "\n";
    log() << pf_type << "\n";
    log() << A << "\n";
    log() << B << "\n";
    log() << C << "\n";
    log() << D << "\n";
    log() << syr << "\n";
    log() << swr << "\n";
    log() << srr << "\n";
    log() << n << "\n";
    log() << delta_p << "\n";
    
    log() << length << "\n";
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
qreal* growth_model::sendModelVar(void)
{ error("macrodyn::growth_model::sendModelVar is not implemented");
  return NULL;
}
void growth_model::sendParameters(int& ,qreal** )
{ error("macrodyn::growth_model::sendParameters is not implemented");
}
void growth_model::receiveParameters(const qreal* )
{ error("macrodyn::growth_model::receiveParameters is not implemented");
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rgrowth_model
// Member function:     rgrowth_model
// Purpose:             constructor
//
// Author:              Uli Middelberg
// Last modified:       Tue Aug 19 14:34:55 METDST 1997
// By:                  Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

rgrowth_model::rgrowth_model() : growth_model()
{
  zvar = NULL;
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//****                                                                     ****
//****                   r g r o w t h _ m o d e l                         ****
//****                                                                     ****
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rgrowth_model
// Member function:     iteration
// Purpose:             perform one iteration of the system
//
// Author:              Uli Middelberg
// Last modified:       Wed Feb 26 14:13:52 MET 1997
// By:                  Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void rgrowth_model::iteration(const long& t)
{
  * zvar_ptr = zvar->dice();	// Set a new random value for the Parameter
  growth_model::iteration(t);	// iterate as usual
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rgrowth_model
// Member function:     loadParamset
// Purpose:             load a parameterset from a specified input file
//
// Author:              Uli Middelberg
// Last modified:       Wed Feb 26 14:22:40 MET 1997
// By:                  Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void rgrowth_model::loadParamset(ifstream& inputFile)
{
  inputFile >> zvar_name;		// read the name of the stochastic parameter
  inputFile >> zvar_expr;		// read the definition
  growth_model::loadParamset(inputFile);// read the parameters for the default
  					// model as usual and initialize
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rgrowth_model
// Member function:     initialize
// Purpose:             initialize the model, define the systems initial state
//
// Author:              Uli Middelberg
// Last modified:       Wed Feb 26 14:22:40 MET 1997
// By:                  Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void rgrowth_model::initialize()
{
    zvar_ptr = setLabels(zvar_name); 	// get a pointer to the parameter
    if ( zvar != NULL ) {		// delete the old zvar
      delete zvar;
    }
    
    zvar = new rand_var( this, "ranf", zvar_expr);
    					// initialize the random number generator

    growth_model::initialize();		// initialize the default model as usual
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//****                                                                     ****
//****                        D E P R E C I A T E                          ****
//****                                                                     ****
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          depreciate
// Member function:     depreciate
// Purpose:             constructor
//
// Author:              Uli Middelberg
// Last modified:       Fri Jun 12 14:16:36 METDST 1998
// By:                  Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
depreciate::depreciate() : growth_model()
{
    trans_x= new qreal[trans_x_max];
    if( !trans_x )
	fatalError("depreciate::depreciate","Can't create trans_x vector");

    trans_a= new qreal[trans_x_max-1];
    if( !trans_a )
	fatalError("depreciate::depreciate","Can't create trans_a vector");

    trans_b= new qreal[trans_x_max-1];
    if( !trans_b )
	fatalError("depreciate::depreciate","Can't create trans_b vector");
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		depreciate
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		Uli Middelberg
// Last modified:	Mon Mar 10 15:43:28 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////
void depreciate::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("depreciate::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&k;
    (*stateSpace)[1]=&delta_p;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		depreciate
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Uli Middelberg
// Last modified:	Mon Mar 10 15:43:24 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////
qreal* depreciate::setLabels(char* label)
{
    if( !strcmp(label,"_z_0") ) return(&_z_0);
    if( !strcmp(label,"_z") ) return(&_z);
//    if( !strcmp(label,"theta_type") ) return(&theta_type);
    if( !strcmp(label,"my") ) return(&my);
//    if( !strcmp(label,"trans_a") ) return(&trans_a);
//    if( !strcmp(label,"trans_b") ) return(&trans_b);

    return growth_model::setLabels(label);
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		depreciate
// Member function:	trans
// Purpose:		transforms z into the interval [a,b]
//
// Author:		Uli Middelberg
// Last modified:	Fri Jun 12 17:15:14 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
qreal  depreciate::trans(qreal z, qreal* x, qreal* a, qreal* b)
{
    int i=0;
    while(x[i]<1) {
	if(  ( (z>=x[i]) && (z<x[i+1]) ) || (x[i+1]==1)  )
//		{
//printf("\nz=%f\tx[%i]=%f\tx[%i]=%f\t",z,i,x[i],i+1,x[i+1]);
		return( a[i] + (z-x[i])*(b[i]-a[i])/(x[i+1]-x[i]) );
//		}
//	if(  ( (z>=x[i]) && (z<x[i+1]) ) || (x[i+1]=1)  )
//		return(a[i]+(b[i]-a[i])*z);
	i++;
//printf("\ni=%i\t",i);
	}
    error("macrodyn::depreciate::trans: can not compute z.  (i=%i) \n", QString::number(i));
    return(-1);
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		depreciate
// Member function:	theta_xxx
// Purpose:		specific types of thetas
//
// Author:		Uli Middelberg
// Last modified:	Fri Jun 12 14:59:42 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
static qreal theta_logistic ( qreal d, qreal m)
{ return (m * d * (1-d)) ;
}

static qreal theta_tent ( qreal d, qreal m)
{ 
  
  if( d<=m ) {
    return (1/m * d);
  } else {
    return (1/(1-m)*(1-d));
  }
}

static qreal theta_saw ( qreal d, qreal m)
{ if( d<m ) {
    return (1/m * d);
  } else {
    return (1/(1-m)*(d-m));
  }
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		depreciate
// Member function:	theta_init
// Purpose:		sets theta to a specific type
//
// Author:		Uli Middelberg
// Last modified:	Fri Aug 22 14:08:03 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////
void depreciate::theta_init ( void )
{
  switch ( int(theta_type)) {
    case 0 :
      theta=theta_logistic;
    break;
    case 1 :
      theta=theta_tent;
    break;
    case 2 :
      theta=theta_saw;
    break;
    default :
      error("macrodyn::depreciate::theta_init: theta type %d not yet implemented : ", QString::number(theta_type));
  }
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          depreciate
// Member function:     initialize
// Purpose:             initialize the model, define the systems initial state
//
// Author:              Uli Middelberg
// Last modified:       
// By:                  
//
///////////////////////////////////////////////////////////////////////////////
void depreciate::initialize()
{
    _z      = _z_0;			// initialize the underlying process
    _z_var_ptr = setLabels(_z_var_name);// get a pointer to the parameter
    if (_z_var_ptr == NULL) 
       fatalError("depreciate::initialize","Var Name not known");
    * _z_var_ptr = trans(_z,trans_x,trans_a,trans_b);	// initialize _z_var_ptr
    theta_init ();
    
    growth_model::initialize();		// initialize the default model as usual
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          depreciate
// Member function:     loadParamset
// Purpose:             load a parameterset from a specified input file
//
// Author:              Uli Middelberg
// Last modified:       Fri Jun 12 14:16:19 METDST 1998
// By:                  Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
void depreciate::loadParamset(ifstream& inputFile)
{
  inputFile >> theta_type;
  inputFile >> my;
  inputFile >> _z_0;
  inputFile >> _z_var_name;

  int	i,j;			// Index
  i=0;
  while(i<trans_x_max) {
	 inputFile >> trans_x[i];
	//printf("x[%i]=%f\n",i,trans_x[i]); 
	 if(trans_x[i]==1) break;
	 i++;
	}
	//printf("i=%i\n",i); 
 	
  if(i==trans_x_max)
	error("macrodyn::depreciate::loadParamset",
	      "the number i of x[i] must be less trans_x_max");

  // remember that a,b have one number less than x
  for (int j=0;j<i;j++) {
	 inputFile >> trans_a[j];
	//printf("a[%j]=%f\n",i,trans_a[j]); 
	}
  for (int j=0;j<i;j++) {
	 inputFile >> trans_b[j];
	//printf("b[%j]=%f\n",i,trans_b[j]); 
	}
	//printf("end trans\n"); 

  growth_model::loadParamset(inputFile);// read the parameters for the default
}  					// model as usual and initialize

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		depreciate
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		Uli Middelberg
// Last modified:	Wed Jun 10 15:54:22 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
void depreciate::saveParamset(ofstream& outFile)
{
    int i,j;
    outFile << theta_type << "\t";
    outFile << my << "\n";
    outFile << _z_0 << "\t";
    outFile << _z_var_name << "\n";
    i=0;
    while(i<trans_x_max) {
	 outFile << trans_x[i] << "\t";
	 if(trans_x[i]==1) break;
	 i++;
	}
    outFile << "\n";
    for (int j=0;j<i;j++) {
	 outFile << trans_a[i] << "\t";
	}
    outFile << "\n";
    for (int j=0;j<i;j++) {
	 outFile << trans_b[i] << "\t";
	}
    outFile << "\n";
    growth_model::saveParamset(outFile);
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		depreciate
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		Uli Middelberg
// Last modified:	Wed Jun 10 16:00:24 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
void depreciate::printParamset()
{
    int i,j;
    log() << theta_type << "\n";
    log() << my << "\n";
    log() << _z_0 << "\n";
    log() << _z_var_name << "\n";
    i=0;
    while(i<trans_x_max) {
     log() << trans_x[i] << "\t";
	 if(trans_x[i]==1) break;
	 i++;
	}
    log() << "\n";
    for (int j=0;j<i;j++) {
     log() << trans_a[i] << "\t";
	}
    log() << "\n";
    for (int j=0;j<i;j++) {
     log() << trans_b[i] << "\t";
	}
    log() << "\n";
    growth_model::printParamset();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          depreciate
// Member function:     iteration
// Purpose:             perform one iteration of the system
//
// Author:              Uli Middelberg
// Last modified:       Wed Jun 10 13:05:42 METDST 1998
// By:                  Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
void depreciate::iteration(const long& t)
{
  _z = (* theta ) (_z, my);			   // iterate _z
  * _z_var_ptr = trans(_z,trans_x,trans_a,trans_b);// transform _z to _z_var_ptr
  growth_model::iteration(t);			   // iterate the rest
};

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//****                                                                     ****
//****                         P A S I N E T T I                           ****
//****                                                                     ****
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		pasinetti
// Member function:	pasinetti
// Purpose:			constructor
//
// Author:			Marc Mueller
// Last modified:	Mon Mar 12 15:57:23 MET 2001
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

pasinetti::pasinetti() : growth_model()
{ 
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		pasinetti
// Member function:	pasinetti
// Purpose:			destructor
//
// Author:			Marc Mueller
// Last modified:	Mon Mar 12 15:57:23 MET 2001
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

pasinetti::~pasinetti()
{ 
}
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		pasinetti
// Member function:	loadParamset
// Purpose:			load a parameterset from a specified input file
//
// Author:			Marc Mueller
// Last modified:	Mon Mar 12 15:57:45 MET 2001
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void pasinetti::loadParamset(ifstream& inFile)
{
    inFile >> k_1 ;
    growth_model::loadParamset(inFile);
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		pasinetti
// Member function:	saveParamset
// Purpose:			write parameterset into a file
//
// Author:			Marc Mueller
// Last modified:	Mon Dec  7 18:43:50 MET 1998
// By:				Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

void pasinetti::saveParamset(ofstream& outFile)
{
    outFile << k_1 << "\t";
    growth_model::saveParamset(outFile);
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		pasinetti
// Member function:	printParamset
// Purpose:			print parameterset on the screen
//
// Author:			Marc Mueller
// Last modified:	Mon Dec  7 18:44:32 MET 1998
// By:				Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

void pasinetti::printParamset()
{
    log() << k_1 << "\n";
    growth_model::printParamset();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		pasinetti
// Member function:	setLabels
// Purpose:			return a pointer to a variable or a parameter specified
//
// Author:			Marc Mueller
// Last modified:	Mon Mar 12 15:58:19 MET 2001
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

qreal* pasinetti::setLabels(char* label)
{
    if( !strcmp(label,"k_1") ) return(&k_1);
    if( !strcmp(label,"kw_0") ) return(&k_1);
    if( !strcmp(label,"k_0") ) return(&k_0);
    if( !strcmp(label,"kr_0") ) return(&k_0);
    if( !strcmp(label,"kw") ) return(&kw);
    if( !strcmp(label,"kr") ) return(&kr);
    if( !strcmp(label,"k_pas") ) return(&k_pas);
    if( !strcmp(label,"Q_r") ) return(&Q_r);
    if( !strcmp(label,"Q_w") ) return(&Q_w);
    if( !strcmp(label,"Q_r_n") ) return(&Q_r_n);
    if( !strcmp(label,"Q_w_n") ) return(&Q_w_n);
    if( !strcmp(label,"Quot_kr") ) return(&Quot_kr);
    if( !strcmp(label,"Quot_kw") ) return(&Quot_kw);
    if( !strcmp(label,"y_ini") ) return(&y_ini);
    if( !strcmp(label,"y_ini_prime") ) return(&y_ini_prime);
    if( !strcmp(label,"r") ) return(&r);
    if( !strcmp(label,"E") ) return(&E);
    if( !strcmp(label,"E_n") ) return(&E_n);
    if( !strcmp(label,"E_f_pr_n") ) return(&E_f_pr_n);
    if( !strcmp(label,"E_steady_leon") ) return(&E_steady_leon);
    if( !strcmp(label,"E_steady_leon_prime") ) return(&E_steady_leon_prime);
    if( !strcmp(label,"bif_point") ) return(&bif_point);
    if( !strcmp(label,"xBundle") ) return(&xBundle);
    if( !strcmp(label,"yBundle") ) return(&yBundle);
    if( !strcmp(label,"steady_leon") ) return(&steady_leon);
    if( !strcmp(label,"steady_kc") ) return(&steady_kc);
    if( !strcmp(label,"steady_kw") ) return(&steady_kw);
    if( !strcmp(label,"steady_kc_cd") ) return(&steady_kc_cd);
    if( !strcmp(label,"steady_kw_cd") ) return(&steady_kw_cd);
    if( !strcmp(label,"Q") ) return(&Q);
    if( !strcmp(label,"Konsum_C") ) return(&Konsum_C);
    if( !strcmp(label,"Konsum_K") ) return(&Konsum_K);
    if( !strcmp(label,"Konsum_A") ) return(&Konsum_A);
    if( !strcmp(label,"Konsum_C_n") ) return(&Konsum_C_n);
    if( !strcmp(label,"Konsum_A_n") ) return(&Konsum_A_n);
    if( !strcmp(label,"lohn") ) return(&lohn);
    if( !strcmp(label,"kap_an") ) return(&kap_an);
    if( !strcmp(label,"kap_ka") ) return(&kap_ka);
    if( !strcmp(label,"lohn_od") ) return(&lohn_od);
    if( !strcmp(label,"kap_an_od") ) return(&kap_an_od);
    if( !strcmp(label,"kap_ka_od") ) return(&kap_ka_od);
    if( !strcmp(label,"eink_an") ) return(&eink_an);
    if( !strcmp(label,"kap_ges") ) return(&kap_ges);
    if( !strcmp(label,"y_steady") ) return(&y_steady);
    if( !strcmp(label,"s_hat") ) return(&s_hat);
    if( !strcmp(label,"Ef_sc") ) return(&Ef_sc);
    if( !strcmp(label,"Z") ) return(&Z);
    if( !strcmp(label,"Z2") ) return(&Z2);

    return growth_model::setLabels(label);
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		pasinetti
// Member function:	initialize
// Purpose:			initialize the model, define the systems initial state
//
// Author:			Marc Mueller
// Last modified:	Mon Mar 12 15:58:42 MET 2001
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void pasinetti::initialize()
{
  E=0;
  Z=0;	
  kr = k_0;
  kw = k_1;
  growth_model::initialize();
  k = k_0 + k_1;
  k_pas = k;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		pasinetti
// Member function:	iteration
// Purpose:			perform one iteration of the system
//
// Author:			Marc Mueller
// Last modified:	Thu Aug 12  18:34:45 MET 1999
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void pasinetti::iteration(const long& t)
{ 

	growth_model::iteration(t);
	
    qreal kr_n;		// shareholders value of k for the next period
    qreal kw_n;		// workers value of k for the next period
	
////Einkommensquoten im Allgemeinen ohne Dynamik/////
	y_ini = (*pf)(k_pas,a,b,c,alpha);
	y_ini_prime = (*pf_prime)(k_pas,a,b,c,alpha);
	E = k_pas*y_ini_prime/y_ini;
	Q_r = k_0*E/k_pas;
	Q_w = 1-Q_r;

// Einkommensanteile ohne Dynamik

	kap_an_od=k_1*y_ini_prime;
	kap_ka_od=k_0*y_ini_prime;
	lohn_od=y_ini-k_pas*y_ini_prime;


////Kapitalintensitaeten im dynamischen Verhalten///

	kr_n=(1/(1+n))*((1-delta_p)*kr+sf_r(r)*kr*r); 
	kw_n=(1/(1+n))*((1-delta_p)*kw+sf_w(w)*y-sf_w(w)*kr*r);

	kr=kr_n;
	kw=kw_n;
	k=kw+kr;

//Einkommensanteile mit Dynamik

	kap_an=kw*(*pf_prime)((kr+kw),a,b,c,alpha);
	kap_ka=kr*(*pf_prime)((kr+kw),a,b,c,alpha);
	lohn=(*pf)((kr+kw),a,b,c,alpha)-((kr+kw)*(*pf_prime)((kr+kw),a,b,c,alpha));
	
	kap_ges=kap_an+kap_ka+lohn;
	eink_an=kap_an+lohn;

////Einkommensquoten und Vermoegensverteilung im dynamischen Verhalten////	
	E_n=(kr+kw)*(*pf_prime)((kr+kw),a,b,c,alpha)/(*pf)((kr+kw),a,b,c,alpha);
	Q_r_n = kr*E_n/(kr+kw);
	Q_w_n = 1-Q_r_n;
	Quot_kr = kr/(kr+kw);
	Quot_kw = kw/(kr+kw);
	Konsum_A_n=(1-swr)*(srr*y-k*(n+delta_p))/(srr-swr);
	
	if(swr<=E_n*srr) {
	Konsum_C_n=(1-srr)*(n+delta_p)/(srr-swr)*k*(1-swr/(E_n*srr));
	}
	else Konsum_C_n=0;

	Z =  Konsum_A_n - Konsum_C_n;
	Z2=  eink_an - kap_ka;

////CD:
////						Steady-State, Steady-State fuer kc,kw
////						Elastizit t im Steady-State							
if(pf_type==2){

    qreal dummy=(n+delta_p)/(srr*a);
    qreal dummy1=1/(b-1);

	steady_cd=pow(dummy,dummy1);

	y_steady = (*pf)(steady_cd,a,b,c,alpha);
//	y_prime_steady = (*pf_prime)(steady_cd,a,b,c,alpha);

	steady_kc_cd=srr/(srr-swr)*steady_cd*(1-swr/(b*srr));
	steady_kw_cd=steady_cd-steady_kc_cd;
	
//    	Q=1/(srr-swr)*(0.8*srr-swr);
	
	if(swr<=b*srr){
		Konsum_A=(1-swr)*(srr*y_steady-steady_cd*(n+delta_p))/(srr-swr);
		Konsum_C=(1-srr)*(n+delta_p)/(srr-swr)*steady_cd*(1-swr/(b*srr));	
	}
	else{
		Konsum_A=0;
	}
}

/////////////CES Technologie////////////////////////77

if(pf_type==0){

    qreal dummy=(n+delta_p)/srr;
    qreal dummy1=c/(1-c);
    qreal dummy2=pow(dummy,dummy1);
    qreal dummy3=-1/c;

	dummy=(dummy2-pow(b,2))/a*b;
	steady_ces=pow(dummy,dummy3);

	y_steady = (*pf)(steady_ces,a,b,c,alpha);
	y_prime_steady=(*pf_prime)(steady_ces,a,b,c,alpha);

	E_f=steady_ces*y_prime_steady/y_steady;
	Ef_sc=E_f*srr;
	
	E_f_pr_n=steady_ces*(*pf_second_prime)(steady_ces,a,b,c,alpha)/(*pf_prime)(steady_ces,a,b,c,alpha);
	bif_point=-2*(n+1)/(n+delta_p);	
//log() << "point =" << bif_point << "\n";
	
	s_hat=srr*(E_f_pr_n + bif_point)/(  E_f_pr_n/E_f-2*((1+n)/(2+n+delta_p) )  )  ;
   
	Konsum_A=(1-swr)*(srr*y_steady-steady_ces*(n+delta_p))/(srr-swr);
	Konsum_C=(1-srr)*(n+delta_p)/(srr-swr)*steady_ces*(1-swr/(E_f*srr));

//log() << "CES Fixpunkt =" << steady_ces << "\n";
}

////Leontief Technologie:
////					Steady-State, Steady-State fuer kc,kw
////					Elastizitaet im Steady-State		

if(pf_type==5){
	steady_leon = C*log(A*srr/(n+delta_p)-1)+B/A;		//Umkehrfunktion der Leontief

	y_steady = (*pf)(steady_leon,a,b,c,alpha);
	y_prime_steady = (*pf_prime)(steady_leon,a,b,c,alpha);

	E_f= steady_leon*y_prime_steady/y_steady;
//	E_steady_leon_prime = steady_leon*y_leon_prime2/y_leon_prime;
//	steady_kc_leon=sf_r(r)/(sf_r(r)-sf_w(w))*steady_leon*(1-sf_w(w)/(E_steady_leon*sf_r(r)));
//	steady_kw_leon=steady_leon-steady_kc_leon;

	Konsum_A=(1-swr)*(srr*y_steady-steady_leon*(n+delta_p))/(srr-swr);
	Konsum_C=(1-srr)*(n+delta_p)/(srr-swr)*steady_leon*(1-swr/(E_f*srr));

//		if(t==length){
//			log() << "Leontief Steady-State =" << steady_leon << "\n";
//			log() << "Leontief Steady-State_kc =" << steady_kc_leon << "\n";
//			log() << "Leontief Steady-State_kw =" << steady_kw_leon << "\n";
//			log() << "Leontief Elastiziaet von f' =" << E_steady_leon_prime << "\n";
//		}
}
}  	
//*****************************************************************************
// eof
