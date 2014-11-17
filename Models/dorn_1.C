// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/dorn_1.C,v 1.1 2000/08/31 15:48:06 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  dorn.C                                                       */
/* Contents:     Member functions of the class defaultModel                   */
/* Last Modified:                          				      */
/* Modified:          							      */
/*                                                                            */
/******************************************************************************/

#include "dorn.h"             
#include "../error.h"
#include <assert.h>

/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: dornModel                                                 */
/* Purpose:         constructor                                               */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/
dorn::dorn() : baseModel(4)
{
  p=NULL;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: ~dornModel                                                */
/* Purpose:         destructor                                                */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/
dorn::~dorn()
{
    if(p!=0)
      {
	delete [] p;
      }
}

/******************************************************************************/
/*                                                                            */
/* Class name:      dorn                                             	      */
/* Member function: pInit                                                     */
/* Purpose:         initialize the p-vector                                   */
/* Last modified:                                  			      */
/*                                                                            */
/******************************************************************************/
void dorn::pInit(real *p)
{
    int i;

    for( i=1; i<tau; i++ )
        p[i] = 0.0;
    p[0]=p0;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, set the initial state               */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/
void dorn::initialize()
{    
    pt=pe=p0;
    wt=w0;
    Bt=B0;
    st=s0;
    iet=ie0;
    xet=xe0;
    //    p[0]=p0;  

//    clog << "allocating p-field of size " << tau<<endl;

    pInit(p);

//    if (p!=0) { delete [] p; }
//
//    p=new real [tau];
//    p[0]=p0;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system (Figs. 7, 8)   */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void dorn::iteration(const long& t)
{ 
 labordemand ();
cout <<"zt="<< zt << endl;
 employment();
 production();
cout <<"yt="<< yt << endl;
 profit();
 income();
 pe = pexpBerechnung(t);
cout <<"pe="<< pe << endl;
 ExpectedInterestRate();
cout <<"iet="<< iet << endl;
 goodsDemand();
cout <<"xtdy="<< xtdy << endl;
 bondMarket();
cout <<"stv="<< stv << endl;
cout <<"st="<< st << endl;
cout <<"it="<< it << endl;
 aggregateDemand();
cout <<"ytd="<< ytd << endl;
 output();
 ExchangeRate();
cout <<"xet="<< xet << endl;
cout <<"xt="<< xt << endl;
 pBerechnung();
cout <<"pt="<< pt << endl;
 WageAdjustment();  
cout <<"wt="<< wt << endl;
 dynamics(p,t);
cout <<"tax="<<tax<<endl;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: dynamics                                                  */
/* Purpose:         describes the dynamics of the system (eqs. 2.28i-2.28iii) */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void dorn::dynamics(real *p,const long t)
{  
  assert(t>0);
  if (t>=tau)
    {
      for(int i=0; i<tau-1; ++i )
	{
	  p[i]=p[i+1]; 
	}
      p[tau-1]=pt;       	
    }   
  else
    {
      assert(t>0);
      p[t]=pt;
    }
cout << "p[0]-vector=" << p[0] << endl;
cout << "p[1]-vector=" << p[1] << endl;
cout << "p[2]-vector=" << p[2] << endl;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: labordemand                                               */
/*									      */	
/* Purpose:         computes the notional labor demand                        */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/
void dorn::labordemand()
{
zt = exp ( log((pt*A)/wt) / (1-B) );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: Employment                                                */        
/*            								      */
/* Purpose:         computes the actual employment                            */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/
void dorn::employment()
{
if ( Lmax > zt )
   Lt = zt;
else
   Lt = Lmax;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: Production                                                */ 
/*									      */
/* Purpose:         computes the actual production                            */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/
void dorn::production()
{
yt = (A/B) * exp( log(Lt) * B ) ;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: Profit                                                    */
/*									      */
/* Purpose:         computes the actual profit                                */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/
void dorn::profit()
{
Pit = pt * yt - wt * Lt;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: Income                                                    */
/*									      */
/* Purpose:         computes the net income                                   */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/
void dorn::income()
{
Ytnet = (1-tax) * (wt * Lt + Pit);
}
/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: ExpectedInflationRate                                     */  
/*									      */
/* Purpose:         computes the expected inflation rate                      */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/

real dorn::pexpBerechnung(const long t)
{
    long T, counter;
    real sum=0.0;
    
    assert(t>0);
    T=MIN(t,tau);
    for( counter=0; counter<T; counter++ )
      {
        sum += p[counter];
      }
    return (sum/T);
}
/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: ExpectedInterestRate                                      */
/*									      */
/* Purpose:         computes the expected interest rate                       */ 
/*									      */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/
void dorn::ExpectedInterestRate()
{
iet = pow ( ( exp ( log(delta)/(rho - 1))) / 
     ( 1- ( (1+iet)*st - d ) *
     ( ( Bt + deltaB )/pt ) * 1/( (1-tax)*yt ) ) -
     exp (log (delta)/(rho -1)),((1-rho)/rho))*
     (pe/pt)-1;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: GoodsDemand                                    	      */
/*									      */
/* Purpose:         computes the young household's goods demand               */
/*									      */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/
void dorn::goodsDemand()
{
xtdy = 1 / (
       1 + exp (log(delta)/(1-rho))*
       ( exp (log(pt)*(rho/(1-rho)))*
         pow((1+iet),(rho/(1-rho)))/
         (exp(log(pe)*(rho/(1-rho)))) ) ) * (1-tax)*yt;       
}
/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: BondMarket                                      	      */
/*									      */
/* Purpose:         computes the equilibrium bond price and effective         */
/*                  interest rate                                             */
/*									      */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/
void dorn::bondMarket()
{
stv = st;

st = pt * (1-tax) * yt / (Bt + deltaB)* (1-
1 / (
       1 + exp (log(delta)/(1-rho))*
       ( exp (log(pt)*(rho/(1-rho)))*
         pow((1+iet),(rho/(1-rho)))/
         (exp(log(pe)*(rho/(1-rho)))) ) ) );

it = ( (d + st) / stv) - 1;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: AggregateDemand                                           */
/*									      */
/* Purpose:         computes the aggregate goods demand                       */
/*									      */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/
void dorn::aggregateDemand()
{
ytd = xtdy + g + (Bt*st)/pt + (Bt*d)/pt;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: Output                                     		      */
/*									      */
/* Purpose:         computes the output and export/import                     */
/*									      */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/
void dorn::output()
{
if ( yt > ytd ) 
  {
    Yt = ytd;
  }
else
  {
    Yt = yt;
  }

deltayt = ytd - yt;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: ExchangeRate                                              */
/*									      */
/* Purpose:         computes the expected and the actual exchange rate;       */
/*									      */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/
void dorn::ExchangeRate()
{
xet = xet * (1+iet) / ( 1+ fi);

xt = xet * (1+fi) / (1+iet);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: PriceAdjustment                                           */
/*									      */
/* Purpose:         computes the adjustment of the goods price                */
/*									      */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/
void dorn::pBerechnung()
{
if (deltayt > 0)
   pt = pt *( 1 + gamma * deltayt / ytd);
else
   pt = pt *( 1 + kappa * deltayt / yt);   
}

/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: WageAdjustment                                            */
/*									      */
/* Purpose:         computes the adjustment of the wages                      */
/*									      */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/
void dorn::WageAdjustment()
{ 
if ( Lmax > zt)
   wt = wt *( 1 + lambda * (zt - Lmax) / Lmax);
else
   wt = wt *( 1 + mu * (zt - Lmax) / zt); 
}
/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:                                			      */
/*                                                                            */
/******************************************************************************/

real* dorn::setLabels(char *name)
{
    if( !strcmp(name,"tau") )
	return( (real*)(&tau) );
    if( !strcmp(name,"pt") )
	return( (real*)(&pt) );
    if( !strcmp(name,"wt") )
	return( (real*)(&wt) );
    if( !strcmp(name,"st") )
	return( (real*)(&st) );
    if( !strcmp(name,"stv") )
	return( (real*)(&stv) );
    if( !strcmp(name,"xt") )
	return( (real*)(&xt) );
    if( !strcmp(name,"it") )
	return( (real*)(&it) );
    if( !strcmp(name,"Bt") )
	return( (real*)(&Bt) );
    if( !strcmp(name,"Lt") )
	return( (real*)(&Lt) );
    if( !strcmp(name,"yt") )
	return( (real*)(&yt) );
    if( !strcmp(name,"iet") )
	return( (real*)(&iet) );
    if( !strcmp(name,"Lmax") )
	return( (real*)(&Lmax) );
    if( !strcmp(name,"tax") )
	return( (real*)(&tax) );
    if( !strcmp(name,"d") )
	return( (real*)(&d) );
    if( !strcmp(name,"deltaB") )
	return( (real*)(&deltaB) );
    if( !strcmp(name,"pe") )
	return( (real*)(&pe) );
    if( !strcmp(name,"g") )
	return( (real*)(&g) );
    if( !strcmp(name,"deltayt") )
	return( (real*)(&deltayt) );
    if( !strcmp(name,"fi") )
	return( (real*)(&fi) );
    if( !strcmp(name,"xet") )
	return( (real*)(&xet) );
    if( !strcmp(name,"B") )
	return( (real*)(&B) );
    return( NULL );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: sendStateSpace                                            */
/* Purpose:         returns pointers to the real balances and the real wage;  */
/*                  returns the dimension of the system for rho=0             */
/* Last modified:                               			      */
/*                                                                            */
/******************************************************************************/

void dorn::sendStateSpace(int &quantity,const real*** stateSpace)
{
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const real* [dimension];
    if( !(*stateSpace) )
	fatalError("defaultModel::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&pt;
    (*stateSpace)[1]=&wt;
    (*stateSpace)[2]=&Bt;
    (*stateSpace)[3]=&st;
}
    
/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:                            			              */
/*                                                                            */
/******************************************************************************/

void dorn::loadParamset(ifstream& inputFile)
{
    inputFile >> tau >> p0 >> w0 >> s0 >>  B0 
              >> A >> B >> Lmax >> tax >> delta >> rho >> d
              >> deltaB >> g >> fi
              >> ie0 >> xe0
              >> gamma >> kappa >> lambda >> mu >> length;
    if( p )
	delete p;
    p = new real[tau];
    if( !p )
	fatalError("defaultModel::loadParamset","Can't create p vector");
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

void dorn::saveParamset(ofstream& outputFile)
{
    outputFile << tau << "\t" << p0 << "\t" << w0 << "\t" << s0 << "\t"
               << B0 << "\t" << A << "\t" << B << "\t" << Lmax << "\t" << tax << "\t"
               << delta << "\t" << rho << "\t" << d << "\t" << deltaB << "\t"  
               << g << "\t" << fi << "\t" << ie0 << "\t" << xe0 << "\t"
               << gamma << "\t" << kappa << "\t" << lambda << "\t"<< mu<< "\t"
	       << length << "\t";
}
/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: saveParamsetWithNames                                     */
/* Purpose:         add  parameterset to printfile                            */
/* Last modified:   Tue Jul 21 12:58:13 METDST 1998  Marc Mueller             */
/*                                                                            */
/******************************************************************************/

void dorn::saveParamsetWithNames(ofstream& outputFile)
{
    outputFile << "dornModel:\n\t";
    outputFile << "Tau = " << tau 
               << "\n\tp_0 = " << p0   
               << "\n\tw_0 = " << w0
               << "\n\ts_0 = " << s0
               << "\n\tB0 = " << B0
               << "\n\tA = " << A
               << "\n\tB = " << B
               << "\n\tLmax = " << Lmax
               << "\n\ttax = " << tax
               << "\n\tdelta = " << delta
               << "\n\trho = " << rho
               << "\n\td = " << d
               << "\n\tdeltaB = " << deltaB
               << "\n\tg = " << g
               << "\n\tfi = " << fi
               << "\n\tie0 = " << ie0
               << "\n\txe0 = " << xe0
               << "\n\tgamma = " << gamma
               << "\n\tkappa = " << kappa
               << "\n\tlambda = " << lambda
               << "\n\tmu = " << mu
               << "\n\tlength = " << length;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   09.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void dorn::printParamset()
{
    cout << tau << "\t" << p0 << "\t" << w0 << "\t" << s0 << "\t"
         << B0 << "\t" << A << "\t" << B << "\t" << Lmax << "\t" << tax << "\t"
         << delta << "\t" << rho << "\t" << d << "\t" << deltaB << "\t"  
         << g << "\t" << fi << "\t" << ie0 << "\t" << xe0 << "\t"
         << gamma << "\t" << kappa << "\t" << lambda << "\t"<< mu<< "\t"
	 << length << "\t";
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/
void dorn::receiveParameters(const real* parameters)
{
  tau=parameters[0];
  p0=parameters[1];
  w0=parameters[2];
  s0=parameters[3];
  B0=parameters[4];
  A=parameters[5];
  B=parameters[6];
  Lmax=parameters[7];
  tax=parameters[8];
  delta=parameters[9];
  rho=parameters[10];
  d=parameters[11];
  deltaB=parameters[12];
  g=parameters[13];
  fi=parameters[14];
  ie0=parameters[15];
  xe0=parameters[16];
  gamma=parameters[17];
  kappa=parameters[18];
  lambda=parameters[19];
  mu=parameters[20];
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

void dorn::sendParameters(int& amount,real** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=2;
    *parameters=new real[amount];
    if( !(*parameters) )
	fatalError("defaultModel::sendParameters",
		   "Can't create array for parameters");
    (*parameters)[0]=tau;
    (*parameters)[1]=p0;
    (*parameters)[2]=w0;
    (*parameters)[3]=s0;
    (*parameters)[4]=B0;
    (*parameters)[5]=A;
    (*parameters)[6]=B;
    (*parameters)[7]=Lmax;
    (*parameters)[8]=tax;
    (*parameters)[9]=delta;
    (*parameters)[10]=rho;
    (*parameters)[11]=d;
    (*parameters)[12]=deltaB;
    (*parameters)[13]=g;
    (*parameters)[14]=fi;
    (*parameters)[15]=ie0;
    (*parameters)[16]=xe0;
    (*parameters)[17]=gamma;
    (*parameters)[18]=kappa;
    (*parameters)[19]=lambda;
    (*parameters)[20]=mu;
}
  
