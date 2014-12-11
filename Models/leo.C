//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/leo.C,v 1.1 2000/08/31 15:58:46 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  leo.C                                                        */
/* Contents:     Member functions of the class leo                            */
/*               This is an extension of the default model. It uses           */
/*               modified dynamics by Leo Kaas.                               */
/*                                                                            */
/* Last Modified: 26.02.1996 (Marc Mueller)                                   */
/* Modified:      23.02.1996 (Marc Mueller)                                   */
/*                                                                            */
/******************************************************************************/

#include "leo.h"
#include "../error.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      leo                                                       */
/* Member function: leo                                                       */
/* Purpose:         constructor                                               */
/* Last modified:   26.02.1996 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/

leo::leo()
{
}

/******************************************************************************/
/*                                                                            */
/* Class name:      leo                                                       */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:   23.02.1996 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/

qreal* leo::setLabels(const QString& name)
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
    if (name == "wtqreal")
        return( &wtqreal );
    if (name == "mtqreal")
        return( &mtqreal );
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
    if (name == "zeta") 
	return( &zeta );

    return( NULL );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      leo                                                       */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   23.02.1996 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/

void leo::loadParamset(QTextStream& inputFile)
{
    inputFile >> A >> B;
    inputFile >> gamm >> kappa >> lambda >> my;
    inputFile >> tau >> length;
    inputFile >> delta >> beta >> eta >> zeta;
    inputFile >> w0 >> p0 >> m0;
    inputFile >> Lmax >> rho >> g >> tax;

    if( theta )
	delete theta;
    theta = new qreal[tau+2];
    if( !theta )
	fatalError("leo::loadParamset","Can't create theta vector");
    
    initialize();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      leo                                                       */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   23.02.1996 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/

void leo::saveParamset(QTextStream& outputFile)
{
    outputFile << A << "\t" << B << "\t";
    outputFile << gamm << "\t" << kappa << "\t" << lambda << "\t" << my << "\t";
    outputFile << tau << "\t" << length << "\t";
    outputFile << delta << "\t" << beta << "\t" << eta << "\t" << zeta << "\t";
    outputFile << w0 << "\t" << p0 << "\t" << m0 << "\t";
    outputFile << Lmax << "\t" << rho << "\t" << g << "\t" << tax;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      leo                                                       */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   23.02.1996 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/

void leo::printParamset()
{
    log() << A << "\t" << B;
    log() << gamm << "\t" << kappa << "\t" << lambda << "\t" << my;
    log() << tau << "\t" << length;
    log() << delta << "\t" << beta << "\t" << eta << "\t" << zeta;
    log() << w0 << "\t" << p0 << "\t" << m0;
    log() << Lmax << "\t" << rho << "\t" << g << "\t" << tax;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      leo                                                       */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   23.02.1996 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/

void leo::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=19;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("leo::sendParameters",
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
    (*parameters)[11]=zeta;
    (*parameters)[12]=w0;
    (*parameters)[13]=p0;
    (*parameters)[14]=m0;
    (*parameters)[15]=Lmax;
    (*parameters)[16]=rho;
    (*parameters)[17]=g;
    (*parameters)[18]=tax;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      leo                                                       */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   23.02.1996 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/

void leo::receiveParameters(const QList<qreal>& parameters)
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
    zeta=parameters[11];
    w0=parameters[12];
    p0=parameters[13];
    m0=parameters[14];
    Lmax=parameters[15];
    rho=parameters[16];
    g=parameters[17];
    tax=parameters[18];
}

/******************************************************************************/
/*                                                                            */
/* Class name:      leo                                                       */
/* Member function: leoDynamics                                               */
/* Purpose:         describes the dynamics of the system                      */
/* Last modified:   26.02.1996 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/

void leo::leoDynamics()
{
    for(int i=0; i<=tau; i++ )
	theta[tau+1-i]=theta[tau-i];     /* p(t) -> price[t+1] */
    theta[0]=eta*theta[0]+(1-eta)*(ptrate*exp(zeta*log(theta[0])));

    wtqreal  = wtqreal * (wtrate / ptrate);

    mtqreal  = ( qMin(output,g+mtqreal) -tax*output ) / ptrate /
                                                exp(zeta*log(theta[0]));
}

/******************************************************************************/
/*                                                                            */
/* Class name:      leo                                                       */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system                */
/* Last modified:   23.02.1996 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/

void leo::iteration(const qint64& t)
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
    leoDynamics();
}
