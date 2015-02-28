//
//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/Duopol.C,v 1.1 2000/08/31 15:19:22 mmeyer Exp $
//
/******************************************************************************/
/*                                                                            */
/* Module name:     Duopol.C                                                  */
/* Contents:        Member functions of the class Duopol                      */
/*                                                                            */
/* Last modified:   Thu Oct  3 13:55:56 MEZ 1996  (Leo Kaas)                  */
/*                                                                            */
/******************************************************************************/

#include "Duopol.h"
#include "../logger.h"
// extern void fatalError(const char*, const char*);


/******************************************************************************/
/*                                                                            */
/* Class name:      Duopol                                                    */
/* Member function: Duopol                                                    */
/* Purpose:         constructor / destructor                                  */
/* Last modified:   Thu     Oct  3 13:55:56 MEZ 1996  (Leo Kaas)              */
/*                                                                            */
/******************************************************************************/

Duopol::Duopol() : baseModel(6)
{
}

Duopol::~Duopol()
{
}


/******************************************************************************/
/*                                                                            */
/* Class name:      Duopol                                                    */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, define the systems initial state    */
/* Last modified:   Thu     Oct  3 13:55:56 MEZ 1996  (Leo Kaas)              */
/*                                                                            */
/******************************************************************************/

void Duopol::initialize()
{
    zeta1=pow(beta10,1/eta1)/w0;
    zeta2=pow(beta20,1/eta2)/w0;
    z1=z10;
    z2=z20;
    m=M0/w0;
    pi=Pi0/w0;
}
    

/******************************************************************************/
/*                                                                            */
/* Class name:      Duopol                                                    */
/* Member function: sendModelVar                                              */
/* Purpose:         return a pointer to the main model variable               */
/* Last modified:   Thu     Oct  3 13:55:56 MEZ 1996  (Leo Kaas)              */
/*                                                                            */
/******************************************************************************/


qreal* Duopol::sendModelVar()
{
    return &m;
}

 

/******************************************************************************/
/*                                                                            */
/* Class name:      Duopol                                                    */
/* Member function: setLabels                                                 */
/* Purpose:         return a pointer to a variable or a parameter specified   */
/*                  by its name                                               */
/* Last modified:   Thu     Oct  3 13:55:56 MEZ 1996  (Leo Kaas)              */
/*                                                                            */
/******************************************************************************/

qreal* Duopol::setLabels(const QString& label)
{
    if (label == "xBundle")
	return &xBundle;
    if (label == "yBundle")
	return &yBundle;
    if (label == "g")
	return( &g);
    if (label == "tau")
	return( &tau);
    if (label == "B1")
	return( &B1);
    if (label == "B2")
	return( &B2);
    if (label == "eta1")
	return( &eta1);
    if (label == "eta2")
	return( &eta2);
    if (label == "alpha1")
	return( &alpha1);
    if (label == "alpha2")
	return( &alpha2);
    if (label == "lambda1")
	return( &lambda1);
    if (label == "lambda2")
	return( &lambda2);
    if (label == "Lmax")
    return( &Lmax);
    if (label == "mu")
	return( &mu);			
    if (label == "dep")
	return( &dep);
    if (label == "rho")
	return( &rho);
    if (label == "delta")
	return( &delta);
    if (label == "zeta1")
	return( &zeta1);
    if (label == "zeta2")
	return( &zeta2);
    if (label == "z1")
	return( &z1);
    if (label == "z2")
	return( &z2);
    if (label == "m")
	return( &m);	
    if (label == "pi")
	return( &pi);	
    if (label == "beta10")
	return( &beta10);
    if (label == "beta20")
	return( &beta20);
    if (label == "z10")
	return( &z10 );
    if (label == "z20")
	return( &z20 );
    if (label == "M0")
	return( &M0);
    if (label == "w0")
	return( &w0 );
    if (label == "Pi0")
	return( &Pi0 );
    return NULL;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      Duopol                                                    */
/* Member function: sendStateSpace                                            */
/* Purpose:         return pointers to the state variables and inform about   */
/*                  the systems dimension                                     */
/* Last modified:   Thu     Oct  3 13:55:56 MEZ 1996  (Leo Kaas)              */
/*                                                                            */
/******************************************************************************/

void Duopol::sendStateSpace(int &quantity,QList<qreal *> *stateSpace)
{
    stateSpace->clear();
    quantity = dimension;
    *stateSpace << &zeta1;
    *stateSpace << &zeta2;
    *stateSpace << &z1;
    *stateSpace << &z2;
    *stateSpace << &m;
    *stateSpace << &pi;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      Duopol                                                    */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   Thu     Oct  3 13:55:56 MEZ 1996  (Leo Kaas)              */
/*                                                                            */
/******************************************************************************/

 void Duopol::loadParamset(QTextStream& inFile)
{
    inFile >> beta10 >> beta20 >> z10 >> z20;
    inFile >> M0 >> w0 >> Pi0;
    inFile >> g >> tau >> B1 >> B2; 
    inFile >> eta1 >> eta2 >> alpha1 >> alpha2;
    inFile >> lambda1 >> lambda2 >> Lmax >> mu >> dep;
    inFile >> rho >> delta >> cert >> length;
    
    initialize();
}


/******************************************************************************/
/*                                                                            */
/* Class name:      Duopol                                                    */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   Thu     Oct  3 13:55:56 MEZ 1996  (Leo Kaas)              */
/*                                                                            */
/******************************************************************************/

void Duopol::saveParamset(QTextStream& outFile)
{
    outFile << beta10 << "\t" << beta20 << "\t" << z10 << "\t" << z20 << "\t";
    outFile << M0 << "\t" << w0 << "\t" << Pi0 << "\t";
    outFile << g << "\t" << tau << "\t" << B1 << "\t" << B2 << "\t";
    outFile << eta1 << "\t" << eta2 << "\t" << alpha1 << "\t" << alpha2 << "\t";    
    outFile << lambda1 << "\t" << lambda2 << "\t" << Lmax << "\t" << mu << "\t";
    outFile << dep << "\t" << rho << "\t" << delta << "\t";
    outFile << cert << "\t" << length;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      Duopol                                                    */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   Thu     Oct  3 13:55:56 MEZ 1996  (Leo Kaas)              */
/*                                                                            */
/******************************************************************************/

void Duopol::printParamset()
{
    log() << beta10 << "\t" << beta20 << "\t" << z10 << "\t" << z20;
    log() << M0 << "\t" << w0 << "\t" << Pi0;
    log() << g << "\t" << tau << "\t" << B1 << "\t" << B2;
    log() << eta1 << "\t" << eta2 << "\t" << alpha1 << "\t" << alpha2;
    log() << lambda1 << "\t" << lambda2 << "\t" << Lmax << "\t" << mu;
    log() << dep << "\t" << rho << "\t" << delta;
    log() << cert << "\t" << length;

}


/******************************************************************************/
/*                                                                            */
/* Class name:      Duopol                                                    */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   Thu     Oct  3 13:55:56 MEZ 1996  (Leo Kaas)              */
/*                                                                            */
/******************************************************************************/

void Duopol::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=14;
    *parameters=new qreal[amount];
    if( !parameters )
	fatalError("Duopol::sendParameters",
		   "Can't create array for parameters");
    (*parameters[0])=beta10;
    (*parameters[1])=beta20;
    (*parameters[2])=z10;
    (*parameters[3])=z20;
    (*parameters[4])=M0;
    (*parameters[5])=w0;
    (*parameters[6])=Pi0;
    (*parameters[7])=g;
    (*parameters[8])=tau;
    (*parameters[9])=B1;
    (*parameters[10])=B2;
    (*parameters[11])=eta1;
    (*parameters[12])=eta2;
    (*parameters[13])=alpha1;
    (*parameters[14])=alpha2;
    (*parameters[15])=lambda1;
    (*parameters[16])=lambda2;
    (*parameters[17])=Lmax;
    (*parameters[18])=mu;
    (*parameters[19])=dep;
    (*parameters[20])=rho;
    (*parameters[21])=delta;
    (*parameters[22])=cert;
    (*parameters[23])=length;
    
}


/******************************************************************************/
/*                                                                            */
/* Class name:      Duopol                                                    */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   Thu     Oct  3 13:55:56 MEZ 1996  (Leo Kaas)              */
/*                                                                            */
/******************************************************************************/

void Duopol::receiveParameters(const QList<qreal>& parameters)
{
    beta10=parameters[0];
    beta20=parameters[1];
    z10=parameters[2];
    z20=parameters[3];
    M0=parameters[4];
    w0=parameters[5];
    Pi0=parameters[6];
    g=parameters[7];
    tau=parameters[8];
    B1=parameters[9];
    B2=parameters[10];
    eta1=parameters[11];
    eta2=parameters[12];
    alpha1=parameters[13];
    alpha2=parameters[14];
    lambda1=parameters[15];
    lambda2=parameters[16];
    Lmax=parameters[17];
    mu=parameters[18];
    dep=parameters[19];
    rho=parameters[20];
    delta=parameters[21];
    cert=parameters[22];
    length=(qint64)(parameters[23]);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      Duopol                                                    */
/* Member function: FF                                                        */
/* Purpose:         production function, computes the output for a given      */
/*                  employment level                                          */
/* Last modified:   Thu     Oct  3 13:55:56 MEZ 1996  (Leo Kaas)              */
/*                                                                            */
/******************************************************************************/

qreal Duopol::FF(const qreal L, const int firm)
{
	qreal result;
   qreal B;
   if(firm == 1)
      B=B1;
   else
      B=B2;
	result= 1/B *pow(L,B);              
	return(result);
}




/******************************************************************************/
/*                                                                            */
/* Class name:      Duopol                                                    */
/* Member function: Finv                                                      */
/* Purpose:         inverse production function, computes the employment      */
/*                  level needed to produce a given output                    */
/* Last modified:   Thu     Oct  3 13:55:56 MEZ 1996  (Leo Kaas)              */
/*                                                                            */
/******************************************************************************/

qreal Duopol::Finv(const qreal y, const int firm)
{
	qreal result;
   qreal B;
   if(firm == 1)
      B=B1;
   else
      B=B2;
	result= pow((B*y),(1/B));             
	return(result);
}


/******************************************************************************/
/*                                                                            */
/* Class name:      Duopol                                                    */
/* Member function: nullst                                                    */
/* Purpose:         zeros of nullst are solutions of Gamma defined below      */
/*                                                                            */
/* Last modified:   Thu     Oct  3 13:55:56 MEZ 1996  (Leo Kaas)              */
/*                                                                            */
/******************************************************************************/

qreal Duopol::nullst(const qreal gamc, const qreal alpha, const qreal eta)
		{
		return (pow((1+gamc),alpha)-gamc*(alpha*eta-1)/(eta-1))-1;
		}
		

/******************************************************************************/
/*                                                                            */
/* Class name:      Duopol                                                    */
/* Member function: Gamma                                                     */
/* Purpose:         computes parameter Gamma for the solution of the          */
/*                  duopolist's problem                                       */
/* Last modified:   Thu     Oct  3 13:55:56 MEZ 1996  (Leo Kaas)              */
/*                                                                            */
/******************************************************************************/

		
qreal Duopol::Gamma(const int firm)
{	
    qreal eta; qreal alpha;
    qreal result, gamit, eps, h; 
    int NqMax;

    if(firm == 1)
      	{
      	eta=eta1;
      	alpha=alpha1;
      	}
    else
    	{
    	eta=eta2;
    	alpha=alpha2;
    	}
    	
if(cert == 1)
{
result=1/(alpha-1);
}
else
{
	eps=0.000001;               // accuracy of Newton algorithm 
	h= 0.000001;                // step size for approximation of derivatives
    NqMax=1000;                  // qMaximal number of Newton iterations
   	if(alpha == 2)
		result=1/(eta-1);
	else if(alpha == 3)
		result=0.5*(sqrt((9*eta-1)/(eta-1))-3);
	else
		{
		gamit = pow((alpha*eta-1)/(eta-1),1/(alpha-1))-1;
        for (int i=1;(i<NqMax && (pow(nullst(gamit,alpha,eta),2) > pow(eps,2))) ;i++)
			{
			gamit=gamit-h*nullst(gamit,alpha,eta)/(nullst(gamit+h,alpha,eta)
					-nullst(gamit,alpha,eta));
			if(gamit <= 0) break;
			}		
		if(pow(nullst(gamit,alpha,eta),2) > eps || gamit <= 0)
			gamit=1/(eta-1);	
		result = gamit;
	    }          
}
	return(result);
	
}


/******************************************************************************/
/*                                                                            */
/* Class name:      Duopol                                                    */
/* Member function: cpar                                                      */
/* Purpose:         computes parameter cpar for the solution of the           */
/*                  duopolist's problem                                       */
/* Last modified:   Thu     Oct  3 13:55:56 MEZ 1996  (Leo Kaas)              */
/*                                                                            */
/******************************************************************************/

qreal Duopol::cpar(const int firm)
{
	qreal result;
	qreal gam;
   qreal eta;
   qreal alpha;

   if(firm == 1)
      {
      eta=eta1;
      alpha=alpha1;
      }
   else
      {
      eta=eta2;
      alpha=alpha2;
      }       	
	
   gam=Gamma(firm);
   
   if(cert == 1)
   {
        result = pow(gam,1/eta);
   }
   else
   {    
   	result = exp((1-eta)/eta*log(gam))*(1/(alpha-1)+exp(alpha*log(1/(1+gam))
	          )*(1+alpha*gam)/(1-alpha))+exp(log(gam)/eta)*exp(alpha*log(
	          1/(1+gam)));
   }	                        
	return(result);
}

 
/******************************************************************************/
/*                                                                            */
/* Class name:      Duopol                                                    */
/* Member function: xnot                                                      */
/* Purpose:         computes the notional production level of the             */
/*                  duopolist                                                 */
/* Last modified:   Thu     Oct  3 13:55:56 MEZ 1996  (Leo Kaas)              */
/*                                                                            */
/******************************************************************************/

qreal Duopol::xnot(const int firm)
{
	qreal result;
	qreal cpa;
    qreal eta;
    qreal B;
    qreal zeta;
	
	cpa = cpar(firm);
   if(firm == 1)
      {
      eta=eta1;
      B=B1;
      zeta=zeta1;
      }
   else
      {
      eta=eta2;
      B=B2;
      zeta=zeta2;
      }
	
   result = pow((pow(B,1/eta)*zeta*cpa*(1-1/eta)),(B*eta/(eta*(1-B)+B)))/B;
   
/*  ACHTUNG: Ergebnis gilt nur fuer z=0 !!! */
         
	return(result);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      Duopol                                                    */
/* Member function: rewa                                                      */
/* Purpose:         computes the duopolist's qreal wage at given output level  */
/*                                                                            */
/* Last modified:   Thu     Oct  3 13:55:56 MEZ 1996  (Leo Kaas)              */
/*                                                                            */
/******************************************************************************/

qreal Duopol::rewa(const qreal x, const int firm)
{
	qreal result;
	qreal gam;
   qreal zeta;
   qreal eta;
   if(firm == 1)
      {
      eta=eta1;
      zeta=zeta1;
      }
   else
      {
      eta=eta2;
      zeta=zeta2;
      }
	gam=Gamma(firm);
	result = pow((x/gam),(1/eta))/zeta;              
	return(result);
}


/******************************************************************************/
/*                                                                            */
/* Class name:      Duopol                                                    */
/* Member function: cprop                                                     */
/* Purpose:         marginal propensity to consume of young households        */
/*                                                                            */
/* Last modified:   Thu     Oct  3 13:55:56 MEZ 1996  (Leo Kaas)              */
/*                                                                            */
/******************************************************************************/

qreal Duopol::cprop(const qreal theta)
{
	qreal result;
	result= 1/(1+pow(delta,rho)*pow(theta,(1-rho)));              
	return(result);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      Duopol                                                    */
/* Member function: iteration                                                 */
/* Purpose:         perforn one iteration                                     */
/*                  Duopol: consumption of young and old households, profits  */
/*                          are distributed to the next young generation      */  
/* Last modified:   Thu     Oct  3 13:55:56 MEZ 1996  (Leo Kaas)              */
/*                                                                            */
/******************************************************************************/


/*  Vorsicht: Alte Version: Definition von zeta stimmt nicht mehr !  */

void Duopol::iteration(const qint64&)
{
   qreal Ltdem1;          //  notional labor demand firm 1
   qreal Ltdem2;          //  notional labor demand firm 2
   qreal Ltdem;           //  aggregate labor demand
   qreal Lt1;		 //  actual employment level firm 1
   qreal rewa1;		 //  qreal wage of firm 1
   qreal xtS1;       //  supply of firm 1
   qreal Lt2;		 //  actual employment level firm 2
   qreal rewa2;		 //  price of firm 2
   qreal xtS2;       //  supply of firm 2
   qreal gam1;       //  constant Gamma of firm 1
   qreal gam2;       //  constant Gamma of firm 2    

   qreal it;        // qreal (per labor unit) income of young household
   qreal ctdY1;     // demand y.h. at firm 1 
   qreal ctdY2;     // demand y.h. at firm 2
   qreal ctdO1;     // demand o.h. at firm 1
   qreal ctdO2;     // demand o.h. at firm 2
   qreal gtd1;      // demand gov. at firm 1
   qreal gtd2;      // demand gov. at firm 2

   qreal ytdem1;		 //  agg. goods demand at firm 1
   qreal ytdem2;		 //  agg. goods demand at firm 2

   qreal ctY1;      // consumption y.h. at firm 1
   qreal ctY2;      // consumption y.h. at firm 2
   qreal ctO1;      // consumption o.h. at firm 1 
   qreal ctO2;      // consumption o.h. at firm 2
   qreal gt1;       // consumption gov. at firm 1
   qreal gt2;       // consumption gov. at firm 2
   qreal xt1;       // trade firm 1
   qreal xt2;       // trade firm 2
 

   
/*   labor market   */

   Ltdem1= Finv(qMax(0.0,xnot(1)-z1),1);
   Ltdem2= Finv(qMax(0.0,xnot(2)-z2),2);
   Ltdem=Ltdem1+Ltdem2;
   if(Ltdem != 0)
      {
      Lt1=Ltdem1*qMin(Lmax/Ltdem,1.0);
      Lt2=Ltdem2*qMin(Lmax/Ltdem,1.0);
      }
   else
      {
      Lt1=0;
      Lt2=0;
      }
   xtS1= FF(Lt1,1)+z1;    //  supply firm 1 ( >0 !!!) 
   xtS2= FF(Lt2,2)+z2;    //  supply firm 2 ( >0 !!!)
   it=(1-tau)*((Lt1+Lt2)+pi);

/*   goods market    */
   rewa1=rewa(xtS1,1);
   rewa2=rewa(xtS2,2);
   if((rewa1 > rewa2))
      {
      /*  market 1    */
      ctdY1=cprop(1)*it*rewa1;          
      ctdO1=m*rewa1;                    
      gtd1=g;                         
      ytdem1=ctdY1+ctdO1+gtd1;        
      ctY1=ctdY1*qMin(1.0,xtS1/ytdem1);
      ctO1=ctdO1*qMin(1.,xtS1/ytdem1);
      gt1=gtd1*qMin(1.0,xtS1/ytdem1);
      xt1 = qMin(xtS1,ytdem1);

      /*  market 2    */
      ctdY2 = qMax(0.0,cprop(rewa1/rewa2)*(it*rewa2-(1+rewa2/rewa1)*ctY1));
      ctdO2 = qMax(0.0,(m-ctO1/rewa1)*rewa2);
      gtd2 = qMax(0.0,g-gt1);
      ytdem2=ctdY2+ctdO2+gtd2;
      xt2 = qMin(xtS2,ytdem2);
      if(ytdem2 != 0)
         ctY2=ctdY2*qMin(1.0,xtS2/ytdem2);
      else
         ctY2=0;
      }
   else if(rewa1 < rewa2)
      {
      /*   market 2    */
      ctdY2=cprop(1)*it*rewa2;
      ctdO2=m*rewa2;
      gtd2=g;
      ytdem2=ctdY2+ctdO2+gtd2;
      ctY2=ctdY2*qMin(1.0,xtS2/ytdem2);
      ctO2=ctdO2*qMin(1.0,xtS2/ytdem2);
      gt2=gtd2*qMin(1.0,xtS2/ytdem2);
      xt2 = qMin(xtS2,ytdem2);

      /*    market 1    */
      ctdY1 = qMax(0.0,cprop(rewa2/rewa1)*(it*rewa1-(1+rewa1/rewa2)*ctY2));
      ctdO1 = qMax(0.0,(m-ctO2/rewa2)*rewa1);
      gtd1 = qMax(0.0,g-gt2);
      ytdem1=ctdY1+ctdO1+gtd1;
      xt1 = qMin(xtS1,ytdem1);
      if(ytdem1 != 0)
         ctY1=ctdY1*qMin(1.0,xtS1/ytdem1);
      else
         ctY1=0;
      }
   else
      {
      ctdY1=cprop(1)*it*rewa1;
      ctdO1=m*rewa1;
      gtd1=g;
      ytdem1=xtS1*(ctdY1+ctdO1+gtd1)/(xtS1+xtS2);
      ytdem2=xtS2*(ctdY1+ctdO1+gtd1)/(xtS1+xtS2);
      ctY1=ctdY1*xtS1/qMax((ctdY1+ctdO1+gtd1),(xtS1+xtS2));
      ctY2=ctdY1*xtS2/qMax((ctdY1+ctdO1+gtd1),(xtS1+xtS2));
      xt1=xtS1*qMin(1.0,((ctdY1+ctdO1+gtd1)/(xtS1+xtS2)));
      xt2=xtS2*qMin(1.0,((ctdY1+ctdO1+gtd1)/(xtS1+xtS2)));
      }
       
/*   Iteration   */
    gam1 = Gamma(1);
    gam2 = Gamma(2);

    zeta1=(alpha1-1)/lambda1/alpha1*zeta1*(1+gam1*ytdem1/xtS1)/
            pow((1+mu*(Ltdem-Lmax)/qMax(Ltdem,Lmax)) ,eta1);
    zeta2=(alpha2-1)/lambda2/alpha2*zeta2*(1+gam2*ytdem2/xtS2)/
            pow((1+mu*(Ltdem-Lmax)/qMax(Ltdem,Lmax)) ,eta2);
    z1 = qMax(0.0,dep*(xtS1-ytdem1));
    z2 = qMax(0.0,dep*(xtS2-ytdem2));
    m=(it-ctY1/rewa1-ctY2/rewa2)/(1+mu*(Ltdem-Lmax)/qMax(Ltdem,Lmax));
    pi=(qMax(0.0,xt1/rewa1-Lt1)+qMax(0.0,xt2/rewa2-Lt2))/
            (1+mu*(Ltdem-Lmax)/qMax(Ltdem,Lmax));
}

/******************************************************************************/
/*                                                                            */
/* Class name:      Duopol2                                                   */
/* Member function: iteration                                                 */
/* Purpose:         perform one iteration                                     */
/*                  Duopol2: No consumption of young generation, profits are  */
/*                           distributed to old generation                    */ 
/* Last modified:   Thu     Oct  3 13:55:56 MEZ 1996  (Leo Kaas)              */
/*                                                                            */
/******************************************************************************/

void Duopol2::iteration(const qint64&)
{
   qreal Ltdem1;          //  notional labor demand firm 1
   qreal Ltdem2;          //  notional labor demand firm 2
   qreal Ltdem;           //  aggregate labor demand
   qreal Lt1;		 //  actual employment level firm 1
   qreal rewa1;		 //  qreal wage of firm 1
   qreal xtS1;       //  supply of firm 1
   qreal Lt2;		 //  actual employment level firm 2
   qreal rewa2;		 //  qreal wage of firm 2
   qreal xtS2;       //  supply of firm 2
   qreal gam1;       //  constant Gamma of firm 1 
   qreal gam2;       //  constant Gamma of firm 2
    
   qreal ctd1;     // demand o.h. at firm 1
   qreal ctd2;     // demand o.h. at firm 2
   qreal gtd1;      // demand gov. at firm 1
   qreal gtd2;      // demand gov. at firm 2

   qreal ytdem1;		 //  agg. goods demand at firm 1
   qreal ytdem2;		 //  agg. goods demand at firm 2

   qreal ct1;      // consumption o.h. at firm 1 
   qreal ct2;      // consumption o.h. at firm 2
   qreal gt1;       // consumption gov. at firm 1
   qreal gt2;       // consumption gov. at firm 2
   qreal xt1;       // trade firm 1
   qreal xt2;       // trade firm 2
 
/*   labor market   */

   Ltdem1= Finv(qMax(0.0,xnot(1)-z1),1);
   Ltdem2= Finv(qMax(0.0,xnot(2)-z2),2);
   Ltdem=Ltdem1+Ltdem2;
   if(Ltdem != 0)
      {
      Lt1=Ltdem1*qMin(Lmax/Ltdem,1.0);
      Lt2=Ltdem2*qMin(Lmax/Ltdem,1.0);
      }
   else
      {
      Lt1=0;
      Lt2=0;
      }
   xtS1= FF(Lt1,1)+z1;    //  supply firm 1 ( >0 !!!) 
   xtS2= FF(Lt2,2)+z2;    //  supply firm 2 ( >0 !!!)
  
/*   goods market    */
   rewa1=rewa(xtS1,1);
   rewa2=rewa(xtS2,2);
   if(rewa2 < rewa1)
      {
      /*  market 1    */
      ctd1=m*rewa1;                    
      gtd1=g;                         
      ytdem1=ctd1+gtd1;        
      ct1=ctd1*qMin(1.0,xtS1/ytdem1);
      gt1=gtd1*qMin(1.0,xtS1/ytdem1);
      xt1 = qMin(xtS1,ytdem1);

      /*  market 2    */
      ctd2 = qMax(0.0,(m-ct1/rewa1)*rewa2);
      gtd2 = qMax(0.0,g-gt1);
      ytdem2=ctd2+gtd2;
      xt2 = qMin(xtS2,ytdem2);
      if (ytdem2 != 0)
      		gt2=gtd2*xt2/ytdem2;
      else
      		gt2=0;		
      
      }
   else if (rewa1 < rewa2)
      {
      /*   market 2    */
      ctd2=m*rewa2;
      gtd2=g;
      ytdem2=ctd2+gtd2;
      ct2=ctd2*qMin(1.0,xtS2/ytdem2);
      gt2=gtd2*qMin(1.0,xtS2/ytdem2);
      xt2 = qMin(xtS2,ytdem2);

      /*    market 1    */
      ctd1 = qMax(0.0,(m-ct2/rewa2)*rewa1);
      gtd1 = qMax(0.0,g-gt2);
      ytdem1=ctd1+gtd1;
      xt1 = qMin(xtS1,ytdem1);
      if (ytdem1 != 0)
      		gt1=gtd1*xt1/ytdem1;
      else
      		gt1=0;
      }
   else
      {
      ctd1=m*rewa1;
      gtd1=g;
      ytdem1=xtS1*(ctd1+gtd1)/(xtS1+xtS2);
      ytdem2=xtS2*(ctd1+gtd1)/(xtS1+xtS2);
      xt1=xtS1*qMin(1.0,((ctd1+gtd1)/(xtS1+xtS2)));
      xt2=xtS2*qMin(1.0,((ctd1+gtd1)/(xtS1+xtS2)));
      gt1=gtd1*xt1/(ctd1+gtd1);
      gt2=gtd1*xt2/(ctd1+gtd1);
      }
      
/*   Iteration   */
    
    gam1 = Gamma(1);
    gam2 = Gamma(2);

    zeta1=zeta1*pow(((alpha1-1)/lambda1/alpha1*(1+gam1*ytdem1/xtS1)),1/eta1)/
            (1+mu*(Ltdem-Lmax)/qMax(Ltdem,Lmax));
    zeta2=zeta2*pow(((alpha2-1)/lambda2/alpha2*(1+gam2*ytdem2/xtS2)),1/eta2)/
            (1+mu*(Ltdem-Lmax)/qMax(Ltdem,Lmax));
    z1 = qMax(0.0,dep*(xtS1-ytdem1));
    z2 = qMax(0.0,dep*(xtS2-ytdem2));
    m=(m+(gt1-tau*xt1)/rewa1+(gt2-tau*xt2)/rewa2)/
             (1+mu*(Ltdem-Lmax)/qMax(Ltdem,Lmax));
            
}

//eof

