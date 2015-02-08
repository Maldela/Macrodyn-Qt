//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/laborMarketFirstTax.C,v 1.1 2000/08/31 15:58:25 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  laborMarketFirstTax.C                                        */
/* Contents:     Member functions of the class laborMarketFirstTax            */
/*               This model is derived from the Model labormarketfirst.       */
/*               The difference is that taxS and taxW are set by tax          */
/*                                                                            */
/* Last Modified: 06.03.1996 (Marc Mueller)                                   */
/* Modified:      18.03.1996 (Marc Mueller)                                   */
/*                                                                            */
/******************************************************************************/

#include "laborMarketFirstTax.h"             
#include "../logger.h"
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirstTax                                       */
/* Member function: laborMarketFirstTax                                       */
/* Purpose:         constructor                                               */
/* Last modified:   06.03.1996 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
laborMarketFirstTax::laborMarketFirstTax() 
{
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirstTax                                       */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   18.03.1996 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void laborMarketFirstTax::loadParamset(QTextStream& inputFile)
{
    inputFile >> A >> B >> deltaP >> Lmax ;
    inputFile >> betaS >> betaW >> rhoS >> rhoW ;
    inputFile >> deltaS >> deltaW >> tauS >> tauW ;
    inputFile >> g >> tax ; 
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
/* Class name:      laborMarketFirstTax                                       */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   18.03.1996 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void laborMarketFirstTax::saveParamset(QTextStream& outputFile)
{
    outputFile << A << "\t" << B << "\t" << deltaP << "\t" << Lmax << "\t";
    outputFile << betaS << "\t" << betaW << "\t";
    outputFile << rhoS << "\t" << rhoW << "\t";
    outputFile << deltaS << "\t" << deltaW << "\t";
    outputFile << tauS << "\t" << tauW << "\t";
    outputFile << g << "\t" << tax << "\t"; 
    outputFile << gamm << "\t" << kappa << "\t";
    outputFile << lambda << "\t" << mu << "\t";
    outputFile << length << "\t";
    outputFile << w0 << "\t" << mS0 << "\t" << mW0 << "\t";
    outputFile << omega0 << "\t" << d0 << "\t" << theta0;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirstTax                                       */
/* Member function: saveParamsetWithNames                                     */
/* Purpose:         add  parameterset to printfile                            */
/* Last modified:                                   */
/*                                                                            */
/******************************************************************************/
void laborMarketFirstTax::saveParamsetWithNames(QTextStream& outputFile)
{
    outputFile << "LaborMarketFirstTax:\n\t";
    outputFile << "A = " << A << "\tB = " << B << "\tdeltaP = " << deltaP << "\tLmax = " << Lmax << "\tbetaS = ";
    outputFile << betaS << "\tbetaW = " << betaW << "\trhoS = ";
    outputFile << rhoS << "\trhoW = " << rhoW << "\tdeltaS = ";
    outputFile << deltaS << "\tdeltaW = " << deltaW << "\ttauS = ";
    outputFile << tauS << "\ttauW = " << tauW << "\tg = ";
    outputFile << g << "\ttax = " << tax << "\tgamma = "; 
    outputFile << gamm << "\tkappa = " << kappa << "\tlambda = ";
    outputFile << lambda << "\tmu = " << mu << "\tlength = ";
    outputFile << length << "\tw0 = ";
    outputFile << w0 << "\tmS0 = " << mS0 << "\tmW0 = " << mW0 << "\tomega0 = ";
    outputFile << omega0 << "\td0 = " << d0 << "\ttheta0 = " << theta0;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirstTax                                       */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   18.03.1996 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void laborMarketFirstTax::printParamset()
{
    log() << A  << "\t" << B << "\t" << deltaP << "\t" << Lmax;
    log() << betaS << "\t" << betaW << "\t" << rhoS << "\t" << rhoW;
    log() << deltaS << "\t" << deltaW << "\t" << tauS << "\t" << tauW;
    log() << g << "\t" << tax;
    log() << gamm << "\t" << kappa << "\t" << lambda << "\t" << mu;
    log() << length;
    log() << w0 << "\t" << mS0 << "\t" << mW0 << "\t" << omega0 << "\t";
    log() << d0 << "\t" << theta0;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirstTax                                       */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   18.03.1996 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void laborMarketFirstTax::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=25;
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
    (*parameters)[13]=tax;
    (*parameters)[14]=gamm;
    (*parameters)[15]=kappa;
    (*parameters)[16]=lambda;
    (*parameters)[17]=mu;
    (*parameters)[18]=length;
    (*parameters)[19]=w0;
    (*parameters)[20]=mS0;
    (*parameters)[21]=mW0;
    (*parameters)[22]=omega0;
    (*parameters)[23]=d0;
    (*parameters)[24]=theta0;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirstTax                                       */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   18.03.1996 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void laborMarketFirstTax::receiveParameters(const QList<qreal>& parameters)
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
    tax=parameters[13];
    gamm=parameters[14];
    kappa=parameters[15];
    lambda=parameters[16];
    mu=parameters[17];
    length=(qint64)(parameters[18]);
    w0=parameters[19];
    mS0=parameters[20];
    mW0=parameters[21];
    omega0=parameters[22];
    d0=parameters[23];
    theta0=parameters[24];
}
/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirstTax                                       */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:   18.03.1996 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
qreal* laborMarketFirstTax::setLabels(const QString& name)
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
    if (name == "tax")
        return( &tax );

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

    if (name == "wtreal")
        return( &wtreal );
    if (name == "mtrealS")
        return( &mtrealS );
    if (name == "mtrealW")
        return( &mtrealW );
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
/* Class name:      laborMarketFirstTax                                       */
/* Member function: setTax                                                    */
/* Purpose:         taxS,taxW is set by tax                                   */
/* Last modified:   18.03.1996 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void laborMarketFirstTax::setTax()
{
	taxS = tax;
	taxW = tax;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      laborMarketFirstTax                                       */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system                */
/* Last modified:   06.03.1996 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void laborMarketFirstTax::iteration(const qint64& t)
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
     
  setTax();				//()
  ztnot=laborDemand();		        //(A,B,wtreal)
  employment=actualEmployment(ztnot);	//(ztnot,Lmax)
  sigmaL=detSigmaL(ztnot);		//(ztnot,Lmax,employment)
  wtrate=detWtRate(sigmaL);		//(sigmaL,Lmax,employment,lambda,mu)
  ptrateexW=expectedInflationRateW(t);  //(t,tauW,theta)
  cpsW=consumptionPropensityW(ptrateexW);//(ptrateexW,rhoTildaW,deltaW)
  ytW=demandYoungW(cpsW);	        //(cpsW,taxW,wtreal,employment)
  ptrateexS=expectedInflationRateS(t);  //(t,tauS,theta)                
  cpsS=consumptionPropensityS(ptrateexS);//(ptrateexS,rhoTildaS,deltaS)
  ytS=demandYoungS(cpsS);	        //(cpsS,taxS,dt)
  ytD=aggregateDemand(ytW,ytS);        //(betaW,betaS,mtrealW,mtrealS,g,ytW,ytS)
  yteff=productionFunction(employment);	//(A,B,employment,deltaP,omagat)
  output=actualOutput(ytD,yteff);	//(ytD,yteff)
  xYoung=remainingOutputYoung();        //(output,g,betaW,betaS,mtrealW,mtrealS)
  xtS=actualConsumptionYoungS(ytW,ytS,xYoung);//(ytW,ytS,xYoung)
  xtW=actualConsumptionYoungW(ytW,ytS,xYoung);//(ytW,ytS,xYoung)
  sigmaC=detSigmaC(yteff,ytD);	        //(yteff,ytD,output)
  ptrate=actualInflationrate(sigmaC,ytD);//(sigmaC,ytD,gamm,kappa)

  dynamics(yteff,xtS,xtW);
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rlaborMarketFirstTax
// Member function:     rlaborMarketFirstTax
// Purpose:             constructor
//
// Author:              Uli Middelberg
// Last modified:       Fri Jul 25 17:00:06 1997
// By:                  Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

rlaborMarketFirstTax::rlaborMarketFirstTax() : laborMarketFirstTax()
{
  zvar = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rlaborMarketFirstTax
// Member function:     iteration
// Purpose:             perform one iteration of the system
//
// Author:              Uli Middelberg
// Last modified:       Fri Jul 25 16:59:54 1997
// By:                  Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void rlaborMarketFirstTax::iteration(const qint64& t)
{
  * zvar_ptr = zvar->dice();	// Set a new random value for the Parameter
  laborMarketFirstTax::iteration(t);	// iterate as usual
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rlaborMarketFirstTax
// Member function:     loadParamset
// Purpose:             load a parameterset from a specified input file
//
// Author:              Uli Middelberg
// Last modified:       Fri Jul 25 16:59:31 1997
// By:                  Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void rlaborMarketFirstTax::loadParamset(QTextStream& inputFile)
{
  inputFile >> zvar_name;		// read the name of the stochastic parameter
  inputFile >> zvar_expr;		// read the definition
  laborMarketFirstTax::loadParamset(inputFile);// read the parameters for the default
  					// model as usual and initialize
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rlaborMarketFirstTax
// Member function:     initialize
// Purpose:             initialize the model, define the systems initial state
//
// Author:              Uli Middelberg
// Last modified:       Fri Jul 25 16:59:21 1997
// By:                  Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void rlaborMarketFirstTax::initialize()
{
    zvar_ptr = setLabels(zvar_name); 	// get a pointer to the parameter
    if ( zvar != NULL ) {		// delete the old zvar
      delete zvar;
    }
    
    zvar = new rand_var( this, "ranf", zvar_expr);
    					// initialize the random number generator

    laborMarketFirstTax::initialize();	// initialize the default model as usual
}

// eof
