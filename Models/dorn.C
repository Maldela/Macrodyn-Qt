
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/dorn.C,v 1.1 2000/08/31 15:26:02 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  dorn.C                                                       */
/* Contents:     Member functions of the class defaultModel                   */
/* Last Modified:  31.3.2000                   				      */
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
/* Last modified:  31.3.2000                                                  */
/*                                                                            */
/******************************************************************************/
dorn::dorn() : baseModel(6)
{
  p=NULL;
 
}
/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: ~dornModel                                                */
/* Purpose:         destructor                                                */
/* Last modified:  31.3.2000                                                  */
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
/* Last modified:   31.3.2000                          			      */
/*                                                                            */
/******************************************************************************/
void dorn::pInit(qreal *p)
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
/* Last modified:   31.3.2000                                                */
/*                                                                            */
/******************************************************************************/
void dorn::initialize()
{    
    pt=pe=p0;
    wt=w0;
    Bt=B0;
    st=s0;
    it=i0;
    xet=xe0;
    xt=x0;
     //    p[0]=p0;  



    pInit(p);

    
//    if (p!=0) { delete [] p; }
//
//    p=new qreal [tau];
//    p[0]=p0;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system (Figs. 7, 8)   */
/* Last modified:   31.3.2000                                                 */
/*                                                                            */
/******************************************************************************/

void dorn::iteration(const qint64& t)
{
  pe = pexpBerechnung (t);
  ExpectedInterestRate (); 
  fullProduction ();
  labordemand ();
  notionalProduction ();
  goodsDemand (); 
  production ();
  employment ();
  bondMarket();
  ExchangeRate();
  newBonds();
  pwBerechnung();
  dynamics(p,t);
}
/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: dynamics                                                  */
/* Purpose:         describes the dynamics of the system (eqs. 2.28i-2.28iii) */
/* Last modified:   31.3.2000                                                 */
/*                                                                            */
/******************************************************************************/

void dorn::dynamics(qreal *p,const qint64 t)
{  
  assert(t>0);
  if (t>tau)
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
}

/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: ExpectedInflationRate                                     */  
/*									      */
/* Purpose:         computes the expected inflation rate                      */
/* Last modified:  31.3.2000                                                  */
/*                                                                            */
/******************************************************************************/

qreal dorn::pexpBerechnung(const qint64 t)
{
    qint64 T, counter;
    qreal sum=0.0;
    
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
/* Last modified:  31.3.2000                                                  */
/*                                                                            */
/******************************************************************************/

void dorn::ExpectedInterestRate()
{
iet = it;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: fullProduction                                                */ 
/*									      */
/* Purpose:         computes the production for full employment               */
/* Last modified:  31.3.2000                                                  */
/*                                                                            */
/******************************************************************************/

void dorn::fullProduction()
{
ytf = (A/B) * pow(Lmax,B);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: labordemand                                               */
/*									      */	
/* Purpose:         computes the notional labor demand                        */
/* Last modified:   31.3.2000                                                 */
/*                                                                            */
/******************************************************************************/

void dorn::labordemand()
{
zt = pow ( ((pt*A)/wt),1/(1-B) );

}

/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: notionalProduction                                            */ 
/*									      */
/* Purpose:         computes the desired production                           */
/* Last modified:  31.3.2000                                                  */
/*                                                                            */
/******************************************************************************/

void dorn::notionalProduction()
{
ytz = (A/B) * pow(zt,B);

}

  /**************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: GoodsDemand                                    	      */
/*									      */
/* Purpose:         computes the young household's goods demand               */
/*									      */
/* Last modified:  31.3.2000                                                  */
/*                                                                            */
/******************************************************************************/

void dorn::goodsDemand()
{
 
if (rho == 0)
  {
    xtdy = (g+((Bt*d)/pt)+((Im*xt)/pt))*
           1 / ( 1-( (1/(1+delta))*(1-tax)*
           (1+(delta*Bt)/(Bt+deltaB))) );
  }
else
  { 
xtdy = (g+((Bt*d)/pt)+((Im*xt)/pt))*
       (Bt+deltaB)/
       (tax*Bt+deltaB*(1- 
       (pow(pe,rho/(1-rho))/(pow(pe,rho/(1-rho))+pow(delta,1/(1-rho))*
       pow(pt,rho/(1-rho))*pow((1+iet),rho/(1-rho))))*(1-tax)));
  }
}

/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: production                                                */ 
/*									      */
/* Purpose:         computes the actual production                            */
/* Last modified:  31.3.2000                                                  */
/*                                                                            */
/******************************************************************************/

void dorn::production()
{
yt=MIN( MIN(ytf,ytz),xtdy );

}

/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function:  employment                                               */ 
/*									      */
/* Purpose:         computes the actual employment                            */
/* Last modified:  31.3.2000                                                  */
/*                                                                            */
/******************************************************************************/

void dorn::employment()
{
Lt=pow(yt*B/A,1/B);

}

/******************************************************************************/
/*                                                                           */
/* Class name:      dornModel                                            */
/* Member function: BondMarket                                 	      */
/*						       		      */
/* Purpose:         computes the equilibrium bond price and effective        */
/*                  interest rate                                            */
/*								             */
/* Last modified:  31.3.2000                                                 */
/*                                                                           */
/****************************************************************************/

void dorn::bondMarket()
{
stv = st;
if (rho == 0)
  {
    st = (pt/(Bt+deltaB))*(delta/(delta+1))*(1-tax)*yt;
  }
else
  { 
st = ( (pt * (1-tax) * yt) / (Bt + deltaB) )* (1-
      (pow(pe,rho/(1-rho)))/(pow(pe,rho/(1-rho))+pow(delta,1/(1-rho))*
       pow(pt,rho/(1-rho))*pow((1+iet),rho/(1-rho))));
  }

it = ( (d + st) / stv) - 1;

}

/***************************************************************************/
/* Class name:      dornModel                                                */
/* Member function: ExchangeRate                                             */
/*								             */
/* Purpose:         computes the expected and the actual exchange rate;      */
/*								             */
/* Last modified:   31.3.2000                                                */
/*                                                                           */
/*****************************************************************************/

void dorn::ExchangeRate()
{
xetv = xet;
xet = xetv * (1+iet)/(1+fi);
V=xet/xetv;
W=(1+iet)/(1+fi);
xt = xet * (1+fi)/(1+iet);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: newBonds                                    	      */
/*									      */
/* Purpose:         computes the new bonds                                   */
/*									      */
/* Last modified:  31.3.2000                                                  */
/*                                                                            */
/******************************************************************************/

void dorn::newBonds()
{
 Bt=Bt+deltaB;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: pwBerechnung                                   	      */
/*									      */
/* Purpose:         price and wage adjustment                                 */
/* Last modified:  31.3.2000                                                 */
/*                                                                            */
/******************************************************************************/

void dorn::pwBerechnung()
{
    if( xtdy > ytz ) {
        if( ytf > ytz)
	  {
            pt = pt *( 1 + gamma *( (xtdy-yt)/xtdy ));
            wt = wt *( 1 + lambda *((Lt-Lmax)/Lmax)) ; 
	  }
	else {
	    if( ytf == ytz )
	      {
                pt = pt *( 1 + gamma *( (xtdy-yt)/xtdy ));
                wt = wt *( 1 + lambda *((Lt-Lmax)/Lmax)) ; 
              }
	    else
              {
                pt = pt *( 1 + gamma *( (xtdy-yt)/xtdy ));
                wt = wt *( 1 + mu *((zt-Lt)/zt)) ; 
              }  
	}
    }
    else {
        if( xtdy == ytz ) {
	    if( ytf > ytz )
              {
                pt = pt *( 1 + gamma *( (xtdy-yt)/xtdy ));
                wt = wt *( 1 + lambda *((Lt-Lmax)/Lmax)) ; 
              }
	    else {
	        if( ytf == ytz )
	      {  
                pt = pt *( 1 + gamma *( (xtdy-yt)/xtdy ));
                wt = wt *( 1 + lambda *((Lt-Lmax)/Lmax)) ; 
              }
		else
	      { 
                pt = pt *( 1 + gamma *( (xtdy-yt)/xtdy ));
                wt = wt *( 1 + mu *((zt-Lt)/zt)) ; 
              }
	    }
	}
	else {
	    if( xtdy < ytz ) {
	        if( ytf > xtdy )
	      {
                pt = pt *( 1 + kappa *((yt-ytz)/ytz));
                wt = wt *( 1 + lambda *((Lt-Lmax)/Lmax)) ;  
              }
		else {
		    if( ytf == xtdy )
	      {
                pt = pt *( 1 + gamma *( (xtdy-yt)/xtdy ));
                wt = wt *( 1 + lambda *((Lt-Lmax)/Lmax)) ; 
              }
		    else
	      {
                pt = pt *( 1 + gamma *( (xtdy-yt)/xtdy ));
                wt = wt *( 1 + mu *((zt-Lt)/zt)) ; 
              }
		}
	    }
	}
    }

  
}

/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:  31.3.2000                         			      */
/*                                                                            */
/******************************************************************************/

qreal* dorn::setLabels(const QString& name)
{
    if (name == "tau")
	return( (qreal*)(&tau) );
    if (name == "pt")
	return( (qreal*)(&pt) );
    if (name == "wt")
	return( (qreal*)(&wt) );
    if (name == "st")
	return( (qreal*)(&st) );
    if (name == "stv")
	return( (qreal*)(&stv) );
    if (name == "xt")
	return( (qreal*)(&xt) );
    if (name == "it")
	return( (qreal*)(&it) );
    if (name == "Bt")
	return( (qreal*)(&Bt) );
    if (name == "Lt")
	return( (qreal*)(&Lt) );
    if (name == "yt")
	return( (qreal*)(&yt) );
    if (name == "iet")
	return( (qreal*)(&iet) );
    if (name == "Lmax")
	return( (qreal*)(&Lmax) );
    if (name == "tax")
	return( (qreal*)(&tax) );
    if (name == "d")
	return( (qreal*)(&d) );
    if (name == "deltaB")
	return( (qreal*)(&deltaB) );
    if (name == "pe")
	return( (qreal*)(&pe) );
    if (name == "g")
	return( (qreal*)(&g) );
    if (name == "ytf")
	return( (qreal*)(&ytf) );
    if (name == "ytz")
	return( (qreal*)(&ytz) );
    if (name == "Im")
	return( (qreal*)(&Im) ); 
    if (name == "fi")
	return( (qreal*)(&fi) );
    if (name == "xet")
	return( (qreal*)(&xet) );
    if (name == "B")
	return( (qreal*)(&B) );
    if (name == "zt")
	return( (qreal*)(&zt) );
    if (name == "A")
	return( (qreal*)(&A) );
    if (name == "delta")
	return( (qreal*)(&delta) );
    if (name == "rho")
	return( (qreal*)(&rho) );
    if (name == "xtdy")
	return( (qreal*)(&xtdy) );
    if (name == "gamma")
	return( (qreal*)(&gamma) );
    if (name == "kappa")
	return( (qreal*)(&kappa) );
    if (name == "lambda")
	return( (qreal*)(&lambda) );
    if (name == "mu")
	return( (qreal*)(&mu) );
    if (name == "V")
	return( (qreal*)(&V) );
    if (name == "W")
	return( (qreal*)(&W) );
    if (name == "xetv")
	return( (qreal*)(&xetv) );
    if (name == "B0")
	return( (qreal*)(&B0) );
    if (name == "xBundle")
	return( (qreal*)(&xBundle) );
    if (name == "yBundle")
	return( (qreal*)(&yBundle) );
    return( NULL );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: sendStateSpace                                            */
/* Purpose:         returns pointers to the qreal balances and the qreal wage;  */
/*                  returns the dimension of the system for rho=0             */
/* Last modified:  31.3.2000                          			      */
/*                                                                            */
/******************************************************************************/

void dorn::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("defaultModel::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&pt;
    (*stateSpace)[1]=&wt;
    (*stateSpace)[2]=&Bt;
    (*stateSpace)[3]=&iet;
    (*stateSpace)[4]=&xt;
    (*stateSpace)[5]=&xet;
}
    
/******************************************************************************/
/*                                                                            */
/* Class name:      dornModel                                                 */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:  31.3.2000                   			              */
/*                                                                            */
/******************************************************************************/

void dorn::loadParamset(QDataStream& inputFile)
{
  /* cerr << "loadps:" << "\n"; */
    inputFile >> tau >> p0 >> w0 >> s0 >>  B0 >> x0
              >> A >> B >> Lmax >> tax >> delta >> rho >> d
              >> deltaB >> g >> fi
              >> i0 >> xe0 >> Im
              >> gamma >> kappa >> lambda >> mu >> length;

    if( p )
    {
      delete [] p;
    }
    p = new qreal[tau];
    if( !p )
	fatalError("defaultModel::loadParamset","Can't create p vector");
    initialize();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   31.3.2000                                                */
/*                                                                            */
/******************************************************************************/

void dorn::saveParamset(QDataStream& outputFile)
{
    outputFile << tau << "\t" << p0 << "\t" << w0 << "\t" << s0 << "\t"
               << B0 << "\t" << x0 << "\t"
               << A << "\t" << B << "\t" << Lmax << "\t" << tax << "\t"
               << delta << "\t" << rho << "\t" << d << "\t" << deltaB << "\t"  
               << g << "\t" << fi << "\t" << i0 << "\t" << xe0 << "\t"
               << Im << "\t" <<gamma << "\t" << kappa << "\t" 
               << lambda << "\t" << mu << "\t" << length << "\t";
}
/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: saveParamsetWithNames                                     */
/* Purpose:         add  parameterset to printfile                            */
/* Last modified:   31.3.2000                                                 */
/*                                                                            */
/******************************************************************************/

void dorn::saveParamsetWithNames(QDataStream& outputFile)
{
    outputFile << "dornModel:\n\t";
    outputFile << "Tau = " << tau 
               << "\n\tp_0 = " << p0   
               << "\n\tw_0 = " << w0
               << "\n\ts_0 = " << s0
               << "\n\tB0 = " << B0
               << "\n\tx_0 = " << x0
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
               << "\n\ti0 = " << i0
               << "\n\txe0 = " << xe0
               << "\n\tIm = " << Im               
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
/* Last modified:   31.3.2000                                                 */
/*                                                                            */
/******************************************************************************/

void dorn::printParamset()
{
    log() << tau << "\t" << p0 << "\t" << w0 << "\t" << s0 << "\t"
         << B0 << "\t" << x0 << "\t" 
         << A << "\t" << B << "\t" << Lmax << "\t" << tax << "\t"
         << delta << "\t" << rho << "\t" << d << "\t" << deltaB << "\t"  
         << g << "\t" << fi << "\t" << i0 << "\t" << xe0 << "\t"
         << Im << "\t" << gamma << "\t" << kappa << "\t" 
         << lambda << "\t"<< mu<< "\t" << length << "\t";
}

/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   31.3.2000                                                 */
/*                                                                            */
/******************************************************************************/
void dorn::receiveParameters(const qreal* parameters)
{
  tau=parameters[0];
  p0=parameters[1];
  w0=parameters[2];
  s0=parameters[3];
  B0=parameters[4];
  x0=parameters[5];
  A=parameters[6];
  B=parameters[7];
  Lmax=parameters[8];
  tax=parameters[9];
  delta=parameters[10];
  rho=parameters[11];
  d=parameters[12];
  deltaB=parameters[13];
  g=parameters[14];
  fi=parameters[15];
  i0=parameters[16];
  xe0=parameters[17];
  Im=parameters[18];
  gamma=parameters[19];
  kappa=parameters[20];
  lambda=parameters[21];
  mu=parameters[22];
}
/******************************************************************************/
/*                                                                            */
/* Class name:      defaultModel                                              */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   31.3.2000                                                 */
/*                                                                            */
/******************************************************************************/

void dorn::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=2;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("defaultModel::sendParameters",
		   "Can't create array for parameters");
    (*parameters)[0]=tau;
    (*parameters)[1]=p0;
    (*parameters)[2]=w0;
    (*parameters)[3]=s0;
    (*parameters)[4]=B0;
    (*parameters)[5]=x0;
    (*parameters)[6]=A;
    (*parameters)[7]=B;
    (*parameters)[8]=Lmax;
    (*parameters)[9]=tax;
    (*parameters)[10]=delta;
    (*parameters)[11]=rho;
    (*parameters)[12]=d;
    (*parameters)[13]=deltaB;
    (*parameters)[14]=g;
    (*parameters)[15]=fi;
    (*parameters)[16]=i0;
    (*parameters)[17]=xe0;
    (*parameters)[18]=Im;
    (*parameters)[19]=gamma;
    (*parameters)[20]=kappa;
    (*parameters)[21]=lambda;
    (*parameters)[22]=mu;
}
  
















