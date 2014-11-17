#include <assert.h>
#include "milano3.h"
#include "../error.h"

static const unsigned int max_iter=10000;
static const unsigned int iter_prec=1;

//$Header: milano3.C,v 1.7 99/07/01 11:53:03 mmueller Exp $

/******************************************************************************/
/*                                                                            */
/* Class name:      milano3                                                    */
/* Member function: milano3                                                    */
/* Purpose:         constructor                                               */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/


milano3::milano3() : baseModel(5)
{
}

milano3::~milano3()
{
}



/******************************************************************************/
/*                                                                            */
/*   Function: Zins_1                                                         */
/*   Purpose:  compute the interest rate in the system8 regime  (main loop)      */
/*   Last modified:                                                           */
/*                                                                            */
/******************************************************************************/

real milano3::Zins_1()
{
    real r1;
   r1=(Ls*alfa*b)/(a*(1-h)*((1-tax)*pgt+zt)+a*(1-h)*alfa*Ls);
    return r1;
}

/****************************************************************************/
/*                                                                           */
/*                                                                       */
/*   Function: Zins_2                                                        */
/*                                                                        */
/*   Purpose:  compute the interest rate in the system3 regime (main loop)      */
/*                                                                        */   
/*****************************************************************************/

real milano3::Zins_2()
{
     real r2;
 	r2=(b*(h*((1-tax)*pgt+zt)+G+mreal))/((1-h)*((1-tax)*pgt+zt)+(a-a*h)*(G+mreal));
    
     return r2;
}    
/******************************************************************************/
/*                                                                            */
/*   Function: Zins_3                                                        */
/*   Input:                                                                   */
/*   Output:                                                                  */
/*   Purpose: compute the interest rate in the systemG6 regime (main loop)                           */
/*   Last modified:                                                          */
/*                                                                            */
/******************************************************************************/


real milano3::zins_3_function(const real& r)
{
    return (1-h)*((1-tax)*pgt+zt)*exp(((1-a)/(1-a-b))*log(r))-n1*exp((a/(1-a-b))*\
    log(a))*exp(((1-a)/(1-a-b))*log(b))*exp(((-a)/(1-a-b))*log(alfa))+\
    (1-h)*n1*exp(((1-b)/(1-a-b))*log(a))*exp((b/(1-a-b))*log(b))*\
    exp(((-a)/(1-a-b))*log(alfa))*r;      
}


real milano3::Zins_3(const real& r1, const real& r2)
{
  //  real r3;
    
  //  clog << setprecision(16);

 real pol(( b/a/(1-h)) );

 
  real oldzero;
  real zero=0;


  real left(0.001);
     int i; 
  for (i=1; i<= (int) pow(10,(double)iter_prec); ++i)
    {
      if ( zins_3_function(i*pol/pow(10.0,(double) iter_prec) )>0 ) break;
    }
  left=pol*((i-1)/pow(10.0,(double)iter_prec) );
  
  
  real right( pol*0.99999 );
  for (i=1; i<=pow(10,(double)iter_prec); ++i)
    {
      if ( zins_3_function(pol*(pow(10,(double)iter_prec)-i)/pow(10.0,(double)iter_prec) )<0 ) break;
    }
  right=pol*( (pow(10,(double)iter_prec)-(i-1))/pow(10.0,(double)iter_prec) );
  right*=1.1;
  int iter=0;

  real m;
  do
    {
      iter++;
      oldzero=zero;
      real value_left( zins_3_function(left) );
      real value_right( zins_3_function(right) );
      m=( value_left-value_right )/(left-right);    
      zero=left-value_left/m;

     
      assert(zero>=left && zero<=right);

      real value_zero( zins_3_function(zero) );

      if ( value_left*value_zero < 0 )
	{
	  right=zero;
	}
      else
	{
	  assert( value_right*value_zero < 0 );
	  left=zero;
	}
    }
	while (   ( left<r1 && left<r2 ) && iter < max_iter  && abs(zero-oldzero) > 1E-8);

  return zero;
}




/******************************************************************************/
/*                                                                            */
/*   Function: diseq_regime                                                   */
/*   Input:                                                                   */
/*   Output:                                                                  */
/*   Purpose: define the disequilibrium regimes (main loop)                   */
/*   Last modified:                                                           */
/*                                                                            */
/******************************************************************************/

void milano3::diseq_regime(const real& r1, const real& r2,
const real& r3)
{
	    if(r1<=r2)
      if(r1<=r3)
	system_8(r1);    
      else 
	system_G6(r3);
    else
      if(r2<=r3)  
	system_3(r2);
      else
	system_G6(r3);


    real rr1( Zins_1() );
    real rr2( Zins_2() );
    real rr3( Zins_3(rr1,rr2) );

    if ( rr1<=rr2 )
      {
	if (rr1<=rr3) 
	  rreal=rr1;
	else
	  rreal=rr3;
      }
    else
      {
	if (rr2<=rr3)
	  rreal=rr2;
	else
	  rreal=rr3;
      }
}
   


/******************************************************************************/
/*                                                                            */
/*   Function: system_3                                                      */
/*   Input:                                                                   */
/*   Output: employment,output,capital stock,profit,wage,interest rate,   */
/*           prices,money stock,interest                                  */
/*   Purpose: analysis of system_3                                        */
/*   Last modified:                                                          */
/*                                                                            */
/******************************************************************************/

void milano3::system_3(const real& r2)
                      
{
    real lambdas;
    real gammas;
    real oldpgt=pgt;
    real oldzt=zt;
//    real teta;

 //   cout << "Hi! System 3" << endl;
    
   rreal=r2;
    empl=1/(alfa*(1/a-h))*(h*((1-tax)*pgt+zt)+mreal+G);
    output=alfa*empl/a;
    capital=alfa*b*empl/(rreal*a);
    lambdas=empl/Ls;
    gammas=exp((1-a-b)*log(output/n1))*exp((-a)*log(a/alfa))*exp((-b)*
    log(b/rreal));
    mtr=mreal;
    wtr=alfa;
    rtr=rreal;
    teta=exp(psi1*log(gammas));
    pgt=(output-alfa*empl-rreal*capital)/teta;
    zt=capital*rreal/teta;
    mreal=(mreal+G+(1-tax)*oldpgt+oldzt)/teta-pgt-zt;
    alfa=(exp(nu1*log(lambdas))*alfa)/teta;
   

    
/*    cout << ":3: empl   =" << empl<<endl;
    cout << ":3: output =" << output<<endl;
    cout << ":3: capital=" << capital<<endl;
    cout << ":3: lambdas=" << lambdas<<endl;
    cout << ":3: gammas =" << gammas<<endl;
    cout << ":3: teta   =" << teta<<endl;
   
*/
    if (pgt<0) cout << "  Gewinn ist negativ." << endl;
    if ( ((1-tax)*pgt + zt)<0 ) cout << "  Anfangsausstattung ist negativ." << endl;

}

/******************************************************************************/
/*                                                                            */
/*   Function: system_G6                                                       */
/*   Input:                                                                   */
/*   Output: employment,output,capital stock,profit,wage,interest rate,      */
/*           prices,money stock,interest                                     */
/*                                                                           */
/*   Purpose: analysis of system_G6                                           */
/*   Last modified:                                                          */
/*                                                                            */
/******************************************************************************/

void milano3::system_G6(const real& r3)
{
    real lambdas;
    real gammad;
    real delta;
    real beta;
    real oldpgt=pgt;
    real oldzt=zt;
//    real teta;

  //  cout << "Hi! System G6" << endl;

    rreal=r3;
    empl=n1*exp((1-b)/(1-a-b)*log(a/alfa))*exp(b/(1-a-b)*log(b/rreal));
    output=alfa*empl/a;
    capital=alfa*b*empl/(rreal*a);
    lambdas=empl/Ls;
    if (output<G){
	beta=output/G;
	delta=0;
	gammad=0;
    }
    else{
	
	beta=1;
	if (output<(G+mreal)){
	    delta=(output-G)/mreal;
	    gammad=0;
	}
	else{
	    delta=1;
	    gammad=(output-mreal-G)/(h*(alfa*empl+(1-tax)*pgt+zt));
	}
    }
    mtr=mreal;
    wtr=alfa;
    rtr=rreal;
    teta=exp(-psi2*log((gammad+delta+beta)/3.0));
   pgt=(output-alfa*empl-rreal*capital)/teta;
    zt=capital*rreal/teta;
    mreal=(delta*mreal+beta*G+(1-tax)*oldpgt+oldzt)/teta-pgt-zt;
    alfa=(exp(nu1*log(lambdas))*alfa)/teta;
   
    
/*    cout << ":G6: empl   =" << empl<<endl; 
    cout << ":G6: output =" << output<<endl;
    cout << ":G6: capital=" << capital<<endl;
    cout << ":G6: lambdas=" << lambdas<<endl;
    cout << ":G6: beta   =" << beta<<endl;
    cout << ":G6: delta  =" << delta<<endl;
    cout << ":G6: gammad =" << gammad<<endl;
    cout << ":G6: teta   =" << teta<<endl;
 */   

    if (pgt<0) cout << "  Gewinn ist negativ." << endl;
    if ( ((1-tax)*pgt + zt)<0 ) cout << "  Anfangsausstattung ist negativ." << endl;
}


/******************************************************************************/
/*                                                                            */
/*   Function: system_8                                                       */
/*   Input:                                                                   */
/*   Output: employment,output,capital stock,profit,wage,interest rate,      */
/*           prices,money stock,interest                                     */
/*   Purpose: analysis of system_8                                           */
/*   Last modified:                                        */
/*                                                                            */
/******************************************************************************/

void milano3::system_8(const real& r1)
{
    real lambdad;
    real gammad;
    real delta;
    real beta;
    real oldpgt=pgt;
    real oldzt=zt;
//    real teta;

//    cout << "Hi! System 8" << endl;
    
    rreal=r1;
    empl=Ls;
    output=alfa*empl/a;
    capital=alfa*b*empl/(rreal*a);
    lambdad=empl/(n1*exp((1-b)/(1-a-b)*log(a/alfa))*
            exp(b/(1-a-b)*log(b/rreal)));

    if (output<G){
	beta=output/G;
	delta=0;
	gammad=0;
    }
    else{
	beta=1;
	if (output<(G+mreal)){
	    delta=(output-G)/mreal;
	    gammad=0;
	}
	else{
	    delta=1;
	    gammad=(output-mreal-G)/(h*(alfa*empl+(1-tax)*pgt+zt));
	}
    }

    mtr=mreal;
    wtr=alfa;
    rtr=rreal;
    teta=exp(-psi2*log((gammad+delta+beta)/3.0));
    pgt=(output-alfa*empl-rreal*capital)/teta;
    zt=capital*rreal/teta;
    mreal=(delta*mreal+beta*G+(1-tax)*oldpgt+oldzt)/teta-pgt-zt;
    alfa=(exp(-nu2*log(lambdad))*alfa)/teta;
  

/*  cout << ":8: empl   =" << empl<<endl;
    cout << ":8: output =" << output<<endl;
    cout << ":8: capital=" << capital<<endl;
    cout << ":8: lambdad=" << lambdad<<endl;
    cout << ":8: gammad =" << gammad<<endl;
    cout << ":8: delta  =" << delta<<endl;
    cout << ":8: beta   =" << beta<<endl;
    cout << ":8: teta   =" << teta<<endl;
  */

    if (pgt<0) cout << "  Gewinn ist negativ." << endl;
    if ( ((1-tax)*pgt + zt)<0 ) cout << "  Anfangsausstattung ist negativ." << endl;

}


/******************************************************************************/
/*                                                                            */
/*   Function: iteration                                                      */
/*   Input:                                                                   */
/*   Output:                                                                  */
/*   Purpose: return to the main loop and continue the simulation             */
/*   Last modified:                                                           */
/*                                                                            */
/******************************************************************************/

void milano3::iteration(const long& t)
{
/*
    real r1;
    real r2;
    real r3;
	
    r1=Zins_1();
    r2=Zins_2();
    r3=Zins_3(r1,r2);

    diseq_regime(r1,r2,r3);
*/
cout << "model not active - contact A. Foerster" << endl;
}




/* loadParamset Last modified: 31.05.1996 (Jorg Nikutta) */

void milano3::loadParamset(ifstream& inFile)
{
  inFile >> a >> b;
  inFile >> psi1 >> psi2 >> nu1 >> nu2;
  inFile >> n1 >> simd;
  inFile >> h >> pg0 >> alfa0 >> mreal0 >> z0;
  inFile >> Ls >> G >> tax;
// printf("%f %f \n %f %f\n  %f %f \n %d \n %d \n %f \n %f %f %f \n %f %f\n  %f %f \n %f" ,a,b,psi1,psi2,nu1,nu2,n1,simd,h,pg0,alfa0,mreal0,p0,z0,Ls,G,tax);

    initialize();
}

/******************************************************************************/
/*                                                                            */
/*   Function:       printParamset                                            */
/*   Purpose:        print current parameterset                               */
/*   Last modified:                                                          */
/*                                                                            */
/******************************************************************************/

 

void milano3::printParamset()
{
    cout << "a      :  " << a <<"\talpha0 : " << alfa0 << endl;
    cout << "b      :  " << b <<"\tmreal0 : " << mreal0 << endl;
    cout << "z0 : " << z0 << endl;
    cout << "n1     :  " << n1 << endl;
    cout << "psi1   :  " << psi1 << "\tpsi2 : " << psi2 << endl;
    cout << "nu1    :  " << nu1 << "\tG   : " << G << endl;
    cout << "nu2    :  " << nu2 << "\ttax : " << tax << endl;
    cout << "h      :  " << h << "\tsimd: " << simd << endl;
    cout << "pg0    :  " << pg0 << "\tLs  : " << Ls << endl;
}

/******************************************************************************/
/*                                                                           */
/*   Function:       saveParamset                                            */
/*   Purpose:        write current parameterset to the disk                  */
/*   Last modified:                                                          */
/*                                                                           */
/*****************************************************************************/
void milano3::saveParamset(ofstream& outFile)
{
    outFile << a << " " << b << endl;
    outFile << psi1 << " " << psi2 << endl; 
    outFile << nu1 << " " << nu2 << endl;
    outFile << n1 << " " << simd << endl;
    outFile << h << endl;
    outFile << pg0 << " " << alfa0 << " " << mreal0 << " " << z0 << endl;
    outFile << Ls << " " << G << " " << tax << endl;
}

/******************************************************************************/
/*                                                                            */
/*   Function:       set_labels                                               */
/*   Input:          pointer to the name of the label                         */
/*   Return:         pointer to the corresponding variable                    */
/*   Purpose:        set pointer to the variable that shall be plotted on the */
/*                   label                                                    */
/*   Last modified:                                                           */
/*                                                                            */
/******************************************************************************/

real* milano3::setLabels(char* name)
{
    if( !strcmp(name,"G") )
	return( &G );
    if( !strcmp(name,"empl") )
	return( &empl );
    if( !strcmp(name,"capital") )
        return( &capital );
    if( !strcmp(name,"output") )
	return( &output );
    if( !strcmp(name,"a") )
	return( &a );
    if( !strcmp(name,"b") )
	return( &b );
    if( !strcmp(name,"alfa") )
	return( &alfa );
   if( !strcmp(name,"mreal") )
	return( &mreal );
    if( !strcmp(name,"pgt") )
	return( &pgt );
    if( !strcmp(name,"rreal") )
        return( &rreal );
    if( !strcmp(name,"teta") )
         return( &teta );
    if( !strcmp(name,"zt") )
        return( &zt ); 
    if( !strcmp(name,"mtr") ) 
	return( &mtr );
    if( !strcmp(name,"wtr") )
	return( &wtr );
    if( !strcmp(name,"rtr") )
        return( &rtr );
    if( !strcmp(name,"psi1") )
	return( &psi1 );
    if( !strcmp(name,"psi2") )
	return( &psi2 );
    if( !strcmp(name,"nu1") )
	return( &nu1 );
    if( !strcmp(name,"nu2") )
	return( &nu2 );
    if( !strcmp(name,"h") )
	return( &h );
    if( !strcmp(name,"tax") )
	return( &tax );
    if( !strcmp(name,"Ls") )
	return( &Ls );
    if( !strcmp(name,"alfa0") )
	return( &alfa0 );
    if( !strcmp(name,"mreal0") )
	return( &mreal0 );
    if( !strcmp(name,"pg0") )
	return( &pg0 );
    if( !strcmp(name,"z0") )
        return( &z0 );
    if( !strcmp(name,"n1") )
	return( (real*) &n1 );
    return( NULL );
}

void milano3::initialize()
{
    alfa=alfa0;
    pgt=pg0;
    mreal=mreal0;
    zt=z0;

    real r1( Zins_1() );
    real r2( Zins_2() );
    real r3( Zins_3(r1,r2) );

    if ( r1<=r2 )
      {
	if (r1<=r3) 
	  rreal=r1;
	else
	  rreal=r3;
      }
    else
      {
	if (r2<=r3)
	  rreal=r2;
	else
	  rreal=r3;
      }
}


real* milano3::sendModelVar()
{
    return &alfa; // return the main model variable
}

void milano3::sendStateSpace(int &quantity,const real*** stateSpace)
{
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const real* [dimension];
    if( !(*stateSpace) )
	fatalError("milano3::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;


	(*stateSpace)[0]=&mreal;
        (*stateSpace)[1]=&alfa;
        (*stateSpace)[2]=&pgt;
        (*stateSpace)[3]=&rreal;
        (*stateSpace)[4]=&zt;      

}
// return pointers to all model
					    // variables and the dimension
					    // of the model
void milano3::sendParameters(int&,real**)
{
} // write all parameters
                   
                             // into an array and return the numbers
				// of parameters
void milano3::receiveParameters(const real*)
{
}
// receive parameter values 






















