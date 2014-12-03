
#include "../error.h"
#include "../strnchr.h"
#include "RBC_basic_model.h"


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		RBC_basic_model
// Member function:	RBC_basic_model
// Purpose:		constructor
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////
RBC_basic_model::RBC_basic_model() : baseModel(1)
{
zvar=NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		RBC_basic_model
// Member function:	RBC_basic_model
// Purpose:		destructor
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

RBC_basic_model::~RBC_basic_model()
{
if(zvar) delete zvar;
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		RBC_basic_model
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void RBC_basic_model::iteration(const long&)
{
qreal Z_old;

// Determine random shock for current period




Z_old = Z;
epsilon = zvar->dice();	// Set a new random value for the Parameter
eta = exp(mu + epsilon); 
Z = pow(Z_old, gamma)*eta;
Z_ln = log(Z);
// Determine output and consumption (per capita) for current period





y = Z*pow(k, alpha);
c = (1 - alpha*beta)*y;
w =	Z*(1-alpha)*pow(k, alpha);
r = Z*alpha*pow(k,alpha-1);

// Note: N_t is normalized to unity 

Y = y;
C = c;
K = k;


//k_diff = k - k_old;
//k_rate = k/k_old;
//k_ln = log(k);

/*
log() << "********************************" << endl;
log() << " New iteration " << endl;
log() << "********************************" << endl;
log() << "k = "  << k << endl;
log() << "k_old = "  << k_old << endl;
log() << "k_diff = "  << k_diff << endl;
log() << "k_rate = "  << k_rate << endl;
log() << "epsilon = "  << epsilon << endl;
log() << "eta = "  << eta << endl;
log() << "Z = "  << Z << endl;
log() << "Z_old = "  << Z_old << endl;
log() << "y = "  << y << endl;
log() << "c = "  << c << endl;
log() << endl;
*/
 

// Determine capital stock for the next period


//Save old values

k_old = k;
k_ln_old = k_ln;
k_rate_old = k_rate;
k_diff_old = k_diff;


// Determine new derived values

k = alpha*beta*y;
k_ln = log(k);
k_rate = k/k_old;
k_diff = k - k_old;
ln_k_rate = log(k_rate); 
}
    
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		RBC_basic_model
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void RBC_basic_model::initialize()
{
	// Initializing random variable  
   	
	zvar = new rand_var(this,"ranf",zvar_expr);
	if(!zvar)	
	fatalError("randvar::initialize stoch_ar:", "can't create rand_var");

	epsilon = 0;
	//epsilon_param = epsilon_param;
	eta = 0;
	k = k_0;
	Z = Z_0;
	Z_ln = log(Z);
	k_old = k_0;
	
	k_ln = log(k);
	k_rate = k/k_old;
	ln_k_rate = log(k_rate);
	k_diff = k - k_old;

	w =	Z*(1-alpha)*pow(k, alpha);
	r = Z*alpha*pow(k,alpha-1);
	

	log() << "********************************" << endl;
	log() << "Parameters of the simulation" << endl;
	log() << "********************************" << endl;
	log() << "epsilon_param = " << epsilon_param << endl;
	log() << "alpha = " << alpha << endl;
	log() << "beta  = " << beta << endl;
	log() << "gamma  = " << gamma << endl;
	log() << "mu  = " << mu << endl;
	log() << "k_0  = " << k_0 << endl;
	log() << "Z_0  = " << Z_0 << endl;


}
 
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		RBC_basic_model
// Member function:	sendModelVar
// Purpose:		return a pointer to the main model variable
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

qreal* RBC_basic_model::sendModelVar()
{
	//log() << "This is function 'sendModelVar()' in RBC_basic_model.C" << endl;
    
	return &k;
	
	//log() << "Now returning...." << endl; 
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		RBC_basic_model
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Marten Hillebrand
// Last modified:
// By:		
//
///////////////////////////////////////////////////////////////////////////////

qreal* RBC_basic_model::setLabels(char* label)
{
	//log() << "This is function 'setLabels()' in RBC_basic_model.C ." << endl;
    if( !strcmp(label,"k") )
	return( &k);
    if( !strcmp(label,"k_rate") )
	return( &k_rate);
	if( !strcmp(label,"k_ln") )
	return( &k_ln);
	if( !strcmp(label,"k_diff") )
	return( &k_diff);
    if( !strcmp(label,"k_0") )
	return( &k_0);
   if( !strcmp(label,"k_old") )
	return( &k_old);
   	if( !strcmp(label,"k_rate_old") )
	return( &k_rate_old);
	if( !strcmp(label,"ln_k_rate") )
	return( &ln_k_rate);
	if( !strcmp(label,"k_ln_old") )
	return( &k_ln_old);
	if( !strcmp(label,"k_diff_old") )
	return( &k_diff_old);
    if( !strcmp(label,"K") )
	return( &K);
    if( !strcmp(label,"Y") )
	return( &Y);
    if( !strcmp(label,"y") )
	return( &y);
	if( !strcmp(label,"C") )
	return( &C);
    if( !strcmp(label,"c") )
	return( &c);
	if( !strcmp(label,"w") )
	return( &w);
	if( !strcmp(label,"r") )
	return( &r);
	if( !strcmp(label,"epsilon") )
	return( &epsilon);
	if( !strcmp(label,"eta") )
	return( &eta);
	if( !strcmp(label,"Z") )
	return( &Z);
	if( !strcmp(label,"Z_ln") )
	return( &Z_ln);
	if( !strcmp(label,"alpha") )
	return( &alpha);
	if( !strcmp(label,"beta") )
	return( &beta);
	if( !strcmp(label,"gamma") )
	return( &gamma);
	if( !strcmp(label,"mu") )
	return( &mu);
	if( !strcmp(label,"epsilon_param") )
	return( &epsilon_param);
	if( !strcmp(label,"xBundle") )
	return &xBundle;
    if( !strcmp(label,"yBundle") )
	return &yBundle;
   	return NULL;
	//log() << "Now returning...." << endl; 
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		RBC_basic_model
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		
// Last modified: unchanged!
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void RBC_basic_model::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
	//log() << "This is function 'sendStateSpace()' in RBC_basic_model.C ." << endl; 
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("RBC_basic_model::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&k;
 	log() << "Now returning...." << endl; 
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		RBC_basic_model
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void RBC_basic_model::loadParamset(ifstream& inFile)
{
//log() << "This is function 'loadParamset()' in 'RBC_basic_model.C' ." << endl; 
 
 	inFile >> zvar_expr;

	inFile >> alpha;
	inFile >> beta;
	inFile >> gamma;
	inFile >> mu;

	inFile >> k_0;
	inFile >> Z_0;


	inFile >> length;


   	initialize();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		RBC_basic_model
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void RBC_basic_model::saveParamset(ofstream& outFile)
{
	//log() << "This is function 'saveParamset()' in 'AFFINE1.C' ." << endl; 
 	outFile << zvar_expr; 
	outFile << alpha;
	outFile << beta;
	outFile << gamma;
	outFile << mu;
	outFile << k_0;
	outFile << Z_0;
	outFile << length;

	//log() << "Now returning...." << endl; 
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		RBC_basic_model
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		Marten Hillebrand
// Last modified:	
// By:		
//
///////////////////////////////////////////////////////////////////////////////

void RBC_basic_model::printParamset()
{
    //log() << "This is function 'printParamset()' in 'AFFINE.C' ." << endl; 
	log() << zvar_expr << endl; 
	log() << alpha << endl;
	log() << beta << endl;
	log() << gamma << endl;
	log() << mu << endl;
	log() << k_0 << endl;
	log() << Z_0 << endl;
	log() << length << endl;
  

	//log() << "Now returning...." << endl; 
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		RBC_basic_model
// Member function:	sendParameters
// Purpose:		write all parameters into an array and return the numbers
//
// Author:		Marten Hillebrand
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void RBC_basic_model::sendParameters(int& amount,qreal** parameters)
{
 	//log() << "This is function 'sendParameters()' in 'RBC_basic_model.C' ." << endl; 
    
    if( *parameters )
	delete *parameters;
    amount=7;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("RBC_basic_model::sendParameters",
		   "Can't create array for parameters");
    (*parameters[0])= alpha;
	(*parameters[1])= beta;
	(*parameters[2])= gamma;
	(*parameters[3])= mu;
	(*parameters[4])= k_0;
	(*parameters[5])= Z_0;
	(*parameters[6])= length;

	//log() << "Now returning...." << endl; 
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		RBC_basic_model
// Member function:	receiveParameters
// Purpose:		receive parameter values
//
// Author:		Markus Lohmann
// Last modified:	Mon Jan 13 15:48:01 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void RBC_basic_model::receiveParameters(const qreal* parameters)
{	
	//log() << "This is function 'receiveParameters()' in 'RBC_basic_model.C' ." << endl; 
	alpha = parameters[0];
	beta  = parameters[1];
	gamma = parameters[2];
	mu 	  = parameters[3];
	k_0	  = parameters[4];
	Z_0	  = parameters[5];
	length= (long) parameters[6];
	//log() << "Now returning...." << endl; 
}

