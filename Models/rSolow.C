///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/rSolow.C,v 1.1 2000/08/31 16:08:52 mmeyer Exp $
//
// Module name:		rSolow.C
//
// Author:		Michael Meyer & Stefan Lueke
// Last modified:	03/12/99
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

#include "rSolow.h"
#include "../error.h"



///////////////////////////////////////////////////////////////////////////////
//
//
// Functions:		rSolow.C
// Purpose:		production function and its derivative: 
//
// Author:		Uli Middelberg
// Last modified:	Mon Mar 10 14:44:08 MET 1997
// By:			Uli Middelberg
//
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
static qreal pf_leo ( qreal k, qreal a, qreal b, qreal c, qreal )
{ return ( a / b * ( 1 + c - exp (-b * k) ) );
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

///////////////////////////////////////////////////////////////////////////////
// Functions:		pf_cd, pf_cd_prime
// Purpose:		production function and its derivative: cobb-douglas
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
// Member functions:	pf_leontiev, pf_leontiev_prime
// Purpose:		production function and its derivative: fraction
///////////////////////////////////////////////////////////////////////////////

static qreal pf_leontiev ( qreal k, qreal a, qreal b, qreal c, qreal )
{ return ( a*k<b ? a*k+c : b+c );
  //
  // expression:	qMin{a*k,b}+c
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
 return (a*k+alpha*a*(log((1+exp(-b/(alpha*a)))/(1+exp((1/alpha)*(k-b/a)))))+c);
}

static qreal pf_app_prime ( qreal k, qreal a, qreal b, qreal , qreal alpha )
{
 return ( a/( 1+exp( (1/alpha)*(k-b/a) ) ) );
}

///////////////////////////////////////////////////////////////////////////////
// Member functions:	pf_klaus, pf_klaus_prime
// Purpose:		production function and its derivative: fraction
///////////////////////////////////////////////////////////////////////////////

static qreal pf_klaus ( qreal k, qreal a, qreal b, qreal , qreal )
{ return ( a * exp( b * log(k) ) );
  //
  // constraints:	a > 0, 0<b<=1
}

static qreal pf_klaus_prime ( qreal k, qreal a, qreal b, qreal , qreal )
{
 return ( a * b * exp( (b-1) * log(k) ) );
}

///////////////////////////////////////////////////////////////////////////////
// Member functions:	pf_klaus_2, pf_klaus_2_prime
// Purpose:		production function and its derivative: fraction
///////////////////////////////////////////////////////////////////////////////

static qreal pf_klaus_2 ( qreal k, qreal a, qreal b, qreal , qreal )
{ return ( a * (exp( b * log(k)) + k ));
  //
  // constraints:	a > 0, 0<b<=1
}

static qreal pf_klaus_prime_2 ( qreal k, qreal a, qreal b, qreal , qreal )
{
 return ( a * ( b*exp( (b-1)*log(k)) +1) );
}


//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//****                                                                     ****
//****                        r S O L O W                                  ****
//****                                                                     ****
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

/////////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rSolow
// Member function:	trans
// Purpose:		transforms z into the interval [a,b]
//
// Author:		Uli Middelberg
// Last modified:	Fri Feb  12 17:15:14 METDST 1999
// By:			Michael Meyer & Stefan Lueke
//
///////////////////////////////////////////////////////////////////////////////

qreal trans(qreal z, qreal* x, qreal* a, qreal* b)
{
    int i=0;
    while(x[i]<1) {
	if(  ( (z>=x[i]) && (z<x[i+1]) ) || (x[i+1]==1)  )
		return( a[i] + (z-x[i])*(b[i]-a[i])/(x[i+1]-x[i]) );
	i++;
	}
		
    return(-1);
    
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rSolow
// Member function:	theta_xxx
// Purpose:		specific types of thetas
//
// Author:		Uli Middelberg
// Last modified:	Fri Feb 12 14:59:42 METDST 1999
// By:			Michael Meyer & Stefan Lueke
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
// Class name:		rSolow
// Member function:	pf_init
// Purpose:		maps normalized (A,B,C,D) -> (a,b,c,alpha) and sets pf, pf_prime
//
// Author:		Uli Middelberg
// Last modified:	Mon Feb 22 12:44:16 METDST 1999
// By:			Michael Meyer & Stefan Lueke
//
///////////////////////////////////////////////////////////////////////////////
void rSolow::pf_init ( void )
{
  a=A; b=B; c=C; alpha=D;
  switch ( int(pf_type)) {
    case 0 :
      pf=pf_ces; pf_prime=pf_ces_prime;
    break;
    case 1 :
      pf=pf_leo; pf_prime=pf_leo_prime;
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
      pf=pf_klaus; pf_prime=pf_klaus_prime;
    break;
    case 7 :
      pf=pf_klaus_2; pf_prime=pf_klaus_prime_2;
    break;

    default :
      error("macrodyn::growth_model::pf_init: pf type %d not yet implemented : ", QString::number(pf_type));
  }
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rSolow
// Member function:	rSolow
// Purpose:		constructor
//
// Author:		Michael Meyer & Stefan Lüke
// Last modified:	Mon Mar 10 14:09:17 MET 1999
// By:			Michael Meyer 
//
///////////////////////////////////////////////////////////////////////////////

rSolow::rSolow() : baseModel(1)
{

a_paramset = new st_paramset;
b_paramset = new st_paramset;
c_paramset = new st_paramset;
d_paramset = new st_paramset;
n_paramset = new st_paramset;
s_paramset = new st_paramset;
delta_p_paramset = new st_paramset;


a_paramset->mc = NULL;
a_paramset->zvar = NULL;

b_paramset->mc = NULL;
b_paramset->zvar = NULL;

c_paramset->mc = NULL;
c_paramset->zvar = NULL;

d_paramset->mc = NULL;
d_paramset->zvar = NULL;

n_paramset->mc = NULL;
n_paramset->zvar = NULL;

s_paramset->mc = NULL;
s_paramset->zvar = NULL;

delta_p_paramset->mc = NULL;
delta_p_paramset->zvar = NULL;

}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rSolow
// Member function:	rSolow
// Purpose:		destructor
//
// Author:		Michael Meyer & Stefan Lüke
// Last modified:	Mon Mar 10 14:09:17 MET 1999
// By:			Michael Meyer 
//
///////////////////////////////////////////////////////////////////////////////

rSolow::~rSolow(void) {
if(a_paramset) delete a_paramset;
if(b_paramset) delete b_paramset;
if(c_paramset) delete c_paramset;
if(d_paramset) delete d_paramset;
if(n_paramset) delete n_paramset;
if(s_paramset) delete s_paramset;
if(delta_p_paramset) delete delta_p_paramset;

}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rSolow
// Member function:     noise_iteration
// Purpose:             perform one iteration of the system
//
// Author:              Michael Meyer & Stefan Lueke
// Last modified:       Wed Feb 26 14:13:52 MET 1999
// By:                  Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void rSolow::noise_iteration(st_paramset *temp_paramset){ 

qreal z_1=0;
qreal z_2=0;

switch  (temp_paramset->type) {
  case -1 :
  break;  

  case 0 :
    z_1 = temp_paramset->z_0;    
    temp_paramset->z_trans = z_1;
    temp_paramset->z_0 = z_1;
  break;  
  
  case 1 :
    switch  (temp_paramset->theta_type) {
      case 0 :	
    	z_1 = theta_logistic (temp_paramset->z_0, temp_paramset->my);    
      break;
      case 1 :		
    	z_1 = theta_tent (temp_paramset->z_0, temp_paramset->my);
      break;
      case 2 :		
    	z_1 = theta_saw (temp_paramset->z_0, temp_paramset->my);
      break;
      
      default:
        error("requested theta_type not available");
      }   
      z_2 = trans(z_1,temp_paramset->trans_x,temp_paramset->trans_a,temp_paramset->trans_b);
      temp_paramset->z_trans = z_2;
      temp_paramset->z_0 = z_1;
  break;
  
  case 2 :
    switch  (temp_paramset->mc_flag) {
      case 0 :	
    	z_1 = temp_paramset->zvar->dice(); 
      break;
      case 1 :		
    	z_1 = temp_paramset->mc->dice();
      break;
      
      default:
        error("requested theta_type not available");
      }
       
     temp_paramset->z_trans = z_1;
     temp_paramset->z_0 = z_1;
  break;   
  
  default:
        error("requested type not available");
  }
  
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rSolow
// Member function:     iteration
// Purpose:             perform one iteration of the system
//
// Author:              Michael Meyer & Stefan Lueke
// Last modified:       Wed Feb 26 14:13:52 MET 1999
// By:                  Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void rSolow::iteration(const qint64& )
{ 
  qreal k_n;		// value of k for the next period

  if (a_paramset->type > -1) {
    noise_iteration(a_paramset);
    A = a_paramset->z_trans;}

  if (b_paramset->type > -1) {
    noise_iteration(b_paramset);
    B = b_paramset->z_trans;}

  if (c_paramset->type > -1) {
    noise_iteration(c_paramset);
    C = c_paramset->z_trans;}
  
  if (d_paramset->type > -1) {
    noise_iteration(d_paramset);
    D = d_paramset->z_trans;}

  if (n_paramset->type > -1) {
    noise_iteration(n_paramset);
    n = n_paramset->z_trans;}

  if (s_paramset->type > -1) {
    noise_iteration(s_paramset);
    s = s_paramset->z_trans;}

  if (delta_p_paramset->type > -1) {
    noise_iteration(delta_p_paramset);
    delta_p = delta_p_paramset->z_trans;}

  time_t += 1;                  
  
  y = (*pf)(k,A,B,C,D);

  consumption = (1-s)*y;
  
 if (time_t > 1000){
  cons_sum += consumption;
  av_cons = cons_sum/(time_t-1000);
 }
 
  k_n = 1 / (1+ n) * ( (1- delta_p) * k + s * y );
  k = k_n;
  
  

}
    

    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rSolow 
// Member function:	initialize 
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Michael Meyer & Stefan Lüke
// Last modified:	Mon Mar 10 15:43:17 MET 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void rSolow::delete_st_paramset_stochptr(st_paramset *temp_paramset){
if (temp_paramset->mc_flag){
	if (temp_paramset->mc != NULL) {
		   delete temp_paramset->mc;	   
	       	   temp_paramset->mc = new markov_chain (this, "ranf", temp_paramset->zvar_expr,temp_paramset->mc_matrix);
	}
	
	
	}
	else {
	if( temp_paramset->zvar != NULL){
	       	   delete temp_paramset->zvar;
	temp_paramset->zvar = new rand_var (this, "ranf", temp_paramset->zvar_expr);
	}
}
}

void rSolow::initialize()
{

k=k_0;
pf_init();
consumption = 0.0;
cons_sum = 0.0;
av_cons = 0.0;
time_t = 0;
 
delete_st_paramset_stochptr(a_paramset);
delete_st_paramset_stochptr(b_paramset);
delete_st_paramset_stochptr(c_paramset);
delete_st_paramset_stochptr(d_paramset);
delete_st_paramset_stochptr(n_paramset);
delete_st_paramset_stochptr(s_paramset);
delete_st_paramset_stochptr(delta_p_paramset);

a_paramset->z_0 = a_0;
b_paramset->z_0 = b_0;
c_paramset->z_0 = c_0;
d_paramset->z_0 = d_0;
n_paramset->z_0 = n_0;
s_paramset->z_0 = s_0;
delta_p_paramset->z_0 = delta_p_0;

}	


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rSolow
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Uli Middelberg
// Last modified:	99/02/23
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

qreal* rSolow::setLabels(const QString& label)
{
    if (label == "K") return(&K);
    if (label == "L") return(&L);
    if (label == "k") return(&k);
    if (label == "k_0") return(&k_0);
    if (label == "y") return(&y);
    if (label == "pf_type") return(&pf_type);
    if (label == "A") return(&A);
    if (label == "B") return(&B);
    if (label == "C") return(&C);
    if (label == "D") return(&D);
    if (label == "alpha") return(&D);
    if (label == "a") return(&a);
    if (label == "b") return(&b);
    if (label == "c") return(&c);
    if (label == "d") return(&alpha);
    if (label == "s") return(&s);
    if (label == "n") return(&n);
    if (label == "delta_p") return(&delta_p);

    if (label == "consumption") return(&consumption);
    if (label == "av_cons") return(&av_cons);


    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rSolow
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		Uli Middelberg
// Last modified:	Mon Mar 10 15:43:28 MET 1997
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void rSolow::sendStateSpace(int &quantity,QList<qreal *> *stateSpace)
{
    stateSpace->clear();
    quantity = dimension;
    *stateSpace << &k;                                
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rSolow
// Member function:	load x_paramset
// Purpose:		
//
// Author:		Michael Meyer & Stefan Lueke
// Last modified:	Mon May 11 13:07:57 METDST 1998
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void rSolow::read_sim(QTextStream& inFile, st_paramset *temp_paramset){

  int	i;			// Index

  	inFile >> temp_paramset->type;

  
	
    switch  (temp_paramset->type) {

      case -1 :	
    	inFile >> temp_paramset->z_0;	
	break;
	
      case 0 :	
    	inFile >> temp_paramset->z_0;	 
        break;

      case 1 :		
  	inFile >> temp_paramset->theta_type;
	inFile >> temp_paramset->my;
    	inFile >> temp_paramset->z_0; 
      	
        i=0;
        while(i<Trans_x_Max) {
	      inFile >> temp_paramset->trans_x[i];
	      //printf("x[%i]=%f\n",i,trans_x[i]); 
	  if(temp_paramset->trans_x[i]==1) break;
	  i++;
	}
	//printf("i=%i\n",i); 
 	
        if(i==Trans_x_Max)
	  error("macrodyn::rSolow",
          "the number i of x[i] must be less Trans_x_Max");

        // length of trans_a and trans_b = trans_x - 1
        for (int j=0;j<i;j++) {
	 inFile >> temp_paramset->trans_a[j];
	//printf("a[%j]=%f\n",i,trans_a[j]); 
 	}
        for (int j=0;j<i;j++) {
	 inFile >> temp_paramset->trans_b[j];
	//printf("b[%j]=%f\n",i,trans_b[j]); 
	}
	//printf("end trans\n"); 
      break;
      
      case 2:		
 	  inFile >> in_string;

          if( in_string[0] == '{' ) {		
	     // we have a definition of a markov chain
            temp_paramset->mc_flag = 1; 
	        
           n_states = in_string.count(';');
           temp_paramset->zvar_expr = in_string;

          	for (int n = 0; n < n_states; n++) {
            	 inFile >> in_string;	
	    	 	// compose the matrix from
		 //temp_paramset->mc_matrix[0] = '\0';	
             temp_paramset->mc_matrix += in_string;
	    	   // the input file with blank at the
             temp_paramset->mc_matrix += " ";
            	  // end of each row
	 	}  
		
		temp_paramset->mc = NULL;
	        temp_paramset->mc = new markov_chain (this, "ranf", temp_paramset->zvar_expr,temp_paramset->mc_matrix);
         } else {
         temp_paramset->mc_flag = 0;
	 temp_paramset->zvar_expr = in_string;
	
	 temp_paramset->zvar = NULL;
	 temp_paramset->zvar = new rand_var (this, "ranf", temp_paramset->zvar_expr);
	 }
	break;
		    	
      default :
        error("macrodyn::rSolow",
	      "error in definition of stochastic parameter types");
    }
  }
   


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rSolow
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Michael Meyer & Stefan Lueke
// Last modified:	Mon May 11 13:07:57 METDST 1998
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void rSolow::loadParamset(QTextStream& inFile){
    
    inFile >> k_0 ;    
    			  // read the starting point
    inFile >> pf_type; 	  // read the type of the pf-funktion

    read_sim(inFile,a_paramset);
 
    read_sim(inFile,b_paramset);
 
    read_sim(inFile,c_paramset);

    read_sim(inFile,d_paramset);
    
    read_sim(inFile,n_paramset);
    
    read_sim(inFile,s_paramset);
    
    read_sim(inFile,delta_p_paramset);

a_0 = a_paramset->z_0;
b_0 = b_paramset->z_0;
c_0 = c_paramset->z_0;
d_0 = d_paramset->z_0;
n_0 = n_paramset->z_0;
s_0 = s_paramset->z_0;
delta_p_0 = delta_p_paramset->z_0;

    inFile >> length;         	    
    initialize();
       
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rSolow
// Member functions:	sendModelVar, sendParameters, receiveParameters
// Purpose:		for compatibity reasons only
//
// Author:		Uli Middelberg
// Last modified:	Tue Mar 11 16:10:38 MET 1997
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

qreal* rSolow::sendModelVar(void)
{ error("macrodyn::rSolow::sendModelVar is not implemented");
  return NULL;
}
void rSolow::sendParameters(int& ,qreal** )
{ error("macrodyn::rSolow::sendParameters is not implemented");
}
void rSolow::receiveParameters(const QList<qreal>&)
{ error("macrodyn::rSolow::receiveParameters is not implemented");
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rSolow
// Member function:	save_paramset
// Purpose:		write parameterset into a file
//
// Author:		Michael Meyer & Stefan Lueke
// Last modified:	Mon Mar 11 13:11:41 METDST 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void rSolow::save_st_Paramset(QTextStream& outFile, st_paramset *temp_paramset){

   int	i;			// Index
  
    	outFile << temp_paramset->type << "\t";
	
    switch  (temp_paramset->type) {
      case -1:
 	outFile << temp_paramset->z_0 << "\t";  
      break;
      case 0 :	
    	outFile << temp_paramset->z_0 << "\t";	 
      break;
	
      case 1 :		
	outFile << temp_paramset->theta_type << "\t";
	outFile << temp_paramset->my << "\t";
    	outFile << temp_paramset->z_0 << "\t"; 
	
        i=0;
        while(i<Trans_x_Max) {
	      outFile << temp_paramset->trans_x[i] << "\t";
	      //printf("x[%i]=%f\n",i,trans_x[i]); 
	      if(temp_paramset->trans_x[i]==1) break;
	      i++;
	}
       outFile << "\t";
       for (int j=0;j<i;j++) {
	 outFile << temp_paramset->trans_a[j] << "\t";
	}
	outFile << "\t";
       for (int j=0;j<i;j++) {
	 outFile << temp_paramset->trans_b[j] << "\t";
	}
        outFile << "\t";
        break;
   
      case 2:
        outFile << temp_paramset->zvar_expr << "\t";
	  if( temp_paramset->mc_flag == 1) {
	     outFile << temp_paramset->mc_matrix << "\t";
          }
       break;
	
    }

 }

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rSolow
// Member function:	saveParamset
// Purpose:		save a parameterset from a specified input file
//
// Author:		Michael Meyer & Stefan Lueke
// Last modified:	Mon May 11 13:07:57 METDST 1998
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void rSolow::saveParamset(QTextStream& outFile)

{     
    outFile << k_0 ;				
    outFile << pf_type;

    save_st_Paramset(outFile,a_paramset);
 
    save_st_Paramset(outFile,b_paramset);
 
    save_st_Paramset(outFile,c_paramset);

    save_st_Paramset(outFile,d_paramset);
    
    save_st_Paramset(outFile,n_paramset);
    
    save_st_Paramset(outFile,s_paramset);
    
    save_st_Paramset(outFile,delta_p_paramset);
 
    outFile << length;
    
    baseModel::saveParamset(outFile);
}            	    
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Class name:      rSolow                                                    //
// Member function: saveParamsetWithNames                                     //
// Purpose:         add  parameterset to printfile                            //
// Last modified:                                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
void rSolow::saveParamsetWithNames(QTextStream& outFile)
{
    outFile << "rSolow:\n";
    outFile << "k_0 =" << k_0 << "\tpf_type =" << pf_type;
    outFile << "A Parameterset" << "\t";
    save_st_ParamsetWithNames(outFile,a_paramset);
    outFile << "B Parameterset" << "\t";
    save_st_ParamsetWithNames(outFile,b_paramset);
    outFile << "C Parameterset" << "\t";
    save_st_ParamsetWithNames(outFile,c_paramset);
    outFile << "D Parameterset" << "\t";
    save_st_ParamsetWithNames(outFile,d_paramset);
    outFile << "n Parameterset" << "\t";
    save_st_ParamsetWithNames(outFile,n_paramset);
    outFile << "s Parameterset" << "\t";
    save_st_ParamsetWithNames(outFile,s_paramset);
    outFile << "delta Parameterset" << "\t";
    save_st_ParamsetWithNames(outFile,delta_p_paramset);
    outFile << "Length = " << length;
}
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Class name:      rSolow                                                    //
// Member function: save_st_ParamsetWithNames                                 //
// Purpose:         add  parameterset to printfile                            //
// Last modified:                                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
void rSolow::save_st_ParamsetWithNames(QTextStream& outFile, st_paramset *temp_paramset)
{
   int	i;			// Index
  	
    	outFile << "type = " << temp_paramset->type << "\t";
	
    switch  (temp_paramset->type) {
      case -1:
 	 outFile << "z_0 = " << temp_paramset->z_0 << "\t";  
      break;
      case 0 :	
    	 outFile << "z_0 = " << temp_paramset->z_0 << "\t";	 
      break;
	
      case 1 :		
	outFile <<  "theta_type = " << temp_paramset->theta_type << "\t";
	outFile <<  "my = " << temp_paramset->my << "\t";
    	outFile <<  "z_0 = " << temp_paramset->z_0 << "\t"; 
	
        i=0;
        while(i<Trans_x_Max) {
	      outFile << "trans_x = " << temp_paramset->trans_x[i] << "\t";
	      //printf("x[%i]=%f\n",i,trans_x[i]); 
	      if(temp_paramset->trans_x[i]==1) break;
	      i++;
	}
       outFile << "\t";
       for (int j=0;j<i;j++) {
	 outFile << "trans_a = " << temp_paramset->trans_a[j] << "\t";
	}
	outFile << "\t";
       for (int j=0;j<i;j++) {
	 outFile << "trans_b = " << temp_paramset->trans_b[j] << "\t";
	}
        outFile << "\t";
        break;
   
      case 2:
        outFile << "zvar_expr = " << temp_paramset->zvar_expr << "\t";
	  if( temp_paramset->mc_flag == 1) {
	     outFile << "mc_matrix = " << temp_paramset->mc_matrix << "\t";
          }
       break;
	
    }
}
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rSolow
// Member function:	printParamset
// Purpose:		print paramset on the screen
//
// Author:		Michael Meyer & Stefan Lueke
// Last modified:	Mon May 11 13:07:57 METDST 1998
// By:			
//
///////////////////////////////////////////////////////////////////////////////
void rSolow::print_st_Paramset(st_paramset *temp_paramset){

    int	i;			// Index
      	
      log() << temp_paramset->type;
      
      switch  (temp_paramset->type) {
      case -1:
    log() << temp_paramset->z_0;
      break;
      case 0 :	
        log() << temp_paramset->z_0;
      break;
	
      case 1 :		
    log() << temp_paramset->theta_type;
    log() << temp_paramset->my;
        log() << temp_paramset->z_0;
	
        i=0;
        while(i<Trans_x_Max) {
          log() << temp_paramset->trans_x[i];
	      //printf("x[%i]=%f\n",i,trans_x[i]); 
	      if(temp_paramset->trans_x[i]==1) break;
	      i++;
	}
       for (int j=0;j<i;j++) {
     log() << temp_paramset->trans_a[j];
	}
       for (int j=0;j<i;j++) {
     log() << temp_paramset->trans_b[j];
	}
        break;
   
      case 2:
        log() << temp_paramset->zvar_expr;
	  if( temp_paramset->mc_flag == 1) {
         log() << temp_paramset->mc_matrix;
          }
       break;
	
    }

 }

  ///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rSolow
// Member function:	saveParamset
// Purpose:		save a parameterset from a specified input file
//
// Author:		Michael Meyer & Stefan Lueke
// Last modified:	Mon May 11 13:07:57 METDST 1998
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void rSolow::printParamset()

{     
    log() << k_0 ;
    log() << pf_type;

    print_st_Paramset(a_paramset);
 
    print_st_Paramset(b_paramset);
 
    print_st_Paramset(c_paramset);

    print_st_Paramset(d_paramset);
    
    print_st_Paramset(n_paramset);
    
    print_st_Paramset(s_paramset);
    
    print_st_Paramset(delta_p_paramset);
 
    log() << length;
    
    baseModel::printParamset();
} 
//*******************************************************************************   
//*******************************************************************************
//*******************************************************************************
//***                                                                         ***
//***									      ***
//***			R B C _ D e l t a _ 1 				      ***
//***  									      ***
//***									      ***
//*******************************************************************************
//*******************************************************************************
//*******************************************************************************
        	        
///////////////////////////////////////////////////////////////////////////////
//
// 
//
// class name:		RBC_delta_1.C
//
// Author:		Michael Meyer 
// Last modified:	04/08/99
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rSolow
// Member function:	rSolow
// Purpose:		constructor
//
// Author:		Michael Meyer & Stefan Lüke
// Last modified:	Mon Mar 10 14:09:17 MET 1999
// By:			Michael Meyer 
//
///////////////////////////////////////////////////////////////////////////////

RBC_delta_1::RBC_delta_1() : rSolow()
{
	beta_paramset = new st_paramset;
	theta_RBC_paramset = new st_paramset;
	rho_paramset = new st_paramset;
	epsilon_paramset = new st_paramset;
	alpha_paramset = new st_paramset;

	beta_paramset->mc = NULL;
	beta_paramset->zvar = NULL;

        theta_RBC_paramset->mc = NULL;
	theta_RBC_paramset->zvar = NULL;
	
        rho_paramset->mc = NULL;
	rho_paramset->zvar = NULL;

        epsilon_paramset->mc = NULL;
	epsilon_paramset->zvar = NULL; 
	
	alpha_paramset->mc = NULL;
	alpha_paramset->zvar = NULL;
}	
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		RBC_delta_1
// Member function:	RBC_delta_1
// Purpose:		destructor
//
// Author:		Michael Meyer 
// Last modified:	Tue April 08 14:09:17 MET 1999
// By:			Michael Meyer 
//
///////////////////////////////////////////////////////////////////////////////

RBC_delta_1::~RBC_delta_1(void)
{
if(beta_paramset) delete beta_paramset;
if(theta_RBC_paramset) delete theta_RBC_paramset;
if(rho_paramset) delete rho_paramset;
if(epsilon_paramset) delete epsilon_paramset;
if(alpha_paramset) delete alpha_paramset;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          RBC_delta_1
// Member function:     iteration
// Purpose:             perform one iteration of the system
//
// Author:              Michael Meyer 
// Last modified:       Wed April 08 14:13:52 MET 1999
// By:                  Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void RBC_delta_1::iteration(const qint64&)
{ 
  qreal k_RBC_n;
  qreal z_RBC_n;
  qreal c_RBC_n;
  if (beta_paramset->type > -1) {
    rSolow::noise_iteration(beta_paramset);
    beta = beta_paramset->z_trans;}

  if (theta_RBC_paramset->type > -1) {
    rSolow::noise_iteration(theta_RBC_paramset);
    theta_RBC = theta_RBC_paramset->z_trans;}

  if (rho_paramset->type > -1) {
    rSolow::noise_iteration(rho_paramset);
    rho = rho_paramset->z_trans;}

  if (epsilon_paramset->type > -1) {
    rSolow::noise_iteration(epsilon_paramset);
    epsilon = epsilon_paramset->z_trans;}
    
  if (alpha_paramset->type > -1) {
    rSolow::noise_iteration(alpha_paramset);
    alpha_RBC = alpha_paramset->z_trans;} 
  
  l_RBC = (theta_RBC*(1-alpha_RBC))/((1-theta_RBC)*(1-alpha_RBC*beta)+theta_RBC*(1-alpha));   
  c_RBC_n = (1-alpha_RBC*beta)*exp((1-alpha_RBC)*log(l_RBC))*z_RBC*exp(alpha_RBC * log(k_RBC));
  c_RBC = c_RBC_n; 
  k_RBC_n = alpha_RBC*beta*exp((1-alpha_RBC)*log(l_RBC))*z_RBC*exp(alpha_RBC*log(k_RBC));
  k_RBC = k_RBC_n;
  z_RBC_n = exp(rho * log(z_RBC))*epsilon;
  z_RBC = z_RBC_n;
} 
        
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		RBC_delta_1
// Member function:	initialize 
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Michael Meyer 
// Last modified:	Mon April 08 15:43:17 MET 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void RBC_delta_1::initialize()
{
	k_RBC=k_RBC_0;
        z_RBC=z_RBC_0;
	c_RBC=c_RBC_0;
	
	rSolow::delete_st_paramset_stochptr(beta_paramset);
	rSolow::delete_st_paramset_stochptr(theta_RBC_paramset);
	rSolow::delete_st_paramset_stochptr(rho_paramset);
	rSolow::delete_st_paramset_stochptr(epsilon_paramset);
	rSolow::delete_st_paramset_stochptr(alpha_paramset);

	beta_paramset->z_0 = beta_0;
	theta_RBC_paramset->z_0 = theta_RBC_0;
	rho_paramset->z_0 = rho_0;
	epsilon_paramset->z_0 = epsilon_0;
	alpha_paramset->z_0 = alpha_RBC_0;
}
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		RBC_delta_1
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Michael Meyer
// Last modified:	99/04/08
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

qreal* RBC_delta_1::setLabels(const QString& label)
{
    if (label == "k_RBC") return(&k_RBC);
    if (label == "l_RBC") return(&l_RBC);
    if (label == "z_RBC") return(&z_RBC);
    if (label == "c_RBC") return(&c_RBC);
    if (label == "z_RBC_0") return(&z_RBC_0);
    if (label == "c_RBC_0") return(&c_RBC_0);
    if (label == "k_RBC_0") return(&k_RBC_0);
    if (label == "beta") return(&beta);
    if (label == "theta_RBC") return(&theta_RBC);
    if (label == "rho") return(&rho);
    if (label == "epsilon") return(&epsilon);
    if (label == "alpha_RBC") return(&alpha_RBC);

    return NULL;   
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		RBC_delta_1
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Michael Meyer 
// Last modified:	Mon April 08 13:07:57 METDST 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void RBC_delta_1::loadParamset(QTextStream& inFile){
    
    inFile >> k_RBC_0 ;
    
    inFile >> z_RBC_0 ;
		
    inFile >> c_RBC_0 ;		

    rSolow::read_sim(inFile,alpha_paramset);
    
    rSolow::read_sim(inFile,beta_paramset);
    
    rSolow::read_sim(inFile,theta_RBC_paramset);
    
    rSolow::read_sim(inFile,rho_paramset);
    
    rSolow::read_sim(inFile,epsilon_paramset);

    beta_0 = beta_paramset->z_0; 
    theta_RBC_0 = theta_RBC_paramset->z_0;
    rho_0 = rho_paramset->z_0; 
    epsilon_0 = epsilon_paramset->z_0; 
    alpha_RBC_0 = alpha_paramset->z_0;

    inFile >> length;  
      	    
    initialize();
       
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		RBC_delta_1
// Member function:	saveParamset
// Purpose:		save a parameterset from a specified input file
//
// Author:		Michael Meyer 
// Last modified:	Mon May 11 13:07:57 METDST 1998
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void RBC_delta_1::saveParamset(QTextStream& outFile)
{ 
    outFile << k_RBC_0 ;	
              
    outFile << z_RBC_0 ;
		
    outFile << c_RBC_0 ;		
    
    rSolow::save_st_Paramset(outFile,beta_paramset);
    
    rSolow::save_st_Paramset(outFile,theta_RBC_paramset);
    
    rSolow::save_st_Paramset(outFile,rho_paramset);
    
    rSolow::save_st_Paramset(outFile,epsilon_paramset);
    
    rSolow::save_st_Paramset(outFile,alpha_paramset);
    
    outFile << length;
             
    baseModel::saveParamset (outFile);
}
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Class name:      RBC_delta_1                                                   //
// Member function: saveParamsetWithNames                                     //
// Purpose:         add  parameterset to printfile                            //
// Last modified:                                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
void RBC_delta_1::saveParamsetWithNames(QTextStream& outFile)
{
outFile << "RBC_delta_1:\n";
outFile << "k_RBC_0 =" << k_RBC_0 << "\tz_RBC_0 =" << z_RBC_0 << "\tc_RBC_0 =" << c_RBC_0 <<"\n";
outFile << "Alpha Parameterset" << "\t";
rSolow::save_st_ParamsetWithNames(outFile,alpha_paramset);
outFile << "Beta Parameterset" << "\t";
rSolow::save_st_ParamsetWithNames(outFile,beta_paramset);  
outFile << "Theta Parameterset" << "\t";
rSolow::save_st_ParamsetWithNames(outFile,theta_RBC_paramset);
outFile << "Rho Parameterset" << "\t";
rSolow::save_st_ParamsetWithNames(outFile,rho_paramset);
outFile << "Epsilon Parameterset" << "\t";
rSolow::save_st_ParamsetWithNames(outFile,epsilon_paramset);

outFile << "Length = " << length;
}
/////////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		RBC_delta_1
// Member function:	print Paramset
// Purpose:		save a parameterset from a specified input file
//
// Author:		Michael Meyer & Stefan Lueke
// Last modified:	Mon May 11 13:07:57 METDST 1998
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void RBC_delta_1::printParamset()
{
   
    log() << k_RBC_0 ;
    
    log() << z_RBC_0 ;
		
    log() << c_RBC_0 ;

    rSolow::print_st_Paramset(beta_paramset);
    
    rSolow::print_st_Paramset(theta_RBC_paramset);
    
    rSolow::print_st_Paramset(rho_paramset);
    
    rSolow::print_st_Paramset(epsilon_paramset);
    
    rSolow::print_st_Paramset(alpha_paramset);
    
    log() << length;
            
    baseModel::printParamset();
}
