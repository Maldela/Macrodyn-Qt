//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/geoExp.C,v 1.1 2000/08/31 15:54:55 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  geoExp.C                                                */
/* Contents:     Member functions of the class geoExp                    */
/*               This is an extension of the default model. It uses a         */
/*               geometric weight on the last inflation rates to obtain the   */
/*               new expected inflation rate.                                 */
/*                                                                            */
/* Last Modified: 24.02.1995 (Markus Lohmann)                                 */
/* Modified:      24.02.95 -- sendParameters and receiveParameters added      */
/*                                                                            */
/******************************************************************************/

#include "geoExp.h"
#include "../logger.h"
/******************************************************************************/
/*                                                                            */
/* Class name:      geoExp                                               */
/* Member function: geoExp                                               */
/* Purpose:         constructor                                               */
/* Last modified:   27.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

geoExp::geoExp()					 
{
}

/******************************************************************************/
/*                                                                            */
/* Class name:      geoExpl                                              */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, set the initial state               */
/* Last modified:   09.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void geoExp::initialize()
{
    thetaInit(theta);
    mtreal=m0/p0;
    wtreal=w0/p0;
    ymax=prodFunction(Lmax);
    rhoTilda=1-rho/(1-rho);
    etaTilda=eta/(1-eta);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      geoExp                                               */
/* Member function: expectedInflationRate                                     */
/* Purpose:         computes the expected inflation rate                      */
/* Last modified:   09.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal geoExp::expectedInflationRate(const qint64 t)
{
    qint64 T,counter;
    qreal help=0.0;
    qreal etaSum=0.0;
    qreal exponent=0.0;
    qreal logEta=log(etaTilda);

    T = qMin(t,tau);
    for( counter=0; counter < T; counter++ ) {
	exponent=exp(counter*logEta);
	etaSum += exponent;
	help += exponent*theta[counter];
    }
    return( help / etaSum );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      geoExp                                               */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system (Figs. 7, 8)   */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void geoExp::iteration(const qint64& t)
{
    qreal ptratex;
    char state[5];
    qreal ztnot;
    qreal ytnot;
    qreal xtnot;
    qreal ct;

    ptratex=expectedInflationRate(t);
    notProd(ztnot,ytnot);
    notCom(xtnot,ct,ptratex);
    empAndOut(xtnot,ytnot);
    detDiseq(xtnot,ytnot,state);
    wageAndPrice(xtnot,ytnot,ztnot,state);
    dynamics();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      geoExp                                               */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:   27.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal* geoExp::setLabels(const QString& name)
{
    if (name == "xBundle")
	return &xBundle;
    if (name == "yBundle")
	return &yBundle;
    if (name == "g")
        return( &g );
    if (name == "L")
        return( &employment );
    if (name == "A")
        return( &A );
    if (name == "B")
        return( &B );
    if (name == "wtreal")
        return( &wtreal );
    if (name == "mtreal")
        return( &mtreal );
    if (name == "theta")
        return( theta );
    if (name == "gamma")
        return( &gamm );
    if (name == "kappa")
        return( &kappa );
    if (name == "lambda")
        return( &lambda );
    if (name == "my")
        return( &my );
    if (name == "delta")
        return( &delta );
    if (name == "rho")
        return( &rho );
    if (name == "tax")
        return( &tax );
    if (name == "Lmax")
        return( &Lmax );
    if (name == "m0")
	return( &m0 );
    if (name == "w0")
	return( &w0 );
    if (name == "eta")
	return( &eta );

    return( NULL );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      geoExp                                               */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   07.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void geoExp::loadParamset(QTextStream& inputFile)
{
    inputFile >> A >> B;
    inputFile >> gamm >> kappa >> lambda >> my;
    inputFile >> tau >> length;
    inputFile >> delta >> beta >> eta;
    inputFile >> w0 >> p0 >> m0;
    inputFile >> Lmax >> rho >> g >> tax;

    if( theta )
	delete theta;
    theta = new qreal[tau+2];
    if( !theta )
	fatalError("geoExp::loadParamset","Can't create theta vector");
    
    initialize();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      geoExp                                              */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   27.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void geoExp::saveParamset(QTextStream& outputFile)
{
    outputFile << A << "\t" << B << "\t";
    outputFile << gamm << "\t" << kappa << "\t" << lambda << "\t" << my << "\t";
    outputFile << tau << "\t" << length << "\t";
    outputFile << delta << "\t" << beta << "\t" << eta << "\t";
    outputFile << w0 << "\t" << p0 << "\t" << m0 << "\t";
    outputFile << Lmax << "\t" << rho << "\t" << g << "\t" << tax;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      geoExp                                               */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   27.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void geoExp::printParamset()
{
    log() << A << "\t" << B;
    log() << gamm << "\t" << kappa << "\t" << lambda << "\t" << my;
    log() << tau << "\t" << length;
    log() << delta << "\t" << beta << "\t" << eta;
    log() << w0 << "\t" << p0 << "\t" << m0;
    log() << Lmax << "\t" << rho << "\t" << g << "\t" << tax;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      geoExp                                               */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void geoExp::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=18;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("geoExp::sendParameters",
		   "Can't create array for parameters");
    (*parameters)[0]=A;
    (*parameters)[1]=B;
    (*parameters)[2]=gamm;
    (*parameters)[3]=kappa;
    (*parameters)[4]=lambda;
    (*parameters)[5]=my;
    (*parameters)[6]=tau;
    (*parameters)[7]=length;
    (*parameters)[8]=delta;
    (*parameters)[9]=beta;
    (*parameters)[10]=eta;
    (*parameters)[11]=w0;
    (*parameters)[12]=p0;
    (*parameters)[13]=m0;
    (*parameters)[14]=Lmax;
    (*parameters)[15]=rho;
    (*parameters)[16]=g;
    (*parameters)[17]=tax;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      geoExp                                               */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void geoExp::receiveParameters(const QList<qreal>& parameters)
{
    A=parameters[0];
    B=parameters[1];
    gamm=parameters[2];
    kappa=parameters[3];
    lambda=parameters[4];
    my=parameters[5];
    tau=(int)(parameters[6]);
    length=(qint64)(parameters[7]);
    delta=parameters[8];
    beta=parameters[9];
    eta=parameters[10];
    w0=parameters[11];
    p0=parameters[12];
    m0=parameters[13];
    Lmax=parameters[14];
    rho=parameters[15];
    g=parameters[16];
    tax=parameters[17];
}
