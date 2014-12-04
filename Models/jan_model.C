///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/jan_model.C,v 1.1 2000/08/31 15:57:41 mmeyer Exp $
//
// Module name:		jan_model.C
// Contents:		Member functions of the class logistic
//
// Author:		Jan
// Last modified:	Fri
// By:			
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "jan_model.h"


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		jan_model
// Member function:	jan_model
// Purpose:		constructor
//
// Author:
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

jan_model::jan_model() : baseModel(1) //  1 fuer 1-dimensionalen Phasenraum;
{
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		jan_model
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void jan_model::iteration(const long&)
{
    qreal oldX=x;

    x       = (1/alpha)*exp(alpha*log(oldX)) +(1-delta)*oldX;
  }
    
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		jan_model
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void jan_model::initialize()
{
    x=x0;
   }
 
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		jan_model
// Member function:	sendModelVar
// Purpose:		return a pointer to the main model variable
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

qreal* jan_model::sendModelVar()
{
    return &x;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		jan_model
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

qreal* jan_model::setLabels(char* label)
{
    if( !strcmp(label,"alpha") )
	return( &alpha);
    if( !strcmp(label,"x") )
	return( &x);
    if( !strcmp(label,"x0") )
	return( &x0 );
    if( !strcmp(label,"delta") )
	return( &delta );
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		jan_model
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void jan_model::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("jan_model::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&x;
    // (*stateSpace)[1]=&y; // (fuer 2-dimensionalen Phasenraum);
};


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		jan_model
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Markus Lohmann
// Last modified:	Mon Jan 13 15:48:01 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void jan_model::loadParamset(ifstream& inFile)
{
    inFile >> x0 ;
    inFile >> alpha;
    inFile >> delta;
    inFile >> length;
    
    initialize();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		jan_model
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		Markus Lohmann
// Last modified:	Mon Jan 13 15:48:01 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void jan_model::saveParamset(ofstream& outFile)
{
    outFile << x0 << "\t";
    outFile << alpha  << "\t";
    outFile << delta  << "\t";
    outFile << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		jan_model
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		Markus Lohmann
// Last modified:	Mon Jan 13 15:48:01 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void jan_model::printParamset()
{
    log() << x0 << "\n";
    log() << alpha << "\n";
    log() << delta << "\n";
    log() << length << "\n";
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		jan_model
// Member function:	sendParameters
// Purpose:		write all parameters into an array and return the numbers
//
// Author:		Markus Lohmann
// Last modified:	Mon Jan 13 15:48:01 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void jan_model::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=4;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("jan_model::sendParameters",
		   "Can't create array for parameters");
    (*parameters[0])=x0;
    (*parameters[1])=alpha;
    (*parameters[2])=delta;
    (*parameters[3])=length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		jan_model
// Member function:	receiveParameters
// Purpose:		receive parameter values
//
// Author:		Markus Lohmann
// Last modified:	Mon Jan 13 15:48:01 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void jan_model::receiveParameters(const qreal* parameters)
{
    x0=parameters[0];
    alpha=parameters[1];
    delta=parameters[2];
    length=(long)(parameters[3]);
}

// eof
