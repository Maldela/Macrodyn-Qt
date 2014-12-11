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

qreal milano::virtual_Kemployment()
{
    qreal K_emp;

    if (h*b<1)
	K_emp=b/(alfa*(1-h*b))*(h*(1-tax)*pgt+mqreal+G);
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

qreal milano::labour_demand()
{
    qreal l_dem;
    
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

void milano::diseq_regime(const qreal& K_emp, const qreal& l_dem)
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

void milano::keynesian_unemployment(const qreal& K_emp)
{
    qreal lambdas;
    qreal gammas;
    qreal oldpgt=pgt;
    qreal teta;

    empl=K_emp;
    output=alfa*empl/b;
    lambdas=empl/Ls;
    gammas=exp((1-b)*log(output/(n1*a)))*exp(b*log(alfa/(a*b)));
    mtr=mqreal;
    wtr=alfa;
    teta=exp(mu1*log(gammas));
    pgt=(output-alfa*empl)/teta;
    mqreal=(mqreal+G+(1-tax)*oldpgt)/teta-pgt;
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

void milano::repressed_inflation (const qreal& l_dem)
{
    qreal lambdad;
    qreal gammad;
    qreal delta;
    qreal epsilon;
    qreal oldpgt=pgt;
    qreal teta;

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
	if (output<G+(mqreal)){
	    delta=(output-G)/mqreal;
	    gammad=0;
	}
	else{
	    delta=1;
	    gammad=(output-mqreal-G)/(h*(1-tax)*pgt+h*alfa*empl);
	}
    }
    teta=exp(-mu2*log((gammad+epsilon+delta)/3.0));
    pgt=(output-alfa*empl)/teta;
    mtr=mqreal;
    wtr=alfa;
    mqreal=(delta*mqreal+epsilon*G+(1-tax)*oldpgt)/teta-pgt;
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

void milano::classical_unemployment(const qreal& l_dem)
{
    qreal lambdas;
    qreal gammad;
    qreal delta;
    qreal epsilon;
    qreal oldpgt=pgt;
    qreal teta;
    
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
	if (output<G+(mqreal)){
	    delta=(output-G)/mqreal;
	    gammad=0;
	}
	else{
	    delta=1;
	    gammad=(output-mqreal-G)/(h*(1-tax)*pgt+h*alfa*empl);
	}
    }
    teta=exp(-mu2*log((delta+epsilon+gammad)/3.0));
    pgt=(output-alfa*empl)/teta;
    mtr=mqreal;
    wtr=alfa;
    mqreal=(delta*mqreal+epsilon*G+(1-tax)*oldpgt)/teta-pgt;
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

void milano::iteration(const qint64& t)
{
    qreal K_emp;
    qreal l_dem;

    K_emp=virtual_Kemployment();
    l_dem=labour_demand();
    diseq_regime(K_emp,l_dem);
}



/* loadParamset Last modified: 31.05.1996 (Jorg Nikutta) */

void milano::loadParamset(QTextStream& inFile)
{
    inFile >> a >> b;
    inFile >> mu1 >> mu2 >> nu1 >> nu2;
    inFile >> n1 >> simd;
    inFile >> h >> pg0 >> alfa0 >> mqreal0;
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
    log() << "a     :  " << a <<"\talpha0 : " << alfa0;
    log() << "b     :  " << b <<"\tmqreal0 : " << mqreal0;
    log() << "n1    :  " << n1;
    log() << "mu1   :  " << mu1 << "\tmu2 : " << mu2;
    log() << "nu1   :  " << nu1 << "\tG   : " << G;
    log() << "nu2   :  " << nu2 << "\ttax : " << tax;
    log() << "h     :  " << h << "\tsimd: " << simd;
    log() << "pg0   :  " << pg0 << "\tLs  : " << Ls;
}

/******************************************************************************/
/*                                                                           */
/*   Function:       saveParamset                                            */
/*   Purpose:        write current parameterset to the disk                  */
/*   Last modified:  31.05.1996 (Jorg Nikutta)                             */
/*                                                                           */
/*****************************************************************************/
void milano::saveParamset(QTextStream& outFile)
{
    outFile << a << " " << b;
    outFile << mu1 << " " << mu2 << " " << nu1 << " " << nu2;
    outFile << n1 << " " << simd;
    outFile << h << " " << pg0 << " " << alfa0 << " " << mqreal0;
    outFile << Ls << " " << G << " " << tax;
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

qreal* milano::setLabels(const QString& name)
{
    if (name == "G")
	return( &G );
    if (name == "empl")
	return( &empl );
    if (name == "output")
	return( &output );
    if (name == "a")
	return( &a );
    if (name == "b")
	return( &b );
    if (name == "alfa")
	return( &alfa );
    if (name == "mqreal")
	return( &mqreal );
    if (name == "pgt")
	return( &pgt );
    if (name == "mtr") 
	return( &mtr );
    if (name == "wtr")
	return( &wtr );
    if (name == "mu1")
	return( &mu1 );
    if (name == "mu2")
	return( &mu2 );
    if (name == "nu1")
	return( &nu1 );
    if (name == "nu2")
	return( &nu2 );
    if (name == "h")
	return( &h );
    if (name == "tax")
	return( &tax );
    if (name == "Ls")
	return( &Ls );
    if (name == "alfa0")
	return( &alfa0 );
    if (name == "mqreal0")
	return( &mqreal0 );
    if (name == "pg0")
	return( &pg0 );
    if (name == "n1")
	return( (qreal*) &n1 );
    return( NULL );
}

void milano::initialize()
{
    alfa=alfa0;
    pgt=pg0;
    mqreal=mqreal0;
}


qreal* milano::sendModelVar()
{
    return &alfa; // return the main model variable
}

void milano::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("milano::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&mqreal;
    (*stateSpace)[1]=&alfa;
/*    (*stateSpace)[2]=&pt;*/
    (*stateSpace)[2]=&pgt;
}


// return pointers to all model
					    // variables and the dimension
					    // of the model
void milano::sendParameters(int&,qreal**)
{
} // write all parameters
                                // into an array and return the numbers
				// of parameters
void milano::receiveParameters(const QList<qreal> &parameters)
{
    if (parameters.size() != 15) log() << "Wrong number of parameters!";
    else
    {
        a = parameters.at(0);
        b = parameters.at(1);
        mu1 = parameters.at(2);
        mu2 = parameters.at(3);
        nu1 = parameters.at(4);
        nu2 = parameters.at(5);
        n1 = parameters.at(6);
        simd = parameters.at(7);
        h = parameters.at(8);
        pg0 = parameters.at(9);
        alfa0 = parameters.at(10);
        mqreal0 = parameters.at(11);
        Ls = parameters.at(12);
        G = parameters.at(13);
        tax = parameters.at(14);

        initialize();
    }
}
// receive parameter values 


