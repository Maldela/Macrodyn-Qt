// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/Monopol.C,v 1.1 2000/08/31 15:22:01 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:     Monopol.C                                                 */
/* Contents:        Member functions of the class Monopol                     */
/*                                                                            */
/* Last modified:   Thu	Sep 19 09:00:00 MESZ 1996 (Leo Kaas)                  */
/*                                                                            */
/******************************************************************************/

#include "Monopol.h"
//extern void fatalError(const char*, const char*);
#include "../error.h"


/******************************************************************************/
/*                                                                            */
/* Class name:      Monopol                                                   */
/* Member function: Monopol                                                   */
/* Purpose:         constructor / destructor                                  */
/* Last modified:   Thu	Sep 19 09:00:00 MESZ 1996 (Leo Kaas)                  */
/*                                                                            */
/******************************************************************************/

Monopol::Monopol() : baseModel(4)
{
}

Monopol::~Monopol()
{
}


/******************************************************************************/
/*                                                                            */
/* Class name:      Monopol                                                   */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, define the systems initial state    */
/* Last modified:   Thu	Sep 19 09:00:00 MESZ 1996 (Leo Kaas)                  */
/*                                                                            */
/******************************************************************************/

void Monopol::initialize()
{
    zeta=pow(beta0,1/eta)/w0;
    m=M0/w0;
    z=z0;		
}
    

/******************************************************************************/
/*                                                                            */
/* Class name:      Monopol                                                   */
/* Member function: sendModelVar                                              */
/* Purpose:         return a pointer to the main model variable               */
/* Last modified:   Thu	Sep 19 09:00:00 MESZ 1996 (Leo Kaas)                  */
/*                                                                            */
/******************************************************************************/


qreal* Monopol::sendModelVar()
{
    return &m;
}

 

/******************************************************************************/
/*                                                                            */
/* Class name:      Monopol                                                   */
/* Member function: setLabels                                                 */
/* Purpose:         return a pointer to a variable or a parameter specified   */
/*                  by its name                                               */
/* Last modified:   Thu	Sep 19 09:00:00 MESZ 1996 (Leo Kaas)                  */
/*                                                                            */
/******************************************************************************/

qreal* Monopol::setLabels(char* label)
{
    if( !strcmp(label,"xBundle") )
	return &xBundle;
    if( !strcmp(label,"yBundle") )
	return &yBundle;
    if( !strcmp(label,"g") )
	return( &g);
    if( !strcmp(label,"tau") )
	return( &tau);
    if( !strcmp(label,"B") )
	return( &B);
    if( !strcmp(label,"eta") )
	return( &eta);
    if( !strcmp(label,"alpha") )
	return( &alpha);
    if( !strcmp(label,"Lmax") )
	return( &Lmax);
    if( !strcmp(label,"lambda") )
	return( &lambda);
    if( !strcmp(label,"mu") )
	return( &mu);			
    if( !strcmp(label,"delta") )
	return( &delta);
    if( !strcmp(label,"zeta") )
	return( &zeta);
    if( !strcmp(label,"m") )
	return( &m);	
    if( !strcmp(label,"z") )
	return( &z);	
    if( !strcmp(label,"beta0") )
	return( &beta0);
    if( !strcmp(label,"M0") )
	return( &M0);
    if( !strcmp(label,"w0") ) 
	return( &w0 );
    if( !strcmp(label,"z0") ) 
	return( &z0 );

    return NULL;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      Monopol                                                   */
/* Member function: sendStateSpace                                            */
/* Purpose:         return pointers to the state variables and inform about   */
/*                  the systems dimension                                     */
/* Last modified:   Thu	Sep 19 09:00:00 MESZ 1996 (Leo Kaas)                  */
/*                                                                            */
/******************************************************************************/

void Monopol::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !stateSpace )
	fatalError("Monopol::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&zeta;
    (*stateSpace)[1]=&m;
    (*stateSpace)[2]=&z;
};


/******************************************************************************/
/*                                                                            */
/* Class name:      Monopol                                                   */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   Thu	Sep 19 09:00:00 MESZ 1996 (Leo Kaas)                  */
/*                                                                            */
/******************************************************************************/

void Monopol::loadParamset(ifstream& inFile)
{
    inFile >> beta0 >> M0 >> w0 >> z0;
    inFile >> g >> tau >> B; 
    inFile >> eta >> alpha >> Lmax;
    inFile >> lambda >> mu >> delta;
    inFile >> cert >> length;
    
    initialize();
}


/******************************************************************************/
/*                                                                            */
/* Class name:      Monopol                                                   */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   Thu	Sep 19 09:00:00 MESZ 1996 (Leo Kaas)                  */
/*                                                                            */
/******************************************************************************/

void Monopol::saveParamset(ofstream& outFile)
{
    outFile << beta0 << "\t" << M0 << "\t" << w0 << "\t";
    outFile << z0 << "\t" << g << "\t" << tau << "\t";
    outFile << B << "\t" << eta << "\t" << alpha << "\t";    
    outFile << Lmax << "\t" << lambda << "\t" << mu << "\t";
    outFile << delta  << "\t" << cert << "\t";
    outFile << length;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      Monopol                                                   */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   Thu	Sep 19 09:00:00 MESZ 1996 (Leo Kaas)                  */
/*                                                                            */
/******************************************************************************/

void Monopol::printParamset()
{
    log() << beta0 << "\t" << M0 << "\t" << w0 << "\n";
    log() << z0 << "\t" << g << "\t" << tau << "\n";
    log() << B << "\t" << eta << "\t" << alpha << "\n";
    log() << Lmax << "\t" << lambda << "\t" << mu << "\n";        
    log() << delta << "\t" << cert << "\n";
    log() << length << "\n";
}


/******************************************************************************/
/*                                                                            */
/* Class name:      Monopol                                                   */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   Thu	Sep 19 09:00:00 MESZ 1996 (Leo Kaas)                  */
/*                                                                            */
/******************************************************************************/

void Monopol::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=14;
    *parameters=new qreal[amount];
    if( !parameters )
	fatalError("Monopol::sendParameters",
		   "Can't create array for parameters");
    (*parameters[0])=beta0;
    (*parameters[1])=M0;
    (*parameters[2])=w0;
    (*parameters[3])=z0;
    (*parameters[4])=g;
    (*parameters[5])=tau;
    (*parameters[6])=B;
    (*parameters[7])=eta;
    (*parameters[8])=alpha;
    (*parameters[9])=Lmax;
    (*parameters[10])=lambda;
    (*parameters[11])=mu;
    (*parameters[12])=delta;
    (*parameters[13])=cert;
    (*parameters[14])=length;
    
}


/******************************************************************************/
/*                                                                            */
/* Class name:      Monopol                                                   */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   Thu	Sep 19 09:00:00 MESZ 1996 (Leo Kaas)                  */
/*                                                                            */
/******************************************************************************/

void Monopol::receiveParameters(const qreal* parameters)
{
    beta0=parameters[0];
    M0=parameters[1];
    w0=parameters[2];
    z0=parameters[3];
    g=parameters[4];
    tau=parameters[5];
    B=parameters[6];
    eta=parameters[7];
    alpha=parameters[8];
    Lmax=parameters[9];
    lambda=parameters[10];
    mu=parameters[11];
    delta=parameters[12];
    cert=parameters[13];
    length=(long)(parameters[14]);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      Monopol                                                   */
/* Member function: FF                                                        */
/* Purpose:         production function, computes the output for a given      */
/*                  employment level                                          */
/* Last modified:   19.09.1996 (Leo Kaas)                                     */
/*                                                                            */
/******************************************************************************/

qreal Monopol::FF(const qreal L)
{
	qreal result;

	result= 1/B * exp( B*log(L) );              
	return(result);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      Monopol                                                   */
/* Member function: Finv                                                      */
/* Purpose:         inverse production function, computes the employment      */
/*                  level needed to produce a given output                    */
/* Last modified:   19.09.1996 (Leo Kaas)                                     */
/*                                                                            */
/******************************************************************************/

qreal Monopol::Finv(const qreal y)
{
	qreal result;

	result= exp( 1/B*log(B*y) );              
	return(result);
}


/******************************************************************************/
/*                                                                            */
/* Class name:      Monopol                                                   */
/* Member function: gamma                                                     */
/* Purpose:         computes parameter gamma for the solution of the          */
/*                  monopolist's problem                                      */
/* Last modified:   19.09.1996 (Leo Kaas)                                     */
/*                                                                            */
/******************************************************************************/

qreal Monopol::gamma(void)
{
	qreal result;
	if(cert == 1)
		{
		result = 1/(alpha-1);
		}
	else
		{
		result = 1/(eta-1);
		}              
	return(result);
}


/******************************************************************************/
/*                                                                            */
/* Class name:      Monopol                                                   */
/* Member function: cpar                                                      */
/* Purpose:         computes parameter cpar for the solution of the           */
/*                  monopolist's problem                                      */
/* Last modified:   19.09.1996 (Leo Kaas)                                     */
/*                                                                            */
/******************************************************************************/

qreal Monopol::cpar(void)
{
	qreal result;
	qreal gam;
	
	gam=gamma();	
	if(cert == 1)
		{
		result = pow(gam,1/eta);
		}
	else
		{
		result =
		exp((1-eta)/eta*log(gam))*(1/(alpha-1)+exp(alpha*log(1/(1+gam))
	          )*(1+alpha*gam)/(1-alpha))+exp(log(gam)/eta)*exp(alpha*log(
	          1/(1+gam))); 
		}              
		             
	return(result);
}


/******************************************************************************/
/*                                                                            */
/* Class name:      Monopol                                                   */
/* Member function: xnot                                                      */
/* Purpose:         computes the notional production level of the             */
/*                  monopolist                                                */
/* Last modified:   19.09.1996 (Leo Kaas)                                     */
/*                                                                            */
/******************************************************************************/

qreal Monopol::xnot(void)
{
	qreal result;
	qreal cpa;
	cpa = cpar();
	result = exp(B*eta/(eta+B-B*eta)*log((1-1/eta)*zeta*
		  cpa*exp((1-1/B)*log(B))));              
	return(result);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      Monopol                                                   */
/* Member function: pmon                                                      */
/* Purpose:         computes the monopolist's price at given output level     */
/*                                                                            */
/* Last modified:   19.09.1996 (Leo Kaas)                                     */
/*                                                                            */
/******************************************************************************/

qreal Monopol::rewa(const qreal x)
{
	qreal result;
	qreal gam;
	gam=gamma();
	result = pow(x/gam,1/eta)/zeta;              
	return(result);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      Monopol                                                   */
/* Member function: iteration                                                 */
/* Purpose:         perforn one iteration                                     */
/* Last modified:   Thu	Sep 19 09:00:00 MESZ 1996 (Leo Kaas)                  */
/*                                                                            */
/******************************************************************************/


void Monopol::iteration(const long&)
{
    qreal Ltdem;          //  notional labor demand
    qreal Lt;		 //  actual employment level
    qreal rewat;		 //  monopolist's price
    qreal ytdem;		 //  goods demand
    qreal xt;             //  goods trade
    qreal gam;
    
    gam=gamma();
    Ltdem= Finv(MAX(0,xnot()-z));
    Lt=MIN(Lmax,Ltdem);
    rewat=rewa(FF(Lt)+z);
    ytdem=m*rewat+g;
    xt=MIN(ytdem,FF(Lt)+z);
       
    zeta=zeta*pow(((alpha-1)/lambda/alpha*(1+gam*ytdem/(FF(Lt)+z))),1/eta)
    		/(1+mu*(Ltdem-Lmax)/MAX(Ltdem,Lmax));
    m= (m+(g-tau*xt)/rewat)/(1+mu*(Ltdem-Lmax)/MAX(Ltdem,Lmax));
    z=MAX(0,delta*(FF(Lt)+z-ytdem));
}




//eof
