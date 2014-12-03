//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/excOlg.C,v 1.1 2000/08/31 15:54:15 mmeyer Exp $

/******************************************************************************/
/*                                                                            */
/* Module name:  excOlg.C                                                */
/* Contents:     Member functions of the class excOlg                    */
/*                                                                            */
/* Last Modified: 24.07.1995 (Markus Lohmann)                                 */
/*                                                                            */
/******************************************************************************/

#include "excOlg.h"             
#include "../error.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlg                                               */
/* Member function: excOlg                                               */
/* Purpose:         constructor                                               */
/* Last modified:   24.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

excOlg::excOlg() : baseModel(1)
{
    thetaVec=NULL;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlg                                               */
/* Member function: ~excOlg                                              */
/* Purpose:         destructor                                                */
/* Last modified:   24.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

excOlg::~excOlg()
{
    if( thetaVec )
	delete thetaVec;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      capOlgModel                                               */
/* Member function: thetaInit                                                 */
/* Purpose:         initialize the theta vector                               */
/* Last modified:   24.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlg::thetaInit(qreal *thetaVec)
{
    int i;

    for( i=1; i<=tau+1; i++ )
        thetaVec[i] = 0.0;
    thetaVec[0]=theta0;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlg                                               */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, set the initial state               */
/* Last modified:   24.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlg::initialize()
{
    thetaInit(thetaVec);
    thetat=oldThetat=theta0;
    oldThetaE=theta0;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlg                                               */
/* Member function: expInflRateYoung                                          */
/* Purpose:         computes the expected inflation rate (eq. 3.4)            */
/* Last modified:   24.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal excOlg::expInflRateYoung(const long t)
{
    long T,counter;
    qreal help=0.0;
    
    T=MIN(t,tau);
    for( counter=0; counter < T; counter++ )
	help += thetaVec[counter];
    return( help / T );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlg                                               */
/* Member function: expInflRateOld                                            */
/* Purpose:         computes the expected inflation rate (eq. 3.4)            */
/* Last modified:   24.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal excOlg::expInflRateOld(const long t)
{
    long T,counter;
    qreal help=0.0;
    
    T=MIN(t,tau);
    for( counter=0; counter < T; counter++ )
	help += thetaVec[counter];
    return( help / T );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlg                                               */
/* Member function: savingsFuncYoung                                          */
/* Purpose:         describes the savings function                            */
/* Last modified:   24.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal excOlg::savingsFuncYoung(qreal& thetaE)
{
    qreal thetaTerm=exp(rho/(1-rho)*log(thetaE));
    
    return( 1/(1+delta*thetaTerm) );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlg                                               */
/* Member function: savingsFuncOld                                            */
/* Purpose:         describes the savings function                            */
/* Last modified:   24.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal excOlg::savingsFuncOld(qreal& thetaE)
{
    qreal thetaTerm=exp(rho/(1-rho)*log(thetaE));
    
    return( 1/(1+delta*thetaTerm) );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlg                                               */
/* Member function: thetaShift                                                */
/* Purpose:         describes the shift operator on the past tau thetas       */
/* Last modified:   24.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlg::thetaShift()
{
    for(int i=0; i<=tau; i++ )
	thetaVec[tau+1-i]=thetaVec[tau-i];
    thetaVec[0]=thetat;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlg                                               */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system                */
/* Last modified:   24.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlg::iteration(const long& t)
{
    qreal ptratex;

    ptratex=expInflRateYoung(t);
    savYoung=savingsFuncYoung(ptratex);
    savOld=savingsFuncOld(oldThetaE);

    oldThetat=thetat;
    thetat=savOld/savYoung;
    thetaShift();
    oldThetaE=ptratex;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlg                                               */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:   24.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal* excOlg::setLabels(char *name)
{
    if( !strcmp(name,"xBundle") )
	return &xBundle;
    if( !strcmp(name,"yBundle") )
	return &yBundle;
    if( !strcmp(name,"thetat") )
        return( &thetat);
    if( !strcmp(name,"oldThetat") )
        return( &oldThetat);
    if( !strcmp(name,"delta") )
        return( &delta );
    if( !strcmp(name,"rho") )
        return( &rho );
    if( !strcmp(name,"theta0") )
	return( &theta0 );
    if( !strcmp(name,"tau") )
	return( (qreal*)(&tau) );
    return( NULL );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlg                                               */
/* Member function: setTheta                                                  */
/* Purpose:         initializes the theta vector with another vector          */
/* Last modified:   24.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlg::setTheta(qreal *newTheta)
{
    for(int i=0;i<tau+2;i++) 
        thetaVec[i]=newTheta[i];
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlg                                               */
/* Member function: getTheta                                                  */
/* Purpose:         copies the the theta vector into another                  */
/* Last modified:   24.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlg::getTheta(qreal* newTheta)
{
    for(int i=0;i<tau+2;i++) 
        newTheta[i]=thetaVec[i];
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlg                                               */
/* Member function: sendModelVar                                              */
/* Purpose:         returns a pointer to the qreal wage, the main model var.   */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal* excOlg::sendModelVar()
{
    return &thetat;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlg                                               */
/* Member function: sendStateSpace                                            */
/* Purpose:         returns pointers to the qreal balances and the qreal wage;  */
/*                  returns the dimension of the system for rho=0             */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlg::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("excOlg::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&thetat;
}
    
/******************************************************************************/
/*                                                                            */
/* Class name:      excOlg                                               */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   07.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlg::loadParamset(ifstream& inputFile)
{
    inputFile >> delta >> rho;
    inputFile >> tau >> length;
    inputFile >> theta0;

    if( thetaVec )
	delete thetaVec;
    thetaVec = new qreal[tau+2];
    if( !thetaVec )
	fatalError("excOlg::loadParamset","Can't create theta vector");
    
    initialize();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlg                                               */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   24.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlg::saveParamset(ofstream& outputFile)
{
    outputFile << delta << "\t" << rho << endl;
    outputFile << tau << "\t" << length << endl;
    outputFile << theta0 << endl;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlg                                               */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   24.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlg::printParamset()
{
    log() << delta << "\t" << rho << endl;
    log() << tau << "\t" << length << "\n";
    log() << theta0 << endl;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlg                                              */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlg::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=5;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("excOlg::sendParameters",
		   "Can't create array for parameters");
    (*parameters)[0]=delta;
    (*parameters)[1]=rho;
    (*parameters)[2]=tau;
    (*parameters)[3]=length;
    (*parameters)[4]=theta0;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlg                                              */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlg::receiveParameters(const qreal* parameters)
{
    delta=parameters[0];
    rho=parameters[1];
    tau=(int)(parameters[2]);
    length=(long)(parameters[3]);
    theta0=parameters[4];
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlgAdapt                                               */
/* Member function: excOlgAdapt                                               */
/* Purpose:         constructor                                               */
/* Last modified:   31.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

excOlgAdapt::excOlgAdapt()					 
{
    oldExpectationsYoung=1.0;
    oldExpectationsOld=1.0;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlgAdapt                                               */
/* Member function: expInflRateYoung                                          */
/* Purpose:         computes the expected inflation rate                      */
/* Last modified:   31.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal excOlgAdapt::expInflRateYoung(const long)
{
    qreal expInflRate;

    expInflRate=etaYoung*oldExpectationsYoung + (1-etaYoung)*thetaVec[0];
    oldExpectationsYoung=expInflRate;

    return( expInflRate );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlgAdapt                                               */
/* Member function: expInflRateOld                                            */
/* Purpose:         computes the expected inflation rate                      */
/* Last modified:   31.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal excOlgAdapt::expInflRateOld(const long)
{
    qreal expInflRate;

    expInflRate=etaOld*oldExpectationsOld + (1-etaOld)*thetaVec[0];
    oldExpectationsOld=expInflRate;

    return( expInflRate );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlgAdapt                                               */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system                */
/* Last modified:   24.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlgAdapt::iteration(const long& t)
{
    qreal ptratex;

    ptratex=expInflRateYoung(t);
    savYoung=savingsFuncYoung(ptratex);
    savOld=savingsFuncOld(oldThetaE);

    oldThetat=thetat;
    thetat=savOld/savYoung;
    thetaShift();
    oldThetaE=ptratex;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlgAdapt                                               */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:   24.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal* excOlgAdapt::setLabels(char *name)
{
    if( !strcmp(name,"xBundle") )
	return &xBundle;
    if( !strcmp(name,"yBundle") )
	return &yBundle;
    if( !strcmp(name,"thetat") )
        return( &thetat);
    if( !strcmp(name,"oldThetat") )
        return( &oldThetat);
    if( !strcmp(name,"delta") )
        return( &delta );
    if( !strcmp(name,"rho") )
        return( &rho );
    if( !strcmp(name,"theta0") )
	return( &theta0 );
    if( !strcmp(name,"tau") )
	return( (qreal*)(&tau) );
    if( !strcmp(name,"etaYoung") )
	return( &etaYoung );
    if( !strcmp(name,"etaOld") )
	return( &etaOld );
    return( NULL );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlgAdapt                                               */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   07.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlgAdapt::loadParamset(ifstream& inputFile)
{
    inputFile >> delta >> rho;
    inputFile >> tau >> length;
    inputFile >> etaYoung >> etaOld;
    inputFile >> theta0;

    if( thetaVec )
	delete thetaVec;
    thetaVec = new qreal[tau+2];
    if( !thetaVec )
	fatalError("excOlgAdapt::loadParamset","Can't create theta vector");
    
    initialize();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlgAdapt                                               */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   24.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlgAdapt::saveParamset(ofstream& outputFile)
{
    outputFile << delta << "\t" << rho << endl;
    outputFile << tau << "\t" << length << endl;
    outputFile << etaYoung << "\t" << etaOld << endl;
    outputFile << theta0 << endl;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlgAdapt                                               */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   24.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlgAdapt::printParamset()
{
    log() << delta << "\t" << rho << endl;
    log() << tau << "\t" << length << "\n";
    log() << etaYoung << "\t" << etaOld << "\n";
    log() << theta0 << endl;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlgAdapt                                              */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlgAdapt::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=7;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("excOlgAdapt::sendParameters",
		   "Can't create array for parameters");
    (*parameters)[0]=delta;
    (*parameters)[1]=rho;
    (*parameters)[2]=tau;
    (*parameters)[3]=length;
    (*parameters)[4]=etaYoung;
    (*parameters)[5]=etaOld;
    (*parameters)[6]=theta0;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlgAdapt                                              */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlgAdapt::receiveParameters(const qreal* parameters)
{
    delta=parameters[0];
    rho=parameters[1];
    tau=(int)(parameters[2]);
    length=(long)(parameters[3]);
    etaYoung=parameters[4];
    etaOld=parameters[5];
    theta0=parameters[6];
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlgGeoExp                                              */
/* Member function: excOlgGeoExp                                              */
/* Purpose:         constructor                                               */
/* Last modified:   31.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

excOlgGeoExp::excOlgGeoExp()					 
{
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlgGeoExp                                              */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, set the initial state               */
/* Last modified:   09.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlgGeoExp::initialize()
{
    thetaInit(thetaVec);
    thetat=oldThetat=theta0;
    oldThetaE=theta0;
    etaTildaYoung=etaYoung/(1-etaYoung);
    etaTildaOld=etaOld/(1-etaOld);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlgGeoExp                                               */
/* Member function: expInflRateYoung                                          */
/* Purpose:         computes the expected inflation rate                      */
/* Last modified:   31.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal excOlgGeoExp::expInflRateYoung(const long t)
{
    long T,counter;
    qreal help=0.0;
    qreal etaSum=0.0;
    qreal exponent=0.0;
    qreal logEta=log(etaTildaYoung);

    T=MIN(t,tau);
    for( counter=0; counter < T; counter++ ) {
	exponent=exp(counter*logEta);
	etaSum += exponent;
	help += exponent*thetaVec[counter];
    }
    return( help / etaSum );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlgGeoExp                                               */
/* Member function: expInflRateOld                                            */
/* Purpose:         computes the expected inflation rate                      */
/* Last modified:   31.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal excOlgGeoExp::expInflRateOld(const long t)
{
    long T,counter;
    qreal help=0.0;
    qreal etaSum=0.0;
    qreal exponent=0.0;
    qreal logEta=log(etaTildaOld);

    T=MIN(t,tau);
    for( counter=0; counter < T; counter++ ) {
	exponent=exp(counter*logEta);
	etaSum += exponent;
	help += exponent*thetaVec[counter];
    }
    return( help / etaSum );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlgGeoExp                                              */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system                */
/* Last modified:   24.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlgGeoExp::iteration(const long& t)
{
    qreal ptratex;

    ptratex=expInflRateYoung(t);
    savYoung=savingsFuncYoung(ptratex);
    savOld=savingsFuncOld(oldThetaE);

    oldThetat=thetat;
    thetat=savOld/savYoung;
    thetaShift();
    oldThetaE=ptratex;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlgGeoExp                                              */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:   24.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal* excOlgGeoExp::setLabels(char *name)
{
    if( !strcmp(name,"xBundle") )
	return &xBundle;
    if( !strcmp(name,"yBundle") )
	return &yBundle;
    if( !strcmp(name,"thetat") )
        return( &thetat);
    if( !strcmp(name,"oldThetat") )
        return( &oldThetat);
    if( !strcmp(name,"delta") )
        return( &delta );
    if( !strcmp(name,"rho") )
        return( &rho );
    if( !strcmp(name,"theta0") )
	return( &theta0 );
    if( !strcmp(name,"tau") )
	return( (qreal*)(&tau) );
    if( !strcmp(name,"etaYoung") )
	return( &etaYoung );
    if( !strcmp(name,"etaOld") )
	return( &etaOld );
    return( NULL );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlgGeoExp                                              */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   07.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlgGeoExp::loadParamset(ifstream& inputFile)
{
    inputFile >> delta >> rho;
    inputFile >> tau >> length;
    inputFile >> etaYoung >> etaOld;
    inputFile >> theta0;

    if( thetaVec )
	delete thetaVec;
    thetaVec = new qreal[tau+2];
    if( !thetaVec )
	fatalError("excOlgGeoExp::loadParamset","Can't create theta vector");
    
    initialize();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlgGeoExp                                              */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   24.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlgGeoExp::saveParamset(ofstream& outputFile)
{
    outputFile << delta << "\t" << rho << endl;
    outputFile << tau << "\t" << length << endl;
    outputFile << etaYoung << "\t" << etaOld << endl;
    outputFile << theta0 << endl;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlgGeoExp                                              */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   24.07.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlgGeoExp::printParamset()
{
    log() << delta << "\t" << rho << endl;
    log() << tau << "\t" << length << "\n";
    log() << etaYoung << "\t" << etaOld << "\n";
    log() << theta0 << endl;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlgGeoExp                                              */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlgGeoExp::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=7;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("excOlgGeoExp::sendParameters",
		   "Can't create array for parameters");
    (*parameters)[0]=delta;
    (*parameters)[1]=rho;
    (*parameters)[2]=tau;
    (*parameters)[3]=length;
    (*parameters)[4]=etaYoung;
    (*parameters)[5]=etaOld;
    (*parameters)[6]=theta0;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      excOlgGeoExp                                              */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void excOlgGeoExp::receiveParameters(const qreal* parameters)
{
    delta=parameters[0];
    rho=parameters[1];
    tau=(int)(parameters[2]);
    length=(long)(parameters[3]);
    etaYoung=parameters[4];
    etaOld=parameters[5];
    theta0=parameters[6];
}
