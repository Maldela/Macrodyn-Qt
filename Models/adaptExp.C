//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/adaptExp.C,v 1.1 2000/08/31 15:22:41 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  adaptExp.C                                              */
/* Contents:     Member functions of the class adaptExp                  */
/*               This is an extension of the default model. It uses adaptive  */
/*               expectations to obtain the expected inflation rate.          */
/*                                                                            */
/* Last Modified: 24.02.1995 (Markus Lohmann)                                 */
/* Modified:      24.02.95 -- sendParameters and receiveParameters added      */
/*                                                                            */
/******************************************************************************/

#include "adaptExp.h"
#include "../error.h"

//extern void fatalError(const char*, const char*);

/******************************************************************************/
/*                                                                            */
/* Class name:      adaptExp                                             */
/* Member function: adaptExp                                             */
/* Purpose:         constructor                                               */
/* Last modified:   31.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

adaptExp::adaptExp()					 
{
    oldExpectations=1.0;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      adaptExp                                             */
/* Member function: expectedInflationRate                                     */
/* Purpose:         computes the expected inflation rate                      */
/* Last modified:   31.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal adaptExp::expectedInflationRate(const qint64)
{
    qreal expInflRate;

    expInflRate=eta*oldExpectations + (1-eta)*theta[0];
    oldExpectations=expInflRate;

    return( expInflRate );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      adaptExp                                             */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system (Figs. 7, 8)   */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void adaptExp::iteration(const qint64& t)
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
/* Class name:      adaptExp                                             */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:   31.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal* adaptExp::setLabels(const QString& name)
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

    return( NULL );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      gepExpModel                                               */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   07.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void adaptExp::loadParamset(QTextStream& inputFile)
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
	fatalError("adaptExp::loadParamset","Can't create theta vector");
    
    initialize();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      adaptExp                                             */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   31.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void adaptExp::saveParamset(QTextStream& outputFile)
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
/* Class name:      adaptExp                                             */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   31.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void adaptExp::printParamset()
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
/* Class name:      adaptExp                                             */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void adaptExp::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=18;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("geoExpModel::sendParameters",
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
/* Class name:      adaptExp                                             */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void adaptExp::receiveParameters(const QList<qreal> parameters)
{
    if (parameters.size() != 18) log() << "Wrong number of parameters!";
    else
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
}
