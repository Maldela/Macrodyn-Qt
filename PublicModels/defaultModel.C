// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/PublicModels/RCS/defaultModel.C,v 1.5 2000/10/16 09:32:28 mmeyer Exp mmeyer $
/******************************************************************************/
/*                                                                            */
/* Module name:  defaultModel.C                                               */
/* Contents:     Member functions of the class defaultModel                   */
/*               Whenever an equation is taken from the paper mentioned in    */
/*               the description of the class definition this is marked by    */
/*               (eq. equation number)                                        */
/*                                                                            */
/* Last Modified: 24.02.1995 (Markus Lohmann)                                 */
/* Modified:      24.02.95 -- sendParameters and receiveParameters added      */
/*                                                                            */
/******************************************************************************/

#include "defaultModel.h"             
#include "../error.h"

//extern void fatalError(const char*, const char*);

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: defaultModel                                              */
/* Purpose:         constructor                                               */
/* Last modified:   08.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

defaultModel::defaultModel() : baseModel(3)
{
    theta=NULL;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: ~defaultModel                                             */
/* Purpose:         destructor                                                */
/* Last modified:   08.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

defaultModel::~defaultModel()
{
    if( theta )
	delete theta;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: prodFunction                                              */
/* Purpose:         production function, computes the output for a given      */
/*                  employment (eq. 3.5)                                      */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal defaultModel::prodFunction(const qreal& L)
{
    qreal result;

	result= A/B * exp( B*log(L) );              
	return(result);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: thetaInit                                                 */
/* Purpose:         initialize the theta vector                               */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultModel::thetaInit(qreal *theta)
{
    int i;

    for( i=1; i<=tau+1; i++ )
        theta[i] = 0.0;
    theta[0]=1.0;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, set the initial state               */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultModel::initialize()
{
    thetaInit(theta);
    mtqreal=m0/p0;
    wtqreal=w0/p0;
    ymax=prodFunction(Lmax);
    rhoTilda=1-rho/(1-rho);
//    kappa=lambda=my=gamm;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: expectedInflationRate                                     */
/* Purpose:         computes the expected inflation rate (eq. 3.4)            */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal defaultModel::expectedInflationRate(const qint64 t)
{
    qint64 T,counter;
    qreal help=0.0;
    
    T = qMin(t,tau);
    for( counter=0; counter < T; counter++ )
	help += theta[counter];
    return( help / T );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: notProd                                                   */
/* Purpose:         computes the notional demand for labor and the notional   */
/*                  supply of goods of the producer (eqs. 2.8, 2.9)           */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultModel::notProd(qreal &ztnot,qreal &ytnot)
{
    ztnot= exp( log(A/wtqreal) / (1-B) ) ;
    ytnot= prodFunction(ztnot);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: notCom                                                    */
/* Purpose:         computes the desired consumption of the consumers         */
/*                  (eq. 2.12)                                                */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultModel::notCom(qreal &xtnot,qreal &ct,qreal &ptratex)
{
    qreal sigma = rhoTilda / (rhoTilda-1);
  
    ct =   1/ ( 1 + exp( log(delta)/(1-rhoTilda) + sigma*log(ptratex) ) );
    xtnot=( g + beta * mtqreal ) / (1 - ct * (1-tax) ) ;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: empAndOutput                                              */
/* Purpose:         compute actual employment and output (eqs. 2.13, 2.14)    */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultModel::empAndOut(qreal &xtnot, qreal &ytnot)
{
    output = qMin( qMin(xtnot,ytnot), ymax );

    if( output == ymax )
    employment = Lmax;
    else
	employment = exp( log(B/A * output) * (1/B) );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: detDiseq                                                  */
/* Purpose:         deterqMination of the type of disequilibrium state         */
/*                  (eq. 2.20)                                                */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultModel::detDiseq(qreal &xtnot,qreal &ytnot,char *x1)
{
    if( xtnot > ytnot ) {
        if( ymax > ytnot)
	    strcpy(x1,"C ");
	else {
        if( ymax == ytnot )
	        strcpy(x1,"CI");
	    else
		strcpy(x1,"I ");
	}
    }
    else {
        if( xtnot == ytnot ) {
        if( ymax > ytnot )
		strcpy(x1,"CK");
	    else {
            if( ymax == ytnot )
		    strcpy(x1,"WE");
		else
		    strcpy(x1,"I ");
	    }
	}
	else {
	    if( xtnot < ytnot ) {
            if( ymax > xtnot )
		    strcpy(x1,"K ");
		else {
            if( ymax == xtnot )
		        strcpy(x1,"IK");
		    else
		        strcpy(x1,"I ");
		}
	    }
	}
    }
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: wageAndPrice                                              */
/* Purpose:         wage and price adjustment (eqs. 3.6, 3.7)                 */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultModel::wageAndPrice(qreal &xtnot, qreal &ytnot,
                qreal &ztnot, char *state)

{
    if( !strcmp(state,"K "))
	ptrate= 1 + kappa*((output-ytnot)/ytnot) ;
    else
	ptrate= 1 + gamm*( (xtnot-output)/xtnot ) ;
    
    if( !strcmp(state,"I "))
	wtrate= 1 + my*((ztnot-employment)/ztnot) ;
    else
    wtrate= 1 + lambda*((employment-Lmax)/Lmax) ;
    
    if (ptrate<1e-5)
	ptrate=1e-5;
    if (wtrate<1e-5)
	wtrate=1e-5;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: dynamics                                                  */
/* Purpose:         describes the dynamics of the system (eqs. 2.28i-2.28iii) */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultModel::dynamics()
{
    mtqreal  = ( qMin( output,g+mtqreal) - tax*output ) / ptrate;
    wtqreal  = wtqreal * (wtrate / ptrate);
    for(int i=0; i<=tau; i++ )
	theta[tau+1-i]=theta[tau-i];     /* p(t) -> price[t+1] */
    theta[0]=ptrate;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system (Figs. 7, 8)   */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultModel::iteration(const qint64& t)
{
    qreal ptratex;
    char state[5];
    qreal ztnot;
    qreal ytnot;
    qreal xtnot;
    qreal ct;

    ymax=prodFunction(Lmax);
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
/* Class name:      defaultModel                                              */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal* defaultModel::setLabels(const QString& name)
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
    if (name == "ymax")
        return( &ymax );
    if (name == "B")
        return( &B );
    if (name == "wtqreal")
        return( &wtqreal );
    if (name == "wtrate")
        return( &wtrate );
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
    if (name == "tau")
    return( (qreal*)(&tau) );
    if (name == "output")
    return( (qreal*)(&output) );
    return( NULL );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: setTheta                                                  */
/* Purpose:         initializes the theta vector with another vector          */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultModel::setTheta(qreal *newTheta)
{
    for(int i=0;i<tau+2;i++) 
        theta[i]=newTheta[i];
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: getTheta                                                  */
/* Purpose:         copies the the theta vector into another                  */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultModel::getTheta(qreal* newTheta)
{
    for(int i=0;i<tau+2;i++) 
        newTheta[i]=theta[i];
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: sendModelVar                                              */
/* Purpose:         returns a pointer to the qreal wage, the main model var.   */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal* defaultModel::sendModelVar()
{
    return &wtqreal;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: sendStateSpace                                            */
/* Purpose:         returns pointers to the qreal balances and the qreal wage;  */
/*                  returns the dimension of the system for rho=0             */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultModel::sendStateSpace(int &quantity, QList<qreal *> *stateSpace)
{
    stateSpace->clear();
    quantity = dimension;
    *stateSpace << &mtqreal;
    *stateSpace << &wtqreal;
    *stateSpace << theta;
}
    
/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   07.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultModel::loadParamset(QTextStream& inputFile)
{
    inputFile >> A >> B;
    inputFile >> gamm >> kappa >> lambda >> my;
    inputFile >> tau >> length;
    inputFile >> delta >> beta;
    inputFile >> w0 >> p0 >> m0;
    inputFile >> Lmax >> rho >> g >> tax;

    if( theta )
	delete theta;
    theta = new qreal[tau+2];
    if( !theta )
	fatalError("defaultModel::loadParamset","Can't create theta vector");
    
    initialize();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultModel::saveParamset(QTextStream& outputFile)
{
    outputFile << A << "\t" << B << "\t";
    outputFile << gamm << "\t" << kappa << "\t" << lambda << "\t" << my << "\t";
    outputFile << tau << "\t" << length << "\t";
    outputFile << delta << "\t" << beta << "\t";
    outputFile << w0 << "\t" << p0 << "\t" << m0 << "\t";
    outputFile << Lmax << "\t" << rho << "\t" << g << "\t" << tax;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: saveParamsetWithNames                                     */
/* Purpose:         add  parameterset to printfile                            */
/* Last modified:   Tue Jul 21 12:58:13 METDST 1998  Marc Mueller             */
/*                                                                            */
/******************************************************************************/

void defaultModel::saveParamsetWithNames(QTextStream& outputFile)
{
    outputFile << "defaultModel:\n\t";
    outputFile << "A = " << A << "\tB = " << B << "\n\tgamma = ";
    outputFile << gamm << "\tkappa = " << kappa << "\tlambda = ";
    outputFile << lambda << "\tmu = " << my << "\ttau = ";
    outputFile << tau << "\n\tlength = " << length << "\n\tdelta = ";
    outputFile << delta << "\tbeta = " << beta << "\n\tw0 = ";
    outputFile << w0 << "\tp0 = " << p0 << "\tm0 = " << m0 << "\n\tLmax = ";
    outputFile << Lmax << "\trho = " << rho << "\tg = ";
    outputFile << g << "\ttax = " << tax;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultModel::printParamset()
{
    log() << A << "\t" << B;
    log() << gamm << "\t" << kappa << "\t" << lambda << "\t" << my;
    log() << tau << "\t" << length;
    log() << delta << "\t" << beta;
    log() << w0 << "\t" << p0 << "\t" << m0;
    log() << Lmax << "\t" << rho << "\t" << g << "\t" << tax;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultModel::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=17;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("defaultModel::sendParameters",
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
    (*parameters)[10]=w0;
    (*parameters)[11]=p0;
    (*parameters)[12]=m0;
    (*parameters)[13]=Lmax;
    (*parameters)[14]=rho;
    (*parameters)[15]=g;
    (*parameters)[16]=tax;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultModel::receiveParameters(const QList<qreal>& parameters)
{
    if (parameters.size() != 17) log() << "Wrong number of parameters!";
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
        w0=parameters[10];
        p0=parameters[11];
        m0=parameters[12];
        Lmax=parameters[13];
        rho=parameters[14];
        g=parameters[15];
        tax=parameters[16];
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rdefaultModel
// Member function:     rdefaultModel
// Purpose:             constructor
//
// Author:              Uli Middelberg
// Last modified:       Wed Feb 26 15:44:48 MET 1997
// By:                  Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

rdefaultModel::rdefaultModel() : defaultModel()
{
  zvar = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rdefaultModel
// Member function:     iteration
// Purpose:             perform one iteration of the system
//
// Author:              Uli Middelberg
// Last modified:       Wed Feb 26 14:13:52 MET 1997
// By:                  Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void rdefaultModel::iteration(const qint64& t)
{
  * zvar_ptr = zvar->dice();	// Set a new random value for the Parameter
  defaultModel::iteration(t);	// iterate as usual
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rdefaultModel
// Member function:     loadParamset
// Purpose:             load a parameterset from a specified input file
//
// Author:              Uli Middelberg
// Last modified:       Wed Feb 26 14:22:40 MET 1997
// By:                  Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void rdefaultModel::loadParamset(QTextStream& inputFile)
{
  inputFile >> zvar_name;		// read the name of the stochastic parameter
  inputFile >> zvar_expr;		// read the definition
  defaultModel::loadParamset(inputFile);// read the parameters for the default
  					// model as usual and initialize
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rdefaultModel
// Member function:     initialize
// Purpose:             initialize the model, define the systems initial state
//
// Author:              Uli Middelberg
// Last modified:       Wed Feb 26 14:22:40 MET 1997
// By:                  Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void rdefaultModel::initialize()
{
    zvar_ptr = setLabels(zvar_name); 	// get a pointer to the parameter
    if ( zvar != NULL ) {		// delete the old zvar
      delete zvar;
    }
    
    zvar = new rand_var( this, "ranf", zvar_expr);
    					// initialize the random number generator

    defaultModel::initialize();		// initialize the default model as usual
}
