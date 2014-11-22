
#include "../error.h"
#include "../strnchr.h"
#include "affine2.h"


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine2
// Member function:	affine2
// Purpose:		constructor
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////
affine2::affine2() : baseModel(1)
{
zvar=NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine2
// Member function:	affine2
// Purpose:		destructor
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

affine2::~affine2()
{
if(zvar) delete zvar;
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine2
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void affine2::iteration(const long&)
{
	//Log::log() << "This is function 'iteration()' in affine2.C" << endl;
    
 	urv = zvar->dice();	// Set a new random value for the Parameter
    qreal oldX=x;
	
	if(urv < p) 
	{
	//Log::log() << "map1 is chosen" << endl;
	count1 = count1 + 1;
	x = a1*oldX;
	}
	else
	{
	//Log::log() << "map2 is chosen" << endl;
	x = a2*oldX + 1;
	count2 = count2 + 1;
	}

	/*Log::log() << "oldX = " << oldX << endl;
	Log::log() << "urv  = " << urv << endl;
	Log::log() << "x = " << x << endl;
	Log::log() << "count1 = " << count1 << endl;
	Log::log() << "count2 = " << count2 << endl;*/

}
    
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine2
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void affine2::initialize()
{
	//Log::log() << "This is function 'initialize()' in AFFINE2.C ."<< endl; 
   	//Log::log() << incase1 << endl;;
	x=x0;
	count1=0;
	count2=0;
    zvar = new rand_var(this,"ranf",zvar_expr);
	if(!zvar)	
	fatalError("randvar::initialize stoch_ar", "can't create rand_var");
	//Log::log() << "Now returning...." << endl; 
	if(incase1==1) 
	{
	//Log::log() << "case1 is valid:" << endl;
	a1 = a;
	a2 = a-1;
	/*Log::log() << "a = " << a << endl;
	Log::log() << "a1= a = " << a1 << endl;
	Log::log() << "a2= a-1 = " << a2 << endl;*/
	}
else
	{
	/*Log::log() << "case2 or case 3 is valid:" << endl;
	Log::log() << "a = " << a << endl;
	Log::log() << "a1= a = " << a1 << endl;
	Log::log() << "a2= a-1 = " << a2 << endl;*/
	}


}
 
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine2
// Member function:	sendModelVar
// Purpose:		return a pointer to the main model variable
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

qreal* affine2::sendModelVar()
{
	//Log::log() << "This is function 'sendModelVar()' in AFFINE2.C" << endl;
    return &x;
	//Log::log() << "Now returning...." << endl; 
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine2
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Marten Hillebrand
// Last modified:
// By:		
//
///////////////////////////////////////////////////////////////////////////////

qreal* affine2::setLabels(char* label)
{
	//Log::log() << "This is function 'setLabels()' in AFFINE2.C ." << endl;
	if( !strcmp(label,"a") )
	return( &a);
    if( !strcmp(label,"a1") )
	return( &a1);
    if( !strcmp(label,"a2") )
	return( &a2);
    if( !strcmp(label,"x") )
	return( &x);
    if( !strcmp(label,"x0") )
	return( &x0 );
	if( !strcmp(label,"p") )
	return( &p );
	if( !strcmp(label,"urv") )
	return( &urv );
	if( !strcmp(label,"xBundle") )
	return( &xBundle );
	if( !strcmp(label,"yBundle") )
	return( &yBundle );
    return NULL;
	//Log::log() << "Now returning...." << endl; 
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine2
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		
// Last modified: unchanged!
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void affine2::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
	//Log::log() << "This is function 'sendStateSpace()' in AFFINE2.C ." << endl; 
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("affine2::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&x;
 	Log::log() << "Now returning...." << endl; 
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine2
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void affine2::loadParamset(ifstream& inFile)
{
//Log::log() << "This is function 'loadParamset()' in 'AFFINE2.C' ." << endl; 
 
 	inFile >> zvar_expr;  
	//Log::log() << zvar_expr << endl;
	inFile >> incase1;
	//Log::log() << incase1 << endl;
	inFile >> a;
	//Log::log() << a << endl;
	if(a<0 || a > 1) fatalError("affine2::loadParamset","Parameter a out of range");
	inFile >> a1;
	//Log::log() << a1 << endl;
	if((a1<=0) || (a1>=1)) fatalError("affine2::loadParamset","Parameter a1 out of range");
	inFile >> a2;
    if(a2<=-1 || a2 >= 0) fatalError("affine2::loadParamset","Parameter a2 out of range");
	
	inFile >> x0;
	inFile >> p;
	inFile >> length;

	/*Log::log() << "a = " << a << endl;
	Log::log() << "a1= " << a1 << endl;
	Log::log() << "a2= " << a2 << endl;*/
 	
    	initialize();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine2
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void affine2::saveParamset(ofstream& outFile)
{
	//Log::log() << "This is function 'saveParamset()' in 'AFFINE1.C' ." << endl; 
   	outFile << a  << "\t";
	outFile << a1  << "\t";
	outFile << a2  << "\t";
	outFile << x0  << "\t";
	outFile << p  << "\t";
    outFile << length;
	//Log::log() << "Now returning...." << endl; 
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine2
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		Marten Hillebrand
// Last modified:	
// By:		
//
///////////////////////////////////////////////////////////////////////////////

void affine2::printParamset()
{
    //Log::log() << "This is function 'printParamset()' in 'AFFINE.C' ." << endl; 
    Log::log() << a << endl;
	Log::log() << a1 << endl;
	Log::log() << a2 << endl;
	Log::log() << x0 << endl;
    Log::log() << p << endl;
    Log::log() << length << endl;
	//Log::log() << "Now returning...." << endl; 
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine2
// Member function:	sendParameters
// Purpose:		write all parameters into an array and return the numbers
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void affine2::sendParameters(int& amount,qreal** parameters)
{
 	//Log::log() << "This is function 'sendParameters()' in 'AFFINE2.C' ." << endl; 
    
    if( *parameters )
	delete *parameters;
    amount=6;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("affine2::sendParameters",
		   "Can't create array for parameters");
    (*parameters[0])=a;
    (*parameters[1])=a1;
	(*parameters[2])=a2;
	(*parameters[3])=x0;
	(*parameters[4])=p;
    (*parameters[5])=length;
	//Log::log() << "Now returning...." << endl; 
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine2
// Member function:	receiveParameters
// Purpose:		receive parameter values
//
// Author:		Markus Lohmann
// Last modified:	Mon Jan 13 15:48:01 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void affine2::receiveParameters(const qreal* parameters)
{	
	//Log::log() << "This is function 'receiveParameters()' in 'AFFINE2.C' ." << endl; 
	a=parameters[0];
	a1=parameters[1];
	a2=parameters[2];
	x0=parameters[3];
	p=parameters[4];
    length=(long)(parameters[5]);
	//Log::log() << "Now returning...." << endl; 
}

