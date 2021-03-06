///////////////////////////////////////////////////////////////////////////////
//
// Module name:		cobweb.C
// Contents:		Member functions of the class cobweb
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

#include "../logger.h"

#include "cobweb.h"
#include <cmath>

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		cobweb
// Member function:	cobweb
// Purpose:		constructor
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

cobweb::cobweb() : baseModel(1)
{
	dummy = 0;
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		cobweb
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void cobweb::iteration(const qint64&)
{
	p_e = (1.0-w)*p_e_old + w*1.0/b*(a-atan(lambda*p_e_old));
	q = atan(lambda*p_e);
	p = (a-q)/b;
	e_t = p - p_e;
	p_e_old = p_e;
}
    
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		cobweb
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void cobweb::initialize()
{
    e_t = 0;
    p = p_e_0;
    p_e = p_e_0;
    p_e_old = p_e_0;
    q = 0;
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		cobweb
// Member function:	sendModelVar
// Purpose:		return a pointer to the main model variable
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

qreal* cobweb::sendModelVar()
{
    return &p;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		cobweb
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

qreal* cobweb::setLabels(const QString& label)
{
	if (label == "xBundle")
		return( &xBundle );
	if (label == "yBundle")
		return( &yBundle );
	if (label == "p")
		return( &p );
	if (label == "p_e")
		return( &p_e);
	if (label == "p_e_old")
		return( &p_e_old);
	if (label == "q")
		return( &q );
	if (label == "e_t")
		return( &e_t );
	if (label == "lambda")
		return( &lambda );
	if (label == "a")
		return( &a );
	if (label == "b")
		return( &b );
	if (label == "w")
		return( &w );
	if (label == "p_e_0")
		return( &p_e_0 );
	if (label == "dummy")
		return( &dummy );
	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		cobweb
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void cobweb::sendStateSpace(int &quantity,QList<qreal *> *stateSpace)
{
    stateSpace->clear();
    quantity = dimension;
    *stateSpace << &p;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		cobweb
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void cobweb::loadParamset(QTextStream& inFile)
{
	inFile >> lambda;
	inFile >> a >> b;
	inFile >> w >> p_e_0;

	inFile >> length;
	
	initialize();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		cobweb
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void cobweb::saveParamset(QTextStream&)
{
//	outFile << c0 << "\t" << c1 << "\t" << qreal_money << "\t";
//	outFile << G << "\t" << T << "\t" << alpha  << "\t" << y0;
//	outFile << "\t" << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		cobweb
// Member function:	saveParamsetWithNames
// Purpose:		write parameterset into a file
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////


void cobweb::saveParamsetWithNames(QTextStream&)
{
//	outFile << "c0 = " << c0;
//	outFile << "\nc1 = " << c1;
//    outFile << "\nqreal_money = " << qreal_money;
//	outFile << "\nG = " << G << "\tT = " << T;
//	outFile << "\nalpha = " << alpha << "\ty0 = " << y0;
//	outFile << "length = " << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		cobweb
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void cobweb::printParamset()
{
/*	log() << c0 << "\t" << c1 << "\t" << qreal_money << "\t";
    log() << G << "\t" << T << "\t" << alpha  << "\t" << y0;
    log() << "\t" << length */
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		cobweb
// Member function:	sendParameters
// Purpose:		write all parameters into an array and return the numbers
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void cobweb::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=2;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("cobweb::sendParameters",
		   "Can't create array for parameters");
    (*parameters[0])=p;
    (*parameters[1])=length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		cobweb
// Member function:	receiveParameters
// Purpose:		receive parameter values
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void cobweb::receiveParameters(const QList<qreal>& parameters)
{
    p=parameters[0];
    length=qint64(parameters[1]);
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rdemand_cobweb
// Member function:	rdemand_cobweb
// Purpose:		constructor
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

rdemand_cobweb::rdemand_cobweb()
{
	zvar = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rdemand_cobweb
// Member function:	iteration
// Purpose:		iterate the system ones
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void rdemand_cobweb::iteration( const qint64&)
{
	*zvar_ptr = zvar->dice();
	p_e = (1.0-w)*p_e_old + w*1.0/b*(a-atan(lambda*p_e_old));
	q = atan(lambda*p_e);
	p = (a-q)/b;
	e_t = p - p_e;
	p_e_old = p_e;
	*zvar_ptr = buffer_a;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rdemand_cobweb
// Member function:	loadParamset
// Purpose:		load parameterset from file
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void rdemand_cobweb::loadParamset( QTextStream& inFile )
{
	inFile >> zvar_name;
	inFile >> zvar_expr;
	cobweb::loadParamset( inFile );
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rdemand_cobweb
// Member function:	initialize
// Purpose:		initialize random parameters
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void rdemand_cobweb::initialize()
{
	zvar_ptr = setLabels( zvar_name );
	if ( zvar != NULL ) delete zvar;
	zvar = new rand_var( this, "ranf", zvar_expr);
	cobweb::initialize();
	buffer_a = *zvar_ptr;	
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		cobweb_RLS
// Member function:	loadParamset
// Purpose:		
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void cobweb_RLS::loadParamset( QTextStream& inFile )
{
	inFile >> p_0 >> X_0 >> theta_0;
	cobweb::loadParamset( inFile );
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		cobweb_RLS
// Member function:	initialize
// Purpose:		initialisation
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void cobweb_RLS::initialize()
{
	cobweb::initialize();
	p = p_0;
//	theta_0 = 1;
//	X_0 = 0.15;
	theta_t = theta_0;
	X_t = X_0;
	e_t = 0;	
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		cobweb_RLS
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void cobweb_RLS::iteration(const qint64&)
{
	p_e_old = p_e;
	p_e = theta_t * p;
	q = atan(lambda*p_e);
	p_t1 = (a-q)/b;
	
	lambda_t = 1.0 / (1+p*X_t*p);
	theta_t = theta_t + lambda_t*X_t*p*(p_t1-theta_t*p);
	X_t = X_t - lambda_t*X_t*p*p*X_t;
	
	e_t = p - p_e_old;
	p = p_t1;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		cobweb_RLS
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

qreal* cobweb_RLS::setLabels(const QString& label)
{
	if (label == "theta_t")
		return( &theta_t );
	if (label == "theta_0")
		return( &theta_0 );
	if (label == "X_t")
		return( &X_t );
	if (label == "X_0")
		return( &X_0);
	if (label == "p_0")
		return( &p_0 );
	if (label == "p_t1")
		return( &p_t1 );
	if (label == "lambda_t")
		return( &lambda_t );
	
	return cobweb::setLabels( label );
}
