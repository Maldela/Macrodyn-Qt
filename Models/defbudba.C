//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/defbudba.C,v 1.1 2000/08/31 15:25:15 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  defbudba.C                                                   */
/* Contents:     Member functions of the class defbudba                       */
/*               Whenever an equation is taken from the paper mentioned in    */
/*               the description of the class definition this is marked by    */
/*               (eq. equation number)                                        */
/*                                                                            */
/* Last Modified: 24.02.1995 (Markus Lohmann)                                 */
/* Modified:      24.02.95 -- sendParameters and receiveParameters added      */
/*                                                                            */
/******************************************************************************/

#include "defbudba.h"             
#include "../error.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      defbudba                                                  */
/* Member function: defbudba                                                  */
/* Purpose:         constructor                                               */
/* Last modified:   08.11.1994 ()                               */
/*                                                                            */
/******************************************************************************/

defbudba::defbudba() : baseModel(4)
{
    theta=NULL;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defbudba                                              */
/* Member function: ~defbudba                                             */
/* Purpose:         destructor                                                */
/* Last modified:   08.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

defbudba::~defbudba()
{
    if( theta )
	delete theta;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      defbudba                                              */
/* Member function: prodFunction                                              */
/* Purpose:         production function, computes the output for a given      */
/*                  employment (eq. 3.5)                                      */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal defbudba::prodFunction(const qreal& L)
{
    qreal result;

	result= A/B * exp( B*log(L) );              
	return(result);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defbudba                                              */
/* Member function: thetaInit                                                 */
/* Purpose:         initialize the theta vector                               */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defbudba::thetaInit(qreal *theta)
{
    int i;

    for( i=1; i<=tau+1; i++ )
        theta[i] = 0.0;
    theta[0]=1.0;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defbudba                                              */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, set the initial state               */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defbudba::initialize()
{
    thetaInit(theta);
    mtqreal=m0/p0;
    wtqreal=w0/p0;
    g=g0;
    ymax=prodFunction(Lmax);
    rhoTilda=1-rho/(1-rho);
//    kappa=lambda=my=gamm;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defbudba                                              */
/* Member function: expectedInflationRate                                     */
/* Purpose:         computes the expected inflation rate (eq. 3.4)            */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal defbudba::expectedInflationRate(const long t)
{
    long T,counter;
    qreal help=0.0;
    
    T=MIN(t,tau);
    for( counter=0; counter < T; counter++ )
	help += theta[counter];
    return( help / T );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defbudba                                              */
/* Member function: notProd                                                   */
/* Purpose:         computes the notional demand for labor and the notional   */
/*                  supply of goods of the producer (eqs. 2.8, 2.9)           */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defbudba::notProd(qreal &ztnot,qreal &ytnot)
{
    ztnot= exp( log(A/wtqreal) / (1-B) ) ;
    ytnot= prodFunction(ztnot);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defbudba                                              */
/* Member function: notCom                                                    */
/* Purpose:         computes the desired consumption of the consumers         */
/*                  (eq. 2.12)                                                */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defbudba::notCom(qreal &xtnot,qreal &ct,qreal &ptratex)
{
    qreal sigma = rhoTilda / (rhoTilda-1);
  
 /*    ct =   1/ ( 1 + exp( log(delta)/(1-rhoTilda) + sigma*log(ptratex) ) );*/
     ct = 0.5;   // Änderung von Leo (kann mir den Einfluss von rho, tau nicht 
 		 // erklären !!)   27.2.97  
     
    xtnot=( g + beta * mtqreal ) / (1 - ct * (1-tax) ) ;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defbudba                                              */
/* Member function: empAndOutput                                              */
/* Purpose:         compute actual employment and output (eqs. 2.13, 2.14)    */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defbudba::empAndOut(qreal &xtnot, qreal &ytnot)
{
    y = MIN( MIN(xtnot,ytnot), ymax );
    if( y == ymax )
	employment = Lmax;
    else
	employment = exp( log(B/A * y) / B );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defbudba                                              */
/* Member function: detDiseq                                                  */
/* Purpose:         determination of the type of disequilibrium state         */
/*                  (eq. 2.20)                                                */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defbudba::detDiseq(qreal &xtnot,qreal &ytnot,char *x1)
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
/* Class name:      defbudba                                              */
/* Member function: wageAndPrice                                              */
/* Purpose:         wage and price adjustment (eqs. 3.6, 3.7)                 */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defbudba::wageAndPrice(qreal &xtnot, qreal &ytnot,
                qreal &ztnot, char *state)

{
    if( !strcmp(state,"K ") )
	ptrate= 1 + kappa*((y-ytnot)/ytnot) ;
    else
	ptrate= 1 + gamm*( (xtnot-y)/xtnot ) ;
    
    if( !strcmp(state,"I ") )
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
/* Class name:      defbudba                                              */
/* Member function: dynamics                                                  */
/* Purpose:         describes the dynamics of the system (eqs. 2.28i-2.28iii) */
/* Last modified:   16.07.1996 (Jorg Nikutta)                               */
/*                                                                            */
/******************************************************************************/

void defbudba::dynamics()
{
    mtqreal  = ( MIN( y,g+mtqreal) - tax*y ) / ptrate;
    wtqreal  = wtqreal * (wtrate / ptrate);
    g       = tax*y/ptrate; // NEW Tue Jul 16 10:42:51 MESZ 1996
    for(int i=0; i<=tau; i++ )
	theta[tau+1-i]=theta[tau-i];     /* p(t) -> price[t+1] */
    theta[0]=ptrate;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defbudba                                              */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system (Figs. 7, 8)   */
/* Last modified:   16.07.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defbudba::iteration(const long& t)
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
/* Class name:      defbudba                                              */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal* defbudba::setLabels(char *name)
{
    if( !strcmp(name,"xBundle") )
	return &xBundle;
    if( !strcmp(name,"yBundle") )
	return &yBundle;
    if( !strcmp(name,"g") )
        return( &g );
    if( !strcmp(name,"g0") )
        return( &g0 );
    if( !strcmp(name,"L") )
        return( &employment );
    if( !strcmp(name,"A") )
        return( &A );
    if( !strcmp(name,"B") )
        return( &B );
    if( !strcmp(name,"wtqreal") )
        return( &wtqreal );
    if( !strcmp(name,"mtqreal") )
        return( &mtqreal );
    if( !strcmp(name,"theta") )
        return( theta );
    if( !strcmp(name,"gamma") )
        return( &gamm );
    if( !strcmp(name,"kappa") )
        return( &kappa );
    if( !strcmp(name,"lambda") )
        return( &lambda );
    if( !strcmp(name,"my") )
        return( &my );
    if( !strcmp(name,"delta") )
        return( &delta );
    if( !strcmp(name,"rho") )
        return( &rho );
    if( !strcmp(name,"tax") )
        return( &tax );
    if( !strcmp(name,"Lmax") )
        return( &Lmax );
    if( !strcmp(name,"y") )
        return( &y );    
    if( !strcmp(name,"m0") )
	return( &m0 );
    if( !strcmp(name,"w0") )
	return( &w0 );
    if( !strcmp(name,"tau") )
    return( (qreal*)(&tau) );
    return( NULL );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defbudba                                              */
/* Member function: setTheta                                                  */
/* Purpose:         initializes the theta vector with another vector          */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defbudba::setTheta(qreal *newTheta)
{
    for(int i=0;i<tau+2;i++) 
        theta[i]=newTheta[i];
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defbudba                                              */
/* Member function: getTheta                                                  */
/* Purpose:         copies the the theta vector into another                  */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defbudba::getTheta(qreal* newTheta)
{
    for(int i=0;i<tau+2;i++) 
        newTheta[i]=theta[i];
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defbudba                                              */
/* Member function: sendModelVar                                              */
/* Purpose:         returns a pointer to the qreal wage, the main model var.   */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal* defbudba::sendModelVar()
{
    return &wtqreal;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defbudba                                              */
/* Member function: sendStateSpace                                            */
/* Purpose:         returns pointers to the qreal balances and the qreal wage;  */
/*                  returns the dimension of the system for rho=0             */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defbudba::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("defbudba::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&mtqreal;
    (*stateSpace)[1]=&wtqreal;
    (*stateSpace)[2]=theta;
    (*stateSpace)[3]=&g;
}
    
/******************************************************************************/
/*                                                                            */
/* Class name:      defbudba                                              */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   07.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defbudba::loadParamset(ifstream& inputFile)
{
    inputFile >> A >> B;
    inputFile >> gamm >> kappa >> lambda >> my;
    inputFile >> tau >> length;
    inputFile >> delta >> beta;
    inputFile >> w0 >> p0 >> m0;
    inputFile >> Lmax >> rho >> g0 >> tax;

    if( theta )
	delete theta;
    theta = new qreal[tau+2];
    if( !theta )
	fatalError("defbudba::loadParamset","Can't create theta vector");
    
    initialize();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defbudba                                              */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defbudba::saveParamset(ofstream& outputFile)
{
    outputFile << A << "\t" << B << "\t";
    outputFile << gamm << "\t" << kappa << "\t" << lambda << "\t" << my << "\t";
    outputFile << tau << "\t" << length << "\t";
    outputFile << delta << "\t" << beta << "\t";
    outputFile << w0 << "\t" << p0 << "\t" << m0 << "\t";
    outputFile << Lmax << "\t" << rho << "\t" << g0 << "\t" << tax;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defbudba                                              */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defbudba::printParamset()
{
    Log::log() << A << "\t" << B << "\n";
    Log::log() << gamm << "\t" << kappa << "\t" << lambda << "\t" << my << "\n";
    Log::log() << tau << "\t" << length << "\n";
    Log::log() << delta << "\t" << beta << "\n";
    Log::log() << w0 << "\t" << p0 << "\t" << m0 << "\n";
    Log::log() << Lmax << "\t" << rho << "\t" << g0 << "\t" << tax << endl;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defbudba                                              */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defbudba::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=17;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("defbudba::sendParameters",
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
    (*parameters)[15]=g0;
    (*parameters)[16]=tax;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defbudba                                              */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void defbudba::receiveParameters(const qreal* parameters)
{
    A=parameters[0];
    B=parameters[1];
    gamm=parameters[2];
    kappa=parameters[3];
    lambda=parameters[4];
    my=parameters[5];
    tau=(int)(parameters[6]);
    length=(long)(parameters[7]);
    delta=parameters[8];
    beta=parameters[9];
    w0=parameters[10];
    p0=parameters[11];
    m0=parameters[12];
    Lmax=parameters[13];
    rho=parameters[14];
    g0=parameters[15];
    tax=parameters[16];
}
