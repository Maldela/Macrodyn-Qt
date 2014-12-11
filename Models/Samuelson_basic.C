///////////////////////////////////////////////////////////////////////////////
//
// Module name:		Samuelson_basic.C
// Contents:		Member functions of the class Samuelson_basic
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "Samuelson_basic.h"


///////////////////////////////////////////////////////////////////////////////
//
// Class name:		Samuelson_basic
// Member function:	Samuelson_basic
// Purpose:		constructor
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

Samuelson_basic::Samuelson_basic() : baseModel(1)
{
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		Samuelson_basic
// Member function:	Samuelson_basic
// Purpose:		constructor
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

Samuelson_basic::~Samuelson_basic()
{
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Samuelson_basic
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void Samuelson_basic::iteration(const qint64&)
{
	noise = distri.rectangular()*2.0*lambda;
	v_0noise = distri.rectangular();
	if (v_0noise < v_0prob){
	v_0=v_0qMin;}
	else {v_0=v_0qMax;}  
    //log() << v_0noise 
	y2 = y1;
	y1 = y;
	y = m_0+v_0+(m+v)*y1-v*y2+noise;
}
    
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Samuelson_basic
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void Samuelson_basic::initialize()
{
	y = y1_0;
	y1 = y2_0;
	if ( myseed!=0 ){
		distri.setseed( myseed );
	}
	v_0prob=0.5;
	v_0qMax=0.8;
	v_0qMin=0.31;
}
 
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Samuelson_basic
// Member function:	sendModelVar
// Purpose:		return a pointer to the main model variable
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

qreal* Samuelson_basic::sendModelVar()
{
    return &y;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Samuelson_basic
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

qreal* Samuelson_basic::setLabels(const QString& label)
{
	if (label == "xBundle")
		return( &xBundle );
	if (label == "yBundle")
		return( &yBundle );
	if (label == "y1_0")
		return( &y1_0);
	if (label == "y2_0")
		return( &y2_0);
	if (label == "m_0")
		return( &m_0);
	if (label == "v_0")
		return( &v_0);
	if (label == "m")
		return( &m);		
	if (label == "v")
		return( &v );
	if (label == "lambda")
		return( &lambda );
	if (label == "y")
		return( &y );
	if (label == "y1")
		return( &y1 );
	if (label == "y2")
		return( &y2 );
	if (label == "noise")
		return( &noise );
		
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Samuelson_basic
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void Samuelson_basic::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("Samuelson_basic::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&y;
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Samuelson_basic
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void Samuelson_basic::loadParamset(QTextStream& inFile)
{
	inFile >> y1_0 >> y2_0 >> m_0 >> v_0 >> m >> v >> lambda >> myseed;
	inFile >> length;
   
	initialize();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Samuelson_basic
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void Samuelson_basic::saveParamset(QTextStream& outFile)
{
	outFile << y1_0 << "\t" << y2_0 << "\t" << m_0 << "\t";
	outFile << v_0 << "\t" << m << "\t" << v << "\t" << lambda << "\t" << myseed;
	outFile << "\t" << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Samuelson_basic
// Member function:	saveParamsetWithNames
// Purpose:		write parameterset into a file
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////


void Samuelson_basic::saveParamsetWithNames(QTextStream& outFile)
{
	outFile << "y1_0 = " << y1_0;
	outFile << "\ny2_0 = " << y2_0;
	outFile << "\nm_0 = " << m_0;
	outFile << "\nv_0 = " << v_0;
	outFile << "\nm = " << m;
	outFile << "\nv = " << v;
	outFile << "\nlambda = " << lambda;
	outFile << "\nseed = " << myseed;
    outFile << "\nlength = " << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Samuelson_basic
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void Samuelson_basic::printParamset()
{
    log() << y1_0 << "\t" << y2_0 << "\t" << m_0 << "\t" << v_0 << "\t";
    log() << m << "\t" << v << "\t" <<  lambda << "\t" << myseed << "\t" << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Samuelson_basic
// Member function:	sendParameters
// Purpose:		write all parameters into an array and return the numbers
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void Samuelson_basic::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=8;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("Samuelson_basic::sendParameters",
		   "Can't create array for parameters");
    (*parameters[0])=y1_0;
    (*parameters[1])=y2_0;
    (*parameters[2])=m_0;
    (*parameters[3])=v_0;
    (*parameters[4])=m;
    (*parameters[5])=v;
    (*parameters[6])=lambda;
    (*parameters[7])=length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Samuelson_basic
// Member function:	receiveParameters
// Purpose:		receive parameter values
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void Samuelson_basic::receiveParameters(const QList<qreal>& parameters)
{
    y1_0=parameters[0];
    y2_0=parameters[1];
    m_0=parameters[2];
    v_0=parameters[3];
    m=parameters[4];
    v=parameters[5];
    lambda=parameters[6];
    length=qint64(parameters[7]);
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Samuelson_ar1
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void Samuelson_ar1::iteration(const qint64&)
{
	noise = distri.rectangular()*2.0*lambda*(1-a);
	xi = a*xi+noise;
	y2 = y1;
	y1 = y;
	y = m_0+v_0+xi+(m+v)*y1-v*y2;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Samuelson_ar1
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void Samuelson_ar1::loadParamset(QTextStream& inFile)
{
	inFile >> a;
	Samuelson_basic::loadParamset( inFile );
   
	initialize();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Samuelson_ar1
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void Samuelson_ar1::initialize()
{
	xi = 0;
	Samuelson_basic::initialize();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Samuelson_ar1
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

qreal* Samuelson_ar1::setLabels(const QString& label)
{
    qreal *plab = NULL;
	plab = Samuelson_basic::setLabels( label );
	if ( plab != NULL ) return plab;

	if (label == "a")
		return( &a);		
	if (label == "xi")
		return( &xi );
		
	return NULL;
}
