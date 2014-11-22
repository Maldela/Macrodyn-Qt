
#include "../error.h"
#include "../strnchr.h"
#include "affine1.h"


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine1
// Member function:	affine1
// Purpose:		constructor
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////
affine1::affine1() : baseModel(1)
{
zvar=NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine1
// Member function:	affine1
// Purpose:		destructor
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

affine1::~affine1()
{
if(zvar) delete zvar;
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine1
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void affine1::iteration(const long&)
{
	//Log::log() << "This is function 'iteration()' in affine1.C" << endl;
    
 	a = zvar->dice();	// Set a new random value for the Parameter
    qreal oldX=x;
	x = a*(oldX + r);
	/*Log::log() << "oldX = " << oldX << endl;
	Log::log() << "a  = " << a << endl;
	Log::log() << "x = " << x << endl;*/
}
    
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine1
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void affine1::initialize()
{
	x=x0;
	zvar = new rand_var(this,"ranf",zvar_expr);
	if(!zvar)	
	fatalError("randvar::initialize stoch_ar", "can't create rand_var");
	
}
 
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine1
// Member function:	sendModelVar
// Purpose:		return a pointer to the main model variable
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

qreal* affine1::sendModelVar()
{
	//Log::log() << "This is function 'sendModelVar()' in affine1.C" << endl;
    return &x;
	//Log::log() << "Now returning...." << endl; 
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine1
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Marten Hillebrand
// Last modified:
// By:		
//
///////////////////////////////////////////////////////////////////////////////

qreal* affine1::setLabels(char* label)
{
	//Log::log() << "This is function 'setLabels()' in affine1.C ." << endl;
	if( !strcmp(label,"a") )
	return( &a);
    if( !strcmp(label,"x") )
	return( &x);
    if( !strcmp(label,"x0") )
	return( &x0 );
	if( !strcmp(label,"r") )
	return( &r );
    return NULL;
	//Log::log() << "Now returning...." << endl; 
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine1
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		
// Last modified: unchanged!
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void affine1::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
	//Log::log() << "This is function 'sendStateSpace()' in affine1.C ." << endl; 
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("affine1::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&x;
 	Log::log() << "Now returning...." << endl; 
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine1
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void affine1::loadParamset(ifstream& inFile)
{
//Log::log() << "This is function 'loadParamset()' in 'affine1.C' ." << endl; 
 
 	inFile >> zvar_expr;  
	inFile >> x0;
	inFile >> r;
	inFile >> length;

	/*Log::log() << "a = " << a << endl;
	Log::log() << "a1= " << a1 << endl;
	Log::log() << "a2= " << a2 << endl;*/
 	
    	initialize();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine1
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void affine1::saveParamset(ofstream& outFile)
{
	//Log::log() << "This is function 'saveParamset()' in 'AFFINE1.C' ." << endl; 
   	outFile << x0  << "\t";
	outFile << r  << "\t";
	outFile << length;
	//Log::log() << "Now returning...." << endl; 
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine1
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		Marten Hillebrand
// Last modified:	
// By:		
//
///////////////////////////////////////////////////////////////////////////////

void affine1::printParamset()
{
    //Log::log() << "This is function 'printParamset()' in 'AFFINE.C' ." << endl; 
    Log::log() << x0 << endl;
	Log::log() << r << endl;
	Log::log() << length << endl;
	//Log::log() << "Now returning...." << endl; 
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine1
// Member function:	sendParameters
// Purpose:		write all parameters into an array and return the numbers
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void affine1::sendParameters(int& amount,qreal** parameters)
{
 	//Log::log() << "This is function 'sendParameters()' in 'affine1.C' ." << endl; 
    
    if( *parameters )
	delete *parameters;
    amount=3;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("affine1::sendParameters",
		   "Can't create array for parameters");
    (*parameters[0])=x0;
    (*parameters[1])=r;
	(*parameters[2])=length;
	//Log::log() << "Now returning...." << endl; 
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine1
// Member function:	receiveParameters
// Purpose:		receive parameter values
//
// Author:		Markus Lohmann
// Last modified:	Mon Jan 13 15:48:01 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void affine1::receiveParameters(const qreal* parameters)
{	
	//Log::log() << "This is function 'receiveParameters()' in 'affine1.C' ." << endl; 
	x0=parameters[0];
	r =parameters[1];
	length=(long)(parameters[5]);
	//Log::log() << "Now returning...." << endl; 
}

