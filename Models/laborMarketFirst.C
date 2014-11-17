//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/laborMarketFirst.C,v 1.1 2000/08/31 15:58:00 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  laborMarketFirst.C                                           */
/* Contents:     Member functions of the class laborMarketFirst               */
/*                                                                            */
/* Modified:     28.11.1995 (Marc Mueller)                                    */
/* Last Modified:20.03.1996 (Marc Mueller)                                    */
/*                                                                            */
/******************************************************************************/

#include "laborMarketFirst.h"             
#include "../error.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: laborMarketFirst                                          */
/* Purpose:         constructor                                               */
/* Last modified:   08.12.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
laborMarketFirst::laborMarketFirst() : baseModel(6)
{
    theta=NULL;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: ~laborMarketFirst                                         */
/* Purpose:         destructor                                                */
/* Last modified:   28.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
laborMarketFirst::~laborMarketFirst()
{
    if( theta )
	delete theta;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: thetaInit                                                 */
/* Purpose:         initialize the theta vector                               */
/* Last modified:   29.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void laborMarketFirst::thetaInit(real *theta)
{
    int i,tau;

    tau = MAX(tauS,tauW);
    for( i=1; i<=tau+1; i++ )
        theta[i] = 0.0;
    theta[0]=theta0;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, set the initial state               */
/* Last modified:   29.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void laborMarketFirst::initialize()
{
    wtreal=w0;
    omegat=omega0;
    thetaInit(theta);
    dt=d0;
    mtrealW=mW0;
    mtrealS=mS0;
    rhoTildaW=1-rhoW/(1-rhoW);
    rhoTildaS=1-rhoS/(1-rhoS);
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   28.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void laborMarketFirst::loadParamset(ifstream& inputFile)
{
    inputFile >> A >> B >> deltaP >> Lmax ;
    inputFile >> betaS >> betaW >> rhoS >> rhoW ;
    inputFile >> deltaS >> deltaW >> tauS >> tauW ;
    inputFile >> g >> taxS >> taxW ; 
    inputFile >> gamm >> kappa >> lambda >> mu ;
    inputFile >> length ;
    inputFile >> w0 >> mS0 >> mW0 >> omega0 >> d0 >> theta0 ;

    if( theta )
	delete theta;
    if( tauS > tauW ) 
        theta = new real[tauS+2];
      else
        theta = new real[tauW+2];
    if( !theta )
	fatalError("defaultModel::loadParamset","Can't create theta vector");
    
    initialize();
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   28.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void laborMarketFirst::saveParamset(ofstream& outputFile)
{
    outputFile << A << "\t" << B << "\t" << deltaP << "\t" << Lmax << "\t";
    outputFile << betaS << "\t" << betaW << "\t";
    outputFile << rhoS << "\t" << rhoW << "\t";
    outputFile << deltaS << "\t" << deltaW << "\t";
    outputFile << tauS << "\t" << tauW << "\t";
    outputFile << g << "\t" << taxS << "\t" << taxW << "\t"; 
    outputFile << gamm << "\t" << kappa << "\t";
    outputFile << lambda << "\t" << mu << "\t";
    outputFile << length << "\t";
    outputFile << w0 << "\t" << mS0 << "\t" << mW0 << "\t";
    outputFile << omega0 << "\t" << d0 << "\t" << theta0;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   28.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void laborMarketFirst::printParamset()
{
    cout << A  << "\t" << B << "\t" << deltaP << "\t" << Lmax << "\n";
    cout << betaS << "\t" << betaW << "\t" << rhoS << "\t" << rhoW << "\n";
    cout << deltaS << "\t" << deltaW << "\t" << tauS << "\t" << tauW << "\n";
    cout << g << "\t" << taxS << "\t" << taxW << "\n"; 
    cout << gamm << "\t" << kappa << "\t" << lambda << "\t" << mu << "\n";
    cout << length << "\n";
    cout << w0 << "\t" << mS0 << "\t" << mW0 << "\t" << omega0 << "\t";
    cout << d0 << "\t" << theta0 << endl;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   29.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void laborMarketFirst::sendParameters(int& amount,real** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=26;
    *parameters=new real[amount];
    if( !(*parameters) )
	fatalError("defaultModel::sendParameters",
		   "Can't create array for parameters");
    (*parameters)[0]=A;
    (*parameters)[1]=B;
    (*parameters)[2]=deltaP;
    (*parameters)[3]=Lmax;
    (*parameters)[4]=betaS;
    (*parameters)[5]=betaW;
    (*parameters)[6]=rhoS;
    (*parameters)[7]=rhoW;
    (*parameters)[8]=deltaS;
    (*parameters)[9]=deltaW;
    (*parameters)[10]=tauS;
    (*parameters)[11]=tauW;
    (*parameters)[12]=g;
    (*parameters)[13]=taxS;
    (*parameters)[14]=taxW;
    (*parameters)[15]=gamm;
    (*parameters)[16]=kappa;
    (*parameters)[17]=lambda;
    (*parameters)[18]=mu;
    (*parameters)[19]=length;
    (*parameters)[20]=w0;
    (*parameters)[21]=mS0;
    (*parameters)[22]=mW0;
    (*parameters)[23]=omega0;
    (*parameters)[24]=d0;
    (*parameters)[25]=theta0;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   29.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void laborMarketFirst::receiveParameters(const real* parameters)
{
    A=parameters[0];
    B=parameters[1];
    deltaP=parameters[2];
    Lmax=parameters[3];
    betaS=parameters[4];
    betaW=parameters[5];
    rhoS=parameters[6];
    rhoW=parameters[7];
    deltaS=parameters[8];
    deltaW=parameters[9];
    tauS=(int)(parameters[10]);
    tauW=(int)(parameters[11]);
    g=parameters[12];
    taxS=parameters[13];
    taxW=parameters[14];
    gamm=parameters[15];
    kappa=parameters[16];
    lambda=parameters[17];
    mu=parameters[18];
    length=(long)(parameters[19]);
    w0=parameters[20];
    mS0=parameters[21];
    mW0=parameters[22];
    omega0=parameters[23];
    d0=parameters[24];
    theta0=parameters[25];
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:   08.12.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real* laborMarketFirst::setLabels(char *name)
{
    if( !strcmp(name,"xBundle") )
	return &xBundle;
    if( !strcmp(name,"yBundle") )
	return &yBundle;

    if( !strcmp(name,"A") )
        return( &A );
    if( !strcmp(name,"B") )
        return( &B );
    if( !strcmp(name,"deltaP") )
        return( &deltaP );
    if( !strcmp(name,"Lmax") )
        return( &Lmax );

    if( !strcmp(name,"betaS") )
        return( &betaS );
    if( !strcmp(name,"betaW") )
        return( &betaW );
    if( !strcmp(name,"rhoS") )
        return( &rhoS );
    if( !strcmp(name,"rhoW") )
        return( &rhoW );
    if( !strcmp(name,"deltaS") )
        return( &deltaS );
    if( !strcmp(name,"deltaW") )
        return( &deltaW );
    if( !strcmp(name,"tauS") )
	return( (real*)(&tauS) );
    if( !strcmp(name,"tauW") )
	return( (real*)(&tauW) );

    if( !strcmp(name,"g") )
        return( &g );
    if( !strcmp(name,"taxS") )
        return( &taxS );
    if( !strcmp(name,"taxW") )
        return( &taxW );

    if( !strcmp(name,"gamma") )
        return( &gamm );
    if( !strcmp(name,"kappa") )
        return( &kappa );
    if( !strcmp(name,"lambda") )
        return( &lambda );
    if( !strcmp(name,"mu") )
        return( &mu );

    if( !strcmp(name,"w0") )
	return( &w0 );
    if( !strcmp(name,"mS0") )
	return( &mS0 );
    if( !strcmp(name,"mW0") )
	return( &mW0 );
    if( !strcmp(name,"omega0") )
	return( &omega0 );
    if( !strcmp(name,"d0") )
	return( &d0 );
    if( !strcmp(name,"theta0") )
        return( &theta0 );

    if( !strcmp(name,"wtreal") )
        return( &wtreal );
    if( !strcmp(name,"mtrealS") )
        return( &mtrealS );
    if( !strcmp(name,"mtrealW") )
        return( &mtrealW );
    if( !strcmp(name,"omegat") )
        return( &omegat );
    if( !strcmp(name,"dt") )
        return( &dt );
    if( !strcmp(name,"L") )
        return( &employment );

    if( !strcmp(name,"theta") )
        return( theta );

    return( NULL );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: sendModelVar                                              */
/* Purpose:         returns a pointer to the real wage, the main model var.   */
/* Last modified:   29.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real* laborMarketFirst::sendModelVar()
{
    return &wtreal;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: sendStateSpace                                            */
/* Purpose:         returns pointers to the real balances and the real wage;  */
/*                  returns the dimension of the system for rho=0             */
/* Last modified:   29.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void laborMarketFirst::sendStateSpace(int &quantity,const real*** stateSpace)
{
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const real* [dimension];
    if( !(*stateSpace) )
	fatalError("defaultModel::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&wtreal;
    (*stateSpace)[1]=&omegat;
    (*stateSpace)[2]=theta;
    (*stateSpace)[3]=&dt;
    (*stateSpace)[4]=&mtrealS;
    (*stateSpace)[5]=&mtrealW;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: laborDemand                                               */
/* Purpose:         computes the national demand for Labor                    */
/* Last modified:   30.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real laborMarketFirst::laborDemand()
{
	return( exp(log(A/wtreal) /(1-B)) ); 
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: actualEmployment                                          */
/* Purpose:         computes the actual employment                            */
/* Last modified:   30.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real laborMarketFirst::actualEmployment(real& ztnot)
{
	return( MIN(ztnot,Lmax) );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: detSigmaL                                                 */
/* Purpose:         computes the percentage surplus of the labor market       */
/* Last modified:   30.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real laborMarketFirst::detSigmaL(real& ztnot)
{
    if ( Lmax > employment )
	return( (employment-Lmax)/Lmax );
    else return( (ztnot-employment)/ztnot ); 
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: detWtRate                                                 */
/* Purpose:         wage adjustment                                           */
/* Last modified:   30.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real laborMarketFirst::detWtRate(real& sigmaL)
{
	if ( Lmax > employment )
		return( (1+lambda*sigmaL) );
	  else	return( (1+mu*sigmaL) );	
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: expectedInflationRateW                                    */
/* Purpose:         compute the expected inflation rate of the worker         */
/* Last modified:   30.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real laborMarketFirst::expectedInflationRateW(const long t)
{
	long I,index;
	real help=0.0;

	I=MIN(t,tauW);
	for ( index=0; index < I; index++ )
		help += theta[index];
	return( help/I ); 
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: consumptionPropensityW                                    */
/* Purpose:         compute the consumptionPropensity of the workers          */
/* Last modified:   08.12.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real laborMarketFirst::consumptionPropensityW(real& ptrateexW)
{
    real cpW;
    real sigma = rhoTildaW / (rhoTildaW-1);
  
    cpW = 1/ ( 1 + exp( log(deltaW)/(1-rhoTildaW) + sigma*log(ptrateexW) ) );
    return( cpW );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: demandYoungW                                              */
/* Purpose:         compute the desired consum of the young workers           */
/* Last modified:   30.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real laborMarketFirst::demandYoungW(real& cpsW)
{
	return( cpsW*(1-taxW)*wtreal*employment );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: expectedInflationRateS                                    */
/* Purpose:         compute the expected inflation rate of the shareholder    */
/* Last modified:   30.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real laborMarketFirst::expectedInflationRateS(const long t)
{
	long I,index;
	real help=0.0;

	I=MIN(t,tauS);
	for ( index=0; index < I; index++ )
		help += theta[index];
	return( help/I ); 
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: consumptionPropensityS                                    */
/* Purpose:         compute the consumptionpropensity of the shareholder      */
/* Last modified:   08.12.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real laborMarketFirst::consumptionPropensityS(real& ptrateexS)
{
    real cpS;
    real sigma = rhoTildaS / (rhoTildaS-1);
  
    cpS = 1/ ( 1 + exp( log(deltaS)/(1-rhoTildaS) + sigma*log(ptrateexS) ) );
    return( cpS );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: demandYoungS                                              */
/* Purpose:         compute the desired consum of the young shareholder       */
/* Last modified:   20.03.1996 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real laborMarketFirst::demandYoungS(real& cpsS)
{
	return( cpsS*dt );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: aggregateDemand                                           */
/* Purpose:         compute the aggregate demand                              */
/* Last modified:   30.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real laborMarketFirst::aggregateDemand(real& ytW,real& ytS)
{
	return( betaW*mtrealW + betaS*mtrealS + g + ytW + ytS );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: productionFunction                                        */
/* Purpose:         compute effectiv production                               */
/* Last modified:   08.12.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real laborMarketFirst::productionFunction(const real& L)
{
	return( (A/B)*exp(B*log(L)) + deltaP*omegat );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: actualOutput                                              */
/* Purpose:         compute the actual sold output                            */
/* Last modified:   01.12.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real laborMarketFirst::actualOutput(real& ytD,real& yteff)
{
	return( MIN(ytD,yteff) ); 
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: remainingOutputYoung                                      */
/* Purpose:         compute remaining output for young workers & shareholders */
/* Last modified:   01.12.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real laborMarketFirst::remainingOutputYoung()
{
	return( MAX( 0,( output - g - betaS*mtrealS - betaW*mtrealW ) ) );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: actualConsumptionYoungS                                   */
/* Purpose:         compute the actual consumption of young shareholders      */
/* Last modified:   01.12.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real laborMarketFirst::actualConsumptionYoungS(real& ytW,real& ytS,real& xYoung)
{
	return( ( ytS / ( ytS+ytW ) ) * xYoung );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: actualConsumptionYoungW                                   */
/* Purpose:         compute the actual consumption of young workers           */
/* Last modified:   01.12.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real laborMarketFirst::actualConsumptionYoungW(real& ytW,real& ytS,real& xYoung)
{
	return( ( ytW / ( ytW + ytS ) ) * xYoung ); 
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: detSigmaC                                                 */
/* Purpose:         computes a percentage signal for price modification       */
/* Last modified:   01.12.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real laborMarketFirst::detSigmaC(real& yteff,real& ytD)
{
	if ( yteff > output )
		return( ( output - yteff ) / yteff );
	  else	return( ( ytD - output ) / ytD );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: actualInflationrate                                       */
/* Purpose:         compute the actual inflationrate                          */
/* Last modified:   01.12.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real laborMarketFirst::actualInflationrate(real& sigmaC,real& ytD)
{
	if ( ytD > output )
		return( 1 + gamm * sigmaC );
	  else	return( 1 + kappa * sigmaC );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: dynamics                                                  */
/* Purpose:         compute modifications for the next period                 */
/* Last modified:   18.03.1996 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void laborMarketFirst::dynamics(real& yteff,real& xtS,real& xtW)
{
	int i,tau;
	
	tau = MAX(tauS,tauW);
	for ( i = 0; i <= tau; i++ ) 
		theta[tau+1-i]=theta[tau-i];
	theta[0]=ptrate;
	omegat = yteff-output;
	mtrealW = ( (1-taxW)*wtreal*employment - xtW )/ptrate;
	mtrealS = ( dt - xtS )/ptrate;
	dt = (1-taxS)*(MAX(0,(output-wtreal*employment)) /ptrate);
	wtreal *= wtrate/ptrate;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system                */
/* Last modified:   08.12.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void laborMarketFirst::iteration(const long& t)
{
  real ztnot;		// labor demand
  real sigmaL;		// labor market imbalance
  real sigmaC;		// supply-demand imbalance
  real ptrateexW;	// expected inflationrate Worker         
  real ptrateexS;	// expected inflationrate Shareholder
  real cpsW,cpsS;	// consumptionpropensity Worker,Shareholder   
  real ytW,ytS;		// demand young Workers,Shareholder
  real ytD;		// aggregate Demand    
  real yteff;		// effectiv production
  real xYoung;		// what the young may consume 
  real xtS,xtW;		// actual consum of the young S and W
     
  ztnot=laborDemand();		         //(A,B,wtreal)
  employment=actualEmployment(ztnot);	 //(ztnot,Lmax)
  sigmaL=detSigmaL(ztnot);		 //(ztnot,Lmax,employment)
  wtrate=detWtRate(sigmaL);		 //(sigmaL,Lmax,employment,lambda,mu)
  ptrateexW=expectedInflationRateW(t);   //(t,tauW,theta)
  cpsW=consumptionPropensityW(ptrateexW);//(ptrateexW,rhoTildaW,deltaW)
  ytW=demandYoungW(cpsW);	         //(cpsW,taxW,wtreal,employment)
  ptrateexS=expectedInflationRateS(t);   //(t,tauS,theta)                
  cpsS=consumptionPropensityS(ptrateexS);//(ptrateexS,rhoTildaS,deltaS)
  ytS=demandYoungS(cpsS);	         //(cpsS,taxS,dt)
  ytD=aggregateDemand(ytW,ytS);        //(betaW,betaS,mtrealW,mtrealS,g,ytW,ytS)
  yteff=productionFunction(employment);	 //(A,B,employment,deltaP,omagat)
  output=actualOutput(ytD,yteff);	 //(ytD,yteff)
  xYoung=remainingOutputYoung();        //(output,g,betaW,betaS,mtrealW,mtrealS)
  xtS=actualConsumptionYoungS(ytW,ytS,xYoung);//(ytW,ytS,xYoung)
  xtW=actualConsumptionYoungW(ytW,ytS,xYoung);//(ytW,ytS,xYoung)
  sigmaC=detSigmaC(yteff,ytD);	         //(yteff,ytD,output)
  ptrate=actualInflationrate(sigmaC,ytD);//(sigmaC,ytD,gamm,kappa)

  dynamics(yteff,xtS,xtW);
}
