///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/det_var.C,v 1.1 2000/08/31 15:25:39 mmeyer Exp $
//
// Module name:		det_var.C
// Contents:		Class definition of the class rSolow
//			
//
// Author:		Michael Meyer & Stefan Lüke
// Last modified:	Mon May 11 12:40:17 METDST 1998
// By:			
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "det_var.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          det_var
// Member function:     det_var
// Purpose:             constructor
//
// Author:              Michael Meyer & Stefan Lüke
// Last modified:       Fri Jun 12 14:16:36 METDST 1998
// By:                  
//
///////////////////////////////////////////////////////////////////////////////
det_var::det_var() : rSolow()
{
    trans_x= new real[trans_x_max];
    if( !trans_x )
	fatalError("det_var::det_var","Can't create trans_x vector");

    trans_a= new real[trans_x_max-1];
    if( !trans_a )
	fatalError("det_var::det_var","Can't create trans_a vector");

    trans_b= new real[trans_x_max-1];
    if( !trans_b )
	fatalError("det_var::det_var","Can't create trans_b vector");
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		det_var
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		Uli Middelberg
// Last modified:	Mon Mar 10 15:43:28 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////
void det_var::sendStateSpace(int &quantity,const real*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const real* [dimension];
    if( !(*stateSpace) )
	fatalError("det_var::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&k;
    (*stateSpace)[1]=&delta_p;
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		det_var
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Uli Middelberg
// Last modified:	Mon Mar 10 15:43:24 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////
real* det_var::setLabels(char* label)
{
    if( !strcmp(label,"_z_0") ) return(&_z_0);
    if( !strcmp(label,"_z") ) return(&_z);
//    if( !strcmp(label,"theta_type") ) return(&theta_type);
    if( !strcmp(label,"my") ) return(&my);
//    if( !strcmp(label,"trans_a") ) return(&trans_a);
//    if( !strcmp(label,"trans_b") ) return(&trans_b);

    return rSolow::setLabels(label);
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		det_var
// Member function:	trans
// Purpose:		transforms z into the interval [a,b]
//
// Author:		Uli Middelberg
// Last modified:	Fri Jun 12 17:15:14 METDST 1998
// By:			
//
///////////////////////////////////////////////////////////////////////////////
real  det_var::trans(real z, real* x, real* a, real* b)
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
    error("macrodyn::det_var::trans: can not compute z.  (i=%i) \n",i);
    return(-1);
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		det_var
// Member function:	theta_xxx
// Purpose:		specific types of thetas
//
// Author:		Uli Middelberg
// Last modified:	Fri Jun 12 14:59:42 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
static real theta_logistic ( real d, real m)
{ return (m * d * (1-d)) ;
}

static real theta_tent ( real d, real m)
{ 
  
  if( d<=m ) {
    return (1/m * d);
  } else {
    return (1/(1-m)*(1-d));
  }
}

static real theta_saw ( real d, real m)
{ if( d<m ) {
    return (1/m * d);
  } else {
    return (1/(1-m)*(d-m));
  }
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		det_var
// Member function:	theta_init
// Purpose:		sets theta to a specific type
//
// Author:		Uli Middelberg
// Last modified:	Fri Aug 22 14:08:03 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////
void det_var::theta_init ( void )
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
      error("macrodyn::det_var::theta_init: theta type %d not yet implemented : ", int(theta_type));
  }
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          det_var
// Member function:     initialize
// Purpose:             initialize the model, define the systems initial state
//
// Author:              Uli Middelberg
// Last modified:       
// By:                  
//
///////////////////////////////////////////////////////////////////////////////
void det_var::initialize()
{
    _z      = _z_0;			// initialize the underlying process
    _z_var_ptr = setLabels(label);// get a pointer to the parameter
    if (_z_var_ptr == NULL) 
       fatalError("det_var::initialize","Var Name not known");
    * _z_var_ptr = trans(_z,trans_x,trans_a,trans_b);	// initialize _z_var_ptr
    theta_init ();
    
    rSolow::initialize();		// initialize the default model as usual
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          det_var
// Member function:     iteration
// Purpose:             perform one iteration of the system
//
// Author:              Uli Middelberg
// Last modified:       Wed Jun 10 13:05:42 METDST 1998
// By:                  Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
void det_var::iteration(const long& t)
{
  _z = (* theta ) (_z, my);			   // iterate _z
  * _z_var_ptr = trans(_z,trans_x,trans_a,trans_b);// transform _z to _z_var_ptr
  rSolow::iteration(t);			   // iterate the rest
};

