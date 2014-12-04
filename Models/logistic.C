///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/logistic.C,v 1.1 2000/08/31 16:00:12 mmeyer Exp $
//
// Module name:		logistic.C
// Contents:		Member functions of the class logistic
//
// Author:		Markus Lohmann
// Last modified:	Fri Jul  4 10:49:11 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "../strnchr.h"
#include "logistic.h"


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		logistic
// Member function:	logistic
// Purpose:		constructor
//
// Author:		Markus Lohmann
// Last modified:	Mon Jan 13 15:48:01 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

logistic::logistic() : baseModel(1)
{
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		logistic
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		Markus Lohmann
// Last modified:	Mon Jan 13 15:48:01 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void logistic::iteration(const long&)
{
    qreal oldX=x;

    x       = alpha * oldX * (1-oldX);
    x_prime = alpha * (1 - 2 * x);
}
    
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		logistic
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Markus Lohmann
// Last modified:	Mon Jan 13 15:48:01 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void logistic::initialize()
{
    x=x0;
    x_prime=0;
}
 
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		logistic
// Member function:	sendModelVar
// Purpose:		return a pointer to the main model variable
//
// Author:		Markus Lohmann
// Last modified:	Mon Jan 13 15:48:01 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

qreal* logistic::sendModelVar()
{
    return &x;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		logistic
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Markus Lohmann
// Last modified:	Mon Jan 13 15:48:01 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

qreal* logistic::setLabels(char* label)
{
    if( !strcmp(label,"alpha") )
	return( &alpha);
    if( !strcmp(label,"x") )
	return( &x);
    if( !strcmp(label,"x0") )
	return( &x0 );
    if( !strcmp(label,"x_prime") )
	return( &x_prime );
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		logistic
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		Markus Lohmann
// Last modified:	Mon Jan 13 15:48:01 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void logistic::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("logistic::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&x;
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		logistic
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Markus Lohmann
// Last modified:	Mon Jan 13 15:48:01 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void logistic::loadParamset(ifstream& inFile)
{
    inFile >> x0 ;
    inFile >> alpha;
    inFile >> length;
    
    initialize();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		logistic
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		Markus Lohmann
// Last modified:	Mon Jan 13 15:48:01 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void logistic::saveParamset(ofstream& outFile)
{
    outFile << x0 << "\t";
    outFile << alpha  << "\t";
    outFile << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		logistic
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		Markus Lohmann
// Last modified:	Mon Jan 13 15:48:01 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void logistic::printParamset()
{
    log() << x0 << "\n";
    log() << alpha << "\n";
    log() << length << "\n";
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		logistic
// Member function:	sendParameters
// Purpose:		write all parameters into an array and return the numbers
//
// Author:		Markus Lohmann
// Last modified:	Mon Jan 13 15:48:01 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void logistic::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=3;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("logistic::sendParameters",
		   "Can't create array for parameters");
    (*parameters[0])=x0;
    (*parameters[1])=alpha;
    (*parameters[2])=length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		logistic
// Member function:	receiveParameters
// Purpose:		receive parameter values
//
// Author:		Markus Lohmann
// Last modified:	Mon Jan 13 15:48:01 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void logistic::receiveParameters(const qreal* parameters)
{
    x0=parameters[0];
    alpha=parameters[1];
    length=(long)(parameters[2]);
}

///////////////////////////////////////////////////////////////////////////////
// rlogistic
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rlogistic
// Member function:	rlogistic
// Purpose:		constructor
//
// Author:		Uli Middelberg
// Last modified:	Tue Jan 21 16:01:19 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

rlogistic::rlogistic() : logistic()
{
  zvar = NULL;
  mc = NULL;
  mc_flag  = 0;
  zvar_ptr = NULL;
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rlogistic
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		Uli Middelberg
// Last modified:	Fri Jul  4 10:49:29 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void rlogistic::iteration(const long& t)
{
  if( mc_flag ) {		// do we use a markov chain ?
    * zvar_ptr = mc->dice();	// Set a new random value for the Parameter
  } else {
    * zvar_ptr = zvar->dice();	// Set a new random value for the Parameter
  }
  logistic::iteration(t);	// iterate as usual
  x += beta;			// in case of additive noise
}
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rlogistic
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Markus Lohmann
// Last modified:	Fri Jul  4 10:49:42 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void rlogistic::loadParamset(ifstream& inputFile)
{
    int n_states;
    
  inputFile >> zvar_name;		// read the name of the stochastic parameter
  inputFile >> zvar_expr;		// read the definition
  
  if( zvar_expr[0] == '{' ) {		// we have a definition of a markov chain
    mc_flag = 1;
    strcpy(mc_states, zvar_expr);
    n_states = strnchr(mc_states, ';');
    mc_matrix[0] = '\0';
    for (int n = 0; n < n_states; n++) {
      inputFile >> zvar_expr;		// compose the matrix from
      strcat(mc_matrix, zvar_expr);	// the input file with blank at the
      strcat(mc_matrix, " ");		// end of each row
    }
  } else {
    mc_flag = 0;
  }
  
  inputFile >> a;			// read two free usable parameters
  inputFile >> b;			//

  logistic::loadParamset(inputFile);	// read the parameters for the logistic
  					// model as usual and initialize
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rlogistic
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Uli Middelberg
// Last modified:	Fri Jul  4 10:49:51 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void rlogistic::initialize()
{
    zvar_ptr = setLabels(zvar_name); 	// get a pointer to the parameter
    if( zvar_ptr == NULL ) {
      error("rlogistic::initialize: unknown label %s",zvar_name);
    }

    if( mc_flag ) {			// do we use a markov chain ?
      if( mc != NULL ) {		// delete the old one
        delete mc;			// if it exists
      }					// and get a new one
      mc = new markov_chain( this, "ranf", mc_states, mc_matrix);
      
    } else {				// we use a random variable
      if( zvar != NULL ) {		// delete the old one
        delete zvar;			// if it exists
      }					// and get a new one
      zvar = new rand_var( this, "ranf", zvar_expr);
    }

   
    beta = 0;				// initially beta should be set to zero
    logistic::initialize();
}
 
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rlogistic
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Markus Lohmann
// Last modified:	Fri Jul  4 10:50:04 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

qreal* rlogistic::setLabels(char* label)
{
    if( !strcmp(label,"a") )
	return( &a);
    if( !strcmp(label,"b") )
	return( &b);
    if( !strcmp(label,"beta") )
	return( &beta);
    return logistic::setLabels(label);
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rlogistic
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		Markus Lohmann
// Last modified:	Fri Jul  4 10:50:11 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void rlogistic::saveParamset(ofstream& outFile)
{
    outFile << zvar_name << "\t";
    outFile << zvar_expr << "\t";
    outFile << a << "\t";
    outFile << b  << "\t";
    logistic::saveParamset(outFile);
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rlogistic
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		Markus Lohmann
// Last modified:	Fri Jul  4 10:50:20 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void rlogistic::printParamset()
{
    log() << zvar_name << "\n";
    log() << zvar_expr << "\n";
    log() << a << "\n";
    log() << b << "\n";
    logistic::printParamset();
}

// eof
