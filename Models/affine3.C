
#include "../error.h"
#include "../strnchr.h"
#include "affine3.h"


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine3
// Member function:	affine3
// Purpose:		constructor
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////
affine3::affine3() : baseModel(1)
{
	zvar1=NULL;
	zvar2=NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine3
// Member function:	affine3
// Purpose:		destructor
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

affine3::~affine3()
{
	if ( zvar1 ) delete zvar1;
	if ( zvar2 ) delete zvar2;
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine3
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void affine3::iteration(const long&)
{   
	urv_p = zvar1->dice();	// pick iteration function according to p
	urv_e = zvar2->dice();	// get distortion value in t
	real oldX = x;
	
	if(urv_p < p){
		//cout << "map1 is chosen" << endl;
		count1++;
		x = (a+urv_e)*oldX;
	} else {
		//cout << "map2 is chosen" << endl;
		count2++;
		x = (a-1+urv_e)*oldX + 1;
	}

	/*cout << "oldX = " << oldX << endl;
	cout << "urv_p = " << urv_p << endl;
	cout << "urv_e = " << urv_e << endl;
	cout << "x = " << x << endl;
	cout << "count1 = " << count1 << endl;
	cout << "count2 = " << count2 << endl;*/
}
    
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine3
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void affine3::initialize()
{
	x = x0;
	count1 = 0;
	count2 = 0;
	zvar1 = new rand_var(this,"ranf",zvar_expr1);
	zvar2 = new rand_var("ranf",1.0,-eta,eta);
	if (!zvar1)
		fatalError("randvar::initialize stoch_ar", "can't create rand_var1");
	if (!zvar2)
		fatalError("randvar::initialize stoch_ar"," can't create rand_var2");
}
 
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine3
// Member function:	sendModelVar
// Purpose:		return a pointer to the main model variable
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

real* affine3::sendModelVar()
{
    return &x;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine3
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Andreas Starke
// Last modified:
// By:		
//
///////////////////////////////////////////////////////////////////////////////

real* affine3::setLabels(char* label)
{
	if( !strcmp(label,"a") )
		return( &a);
	if( !strcmp(label,"x") )
		return( &x);
	if( !strcmp(label,"x0") )
		return( &x0 );
	if( !strcmp(label,"p") )
		return( &p );
	if( !strcmp(label,"urv_p") )
		return( &urv_p );
	if( !strcmp(label,"urv_e") )
		return( &urv_e );
	if( !strcmp(label,"eta") )
		return( &eta );
	if( !strcmp(label,"xBundle") )
		return( &xBundle );
	if( !strcmp(label,"yBundle") )
		return( &yBundle );
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine3
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		Andreas Starke
// Last modified: 
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void affine3::sendStateSpace(int &quantity,const real*** stateSpace)
{
	if( stateSpace )
		delete stateSpace;
	*stateSpace = new const real* [dimension];
	if( !(*stateSpace) )
		fatalError("affine3::sendStateSpace",
			   "Can't create state space vector");
	quantity=dimension;
	(*stateSpace)[0]=&x;
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine3
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void affine3::loadParamset(ifstream& inFile)
{
 	inFile >> zvar_expr1;
	//cout << zvar_expr1 << endl;
	//inFile >> zvar_expr2;
	//cout << zvar_expr2 << endl;
	inFile >> eta;
	inFile >> a;
	if( (a<0) || (a>1)) fatalError("affine3::loadParamset","Parameter a out of range");

	inFile >> x0;
	inFile >> p;
	inFile >> length;

	/*cout << "eta = " << eta << endl;
	cout << "a = " << a << endl;
	cout << "x0 = " << x0 << endl;
	cout << "p = " << p << endl;
	cout << "length = " << length << endl;*/
 	
    	initialize();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine3
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void affine3::saveParamset(ofstream& outFile)
{
	outFile << eta << "\t";
   	outFile << a  << "\t";
	outFile << x0 << "\t";
	outFile << p  << "\t";
	outFile << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine3
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		Andreas Starke
// Last modified:	
// By:		
//
///////////////////////////////////////////////////////////////////////////////

void affine3::printParamset()
{
	cout << eta << endl;
	cout << a << endl;
	cout << x0 << endl;
	cout << p << endl;
	cout << length << endl;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine3
// Member function:	sendParameters
// Purpose:		write all parameters into an array and return the numbers
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void affine3::sendParameters(int& amount,real** parameters)
{   
	if( *parameters )
		delete *parameters;
	amount=5;
	*parameters=new real[amount];
	if( !(*parameters) )
		fatalError("affine3::sendParameters",
			   "Can't create array for parameters");
	(*parameters[0])=eta;
	(*parameters[1])=a;
	(*parameters[2])=x0;
	(*parameters[3])=p;
	(*parameters[4])=length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		affine3
// Member function:	receiveParameters
// Purpose:		receive parameter values
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void affine3::receiveParameters(const real* parameters)
{	
	eta=parameters[0];
	a=parameters[1];
	x0=parameters[2];
	p=parameters[3];
	length=(long)(parameters[4]);
}
