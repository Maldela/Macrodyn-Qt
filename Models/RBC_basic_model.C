
#include "../error.h"

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

void RBC_basic_model::iteration(const qint64&)
{
qreal Z_old;

// DeterqMine random shock for current period




Z_old = Z;
epsilon = zvar->dice();	// Set a new random value for the Parameter
eta = exp(mu + epsilon); 
Z = pow(Z_old, gamma)*eta;
Z_ln = log(Z);
// DeterqMine output and consumption (per capita) for current period





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
log() << "********************************" 
log() << " New iteration " 
log() << "********************************" 
log() << "k = "  << k 
log() << "k_old = "  << k_old 
log() << "k_diff = "  << k_diff 
log() << "k_rate = "  << k_rate 
log() << "epsilon = "  << epsilon 
log() << "eta = "  << eta 
log() << "Z = "  << Z 
log() << "Z_old = "  << Z_old 
log() << "y = "  << y 
log() << "c = "  << c 
log() 
*/
 

// DeterqMine capital stock for the next period


//Save old values

k_old = k;
k_ln_old = k_ln;
k_rate_old = k_rate;
k_diff_old = k_diff;


// DeterqMine new derived values

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
	

    log() << "********************************";
    log() << "Parameters of the simulation";
    log() << "********************************";
    log() << "epsilon_param = " << epsilon_param;
    log() << "alpha = " << alpha;
    log() << "beta  = " << beta;
    log() << "gamma  = " << gamma;
    log() << "mu  = " << mu;
    log() << "k_0  = " << k_0;
    log() << "Z_0  = " << Z_0;


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
	//log() << "This is function 'sendModelVar()' in RBC_basic_model.C" 
    
	return &k;
	
	//log() << "Now returning...."  
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

qreal* RBC_basic_model::setLabels(const QString& label)
{
	//log() << "This is function 'setLabels()' in RBC_basic_model.C ." 
    if (label == "k")
	return( &k);
    if (label == "k_rate")
	return( &k_rate);
	if (label == "k_ln")
	return( &k_ln);
	if (label == "k_diff")
	return( &k_diff);
    if (label == "k_0")
	return( &k_0);
   if (label == "k_old")
	return( &k_old);
   	if (label == "k_rate_old")
	return( &k_rate_old);
	if (label == "ln_k_rate")
	return( &ln_k_rate);
	if (label == "k_ln_old")
	return( &k_ln_old);
	if (label == "k_diff_old")
	return( &k_diff_old);
    if (label == "K")
	return( &K);
    if (label == "Y")
	return( &Y);
    if (label == "y")
	return( &y);
	if (label == "C")
	return( &C);
    if (label == "c")
	return( &c);
	if (label == "w")
	return( &w);
	if (label == "r")
	return( &r);
	if (label == "epsilon")
	return( &epsilon);
	if (label == "eta")
	return( &eta);
	if (label == "Z")
	return( &Z);
	if (label == "Z_ln")
	return( &Z_ln);
	if (label == "alpha")
	return( &alpha);
	if (label == "beta")
	return( &beta);
	if (label == "gamma")
	return( &gamma);
	if (label == "mu")
	return( &mu);
	if (label == "epsilon_param")
	return( &epsilon_param);
	if (label == "xBundle")
	return &xBundle;
    if (label == "yBundle")
	return &yBundle;
   	return NULL;
	//log() << "Now returning...."  
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

void RBC_basic_model::sendStateSpace(int &quantity,QList<qreal *> *stateSpace)
{
	//log() << "This is function 'sendStateSpace()' in RBC_basic_model.C ."  
    stateSpace->clear();
    quantity = dimension;
    *stateSpace << &k;
    log() << "Now returning....";
}

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

void RBC_basic_model::loadParamset(QTextStream& inFile)
{
//log() << "This is function 'loadParamset()' in 'RBC_basic_model.C' ."  
 
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

void RBC_basic_model::saveParamset(QTextStream& outFile)
{
	//log() << "This is function 'saveParamset()' in 'AFFINE1.C' ."  
 	outFile << zvar_expr; 
	outFile << alpha;
	outFile << beta;
	outFile << gamma;
	outFile << mu;
	outFile << k_0;
	outFile << Z_0;
	outFile << length;

	//log() << "Now returning...."  
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
    //log() << "This is function 'printParamset()' in 'AFFINE.C' .";
    log() << zvar_expr;
    log() << alpha;
    log() << beta;
    log() << gamma;
    log() << mu;
    log() << k_0;
    log() << Z_0;
    log() << length;
  

	//log() << "Now returning...."  
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
 	//log() << "This is function 'sendParameters()' in 'RBC_basic_model.C' ."  
    
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

	//log() << "Now returning...."  
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

void RBC_basic_model::receiveParameters(const QList<qreal>& parameters)
{	
	//log() << "This is function 'receiveParameters()' in 'RBC_basic_model.C' ."  
	alpha = parameters[0];
	beta  = parameters[1];
	gamma = parameters[2];
	mu 	  = parameters[3];
	k_0	  = parameters[4];
	Z_0	  = parameters[5];
	length= (qint64) parameters[6];
	//log() << "Now returning...."  
}

