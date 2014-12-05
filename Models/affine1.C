
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

void affine1::iteration(const qint64&)
{
	//log() << "This is function 'iteration()' in affine1.C" << "\n";
    
 	a = zvar->dice();	// Set a new random value for the Parameter
    qreal oldX=x;
	x = a*(oldX + r);
	/*log() << "oldX = " << oldX << "\n";
	log() << "a  = " << a << "\n";
	log() << "x = " << x << "\n";*/
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
	//log() << "This is function 'sendModelVar()' in affine1.C" << "\n";
    return &x;
	//log() << "Now returning...." << "\n"; 
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

qreal* affine1::setLabels(const QString& label)
{
	//log() << "This is function 'setLabels()' in affine1.C ." << "\n";
    if (label == "a")
	return( &a);
    if (label == "x")
	return( &x);
    if (label == "x0")
	return( &x0 );
    if (label == "r")
	return( &r );
    return NULL;
	//log() << "Now returning...." << "\n"; 
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
	//log() << "This is function 'sendStateSpace()' in affine1.C ." << "\n"; 
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("affine1::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&x;
 	log() << "Now returning...." << "\n"; 
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

void affine1::loadParamset(QDataStream& inFile)
{
//log() << "This is function 'loadParamset()' in 'affine1.C' ." << "\n"; 
 
 	inFile >> zvar_expr;  
	inFile >> x0;
	inFile >> r;
	inFile >> length;

	/*log() << "a = " << a << "\n";
	log() << "a1= " << a1 << "\n";
	log() << "a2= " << a2 << "\n";*/
 	
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

void affine1::saveParamset(QDataStream& outFile)
{
	//log() << "This is function 'saveParamset()' in 'AFFINE1.C' ." << "\n"; 
   	outFile << x0  << "\t";
	outFile << r  << "\t";
	outFile << length;
	//log() << "Now returning...." << "\n"; 
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
    //log() << "This is function 'printParamset()' in 'AFFINE.C' ." << "\n"; 
    log() << x0 << "\n";
	log() << r << "\n";
	log() << length << "\n";
	//log() << "Now returning...." << "\n"; 
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
 	//log() << "This is function 'sendParameters()' in 'affine1.C' ." << "\n"; 
    
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
	//log() << "Now returning...." << "\n"; 
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
	//log() << "This is function 'receiveParameters()' in 'affine1.C' ." << "\n"; 
	x0=parameters[0];
	r =parameters[1];
	length=(qint64)(parameters[5]);
	//log() << "Now returning...." << "\n"; 
}

