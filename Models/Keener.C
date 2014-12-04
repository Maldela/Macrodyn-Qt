///////////////////////////////////////////////////////////////////////////////
//
// Module name:		Keener.C
// Contents:		Member functions of the class Keener
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "../strnchr.h"
#include "Keener.h"


///////////////////////////////////////////////////////////////////////////////
//
// Class name:		Keener
// Member function:	Keener
// Purpose:		constructor
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

Keener::Keener() : baseModel(1)
{
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		Keener
// Member function:	Keener
// Purpose:		constructor
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

Keener::~Keener() 
{
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Keener
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void Keener::iteration(const long&)
{
	bias = distri.rectangular()*2.0*lambda;
	x = fmod(a*x+b+bias , Modulo);
/*	//Modulo
	x -= floor(x);
	while ( fabs(x)>=1.0 ) {
		x = x - sign(x);
	}
*/
}
    
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Keener
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void Keener::initialize()
{
	x = x_0;
	if ( myseed!=0 ){
		distri.setseed( myseed );
	}
}
 
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Keener
// Member function:	sendModelVar
// Purpose:		return a pointer to the main model variable
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

qreal* Keener::sendModelVar()
{
    return &x;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Keener
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

qreal* Keener::setLabels(char* label)
{
	if( !strcmp(label,"xBundle") )
		return( &xBundle );
	if( !strcmp(label,"yBundle") )
		return( &yBundle );
	if( !strcmp(label,"x_0") )
		return( &x_0);
	if( !strcmp(label,"a") )
		return( &a);		
	if( !strcmp(label,"b") )
		return( &b );
	if( !strcmp(label,"lambda") )
		return( &lambda );
	if( !strcmp(label,"Modulo") )
		return( &Modulo );	
	if( !strcmp(label,"x") )
		return( &x );
	if( !strcmp(label,"bias") )
		return( &bias );
		
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Keener
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void Keener::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("Keener::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&x;
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Keener
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void Keener::loadParamset(ifstream& inFile)
{
	inFile >> x_0 >> a >> b >> Modulo >> lambda >> myseed;
	inFile >> length;
   
	initialize();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Keener
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void Keener::saveParamset(ofstream& outFile)
{
	outFile << x_0 << "\t" << a << "\t" << b << "\t";
	outFile << Modulo << "\t" << lambda << "\t" << myseed;
	outFile << "\t" << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Keener
// Member function:	saveParamsetWithNames
// Purpose:		write parameterset into a file
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////


void Keener::saveParamsetWithNames(ofstream& outFile)
{
	outFile << "x_0 = " << x_0;
	outFile << "\na = " << a;
	outFile << "\nb = " << b;
	outFile << "\nModulo = " << Modulo;
	outFile << "\nlambda = " << lambda;
	outFile << "\nseed = " << myseed;
	outFile << "\nlength = " << length << "\n";
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Keener
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void Keener::printParamset()
{
	log() << x_0 << "\t" << a << "\t" << b << "\t" << Modulo << "\t";
	log() << lambda << "\t" << myseed << "\t" << length << "\n";
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Keener
// Member function:	sendParameters
// Purpose:		write all parameters into an array and return the numbers
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void Keener::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=6;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("Keener::sendParameters",
		   "Can't create array for parameters");
    (*parameters[0])=x_0;
    (*parameters[1])=a;
    (*parameters[2])=b;
    (*parameters[3])=Modulo;
    (*parameters[4])=lambda;
    (*parameters[5])=length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Keener
// Member function:	receiveParameters
// Purpose:		receive parameter values
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void Keener::receiveParameters(const qreal* parameters)
{
    x_0=parameters[0];
    a=parameters[1];
    b=parameters[2];
    Modulo=parameters[3];
    lambda=parameters[4];
    length=long(parameters[5]);
}

