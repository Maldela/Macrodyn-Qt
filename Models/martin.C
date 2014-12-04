/******************************************************************************/
/*                                                                            */
/* Module name:     martin.C                                                  */
/* Contents:        Member functions of the class martin                      */
/*                                                                            */
/* Last modified:   Tue May 14 15:45:06 MESZ 1996 (Uli Middelberg)            */
/*                                                                            */
/******************************************************************************/

#include "martin.h"
#include "../error.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      martin                                                    */
/* Member function: martin                                                    */
/* Purpose:         constructor                                               */
/* Last modified:   20.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

martin::martin() : baseModel(3)
{
	zvar = NULL;
}

martin::~martin()
{
	if( zvar ) delete zvar;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      martin                                                    */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, define the systems initial state    */
/* Last modified:   20.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void martin::initialize()
{
    beta=beta0;
    oldBeta=beta;
    k=k0;
    g=g0;
    p=p0;

    if ( zvar != NULL ) delete zvar;
    zvar = new rand_var( this, "ranf", "1[0,25];" );
    if( !(zvar) )
	   fatalError("martin::initialize",
	              "can't create new rand_var");
}
  

/******************************************************************************/
/*                                                                            */
/* Class name:      martin                                                    */
/* Member function: sendModelVar                                              */
/* Purpose:         return a pointer to the main model variable               */
/* Last modified:   09.06.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal* martin::sendModelVar()
{
    return &beta;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      martin                                                    */
/* Member function: setLabels                                                 */
/* Purpose:         return a pointer to a variable or a parameter specified   */
/*                  by its name                                               */
/* Last modified:   09.06.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal* martin::setLabels(char* label)
{
    if( !strcmp(label,"xBundle") )
	return &xBundle;
    if( !strcmp(label,"yBundle") )
	return &yBundle;
    if( !strcmp(label,"theta") )
	return( &theta);
    if( !strcmp(label,"then") )
//    if( !strcmp(label,"beta") )
	return( &beta);
//    if( !strcmp(label,"beta_old") )
    if( !strcmp(label,"theo") )
	return( &oldBeta);
    if( !strcmp(label,"rho") )
	return( &rho);
    if( !strcmp(label,"k") )
	return( &k);
    if( !strcmp(label,"p") ) 
	return( &p );
    if( !strcmp(label,"perr") ) 
	return( &perr );
    if( !strcmp(label,"error") ) 
	return( &perr );
    if( !strcmp(label,"perrp") ) 
	return( &perrp );
    if( !strcmp(label,"perrlnp") ) 
	return( &perrlnp );
    if( !strcmp(label,"g") )
	return( &g);
    if( !strcmp(label,"beta0") )
	return( &beta0 );
    if( !strcmp(label,"k0") )
	return( &k0);
    if( !strcmp(label,"g0") )
	return( &g0 );
    if( !strcmp(label,"p0") )
	return( &p0 );

    return NULL;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      martin                                                    */
/* Member function: sendStateSpace                                            */
/* Purpose:         return pointers to the state variables and inform about   */
/*                  the systems dimension                                     */
/* Last modified:   09.06.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void martin::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !stateSpace )
	fatalError("martin::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&beta;
    (*stateSpace)[1]=&k;
    (*stateSpace)[2]=&g;
};


/******************************************************************************/
/*                                                                            */
/* Class name:      martin                                                  */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   09.06.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void martin::loadParamset(ifstream& inFile)
{
    inFile >> beta0 >> k0 >> g0 >> p0;
    inFile >> theta >> rho;
    inFile >> length;
    
    initialize();
}


/******************************************************************************/
/*                                                                            */
/* Class name:      martin                                                    */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   09.06.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void martin::saveParamset(ofstream& outFile)
{
    outFile << beta0 << "\t" << k0 << "\t" << g0 << "\t" << p0 << "\t";
    outFile << theta  << "\t" << rho << "\t";
    outFile << length;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      martin                                                    */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   09.06.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void martin::printParamset()
{
    log() << beta0 << "\t" << k0 << "\t" << g0 << "\t" << p0 << "\n";
    log() << theta << "\t" << rho << "\n";
    log() << length << "\n";
}


/******************************************************************************/
/*                                                                            */
/* Class name:      martin                                                    */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void martin::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=7;
    *parameters=new qreal[amount];
    if( !parameters )
	fatalError("martin::sendParameters",
		   "Can't create array for parameters");
    (*parameters[0])=beta0;
    (*parameters[1])=k0;
    (*parameters[2])=g0;
    (*parameters[3])=p0;
    (*parameters[4])=theta;
    (*parameters[5])=rho;
    (*parameters[6])=length;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      martin                                                    */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void martin::receiveParameters(const qreal* parameters)
{
    beta0=parameters[0];
    k0=parameters[1];
    g0=parameters[2];
    p0=parameters[3];
    theta=parameters[4];
    rho=parameters[5];
    length=(long)(parameters[6]);
}

qreal martin::savings(const qreal& x)
{
    qreal r=rho/(rho-1);
    return( 1/exp( cos(10/(1+exp(r*log(x))))));
}

void martin::iteration(const long&)
{
    qreal oldP=p;
    qreal term;
    qreal term2;
    oldBeta=beta;
 
//	beta=zvar->dice();
 
     
    term=theta*savings(k)/savings(beta);
    term2=g*term*term;
    
    p=theta*savings(k)/savings(beta)*p;
	
    beta = beta+ g * (term-beta);
    k=oldBeta;
    g=term2/(1+term2);

    perr=theta*savings(k)/savings(beta)-k;
    perrp=p-oldBeta*oldP;
    if( (p>0) && (oldBeta*oldP>0) ) {
	perrlnp=log(p)-log(oldBeta*oldP);
    } else {
    	perrlnp=0;
    }
}


/* 22222222222222222222222222222222222222222222222222222222222222222222222222 */
/*                                                                            */
/* Class name:      martin2                                                   */
/* Member function: setLabels                                                 */
/* Purpose:         return a pointer to a variable or a parameter specified   */
/*                  by its name                                               */
/* Last modified:   13.06.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal* martin2::setLabels(char* label)
{
    if( !strcmp(label,"xBundle") )
	return &xBundle;
    if( !strcmp(label,"yBundle") )
	return &yBundle;
    if( !strcmp(label,"theta") )
	return( &theta);
    if( !strcmp(label,"rho") )
	return( &rho);
    if( !strcmp(label,"lambda") )
	return( &lambda );
    if( !strcmp(label,"then") )
//    if( !strcmp(label,"beta") )
	return( &beta);
//    if( !strcmp(label,"beta_old") )
    if( !strcmp(label,"theo") )
	return( &oldBeta);
    if( !strcmp(label,"k") )
	return( &k);
    if( !strcmp(label,"g") )
	return( &g);
    if( !strcmp(label,"perr") ) 
	return( &perr );
    if( !strcmp(label,"beta0") )
	return( &beta0 );
    if( !strcmp(label,"k0") )
	return( &k0);
    if( !strcmp(label,"g0") )
	return( &g0 );

    return NULL;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      martin2                                                   */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   Thu May 30 17:12:40 MESZ 1996 (Uli Middelberg)            */
/*                                                                            */
/******************************************************************************/

void martin2::loadParamset(ifstream& inFile)
{
    inFile >> beta0 >> k0 >> g0 >> p0;
    inFile >> theta >> rho >> lambda;
    inFile >> length;
    
    initialize();
}


/******************************************************************************/
/*                                                                            */
/* Class name:      martin2                                                   */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   09.06.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void martin2::saveParamset(ofstream& outFile)
{
    outFile << beta0 << "\t" << k0 << "\t" << g0 << "\t" << p0 << "\t";
    outFile << theta  << "\t" << rho << "\t" << lambda << "\t";
    outFile << length;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      martin2                                                   */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   09.06.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void martin2::printParamset()
{
    log() << beta0 << "\t" << k0 << "\t" << g0 << "\n";
    log() << theta << "\t" << rho << "\t" << lambda << "\n";
    log() << length << "\n";
}


/******************************************************************************/
/*                                                                            */
/* Class name:      martin2                                                   */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void martin2::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=8;
    *parameters=new qreal[amount];
    if( !parameters )
	fatalError("martin::sendParameters",
		   "Can't create array for parameters");
    (*parameters[0])=beta0;
    (*parameters[1])=k0;
    (*parameters[2])=g0;
    (*parameters[3])=p0;
    (*parameters[4])=theta;
    (*parameters[5])=rho;
    (*parameters[6])=lambda;
    (*parameters[7])=length;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      martin2                                                   */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void martin2::receiveParameters(const qreal* parameters)
{
    beta0=parameters[0];
    k0=parameters[1];
    g0=parameters[2];
    p0=parameters[3];
    theta=parameters[4];
    rho=parameters[5];
    lambda=parameters[6];
    length=(long)(parameters[7]);
}


qreal martin2::savings(const qreal& x)
{
    return( 1-lambda*x);
}


qreal martin3::savings(const qreal& x)
{
    qreal r=rho/(rho-1);

    return( 2-(2+2*lambda*x)/(1+exp(r*log(x))) );
}


/* 44444444444444444444444444444444444444444444444444444444444444444444444444 */
/*                                                                            */
/* Class name:      martin4                                                   */
/* Member function: setLabels                                                 */
/* Purpose:         return a pointer to a variable or a parameter specified   */
/*                  by its name                                               */
/* Last modified:   Tue May 14 15:40:49 MESZ 1996 (Uli Middelberg)            */
/*                                                                            */
/******************************************************************************/

qreal* martin4::setLabels(char* label)
{
    if( !strcmp(label,"xBundle") )
	return &xBundle;
    if( !strcmp(label,"yBundle") )
	return &yBundle;
    if( !strcmp(label,"theta") )
	return( &theta);
    if( !strcmp(label,"rho") )
	return( &rho);
    if( !strcmp(label,"lambda") )
	return( &lambda );
     if( !strcmp(label,"then") )
//    if( !strcmp(label,"beta") )
	return( &beta);
//    if( !strcmp(label,"beta_old") )
    if( !strcmp(label,"theo") )
	return( &oldBeta);
    if( !strcmp(label,"k") )
	return( &k);
    if( !strcmp(label,"g") )
	return( &g);
    if( !strcmp(label,"perr") ) 
	return( &perr );
    if( !strcmp(label,"beta0") )
	return( &beta0 );
    if( !strcmp(label,"k0") )
	return( &k0);
    if( !strcmp(label,"g0") )
	return( &g0 );
    if( !strcmp(label,"a") )
	return( &a );
    if( !strcmp(label,"b") )
	return( &b );
    if( !strcmp(label,"c") )
	return( &c );

    return NULL;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      martin4                                                   */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   Thu May 30 17:12:40 MESZ 1996 (Uli Middelberg)            */
/*                                                                            */
/******************************************************************************/

void martin4::loadParamset(ifstream& inFile)
{
    inFile >> beta0 >> k0 >> g0 >> p0;
    inFile >> theta >> rho >> lambda;
    inFile >> a >> b >> c;
    inFile >> length;
    
    initialize();
}


/******************************************************************************/
/*                                                                            */
/* Class name:      martin4                                                   */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   Thu May 30 17:12:40 MESZ 1996 (Uli Middelberg)            */
/*                                                                            */
/******************************************************************************/

void martin4::saveParamset(ofstream& outFile)
{
    outFile << beta0 << "\t" << k0 << "\t" << g0 << "\t" << p0 << "\t";
    outFile << theta  << "\t" << rho << "\t" << lambda << "\t";
    outFile << a  << "\t" << b << "\t" << c << "\t";
    outFile << length;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      martin4                                                   */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   Thu May 30 17:12:40 MESZ 1996 (Uli Middelberg)            */
/*                                                                            */
/******************************************************************************/

void martin4::printParamset()
{
    log() << beta0 << "\t" << k0 << "\t" << g0 << "\n";
    log() << theta << "\t" << rho << "\t" << lambda << "\n";
    log() << a << "\t" << b << "\t" << c << "\n";
    log() << length << "\n";
}


/******************************************************************************/
/*                                                                            */
/* Class name:      martin4                                                   */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   Thu May 30 17:12:40 MESZ 1996 (Uli Middelberg)            */
/*                                                                            */
/******************************************************************************/

void martin4::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=11;
    *parameters=new qreal[amount];
    if( !parameters )
	fatalError("martin::sendParameters",
		   "Can't create array for parameters");
    (*parameters[0])=beta0;
    (*parameters[1])=k0;
    (*parameters[2])=g0;
    (*parameters[3])=p0;
    (*parameters[4])=theta;
    (*parameters[5])=rho;
    (*parameters[6])=lambda;
    (*parameters[7])=a;
    (*parameters[8])=b;
    (*parameters[9])=c;    
    (*parameters[10])=length;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      martin4                                                   */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   Thu May 30 17:12:40 MESZ 1996 (Uli Middelberg)            */
/*                                                                            */
/******************************************************************************/

void martin4::receiveParameters(const qreal* parameters)
{
    beta0=parameters[0];
    k0=parameters[1];
    g0=parameters[2];
    p0=parameters[3];
    theta=parameters[4];
    rho=parameters[5];
    lambda=parameters[6];
    a=parameters[7];
    b=parameters[8];
    c=parameters[9];
    length=(long)(parameters[10]);
}


qreal martin4::savings(const qreal& x)
{
    return( (1/x) - a * ((x-b)*(x-b)+c));
}

//eof
