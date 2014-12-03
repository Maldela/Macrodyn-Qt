///////////////////////////////////////////////////////////////////////////////
//
// Module name:		demog_model.C
// Contents:		Member functions of the class demog_model
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "../strnchr.h"
#include "demog_model.h"


///////////////////////////////////////////////////////////////////////////////
//
// Class name:		demog_model
// Member function:	demog_model
// Purpose:		constructor
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

demog_model::demog_model() : baseModel(1)
{

}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		demog_model
// Member function:	~demog_model
// Purpose:		destructor
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

demog_model::~demog_model()
{

}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		demog_model
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void demog_model::iteration(const long& time)
{
	switch ( type ){
	case 1 :
		s1_0_old = s1_0_t;
		eta0_old = eta0;
		eta1_old = eta1;
		eta2_old = eta2;
		if ( w_1 > (1.0/(1.0+delta+pow(delta,2.0))*(w_1+w_2/R_t+w_3/R_t/R_t)) ){
		s1_0_t = w_1 - 1.0/(1.0+delta+pow(delta,2.0))*(w_1+w_2/R_t+w_3/R_t/R_t);
		s1_1_t = s1_0_t;
		s1_2_t = s1_1_t;
		} else { s1_0_t = s1_1_t = s1_2_t = 0; }
		s2_0_t = delta/(1+delta)*(w_2+R_t*s1_0_old+w_3/R_t)-w_3/R_t;
		s2_1_t = s2_0_t;
		s2_2_t = s2_1_t;
		eta2 = 1.0-1.0/2.0 * log(1.0+2.0*varphi) / alpha_max;
		eta2 = MAX(eta2,0);
		eta0 = 1.0- eta2;
		eta1 = 0;
	break;
	case 2:
		s1_0_old = s1_0_t;
		eta0_old = eta0;
		eta1_old = eta1;
		eta2_old = eta2;
		
		// Erwartung anhand der heutigen Einzahlungen
		w_3 = tax_rate_0*(n1_t*w_1+n2_t*w_2)/n3_t;
		
		s1_0_t = (1.0-tax_rate_0)*w_1 - 1.0/(1.0+delta+pow(delta,2.0))*((1.0-tax_rate_0)*w_1+(1.0-tax_rate_0)*w_2/R_t+w_3/R_t/R_t);
		s1_1_t = s1_0_t;
		s1_2_t = s1_1_t;
		
		s2_0_t = delta/(1.0+delta)*((1.0-tax_rate_0)*w_2+R_t*s1_0_old+w_3/R_t)-w_3/R_t;
		s2_1_t = s2_0_t;
		s2_2_t = s2_1_t;
		eta2 = 1.0-1.0/2.0 * log(1.0+2.0*varphi) / alpha_max;
		eta2 = MAX(eta2,0);
		eta0 = 1.0- eta2;
		eta1 = 0;
	break;
	case 3:
		s1_0_old = s1_0_t;
		s1_1_old = s1_1_t;
		s1_2_old = s1_2_t;
		eta0_old = eta0;
		eta1_old = eta1;
		eta2_old = eta2;
		
		// Erwartung anhand der heutigen Einzahlungen
		w_3 = (tax_rate_0*n1_t*w_1+n2_t*w_2*(eta0_old*tax_rate_0+eta1_old*tax_rate_1+eta2_old*tax_rate_2))/n3_t;
		
		s1_0_t = (1.0-tax_rate_0)*w_1 - 1.0/(1.0+delta+pow(delta,2.0))*((1.0-tax_rate_0)*w_1+(1.0-tax_rate_0)*w_2/R_t+w_3/R_t/R_t);
		s1_1_t = (1.0-tax_rate_0)*w_1 - 1.0/(1.0+delta+pow(delta,2.0))*((1.0-tax_rate_0)*w_1+(1.0-tax_rate_1)*w_2/R_t+w_3/R_t/R_t);
		s1_2_t = (1.0-tax_rate_0)*w_1 - 1.0/(1.0+delta+pow(delta,2.0))*((1.0-tax_rate_0)*w_1+(1.0-tax_rate_2)*w_2/R_t+w_3/R_t/R_t);
		
		s2_0_t = delta/(1.0+delta)*((1.0-tax_rate_0)*w_2+R_t*s1_0_old+w_3/R_t)-w_3/R_t;
		s2_1_t = delta/(1.0+delta)*((1.0-tax_rate_1)*w_2+R_t*s1_1_old+w_3/R_t)-w_3/R_t;
		s2_2_t = delta/(1.0+delta)*((1.0-tax_rate_2)*w_2+R_t*s1_2_old+w_3/R_t)-w_3/R_t;
		eta2 = 1.0-1.0/2.0 * log(1.0+2.0*varphi) / alpha_max;
		eta2 = MAX(eta2,0);
		eta0 = 1.0- eta2;
		eta1 = 0;
	break;
	case 4:
		s1_0_old = s1_0_t;
		s1_1_old = s1_1_t;
		s1_2_old = s1_2_t;
		eta0_old = eta0;
		eta1_old = eta1;
		eta2_old = eta2;
		
		// Erwartung anhand der heutigen Einzahlungen
		w_3 = 0;
		
		W_0 = w_1+w_2/R_t+w_3/R_t/R_t;
		W_1 = w_1+(w_2-c_child)/R_t+w_3/R_t/R_t;
		W_2 = w_1+(w_2-2.0*c_child)/R_t+w_3/R_t/R_t;
		
		s1_0_t = w_1 - 1.0/(1.0+delta+pow(delta,2.0))*W_0;
		s1_1_t = w_1 - 1.0/(1.0+delta+pow(delta,2.0))*W_1;
		s1_2_t = w_1 - 1.0/(1.0+delta+pow(delta,2.0))*W_2;
		
		s2_0_t = delta/(1.0+delta)*(w_2+R_t*s1_0_old+w_3/R_t)-w_3/R_t;
		s2_1_t = delta/(1.0+delta)*(w_2-c_child+R_t*s1_1_old+w_3/R_t)-w_3/R_t;
		s2_2_t = delta/(1.0+delta)*(w_2-2.0*c_child+R_t*s1_2_old+w_3/R_t)-w_3/R_t;
		
		alpha0 = (1.0+delta+delta*delta)/delta*log(W_0/W_1);
		alpha1 = (1.0+delta+delta*delta)/delta*log(W_1/W_2);
		
		eta0 = MIN(alpha0/alpha_max , 1.0);
		eta2 = MAX(1.0-alpha1/alpha_max , 0.0);
		eta1 = 1.0-eta0-eta2;
	break;
	case 5:
		s1_0_old = s1_0_t;
		s1_1_old = s1_1_t;
		s1_2_old = s1_2_t;
		eta0_old = eta0;
		eta1_old = eta1;
		eta2_old = eta2;
		
		// Erwartung anhand der heutigen Einzahlungen
		w_3 = tax_rate_0*(n1_t*w_1+n2_t*w_2)/n3_t;
		
		W_0 = (1.0-tax_rate_0)*w_1+(1.0-tax_rate_0)*w_2/R_t+w_3/R_t/R_t;
		W_1 = (1.0-tax_rate_0)*w_1+((1.0-tax_rate_0)*w_2-c_child)/R_t+w_3/R_t/R_t;
		W_2 = (1.0-tax_rate_0)*w_1+((1.0-tax_rate_0)*w_2-2.0*c_child)/R_t+w_3/R_t/R_t;
		
		s1_0_t = (1.0-tax_rate_0)*w_1 - 1.0/(1.0+delta+pow(delta,2.0))*W_0;
		s1_1_t = (1.0-tax_rate_0)*w_1 - 1.0/(1.0+delta+pow(delta,2.0))*W_1;
		s1_2_t = (1.0-tax_rate_0)*w_1 - 1.0/(1.0+delta+pow(delta,2.0))*W_2;
		
		s2_0_t = delta/(1.0+delta)*((1.0-tax_rate_0)*w_2+R_t*s1_0_old+w_3/R_t)-w_3/R_t;
		s2_1_t = delta/(1.0+delta)*((1.0-tax_rate_0)*w_2-c_child+R_t*s1_1_old+w_3/R_t)-w_3/R_t;
		s2_2_t = delta/(1.0+delta)*((1.0-tax_rate_0)*w_2-2.0*c_child+R_t*s1_2_old+w_3/R_t)-w_3/R_t;
		
		alpha0 = (1.0+delta+delta*delta)/delta*log(W_0/W_1);
		alpha1 = (1.0+delta+delta*delta)/delta*log(W_1/W_2);
		
		eta0 = MIN(alpha0/alpha_max , 1.0);
		eta2 = MAX(1.0-alpha1/alpha_max , 0.0);
		eta1 = 1.0-eta0-eta2;
	break;
	case 6:
		s1_0_old = s1_0_t;
		s1_1_old = s1_1_t;
		s1_2_old = s1_2_t;
		eta0_old = eta0;
		eta1_old = eta1;
		eta2_old = eta2;
		
		// Erwartung anhand der heutigen Einzahlungen/Auszahlung
		w_3 = (tax_rate_0*n1_t*w_1+n2_t*w_2*(eta0_old*tax_rate_0+eta1_old*tax_rate_1+eta2_old*tax_rate_2))/n3_t;
		
		W_0 = (1.0-tax_rate_0)*w_1+(1.0-tax_rate_0)*w_2/R_t+w_3/R_t/R_t;
		W_1 = (1.0-tax_rate_0)*w_1+((1.0-tax_rate_1)*w_2-c_child)/R_t+w_3/R_t/R_t;
		W_2 = (1.0-tax_rate_0)*w_1+((1.0-tax_rate_2)*w_2-2.0*c_child)/R_t+w_3/R_t/R_t;
		
		s1_0_t = (1.0-tax_rate_0)*w_1 - 1.0/(1.0+delta+pow(delta,2.0))*W_0;
		s1_1_t = (1.0-tax_rate_0)*w_1 - 1.0/(1.0+delta+pow(delta,2.0))*W_1;
		s1_2_t = (1.0-tax_rate_0)*w_1 - 1.0/(1.0+delta+pow(delta,2.0))*W_2;
		
		s2_0_t = delta/(1.0+delta)*((1.0-tax_rate_0)*w_2+R_t*s1_0_old+w_3/R_t)-w_3/R_t;
		s2_1_t = delta/(1.0+delta)*((1.0-tax_rate_1)*w_2-c_child+R_t*s1_1_old+w_3/R_t)-w_3/R_t;
		s2_2_t = delta/(1.0+delta)*((1.0-tax_rate_2)*w_2-2.0*c_child+R_t*s1_2_old+w_3/R_t)-w_3/R_t;
		
		alpha0 = (1.0+delta+delta*delta)/delta*log(W_0/W_1);
		alpha1 = (1.0+delta+delta*delta)/delta*log(W_1/W_2);
		alpha2 = (1.0+delta+delta*delta)/(2.0*delta)*log(W_0/W_2);
		
		if ( alpha0 > alpha1 ){
			eta0 = MIN(alpha0/alpha_max , 1.0);
			eta2 = MAX(1.0-alpha1/alpha_max , 0.0);
			eta1 = 1.0-eta0-eta2;
		} else {
			eta0 = MIN(alpha2/alpha_max , 1.0 );
			eta2 = 1.0 - eta0;
			eta1 = 0.0;
		}
	break;
	default:
        log() << "error in iteration!";
		exit(-1);
	};
	theta_K = K_t;
	K_t = n1_t*(eta0*s1_0_t+eta1*s1_1_t+eta2*s1_2_t)+ n2_t*(eta0_old*s2_0_t+eta1_old*s2_1_t+eta2_old*s2_2_t);
	theta_K = K_t / theta_K;
	n3_t = n2_t;
	n2_t = n1_t;
	n1_t = n0_t;
	n0_t = eta1*n2_t+2.0*eta2*n2_t;
	theta_N = n0_t / n1_t;
	theta_L = L_t;
	L_t = l_1 * n1_t + l_2 * n2_t;
	theta_L = L_t / theta_L;
	theta_k = k_t;
	k_t = K_t / L_t;
	theta_k = k_t / theta_k;
	r_t = A*B*pow(k_t, B-1.0);
	R_t = 1 + r_t;
	theta_W = w_t;
	w_t = A*(1-B)*pow(k_t, B);
	theta_W = w_t / theta_W;
	w_1 = l_1 * w_t;
	w_2 = l_2 * w_t;
}
    
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		demog_model
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void demog_model::initialize()
{
	n0_t = n0_0;
	n1_t = n1_0;
	n2_t = n2_0;
	n3_t = n3_0;
	L_t = l_1 * n1_t + l_2 * n2_t;
	K_t = K_0;
	k_t = K_t / L_t;
	r_t = A*B*pow(k_t, B-1.0);
	R_t = 1+ r_t;
	w_t = A*(1-B)*pow(k_t, B);
	w_1 = l_1 * w_t;
	w_2 = l_2 * w_t;
	w_3 = 0;
	s1_0_t = 0;
	s1_1_t = 0;
	s1_2_t = 0;
	s2_0_t = 0;
	s2_1_t = 0;
	s2_2_t = 0;
	eta0 = eta1 = eta2 = eta0_old = eta1_old =eta2_old = 0;
	// naive Erwartungen
	R_1 = R_t;
	R_2 = R_t;
	S_t = 0;
	theta_L = 1;
	theta_K = 1;
	theta_W = 1;
	theta_N = 1;
	theta_k = 1;
}
 
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		demog_model
// Member function:	sendModelVar
// Purpose:		return a pointer to the main model variable
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

qreal* demog_model::sendModelVar()
{
    return &k_t;
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		demog_model
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

qreal* demog_model::setLabels(char* label)
{
	if ( !strcmp(label, "xBundle") ) return( &xBundle );
	if ( !strcmp(label, "yBundle") ) return( &yBundle );
	
	if ( !strcmp(label, "l_1") ) return( &l_1 );
	if ( !strcmp(label, "l_2") ) return( &l_2 );
	
	if ( !strcmp(label, "k_t") ) return( &k_t );
	if ( !strcmp(label, "K_t") ) return( &K_t );	
	if ( !strcmp(label, "L_t") ) return( &L_t );
	if ( !strcmp(label, "r_t") ) return( &r_t );
	if ( !strcmp(label, "R_t") ) return( &R_t );
	if ( !strcmp(label, "w_t") ) return( &w_t );
	if ( !strcmp(label, "w_3") ) return( &w_3 );
	
	if ( !strcmp(label, "K_0") ) return( &K_0 );	
	if ( !strcmp(label, "c_child") ) return( &c_child );
	if ( !strcmp(label, "varphi") ) return( &varphi );
	if ( !strcmp(label, "alpha0") ) return( &alpha0 );
	if ( !strcmp(label, "alpha1") ) return( &alpha1 );
	if ( !strcmp(label, "alpha2") ) return( &alpha2 );
	if ( !strcmp(label, "alpha_max") ) return( &alpha_max );		
	if ( !strcmp(label, "B") ) return( &B );
	if ( !strcmp(label, "A") ) return( &A );
	
	if ( !strcmp(label, "n0_t") ) return( &n0_t );
	if ( !strcmp(label, "n1_t") ) return( &n1_t );
	if ( !strcmp(label, "n2_t") ) return( &n2_t );
	if ( !strcmp(label, "n3_t") ) return( &n3_t );
	
	if ( !strcmp(label, "eta0") ) return( &eta0 );
	if ( !strcmp(label, "eta1") ) return( &eta1 );
	if ( !strcmp(label, "eta2") ) return( &eta2 );
	
	if ( !strcmp(label, "delta") ) return( &delta );
	
	if ( !strcmp(label, "s1_0_t") ) return( &s1_0_t );
	if ( !strcmp(label, "s1_1_t") ) return( &s1_1_t );
	if ( !strcmp(label, "s1_2_t") ) return( &s1_2_t );
	
	if ( !strcmp(label, "s2_0_t") ) return( &s2_0_t );
	if ( !strcmp(label, "s2_1_t") ) return( &s2_1_t );
	if ( !strcmp(label, "s2_2_t") ) return( &s2_2_t );
	
	if ( !strcmp(label, "theta_L") ) return( &theta_L );
	if ( !strcmp(label, "theta_K") ) return( &theta_K );
	if ( !strcmp(label, "theta_W") ) return( &theta_W );
	if ( !strcmp(label, "theta_N") ) return( &theta_N );
	if ( !strcmp(label, "theta_k") ) return( &theta_k );
	
	if ( !strcmp(label, "tax_rate_0") ) return( &tax_rate_0 );
	if ( !strcmp(label, "tax_rate_1") ) return( &tax_rate_1 );
	if ( !strcmp(label, "tax_rate_2") ) return( &tax_rate_2 );
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		demog_model
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void demog_model::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace = new const qreal* [1];
    if( !(*stateSpace) )
	fatalError("demog_model::sendStateSpace",
		   "Can't create state space vector");
    quantity=1;
    (*stateSpace)[0]=&k_t;
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		demog_model
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void demog_model::loadParamset(ifstream& inFile)
{
	inFile >> type;
	switch ( type ){
	case 1:	
		inFile >> K_0 >> w_0 >> R_0;
		inFile >> n0_0 >> n1_0 >> n2_0 >> n3_0;
		inFile >> l_1 >> l_2 >> alpha_max >> varphi;
		inFile >> A >> B >> delta;
	break;
	case 2:
		inFile >> K_0 >> w_0 >> R_0;
		inFile >> n0_0 >> n1_0 >> n2_0 >> n3_0;
		inFile >> l_1 >> l_2 >> alpha_max >> varphi;
		inFile >> A >> B >> delta;
		inFile >> tax_rate_0;
	break;
	case 3:
		inFile >> K_0 >> w_0 >> R_0;
		inFile >> n0_0 >> n1_0 >> n2_0 >> n3_0;
		inFile >> l_1 >> l_2 >> alpha_max >> varphi;
		inFile >> A >> B >> delta;
		inFile >> tax_rate_0 >> tax_rate_1 >> tax_rate_2;
	break;
	case 4:
		inFile >> K_0 >> w_0 >> R_0;
		inFile >> n0_0 >> n1_0 >> n2_0 >> n3_0;
		inFile >> l_1 >> l_2 >> alpha_max >> varphi;
		inFile >> A >> B >> delta;
		inFile >> c_child;
	break;
	case 5:
		inFile >> K_0 >> w_0 >> R_0;
		inFile >> n0_0 >> n1_0 >> n2_0 >> n3_0;
		inFile >> l_1 >> l_2 >> alpha_max >> varphi;
		inFile >> A >> B >> delta;
		inFile >> c_child;
		inFile >> tax_rate_0;
	break;
	case 6:
		inFile >> K_0 >> w_0 >> R_0;
		inFile >> n0_0 >> n1_0 >> n2_0 >> n3_0;
		inFile >> l_1 >> l_2 >> alpha_max >> varphi;
		inFile >> A >> B >> delta;
		inFile >> c_child;
		inFile >> tax_rate_0 >> tax_rate_1 >> tax_rate_2;
	break;
	default: 
        log() << "demog_model::loadParamset unknown model type";
		exit(-1);
	};

	inFile >> length;
	
	initialize();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		demog_model
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void demog_model::saveParamset(ofstream& outFile)
{
	outFile << k_t;
	outFile << "\t" << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		demog_model
// Member function:	saveParamsetWithNames
// Purpose:		write parameterset into a file
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////


void demog_model::saveParamsetWithNames(ofstream& outFile)
{
	outFile << "no. generations = " << k_t << endl;
	outFile << "length = " << length << endl;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		demog_model
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void demog_model::printParamset()
{
    log() << "no. generations = " << k_t << endl;
    log() << "length = " << length << endl;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		demog_model
// Member function:	sendParameters
// Purpose:		write all parameters into an array and return the numbers
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void demog_model::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=2;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("demog_model::sendParameters",
		   "Can't create array for parameters");
    (*parameters[0])=k_t;
    (*parameters[1])=length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		demog_model
// Member function:	receiveParameters
// Purpose:		receive parameter values
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void demog_model::receiveParameters(const qreal* parameters)
{
    k_t=parameters[0];
    length=long(parameters[1]);
}

