
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

void affine3::iteration(const qint64&)
{   
	urv_p = zvar1->dice();	// pick iteration function according to p
	urv_e = zvar2->dice();	// get distortion value in t
	qreal oldX = x;
	
	if(urv_p < p){
		//log() << "map1 is chosen" << "\n";
		count1++;
		x = (a+urv_e)*oldX;
	} else {
		//log() << "map2 is chosen" << "\n";
		count2++;
		x = (a-1+urv_e)*oldX + 1;
	}

	/*log() << "oldX = " << oldX << "\n";
	log() << "urv_p = " << urv_p << "\n";
	log() << "urv_e = " << urv_e << "\n";
	log() << "x = " << x << "\n";
	log() << "count1 = " << count1 << "\n";
	log() << "count2 = " << count2 << "\n";*/
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

qreal* affine3::sendModelVar()
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

qreal* affine3::setLabels(const QString& label)
{
    if (label == "a")
		return( &a);
    if (label == "x")
		return( &x);
    if (label == "x0")
		return( &x0 );
    if (label == "p")
		return( &p );
    if (label == "urv_p")
		return( &urv_p );
    if (label == "urv_e")
		return( &urv_e );
    if (label == "eta")
		return( &eta );
    if (label == "xBundle")
		return( &xBundle );
    if (label == "yBundle")
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

void affine3::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
	if( stateSpace )
		delete stateSpace;
	*stateSpace = new const qreal* [dimension];
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

void affine3::loadParamset(QDataStream& inFile)
{
 	inFile >> zvar_expr1;
	//log() << zvar_expr1 << "\n";
	//inFile >> zvar_expr2;
	//log() << zvar_expr2 << "\n";
	inFile >> eta;
	inFile >> a;
	if( (a<0) || (a>1)) fatalError("affine3::loadParamset","Parameter a out of range");

	inFile >> x0;
	inFile >> p;
	inFile >> length;

	/*log() << "eta = " << eta << "\n";
	log() << "a = " << a << "\n";
	log() << "x0 = " << x0 << "\n";
	log() << "p = " << p << "\n";
	log() << "length = " << length << "\n";*/
 	
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

void affine3::saveParamset(QDataStream& outFile)
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
	log() << eta << "\n";
	log() << a << "\n";
	log() << x0 << "\n";
	log() << p << "\n";
	log() << length << "\n";
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

void affine3::sendParameters(int& amount,qreal** parameters)
{   
	if( *parameters )
		delete *parameters;
	amount=5;
	*parameters=new qreal[amount];
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

void affine3::receiveParameters(const qreal* parameters)
{	
	eta=parameters[0];
	a=parameters[1];
	x0=parameters[2];
	p=parameters[3];
	length=(qint64)(parameters[4]);
}
