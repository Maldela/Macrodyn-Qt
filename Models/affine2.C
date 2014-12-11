
#include "../error.h"
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

void affine2::iteration(const qint64&)
{
	//log() << "This is function 'iteration()' in affine2.C" 
    
 	urv = zvar->dice();	// Set a new random value for the Parameter
    qreal oldX=x;
	
	if(urv < p) 
	{
	//log() << "map1 is chosen" 
	count1 = count1 + 1;
	x = a1*oldX;
	}
	else
	{
	//log() << "map2 is chosen" 
	x = a2*oldX + 1;
	count2 = count2 + 1;
	}

	/*log() << "oldX = " << oldX 
	log() << "urv  = " << urv 
	log() << "x = " << x 
	log() << "count1 = " << count1 
	log() << "count2 = " << count2 */

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
	//log() << "This is function 'initialize()' in AFFINE2.C ." 
   	//log() << incase1 ;
	x=x0;
	count1=0;
	count2=0;
    zvar = new rand_var(this,"ranf",zvar_expr);
	if(!zvar)	
	fatalError("randvar::initialize stoch_ar", "can't create rand_var");
	//log() << "Now returning...."  
	if(incase1==1) 
	{
	//log() << "case1 is valid:" 
	a1 = a;
	a2 = a-1;
	/*log() << "a = " << a 
	log() << "a1= a = " << a1 
	log() << "a2= a-1 = " << a2 */
	}
else
	{
	/*log() << "case2 or case 3 is valid:" 
	log() << "a = " << a 
	log() << "a1= a = " << a1 
	log() << "a2= a-1 = " << a2 */
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
	//log() << "This is function 'sendModelVar()' in AFFINE2.C" 
    return &x;
	//log() << "Now returning...."  
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

qreal* affine2::setLabels(const QString& label)
{
	//log() << "This is function 'setLabels()' in AFFINE2.C ." 
    if (label == "a")
	return( &a);
    if (label == "a1")
	return( &a1);
    if (label == "a2")
	return( &a2);
    if (label == "x")
	return( &x);
    if (label == "x0")
	return( &x0 );
    if (label == "p")
	return( &p );
    if (label == "urv")
	return( &urv );
    if (label == "xBundle")
	return( &xBundle );
    if (label == "yBundle")
	return( &yBundle );
    return NULL;
	//log() << "Now returning...."  
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
	//log() << "This is function 'sendStateSpace()' in AFFINE2.C ."  
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("affine2::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&x;
    log() << "Now returning....";
}

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

void affine2::loadParamset(QTextStream& inFile)
{
//log() << "This is function 'loadParamset()' in 'AFFINE2.C' ."  
 
 	inFile >> zvar_expr;  
	//log() << zvar_expr 
    inFile >> incase1;
	//log() << incase1 
	inFile >> a;
	//log() << a 
	if(a<0 || a > 1) fatalError("affine2::loadParamset","Parameter a out of range");
	inFile >> a1;
	//log() << a1 
	if((a1<=0) || (a1>=1)) fatalError("affine2::loadParamset","Parameter a1 out of range");
	inFile >> a2;
    if(a2<=-1 || a2 >= 0) fatalError("affine2::loadParamset","Parameter a2 out of range");
	
	inFile >> x0;
	inFile >> p;
	inFile >> length;

	/*log() << "a = " << a 
	log() << "a1= " << a1 
	log() << "a2= " << a2 */
 	
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

void affine2::saveParamset(QTextStream& outFile)
{
	//log() << "This is function 'saveParamset()' in 'AFFINE1.C' ."  
   	outFile << a  << "\t";
	outFile << a1  << "\t";
	outFile << a2  << "\t";
	outFile << x0  << "\t";
	outFile << p  << "\t";
    outFile << length;
	//log() << "Now returning...."  
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
    //log() << "This is function 'printParamset()' in 'AFFINE.C' ."  
    log() << a;
    log() << a1;
    log() << a2;
    log() << x0;
    log() << p;
    log() << length;
    //log() << "Now returning....";
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
 	//log() << "This is function 'sendParameters()' in 'AFFINE2.C' ."  
    
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
	//log() << "Now returning...."  
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

void affine2::receiveParameters(const QList<qreal>& parameters)
{	
    if (parameters.size() != 6) log() << "Wrong number of parameters!";
    else
    {
        //log() << "This is function 'receiveParameters()' in 'AFFINE2.C' ." 
        a=parameters[0];
        a1=parameters[1];
        a2=parameters[2];
        x0=parameters[3];
        p=parameters[4];
        length=(qint64)(parameters[5]);
        //log() << "Now returning...." 
    }
}

