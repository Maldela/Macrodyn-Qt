
#include "../error.h"
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
	//log() << "This is function 'iteration()' in affine1.C" 
    
 	a = zvar->dice();	// Set a new random value for the Parameter
    qreal oldX=x;
	x = a*(oldX + r);
	/*log() << "oldX = " << oldX 
	log() << "a  = " << a 
	log() << "x = " << x */
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
    else log() << "New zvar from zvar_expr: " << zvar_expr;
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
	//log() << "This is function 'sendModelVar()' in affine1.C" 
    return &x;
	//log() << "Now returning...."  
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
	//log() << "This is function 'setLabels()' in affine1.C ." 
    if (label == "a")
	return( &a);
    if (label == "x")
	return( &x);
    if (label == "x0")
	return( &x0 );
    if (label == "r")
	return( &r );
    return NULL;
	//log() << "Now returning...."  
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
	//log() << "This is function 'sendStateSpace()' in affine1.C ."  
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("affine1::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&x;
    log() << "Now returning....";
}

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

void affine1::loadParamset(QTextStream& inFile)
{
    log() << "This is function 'loadParamset()' in 'affine1.C' .";
 
 	inFile >> zvar_expr;  
	inFile >> x0;
	inFile >> r;
	inFile >> length;

	/*log() << "a = " << a 
	log() << "a1= " << a1 
	log() << "a2= " << a2 */
 	
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

void affine1::saveParamset(QTextStream& outFile)
{
	//log() << "This is function 'saveParamset()' in 'AFFINE1.C' ."  
   	outFile << x0  << "\t";
	outFile << r  << "\t";
	outFile << length;
	//log() << "Now returning...."  
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
    log() << "This is function 'printParamset()' in 'AFFINE.C' .";
    log() << "x0: " << x0;
    log() << "r: " << r;
    log() << "length: " << length;
    log() << "Now returning....";
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
 	//log() << "This is function 'sendParameters()' in 'affine1.C' ."  
    
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
	//log() << "Now returning...."  
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

void affine1::receiveParameters(const QList<qreal>& parameters)
{	
    if (parameters.size() != 3) log() << "Wrong number of parameters!";
    else
    {
        //log() << "This is function 'receiveParameters()' in 'affine1.C' ." 
        x0 = parameters[0];
        r =parameters[1];
        length = (qint64)(parameters[2]);
        //log() << "Now returning...." 
    }
}

