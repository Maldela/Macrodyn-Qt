// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/capOlg.C,v 1.1 2000/08/31 15:24:02 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  capOlg.C                                                */
/* Contents:     Member functions of the class capOlg                    */
/*                                                                            */
/* Last Modified: 04.07.1995 (Markus Lohmann)                                 */
/*                                                                            */
/******************************************************************************/

#include "capOlg.h"             
#include "../error.h"

//extern void fatalError(const char*, const char*);

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlg                                               */
/* Member function: capOlg                                               */
/* Purpose:         constructor                                               */
/* Last modified:   04.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

capOlg::capOlg() : baseModel(2)
{
    rVec=NULL;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlg                                               */
/* Member function: ~capOlg                                              */
/* Purpose:         destructor                                                */
/* Last modified:   04.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

capOlg::~capOlg()
{
    if( rVec )
	delete rVec;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      capOlg                                               */
/* Member function: prodFunction                                              */
/* Purpose:         production function, computes the output for a given      */
/*                  capital stock                                             */
/* Last modified:   04.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal capOlg::prodFunction(const qreal& k)
{
	qreal result;

	result= A/B * exp( (1-B)*log(k) );              
	return(result);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlg                                               */
/* Member function: rInit                                                     */
/* Purpose:         initialize the r vector                                   */
/* Last modified:   04.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void capOlg::rInit(qreal *rVec)
{
    int i;

    for( i=1; i<=tau+1; i++ )
        rVec[i] = 0.0;
    rVec[0]=1.0;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlg                                               */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, set the initial state               */
/* Last modified:   04.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void capOlg::initialize()
{
    rInit(rVec);
    kt=k0;
    rt=r0;
    rhoTilda=1-rho/(1-rho);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlg                                               */
/* Member function: expReturnRate                                             */
/* Purpose:         computes the expected return rate                         */
/* Last modified:   04.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal capOlg::expReturnRate(const long t)
{
    long T,counter;
    qreal help=0.0;
    
    T=MIN(t,tau);
    for( counter=0; counter < T; counter++ )
	help += rVec[counter];
    return( help / T );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlg                                               */
/* Member function: savingsFunc                                               */
/* Purpose:         describes the savings function                            */
/* Last modified:   04.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal capOlg::savingsFunc(qreal& rE)
{
    qreal deltaTerm=exp(1/(1-rho)*log(delta));
    qreal rTerm=exp(rho/(1-rho)*log(rE));
    
    return( deltaTerm*rTerm/(1+deltaTerm*rTerm) );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlg                                               */
/* Member function: rShift                                                    */
/* Purpose:         describes the shift operator on the past tau rs           */
/* Last modified:   04.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void capOlg::rShift()
{
    for(int i=0; i<=tau; i++ )
	rVec[tau+1-i]=rVec[tau-i];
    rVec[0]=rt;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlg                                               */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system                */
/* Last modified:   04.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void capOlg::iteration(const long& t)
{
    qreal rE;
    qreal term;

    rE=expReturnRate(t);
    wt=prodFunction(kt);
    st=savingsFunc(rE)*wt;

    term=st+(1-d)*kt;
    kt= term/(1+n);
    rt=(1-B)*A*(1+n)/(B*st)*exp((1-B)*log( term/(1+n)));
    rShift();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlg                                              */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:   04.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal* capOlg::setLabels(char *name)
{
    if( !strcmp(name,"xBundle") )
	return &xBundle;
    if( !strcmp(name,"yBundle") )
	return &yBundle;
    if( !strcmp(name,"n") )
        return( &n );
    if( !strcmp(name,"d") )
        return( &d );
    if( !strcmp(name,"A") )
        return( &A );
    if( !strcmp(name,"B") )
        return( &B );
    if( !strcmp(name,"wt") )
        return( &wt );
    if( !strcmp(name,"rt") )
        return( &rt);
    if( !strcmp(name,"kt") )
        return( &kt );
    if( !strcmp(name,"delta") )
        return( &delta );
    if( !strcmp(name,"rho") )
        return( &rho );
    if( !strcmp(name,"k0") )
	return( &k0 );
    if( !strcmp(name,"r0") )
	return( &r0 );
    if( !strcmp(name,"tau") )
	return( (qreal*)(&tau) );
    return( NULL );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlg                                              */
/* Member function: sendModelVar                                              */
/* Purpose:         returns a pointer to the qreal wage, the main model var.   */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal* capOlg::sendModelVar()
{
    return &kt;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlg                                              */
/* Member function: sendStateSpace                                            */
/* Purpose:         returns pointers to the qreal balances and the qreal wage;  */
/*                  returns the dimension of the system for rho=0             */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void capOlg::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("capOlg::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&kt;
    (*stateSpace)[1]=&rt;
}
    
/******************************************************************************/
/*                                                                            */
/* Class name:      capOlg                                              */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   07.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void capOlg::loadParamset(ifstream& inputFile)
{
    inputFile >> A >> B >> d;
    inputFile >> n >> delta >> rho;
    inputFile >> tau >> length;
    inputFile >> k0 >> r0;

    if( rVec )
	delete rVec;
    rVec = new qreal[tau+2];
    if( !rVec )
	fatalError("capOlg::loadParamset","Can't create r vector");
    
    initialize();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlg                                              */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   04.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void capOlg::saveParamset(ofstream& outputFile)
{
    outputFile << A << "\t" << B << "\t" << d << "\n";
    outputFile << n << "\t" << delta << "\t" << rho << "\n";
    outputFile << tau << "\t" << length << "\n";
    outputFile << k0 << "\t" << r0 << "\n";
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlg                                              */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   04.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void capOlg::printParamset()
{
    log() << A << "\t" << B <<  "\t" << d << "\n";
    log() << n << "\t" << delta << "\t" << rho << "\n";
    log() << tau << "\t" << length << "\n";
    log() << k0 << "\t" << r0 << "\n";
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlg                                              */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void capOlg::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=10;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("capOlg::sendParameters",
		   "Can't create array for parameters");
    (*parameters)[0]=A;
    (*parameters)[1]=B;
    (*parameters)[2]=d;
    (*parameters)[3]=n;
    (*parameters)[4]=delta;
    (*parameters)[5]=rho;
    (*parameters)[6]=tau;
    (*parameters)[7]=length;
    (*parameters)[8]=k0;
    (*parameters)[9]=r0;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlg                                               */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void capOlg::receiveParameters(const qreal* parameters)
{
    A=parameters[0];
    B=parameters[1];
    d=parameters[2];
    n=parameters[3];
    delta=parameters[4];
    rho=parameters[5];
    tau=(int)(parameters[6]);
    length=(long)(parameters[7]);
    k0=parameters[8];
    r0=parameters[9];
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlgAdapt                                               */
/* Member function: capOlgAdapt                                               */
/* Purpose:         constructor                                               */
/* Last modified:   31.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

capOlgAdapt::capOlgAdapt()					 
{
    oldExpectations=1.0;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlgAdapt                                               */
/* Member function: expectedInflationRate                                     */
/* Purpose:         computes the expected inflation rate                      */
/* Last modified:   31.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal capOlgAdapt::expectedReturnRate(const long)
{
    qreal expReturnRate;

    expReturnRate=eta*oldExpectations + (1-eta)*rVec[0];
    oldExpectations=expReturnRate;

    return( expReturnRate );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlgAdapt                                               */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system                */
/* Last modified:   04.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void capOlgAdapt::iteration(const long& t)
{
    qreal rE;
    qreal term;

    rE=expReturnRate(t);
    wt=prodFunction(kt);
    st=savingsFunc(rE)*wt;

    term=st+(1-d)*kt;
    kt= term/(1+n);
    rt=(1-B)*A*(1+n)/(B*st)*exp((1-B)*log( term/(1+n)));
    rShift();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlgAdapt                                               */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:   31.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal* capOlgAdapt::setLabels(char *name)
{
    if( !strcmp(name,"xBundle") )
	return &xBundle;
    if( !strcmp(name,"yBundle") )
	return &yBundle;
    if( !strcmp(name,"n") )
        return( &n );
    if( !strcmp(name,"d") )
        return( &d );
    if( !strcmp(name,"A") )
        return( &A );
    if( !strcmp(name,"B") )
        return( &B );
    if( !strcmp(name,"wt") )
        return( &wt );
    if( !strcmp(name,"rt") )
        return( &rt);
    if( !strcmp(name,"kt") )
        return( &kt );
    if( !strcmp(name,"delta") )
        return( &delta );
    if( !strcmp(name,"rho") )
        return( &rho );
    if( !strcmp(name,"k0") )
	return( &k0 );
    if( !strcmp(name,"r0") )
	return( &r0 );
    if( !strcmp(name,"tau") )
	return( (qreal*)(&tau) );
    if( !strcmp(name,"eta") ) 
	return( &eta );

    return( NULL );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlgAdapt                                               */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   07.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void capOlgAdapt::loadParamset(ifstream& inputFile)
{
    inputFile >> A >> B >> d;
    inputFile >> n >> delta >> rho;
    inputFile >> tau >> eta >> length;
    inputFile >> k0 >> r0;

    if( rVec )
	delete rVec;
    rVec = new qreal[tau+2];
    if( !rVec )
	fatalError("capOlgAdapt::loadParamset","Can't create r vector");
    
    initialize();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlgAdapt                                               */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   31.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void capOlgAdapt::saveParamset(ofstream& outputFile)
{
    outputFile << A << "\t" << B << "\t" << d << "\n";
    outputFile << n << "\t" << delta << "\t" << rho << "\n";
    outputFile << tau << "\t" << eta << "\t" << length << "\n";
    outputFile << k0 << "\t" << r0 << "\n";
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlgAdapt                                               */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   31.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void capOlgAdapt::printParamset()
{
    log() << A << "\t" << B <<  "\t" << d << "\n";
    log() << n << "\t" << delta << "\t" << rho << "\n";
    log() << tau << "\t" << eta << "\t" << length << "\n";
    log() << k0 << "\t" << r0 << "\n";
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlgAdapt                                               */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void capOlgAdapt::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=11;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("capOlg::sendParameters",
		   "Can't create array for parameters");
    (*parameters)[0]=A;
    (*parameters)[1]=B;
    (*parameters)[2]=d;
    (*parameters)[3]=n;
    (*parameters)[4]=delta;
    (*parameters)[5]=rho;
    (*parameters)[6]=tau;
    (*parameters)[7]=eta;
    (*parameters)[8]=length;
    (*parameters)[9]=k0;
    (*parameters)[10]=r0;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlgAdapt                                             */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void capOlgAdapt::receiveParameters(const qreal* parameters)
{
    A=parameters[0];
    B=parameters[1];
    d=parameters[2];
    n=parameters[3];
    delta=parameters[4];
    rho=parameters[5];
    tau=(int)(parameters[6]);
    eta=parameters[7];
    length=(long)(parameters[8]);
    k0=parameters[9];
    r0=parameters[10];
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlgGeoExp                                              */
/* Member function: capOlgGeoExp                                              */
/* Purpose:         constructor                                               */
/* Last modified:   31.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

capOlgGeoExp::capOlgGeoExp()					 
{
}

/******************************************************************************/
/*                                                                            */
/* Class name:      geoExpModell                                              */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, set the initial state               */
/* Last modified:   09.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void capOlgGeoExp::initialize()
{
    rInit(rVec);
    kt=k0;
    rt=r0;
    rhoTilda=1-rho/(1-rho);
    etaTilda=eta/(1-eta);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlgGeoExp                                              */
/* Member function: expectedReturnRate                                        */
/* Purpose:         computes the expected return rate                         */
/* Last modified:   31.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal capOlgGeoExp::expectedReturnRate(const long t)
{
    long T,counter;
    qreal help=0.0;
    qreal etaSum=0.0;
    qreal exponent=0.0;
    qreal logEta=log(etaTilda);

    T=MIN(t,tau);
    for( counter=0; counter < T; counter++ ) {
	exponent=exp(counter*logEta);
	etaSum += exponent;
	help += exponent*rVec[counter];
    }
    return( help / etaSum );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlgGeoExp                                              */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system                */
/* Last modified:   04.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void capOlgGeoExp::iteration(const long& t)
{
    qreal rE;
    qreal term;

    rE=expReturnRate(t);
    wt=prodFunction(kt);
    st=savingsFunc(rE)*wt;

    term=st+(1-d)*kt;
    kt= term/(1+n);
    rt=(1-B)*A*(1+n)/(B*st)*exp((1-B)*log( term/(1+n)));
    rShift();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlgGeoExp                                              */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:   31.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal* capOlgGeoExp::setLabels(char *name)
{
    if( !strcmp(name,"xBundle") )
	return &xBundle;
    if( !strcmp(name,"yBundle") )
	return &yBundle;
    if( !strcmp(name,"n") )
        return( &n );
    if( !strcmp(name,"d") )
        return( &d );
    if( !strcmp(name,"A") )
        return( &A );
    if( !strcmp(name,"B") )
        return( &B );
    if( !strcmp(name,"wt") )
        return( &wt );
    if( !strcmp(name,"rt") )
        return( &rt);
    if( !strcmp(name,"kt") )
        return( &kt );
    if( !strcmp(name,"delta") )
        return( &delta );
    if( !strcmp(name,"rho") )
        return( &rho );
    if( !strcmp(name,"k0") )
	return( &k0 );
    if( !strcmp(name,"r0") )
	return( &r0 );
    if( !strcmp(name,"tau") )
	return( (qreal*)(&tau) );
    if( !strcmp(name,"eta") ) 
	return( &eta );

    return( NULL );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlgGeoExp                                              */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   07.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void capOlgGeoExp::loadParamset(ifstream& inputFile)
{
    inputFile >> A >> B >> d;
    inputFile >> n >> delta >> rho;
    inputFile >> tau >> eta >> length;
    inputFile >> k0 >> r0;

    if( rVec )
	delete rVec;
    rVec = new qreal[tau+2];
    if( !rVec )
	fatalError("capOlgGeoExp::loadParamset","Can't create r vector");
    
    initialize();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlgGeoExp                                              */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   31.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void capOlgGeoExp::saveParamset(ofstream& outputFile)
{
    outputFile << A << "\t" << B << "\t" << d << "\n";
    outputFile << n << "\t" << delta << "\t" << rho << "\n";
    outputFile << tau << "\t" << eta << "\t" << length << "\n";
    outputFile << k0 << "\t" << r0 << "\n";
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlgGeoExp                                              */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   31.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void capOlgGeoExp::printParamset()
{
    log() << A << "\t" << B <<  "\t" << d << "\n";
    log() << n << "\t" << delta << "\t" << rho << "\n";
    log() << tau << "\t" << eta << "\t" << length << "\n";
    log() << k0 << "\t" << r0 << "\n";
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlgGeoExp                                              */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void capOlgGeoExp::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=11;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("capOlgGeoExp::sendParameters",
		   "Can't create array for parameters");
    (*parameters)[0]=A;
    (*parameters)[1]=B;
    (*parameters)[2]=d;
    (*parameters)[3]=n;
    (*parameters)[4]=delta;
    (*parameters)[5]=rho;
    (*parameters)[6]=tau;
    (*parameters)[7]=eta;
    (*parameters)[8]=length;
    (*parameters)[9]=k0;
    (*parameters)[10]=r0;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlgGeoExp                                              */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void capOlgGeoExp::receiveParameters(const qreal* parameters)
{
    A=parameters[0];
    B=parameters[1];
    d=parameters[2];
    n=parameters[3];
    delta=parameters[4];
    rho=parameters[5];
    tau=(int)(parameters[6]);
    eta=parameters[7];
    length=(long)(parameters[8]);
    k0=parameters[9];
    r0=parameters[10];
}
