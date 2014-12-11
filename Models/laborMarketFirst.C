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
void laborMarketFirst::thetaInit(qreal *theta)
{
    int i,tau;

    tau = qMax(tauS,tauW);
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
    wtqreal=w0;
    omegat=omega0;
    thetaInit(theta);
    dt=d0;
    mtqrealW=mW0;
    mtqrealS=mS0;
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
void laborMarketFirst::loadParamset(QTextStream& inputFile)
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
        theta = new qreal[tauS+2];
      else
        theta = new qreal[tauW+2];
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
void laborMarketFirst::saveParamset(QTextStream& outputFile)
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
    log() << A  << "\t" << B << "\t" << deltaP << "\t" << Lmax;
    log() << betaS << "\t" << betaW << "\t" << rhoS << "\t" << rhoW;
    log() << deltaS << "\t" << deltaW << "\t" << tauS << "\t" << tauW;
    log() << g << "\t" << taxS << "\t" << taxW;
    log() << gamm << "\t" << kappa << "\t" << lambda << "\t" << mu;
    log() << length;
    log() << w0 << "\t" << mS0 << "\t" << mW0 << "\t" << omega0 << "\t";
    log() << d0 << "\t" << theta0;
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
void laborMarketFirst::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=26;
    *parameters=new qreal[amount];
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
void laborMarketFirst::receiveParameters(const QList<qreal>& parameters)
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
    length=(qint64)(parameters[19]);
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
qreal* laborMarketFirst::setLabels(const QString& name)
{
    if (name == "xBundle")
	return &xBundle;
    if (name == "yBundle")
	return &yBundle;

    if (name == "A")
        return( &A );
    if (name == "B")
        return( &B );
    if (name == "deltaP")
        return( &deltaP );
    if (name == "Lmax")
        return( &Lmax );

    if (name == "betaS")
        return( &betaS );
    if (name == "betaW")
        return( &betaW );
    if (name == "rhoS")
        return( &rhoS );
    if (name == "rhoW")
        return( &rhoW );
    if (name == "deltaS")
        return( &deltaS );
    if (name == "deltaW")
        return( &deltaW );
    if (name == "tauS")
	return( (qreal*)(&tauS) );
    if (name == "tauW")
	return( (qreal*)(&tauW) );

    if (name == "g")
        return( &g );
    if (name == "taxS")
        return( &taxS );
    if (name == "taxW")
        return( &taxW );

    if (name == "gamma")
        return( &gamm );
    if (name == "kappa")
        return( &kappa );
    if (name == "lambda")
        return( &lambda );
    if (name == "mu")
        return( &mu );

    if (name == "w0")
	return( &w0 );
    if (name == "mS0")
	return( &mS0 );
    if (name == "mW0")
	return( &mW0 );
    if (name == "omega0")
	return( &omega0 );
    if (name == "d0")
	return( &d0 );
    if (name == "theta0")
        return( &theta0 );

    if (name == "wtqreal")
        return( &wtqreal );
    if (name == "mtqrealS")
        return( &mtqrealS );
    if (name == "mtqrealW")
        return( &mtqrealW );
    if (name == "omegat")
        return( &omegat );
    if (name == "dt")
        return( &dt );
    if (name == "L")
        return( &employment );

    if (name == "theta")
        return( theta );

    return( NULL );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: sendModelVar                                              */
/* Purpose:         returns a pointer to the qreal wage, the main model var.   */
/* Last modified:   29.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
qreal* laborMarketFirst::sendModelVar()
{
    return &wtqreal;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: sendStateSpace                                            */
/* Purpose:         returns pointers to the qreal balances and the qreal wage;  */
/*                  returns the dimension of the system for rho=0             */
/* Last modified:   29.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void laborMarketFirst::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("defaultModel::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&wtqreal;
    (*stateSpace)[1]=&omegat;
    (*stateSpace)[2]=theta;
    (*stateSpace)[3]=&dt;
    (*stateSpace)[4]=&mtqrealS;
    (*stateSpace)[5]=&mtqrealW;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: laborDemand                                               */
/* Purpose:         computes the national demand for Labor                    */
/* Last modified:   30.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
qreal laborMarketFirst::laborDemand()
{
	return( exp(log(A/wtqreal) /(1-B)) ); 
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: actualEmployment                                          */
/* Purpose:         computes the actual employment                            */
/* Last modified:   30.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
qreal laborMarketFirst::actualEmployment(qreal& ztnot)
{
    return( qMin(ztnot,Lmax) );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: detSigmaL                                                 */
/* Purpose:         computes the percentage surplus of the labor market       */
/* Last modified:   30.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
qreal laborMarketFirst::detSigmaL(qreal& ztnot)
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
qreal laborMarketFirst::detWtRate(qreal& sigmaL)
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
qreal laborMarketFirst::expectedInflationRateW(const qint64 t)
{
	qint64 I,index;
	qreal help=0.0;

    I = qMin(t,tauW);
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
qreal laborMarketFirst::consumptionPropensityW(qreal& ptrateexW)
{
    qreal cpW;
    qreal sigma = rhoTildaW / (rhoTildaW-1);
  
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
qreal laborMarketFirst::demandYoungW(qreal& cpsW)
{
	return( cpsW*(1-taxW)*wtqreal*employment );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: expectedInflationRateS                                    */
/* Purpose:         compute the expected inflation rate of the shareholder    */
/* Last modified:   30.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
qreal laborMarketFirst::expectedInflationRateS(const qint64 t)
{
	qint64 I,index;
	qreal help=0.0;

    I = qMin(t,tauS);
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
qreal laborMarketFirst::consumptionPropensityS(qreal& ptrateexS)
{
    qreal cpS;
    qreal sigma = rhoTildaS / (rhoTildaS-1);
  
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
qreal laborMarketFirst::demandYoungS(qreal& cpsS)
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
qreal laborMarketFirst::aggregateDemand(qreal& ytW,qreal& ytS)
{
	return( betaW*mtqrealW + betaS*mtqrealS + g + ytW + ytS );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: productionFunction                                        */
/* Purpose:         compute effectiv production                               */
/* Last modified:   08.12.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
qreal laborMarketFirst::productionFunction(const qreal& L)
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
qreal laborMarketFirst::actualOutput(qreal& ytD,qreal& yteff)
{
    return( qMin(ytD,yteff) );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: remainingOutputYoung                                      */
/* Purpose:         compute remaining output for young workers & shareholders */
/* Last modified:   01.12.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
qreal laborMarketFirst::remainingOutputYoung()
{
    return( qMax( 0.0,( output - g - betaS*mtqrealS - betaW*mtqrealW ) ) );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: actualConsumptionYoungS                                   */
/* Purpose:         compute the actual consumption of young shareholders      */
/* Last modified:   01.12.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
qreal laborMarketFirst::actualConsumptionYoungS(qreal& ytW,qreal& ytS,qreal& xYoung)
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
qreal laborMarketFirst::actualConsumptionYoungW(qreal& ytW,qreal& ytS,qreal& xYoung)
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
qreal laborMarketFirst::detSigmaC(qreal& yteff,qreal& ytD)
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
qreal laborMarketFirst::actualInflationrate(qreal& sigmaC,qreal& ytD)
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
void laborMarketFirst::dynamics(qreal& yteff,qreal& xtS,qreal& xtW)
{
	int i,tau;
	
    tau = qMax(tauS,tauW);
	for ( i = 0; i <= tau; i++ ) 
		theta[tau+1-i]=theta[tau-i];
	theta[0]=ptrate;
	omegat = yteff-output;
	mtqrealW = ( (1-taxW)*wtqreal*employment - xtW )/ptrate;
	mtqrealS = ( dt - xtS )/ptrate;
    dt = (1-taxS)*(qMax(0.0,(output-wtqreal*employment)) /ptrate);
	wtqreal *= wtrate/ptrate;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirst                                          */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system                */
/* Last modified:   08.12.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void laborMarketFirst::iteration(const qint64& t)
{
  qreal ztnot;		// labor demand
  qreal sigmaL;		// labor market imbalance
  qreal sigmaC;		// supply-demand imbalance
  qreal ptrateexW;	// expected inflationrate Worker         
  qreal ptrateexS;	// expected inflationrate Shareholder
  qreal cpsW,cpsS;	// consumptionpropensity Worker,Shareholder   
  qreal ytW,ytS;		// demand young Workers,Shareholder
  qreal ytD;		// aggregate Demand    
  qreal yteff;		// effectiv production
  qreal xYoung;		// what the young may consume 
  qreal xtS,xtW;		// actual consum of the young S and W
     
  ztnot=laborDemand();		         //(A,B,wtqreal)
  employment=actualEmployment(ztnot);	 //(ztnot,Lmax)
  sigmaL=detSigmaL(ztnot);		 //(ztnot,Lmax,employment)
  wtrate=detWtRate(sigmaL);		 //(sigmaL,Lmax,employment,lambda,mu)
  ptrateexW=expectedInflationRateW(t);   //(t,tauW,theta)
  cpsW=consumptionPropensityW(ptrateexW);//(ptrateexW,rhoTildaW,deltaW)
  ytW=demandYoungW(cpsW);	         //(cpsW,taxW,wtqreal,employment)
  ptrateexS=expectedInflationRateS(t);   //(t,tauS,theta)                
  cpsS=consumptionPropensityS(ptrateexS);//(ptrateexS,rhoTildaS,deltaS)
  ytS=demandYoungS(cpsS);	         //(cpsS,taxS,dt)
  ytD=aggregateDemand(ytW,ytS);        //(betaW,betaS,mtqrealW,mtqrealS,g,ytW,ytS)
  yteff=productionFunction(employment);	 //(A,B,employment,deltaP,omagat)
  output=actualOutput(ytD,yteff);	 //(ytD,yteff)
  xYoung=remainingOutputYoung();        //(output,g,betaW,betaS,mtqrealW,mtqrealS)
  xtS=actualConsumptionYoungS(ytW,ytS,xYoung);//(ytW,ytS,xYoung)
  xtW=actualConsumptionYoungW(ytW,ytS,xYoung);//(ytW,ytS,xYoung)
  sigmaC=detSigmaC(yteff,ytD);	         //(yteff,ytD,output)
  ptrate=actualInflationrate(sigmaC,ytD);//(sigmaC,ytD,gamm,kappa)

  dynamics(yteff,xtS,xtW);
}
