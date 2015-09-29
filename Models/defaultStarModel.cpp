// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/PublicModels/RCS/defaultStarModel.C,v 1.5 2000/10/16 09:32:28 mmeyer Exp mmeyer $
/******************************************************************************/
/*                                                                            */
/* Module name:  defaultStarModel.C                                           */
/* Contents:     Member functions of the class defaultStarModel               */
/*               Whenever an equation is taken from the paper mentioned in    */
/*               the description of the class definition this is marked by    */
/*               (eq. equation number)                                        */
/*                                                                            */
/* Last Modified: 24.02.1995 (Markus Lohmann)                                 */
/* Modified:      24.02.95 -- sendParameters and receiveParameters added      */
/*                                                                            */
/******************************************************************************/

#include "defaultStarModel.h"
//#include "../error.h"

//extern void fatalError(const char*, const char*);

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultStarModel                                          */
/* Member function: defaultStarModel                                          */
/* Purpose:         constructor                                               */
/* Last modified:   08.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

defaultStarModel::defaultStarModel() : baseModel(3)
{
    theta=NULL;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultStarModel                                          */
/* Member function: ~defaultStarModel                                         */
/* Purpose:         destructor                                                */
/* Last modified:   08.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

defaultStarModel::~defaultStarModel()
{
    if( theta )
	delete theta;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      defaultStarModel                                          */
/* Member function: prodFunction                                              */
/* Purpose:         production function, computes the output for a given      */
/*                  employment (eq. 3.5)                                      */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal defaultStarModel::prodFunction(const qreal& L)
{
    qreal result;

	result= A/B * exp( B*log(L) );              
	return(result);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultStarModel                                          */
/* Member function: Lstar                                                     */
/* Purpose:         labor supply function, computes the labor supply          */
/*                                                                            */
/* Last modified:   01.10.2008 (Thorsten Pampel)                              */
/*                                                                            */
/******************************************************************************/

qreal defaultStarModel::LstarFunction(const qreal& x,const qreal& y, const qreal& z)
{
    qreal result;

	if (z==0)
	{
	    result=x;
	}
	else
	{
	    if (y==0)
	    {
		result=0.0;
	    }	
	    else
	    {
		result= x * exp( z*log(y) );              
	    }
	}
	return(result);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultStarModel                                          */
/* Member function: thetaInit                                                 */
/* Purpose:         initialize the theta vector                               */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultStarModel::thetaInit(qreal *theta)
{
    int i;

    for( i=1; i<=tau+1; i++ )
        theta[i] = 0.0;
    theta[0]=1.0;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultStarModel                                          */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, set the initial state               */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultStarModel::initialize()
{
    qreal ptratex;
    thetaInit(theta);
    mtreal=m0/p0;
    wtreal=w0/p0;
    ptratex=expectedInflationRate(1);
    theta1=ptratex;
    Lstar=LstarFunction(Lmax,wtreal/ptratex,D);
    ymax=prodFunction(Lstar);
    rhoTilda=1-rho/(1-rho);
    noK=0;noI=0;noC=0;
//    kappa=lambda=my=gamm;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultStarModel                                          */
/* Member function: expectedInflationRate                                     */
/* Purpose:         computes the expected inflation rate (eq. 3.4)            */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal defaultStarModel::expectedInflationRate(const qint64 t)
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
/* Class name:      defaultStarModel                                          */
/* Member function: notProd                                                   */
/* Purpose:         computes the notional demand for labor and the notional   */
/*                  supply of goods of the producer (eqs. 2.8, 2.9)           */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultStarModel::notProd(qreal &ztnot,qreal &ytnot)
{
    ztnot= exp( log(A/wtreal) / (1-B) ) ;
    ytnot= prodFunction(ztnot);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultStarModel                                          */
/* Member function: notCom                                                    */
/* Purpose:         computes the desired consumption of the consumers         */
/*                  (eq. 2.12)                                                */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultStarModel::notCom(qreal &xtnot,qreal &ct,qreal &ptratex)
{
    qreal sigma = rhoTilda / (rhoTilda-1);
  
    ct =   1/ ( 1 + exp( log(delta)/(1-rhoTilda) + sigma*log(ptratex) ) );
    xtnot=( g + beta * mtreal ) / (1 - ct * (1-tax) ) ;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultStarModel                                          */
/* Member function: empAndOutput                                              */
/* Purpose:         compute actual employment and output (eqs. 2.13, 2.14)    */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultStarModel::empAndOut(qreal &xtnot, qreal &ytnot)
{
    output = qMin( qMin(xtnot,ytnot), ymax );

    if( output == ymax )
	employment = Lstar;
    else
	employment = exp( log(B/A * output) * (1/B) );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultStarModel                                          */
/* Member function: detDiseq                                                  */
/* Purpose:         deterqqMination of the type of disequilibrium state         */
/*                  (eq. 2.20)                                                */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultStarModel::detDiseq(qreal &xtnot,qreal &ytnot,char *x1)
{
    KCI=5;
    if( xtnot > ytnot ) {
        if( ymax > ytnot){
	    strcpy(x1,"C ");
            noC=noC+1;
	    KCI=2;
        }
	else {
	    if( ymax == ytnot ){
	        strcpy(x1,"CI");
		KCI=4;
	    }
	    else{
		strcpy(x1,"I ");
		noI=noI+1;
		KCI=3;
            }
	}
    }
    else {
        if( xtnot == ytnot ) {
	    if( ymax > ytnot ){
		strcpy(x1,"CK");
		KCI=4;
	    }
	    else {
	        if( ymax == ytnot ){
		    strcpy(x1,"WE");
		    KCI=4;
		}
		else{
		    strcpy(x1,"I ");
		    noI=noI+1;
		    KCI=3;
		}
	    }
	}
	else {
	    if( xtnot < ytnot ) {
	        if( ymax > xtnot ){
		    strcpy(x1,"K ");
		    noK=noK+1;
		    KCI=1;
		}
		else {
		    if( ymax == xtnot ){
		        strcpy(x1,"IK");
			KCI=4;
		    }
		    else{
		        strcpy(x1,"I ");
			noI=noI+1;
			KCI=3;
		    }
		}
	    }
	}
    }
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultStarModel                                          */
/* Member function: wageAndPrice                                              */
/* Purpose:         wage and price adjustment (eqs. 3.6, 3.7)                 */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultStarModel::wageAndPrice(qreal &xtnot, qreal &ytnot,
                qreal &ztnot, char *state)

{
    if( !strcmp(state,"K ") )
	ptrate= 1 + kappa*((output-ytnot)/ytnot) ;
    else
	ptrate= 1 + gamm*( (xtnot-output)/xtnot ) ;
    
    if( !strcmp(state,"I ") )
	wtrate= 1 + my*((ztnot-employment)/ztnot) ;
    else
	wtrate= 1 + lambda*((employment-Lstar)/Lstar) ;
    
    if (ptrate<1e-5)
	ptrate=1e-5;
    if (wtrate<1e-5)
	wtrate=1e-5;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultStarModel                                          */
/* Member function: dynamics                                                  */
/* Purpose:         describes the dynamics of the system (eqs. 2.28i-2.28iii) */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultStarModel::dynamics()
{
    mtreal  = ( qMin( output,g+mtreal) - tax*output ) / ptrate;
    wtreal  = wtreal * (wtrate / ptrate);
    theta1=theta[0];
    for(int i=0; i<=tau; i++ )
	theta[tau+1-i]=theta[tau-i];     /* p(t) -> price[t+1] */
    theta[0]=ptrate;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultStarModel                                          */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system (Figs. 7, 8)   */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultStarModel::iteration(const qint64& t)
{
    qreal ptratex;
    char state[5];
    qreal ztnot;
    qreal ytnot;
    qreal xtnot;
    qreal ct;

    ptratex=expectedInflationRate(t);
    Lstar=LstarFunction(Lmax,wtreal/ptratex,D);
    ymax=prodFunction(Lstar);
    notProd(ztnot,ytnot);
    notCom(xtnot,ct,ptratex);
    empAndOut(xtnot,ytnot);
    detDiseq(xtnot,ytnot,state);
    wageAndPrice(xtnot,ytnot,ztnot,state);
    dynamics();
    if(t==int(length/10)){
	noK=0; noC=0; noI=0;
    }
    if(t==length){
	relNoK=noK/(noK+noI+noC); relNoC=noC/(noK+noI+noC); relNoI=noI/(noK+noI+noC);
    //cout << "\n\nK: " << relNoK << "\t" << "C: " << relNoC << "\t" << "I: " << relNoI << "\n\n";
    }
    }

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultStarModel                                          */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal* defaultStarModel::setLabels(const QString& name)
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
    if (name == "wtreal")
        return( &wtreal );
    if (name == "wtrate")
        return( &wtrate );
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
    if (name == "tau")
    return( (qreal*)(&tau) );
    if (name == "output")
    return( (qreal*)(&output) );

    if(name == "D")
        return( &D );
    if(name == "KCI" )
    return( (qreal*)(&KCI) );
    if(name == "noK" )
    return( (qreal*)(&noK) );
    if(name == "noC" )
    return( (qreal*)(&noC) );
    if(name == "noI" )
    return( (qreal*)(&noI) );
    if(name == "relNoK" )
    return( (qreal*)(&relNoK) );
    if(name == "relNoC" )
    return( (qreal*)(&relNoC) );
    if(name == "relNoI" )
    return( (qreal*)(&relNoI) );
    return( NULL );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultStarModel                                          */
/* Member function: setTheta                                                  */
/* Purpose:         initializes the theta vector with another vector          */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultStarModel::setTheta(qreal *newTheta)
{
    for(int i=0;i<tau+2;i++) 
        theta[i]=newTheta[i];
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultStarModel                                          */
/* Member function: getTheta                                                  */
/* Purpose:         copies the the theta vector into another                  */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultStarModel::getTheta(qreal* newTheta)
{
    for(int i=0;i<tau+2;i++) 
        newTheta[i]=theta[i];
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultStarModel                                          */
/* Member function: sendModelVar                                              */
/* Purpose:         returns a pointer to the qreal wage, the main model var.   */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal* defaultStarModel::sendModelVar()
{
    return &wtreal;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultStarModel                                          */
/* Member function: sendStateSpace                                            */
/* Purpose:         returns pointers to the real balances and the qreal wage;  */
/*                  returns the dimension of the system for rho=0             */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultStarModel::sendStateSpace(int &quantity,QList<qreal *> *stateSpace)
{
    stateSpace->clear();
    quantity = dimension;
    *stateSpace << &mtreal;
    *stateSpace << &wtreal;
    *stateSpace << theta;
}
    
/******************************************************************************/
/*                                                                            */
/* Class name:      defaultStarModel                                          */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   07.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultStarModel::loadParamset(QTextStream& inputFile)
{
    inputFile >> A >> B >> D;
    inputFile >> gamm >> kappa >> lambda >> my;
    inputFile >> tau >> length;
    inputFile >> delta >> beta;
    inputFile >> w0 >> p0 >> m0;
    inputFile >> Lmax >> rho >> g >> tax;
    if( theta )
	delete theta;
    theta = new qreal[tau+2];
    if( !theta )
        fatalError("defaultStarModel::loadParamset","Can't create theta vector");
    
    initialize();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultStarModel                                          */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultStarModel::saveParamset(QTextStream& outputFile)
{
    outputFile << A << "\t" << B << "\t" << D << "\t";
    outputFile << gamm << "\t" << kappa << "\t" << lambda << "\t" << my << "\t";
    outputFile << tau << "\t" << length << "\t";
    outputFile << delta << "\t" << beta << "\t";
    outputFile << w0 << "\t" << p0 << "\t" << m0 << "\t";
    outputFile << Lmax << "\t" << rho << "\t" << g << "\t" << tax;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultStarModel                                          */
/* Member function: saveParamsetWithNames                                     */
/* Purpose:         add  parameterset to printfile                            */
/* Last modified:   Tue Jul 21 12:58:13 METDST 1998  Marc Mueller             */
/*                                                                            */
/******************************************************************************/

void defaultStarModel::saveParamsetWithNames(QTextStream& outputFile)
{
    outputFile << "defaultStarModel:\n\t";
    outputFile << "A = " << A << "\tB = " << B << "\tD = " << D << "\n\tgamma = ";
    outputFile << gamm << "\tkappa = " << kappa << "\tlambda = ";
    outputFile << lambda << "\tmu = " << my << "\ttau = ";
    outputFile << tau << "\n\tlength = " << length << "\n\tdelta = ";
    outputFile << delta << "\tbeta = " << beta << "\n\tw0 = ";
    outputFile << w0 << "\tp0 = " << p0 << "\tm0 = " << m0 << "\n\tLmax = ";
    outputFile << Lmax << "\trho = " << rho << "\tg = ";
    outputFile << g << "\ttax = " << tax << "\n";
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultStarModel                                          */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultStarModel::printParamset()
{

}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultStarModel                                          */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultStarModel::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=18;
    *parameters=new qreal[amount];
    if( !(*parameters) )
        fatalError("defaultStarModel::sendParameters",
		   "Can't create array for parameters");
    (*parameters)[0]=A;
    (*parameters)[1]=B;
    (*parameters)[2]=D;
    (*parameters)[3]=gamm;
    (*parameters)[4]=kappa;
    (*parameters)[5]=lambda;
    (*parameters)[6]=my;
    (*parameters)[7]=tau;
    (*parameters)[8]=length;
    (*parameters)[9]=delta;
    (*parameters)[10]=beta;
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
/* Class name:      defaultStarModel                                          */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defaultStarModel::receiveParameters(const QList<qreal>& parameters)
{
    A=parameters[0];
    B=parameters[1];
    D=parameters[2];
    gamm=parameters[3];
    kappa=parameters[4];
    lambda=parameters[5];
    my=parameters[6];
    tau=(int)(parameters[7]);
    length=(long)(parameters[8]);
    delta=parameters[9];
    beta=parameters[10];
    w0=parameters[11];
    p0=parameters[12];
    m0=parameters[13];
    Lmax=parameters[14];
    rho=parameters[15];
    g=parameters[16];
    tax=parameters[17];
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rdefaultStarModel
// Member function:     rdefaultStarModel
// Purpose:             constructor
//
// Author:              Uli Middelberg
// Last modified:       Wed Feb 26 15:44:48 MET 1997
// By:                  Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

rdefaultStarModel::rdefaultStarModel() : defaultStarModel()
{
  zvar = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rdefaultStarModel
// Member function:     iteration
// Purpose:             perform one iteration of the system
//
// Author:              Uli Middelberg
// Last modified:       Wed Feb 26 14:13:52 MET 1997
// By:                  Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void rdefaultStarModel::iteration(const qint64& t)
{
  * zvar_ptr = zvar->dice();	// Set a new random value for the Parameter
  defaultStarModel::iteration(t);	// iterate as usual
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rdefaultStarModel
// Member function:     loadParamset
// Purpose:             load a parameterset from a specified input file
//
// Author:              Uli Middelberg
// Last modified:       Wed Feb 26 14:22:40 MET 1997
// By:                  Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void rdefaultStarModel::loadParamset(QTextStream& inputFile)
{
  inputFile >> zvar_name;		// read the name of the stochastic parameter
  inputFile >> zvar_expr;		// read the definition
  defaultStarModel::loadParamset(inputFile);// read the parameters for the defaultStar
  					// model as usual and initialize
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rdefaultStarModel
// Member function:     initialize
// Purpose:             initialize the model, define the systems initial state
//
// Author:              Uli Middelberg
// Last modified:       Wed Feb 26 14:22:40 MET 1997
// By:                  Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void rdefaultStarModel::initialize()
{
    zvar_ptr = setLabels(zvar_name); 	// get a pointer to the parameter
    if ( zvar != NULL ) {		// delete the old zvar
      delete zvar;
    }
    
    zvar = new rand_var( this, "ranf", zvar_expr);
    					// initialize the random number generator

    defaultStarModel::initialize();		// initialize the defaultStar model as usual
}
