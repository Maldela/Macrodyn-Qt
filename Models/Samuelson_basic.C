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
#include "../strnchr.h"
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

void Samuelson_basic::iteration(const long&)
{
	noise = distri.rectangular()*2.0*lambda;
	v_0noise = distri.rectangular();
	if (v_0noise < v_0prob){
	v_0=v_0min;}
	else {v_0=v_0max;}  
	//cout << v_0noise << endl;
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
	v_0max=0.8;
	v_0min=0.31;
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

real* Samuelson_basic::sendModelVar()
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

real* Samuelson_basic::setLabels(char* label)
{
	if( !strcmp(label,"xBundle") )
		return( &xBundle );
	if( !strcmp(label,"yBundle") )
		return( &yBundle );
	if( !strcmp(label,"y1_0") )
		return( &y1_0);
	if( !strcmp(label,"y2_0") )
		return( &y2_0);
	if( !strcmp(label,"m_0") )
		return( &m_0);
	if( !strcmp(label,"v_0") )
		return( &v_0);
	if( !strcmp(label,"m") )
		return( &m);		
	if( !strcmp(label,"v") )
		return( &v );
	if( !strcmp(label,"lambda") )
		return( &lambda );
	if( !strcmp(label,"y") )
		return( &y );
	if( !strcmp(label,"y1") )
		return( &y1 );
	if( !strcmp(label,"y2") )
		return( &y2 );
	if( !strcmp(label,"noise") )
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

void Samuelson_basic::sendStateSpace(int &quantity,const real*** stateSpace)
{
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const real* [dimension];
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

void Samuelson_basic::loadParamset(ifstream& inFile)
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

void Samuelson_basic::saveParamset(ofstream& outFile)
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


void Samuelson_basic::saveParamsetWithNames(ofstream& outFile)
{
	outFile << "y1_0 = " << y1_0;
	outFile << "\ny2_0 = " << y2_0;
	outFile << "\nm_0 = " << m_0;
	outFile << "\nv_0 = " << v_0;
	outFile << "\nm = " << m;
	outFile << "\nv = " << v;
	outFile << "\nlambda = " << lambda;
	outFile << "\nseed = " << myseed;
	outFile << "\nlength = " << length << endl;
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
	cout << y1_0 << "\t" << y2_0 << "\t" << m_0 << "\t" << v_0 << "\t";
	cout << m << "\t" << v << "\t" <<  lambda << "\t" << myseed << "\t" << length << endl;
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

void Samuelson_basic::sendParameters(int& amount,real** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=8;
    *parameters=new real[amount];
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

void Samuelson_basic::receiveParameters(const real* parameters)
{
    y1_0=parameters[0];
    y2_0=parameters[1];
    m_0=parameters[2];
    v_0=parameters[3];
    m=parameters[4];
    v=parameters[5];
    lambda=parameters[6];
    length=long(parameters[7]);
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

void Samuelson_ar1::iteration(const long&)
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

void Samuelson_ar1::loadParamset(ifstream& inFile)
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

real* Samuelson_ar1::setLabels(char* label)
{
	real *plab = NULL;
	plab = Samuelson_basic::setLabels( label );
	if ( plab != NULL ) return plab;

	if( !strcmp(label,"a") )
		return( &a);		
	if( !strcmp(label,"xi") )
		return( &xi );
		
	return NULL;
}
