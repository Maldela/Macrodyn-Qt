#include "milano.h"
#include "../error.h"

//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/milano.C,v 1.1 2000/08/31 16:06:10 mmeyer Exp $

/******************************************************************************/
/*                                                                            */
/* Class name:      milano                                                    */
/* Member function: milano                                                    */
/* Purpose:         constructor                                               */
/* Last modified:   14.06.1996 (Uli Middelberg)                               */
/*                                                                            */
/******************************************************************************/

milano::milano() : baseModel(3)
{
}

milano::~milano()
{
}


/******************************************************************************/
/*                                                                            */
/*   Function: virtual_Kemployment                                            */
/*   Purpose:  compute the employment in the keynesian regime  (main loop)    */
/*   Last modified: 31.05.1996 (Jorg Nikutta)                                */
/*                                                                            */
/******************************************************************************/

real milano::virtual_Kemployment()
{
    real K_emp;

    if (h*b<1)
	K_emp=b/(alfa*(1-h*b))*(h*(1-tax)*pgt+mreal+G);
    else
	K_emp=Ls+1;
    return K_emp;
}

/******************************************************************************/
/*                                                                            */
/*   Function: labour_demand                                                  */
/*   Input:                                                                   */
/*   Output:                                                                  */
/*   Purpose: compute the labour demand (main loop)                           */
/*   Last modified: 31.05.1996 (Jorg Nikutta)                               */
/*                                                                            */
/******************************************************************************/

real milano::labour_demand()
{
    real l_dem;
    
    l_dem=n1*exp(1/(b-1)*log(alfa/(a*b)));
    return l_dem;
}

/******************************************************************************/
/*                                                                            */
/*   Function: diseq_regime                                                   */
/*   Input:                                                                   */
/*   Output:                                                                  */
/*   Purpose: define the disequilibrium regimes (main loop)                   */
/*   Last modified: 07.02.1995                                                */
/*                                                                            */
/******************************************************************************/

void milano::diseq_regime(const real& K_emp, const real& l_dem)
{
    if (K_emp<=l_dem)
	if (K_emp<=Ls)
	    keynesian_unemployment(K_emp);
	else
	    repressed_inflation(l_dem);
    else
	if (l_dem<=Ls)
	    classical_unemployment(l_dem);
	else
	    repressed_inflation(l_dem);
}

/******************************************************************************/
/*                                                                            */
/*   Function: keynesian_unemployment                                         */
/*   Input:                                                                   */
/*   Output: employment,output,profit,wage,prices,money stock                 */
/*   Purpose: analysis of the keynesian unemployment regime                   */
/*   Last modified: 31.05.1996 (Jorg Nikutta)                                */
/*                                                                            */
/******************************************************************************/

void milano::keynesian_unemployment(const real& K_emp)
{
    real lambdas;
    real gammas;
    real oldpgt=pgt;
    real teta;

    empl=K_emp;
    output=alfa*empl/b;
    lambdas=empl/Ls;
    gammas=exp((1-b)*log(output/(n1*a)))*exp(b*log(alfa/(a*b)));
    mtr=mreal;
    wtr=alfa;
    teta=exp(mu1*log(gammas));
    pgt=(output-alfa*empl)/teta;
    mreal=(mreal+G+(1-tax)*oldpgt)/teta-pgt;
    alfa=(exp(nu1*log(lambdas))*alfa)/teta;
}

/******************************************************************************/
/*                                                                            */
/*   Function: repressed_inflation                                            */
/*   Input:                                                                   */
/*   Output: employment,output,profit,wage,prices,money stock                 */
/*   Purpose: analysis of the repressed inflation regime                      */
/*   Last modified: 31.05.1996 (Jorg Nikutta)                                */
/*                                                                            */
/******************************************************************************/

void milano::repressed_inflation (const real& l_dem)
{
    real lambdad;
    real gammad;
    real delta;
    real epsilon;
    real oldpgt=pgt;
    real teta;

    empl=Ls;
    output=alfa*empl/b;
    lambdad=Ls/l_dem;
    if (output<G){
	epsilon=output/G;
	delta=0;
	gammad=0;
    }
    else{
	
	epsilon=1;
	if (output<G+(mreal)){
	    delta=(output-G)/mreal;
	    gammad=0;
	}
	else{
	    delta=1;
	    gammad=(output-mreal-G)/(h*(1-tax)*pgt+h*alfa*empl);
	}
    }
    teta=exp(-mu2*log((gammad+epsilon+delta)/3.0));
    pgt=(output-alfa*empl)/teta;
    mtr=mreal;
    wtr=alfa;
    mreal=(delta*mreal+epsilon*G+(1-tax)*oldpgt)/teta-pgt;
    alfa=(exp(-nu2*log(lambdad))*alfa)/teta;
}

/******************************************************************************/
/*                                                                            */
/*   Function: classical_unemployment                                         */
/*   Input:                                                                   */
/*   Output: employment,output,profit,wage,prices,money stock                 */
/*   Purpose: analysis of the classical unemployment regime                   */
/*   Last modified:31.05.1996 Jorg Nikutta*/
/*                                                                            */
/******************************************************************************/

void milano::classical_unemployment(const real& l_dem)
{
    real lambdas;
    real gammad;
    real delta;
    real epsilon;
    real oldpgt=pgt;
    real teta;
    
    empl=l_dem;
    output=alfa*empl/b;
    lambdas=empl/Ls;
    if (output<G){
	epsilon=output/G;
	delta=0;
	gammad=0;
    }
    else{
	epsilon=1;
	if (output<G+(mreal)){
	    delta=(output-G)/mreal;
	    gammad=0;
	}
	else{
	    delta=1;
	    gammad=(output-mreal-G)/(h*(1-tax)*pgt+h*alfa*empl);
	}
    }
    teta=exp(-mu2*log((delta+epsilon+gammad)/3.0));
    pgt=(output-alfa*empl)/teta;
    mtr=mreal;
    wtr=alfa;
    mreal=(delta*mreal+epsilon*G+(1-tax)*oldpgt)/teta-pgt;
    alfa=(exp(nu1*log(lambdas))*alfa)/teta;
}

/******************************************************************************/
/*                                                                            */
/*   Function: iteration                                                      */
/*   Input:                                                                   */
/*   Output:                                                                  */
/*   Purpose: return to the main loop and continue the simulation             */
/*   Last modified:07.02.1995                                                 */
/*                                                                            */
/******************************************************************************/

void milano::iteration(const long& t)
{
    real K_emp;
    real l_dem;

    K_emp=virtual_Kemployment();
    l_dem=labour_demand();
    diseq_regime(K_emp,l_dem);
}



/* loadParamset Last modified: 31.05.1996 (Jorg Nikutta) */

void milano::loadParamset(ifstream& inFile)
{
    inFile >> a >> b;
    inFile >> mu1 >> mu2 >> nu1 >> nu2;
    inFile >> n1 >> simd;
    inFile >> h >> pg0 >> alfa0 >> mreal0;
    inFile >> Ls >> G >> tax;

    initialize();
}

/******************************************************************************/
/*                                                                            */
/*   Function:       printParamset                                            */
/*   Purpose:        print current parameterset                               */
/*   Last modified:  31.05.1996 (Jorg Nikutta)                              */
/*                                                                            */
/******************************************************************************/

void milano::printParamset()
{
    cout << "a     :  " << a <<"\talpha0 : " << alfa0 << endl;
    cout << "b     :  " << b <<"\tmreal0 : " << mreal0 << endl;
    cout << "n1    :  " << n1 << endl;
    cout << "mu1   :  " << mu1 << "\tmu2 : " << mu2 << endl;
    cout << "nu1   :  " << nu1 << "\tG   : " << G << endl;
    cout << "nu2   :  " << nu2 << "\ttax : " << tax << endl;
    cout << "h     :  " << h << "\tsimd: " << simd << endl;
    cout << "pg0   :  " << pg0 << "\tLs  : " << Ls << endl;
}

/******************************************************************************/
/*                                                                           */
/*   Function:       saveParamset                                            */
/*   Purpose:        write current parameterset to the disk                  */
/*   Last modified:  31.05.1996 (Jorg Nikutta)                             */
/*                                                                           */
/*****************************************************************************/
void milano::saveParamset(ofstream& outFile)
{
    outFile << a << " " << b << endl;
    outFile << mu1 << " " << mu2 << " " << nu1 << " " << nu2 << endl;
    outFile << n1 << " " << simd << endl;
    outFile << h << " " << pg0 << " " << alfa0 << " " << mreal0 << endl;
    outFile << Ls << " " << G << " " << tax << endl;
}

/******************************************************************************/
/*                                                                            */
/*   Function:       set_labels                                               */
/*   Input:          pointer to the name of the label                         */
/*   Return:         pointer to the corresponding variable                    */
/*   Purpose:        set pointer to the variable that shall be plotted on the */
/*                   label                                                    */
/*   Last modified:  31.05.1996 (Jorg Nikutta)                                */
/*                                                                            */
/******************************************************************************/

real* milano::setLabels(char* name)
{
    if( !strcmp(name,"G") )
	return( &G );
    if( !strcmp(name,"empl") )
	return( &empl );
    if( !strcmp(name,"output") )
	return( &output );
    if( !strcmp(name,"a") )
	return( &a );
    if( !strcmp(name,"b") )
	return( &b );
    if( !strcmp(name,"alfa") )
	return( &alfa );
    if( !strcmp(name,"mreal") )
	return( &mreal );
    if( !strcmp(name,"pgt") )
	return( &pgt );
    if( !strcmp(name,"mtr") ) 
	return( &mtr );
    if( !strcmp(name,"wtr") )
	return( &wtr );
    if( !strcmp(name,"mu1") )
	return( &mu1 );
    if( !strcmp(name,"mu2") )
	return( &mu2 );
    if( !strcmp(name,"nu1") )
	return( &nu1 );
    if( !strcmp(name,"nu2") )
	return( &nu2 );
    if( !strcmp(name,"h") )
	return( &h );
    if( !strcmp(name,"tax") )
	return( &tax );
    if( !strcmp(name,"Ls") )
	return( &Ls );
    if( !strcmp(name,"alfa0") )
	return( &alfa0 );
    if( !strcmp(name,"mreal0") )
	return( &mreal0 );
    if( !strcmp(name,"pg0") )
	return( &pg0 );
    if( !strcmp(name,"n1") )
	return( (real*) &n1 );
    return( NULL );
}

void milano::initialize()
{
    alfa=alfa0;
    pgt=pg0;
    mreal=mreal0;
}


real* milano::sendModelVar()
{
    return &alfa; // return the main model variable
}

void milano::sendStateSpace(int &quantity,const real*** stateSpace)
{
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const real* [dimension];
    if( !(*stateSpace) )
	fatalError("milano::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&mreal;
    (*stateSpace)[1]=&alfa;
/*    (*stateSpace)[2]=&pt;*/
    (*stateSpace)[2]=&pgt;
}


// return pointers to all model
					    // variables and the dimension
					    // of the model
void milano::sendParameters(int&,real**)
{
} // write all parameters
                                // into an array and return the numbers
				// of parameters
void milano::receiveParameters(const real*)
{
}
// receive parameter values 


