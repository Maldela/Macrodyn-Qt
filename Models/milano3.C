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

qreal milano3::Zins_1()
{
    qreal r1;
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

qreal milano3::Zins_2()
{
     qreal r2;
 	r2=(b*(h*((1-tax)*pgt+zt)+G+mqreal))/((1-h)*((1-tax)*pgt+zt)+(a-a*h)*(G+mqreal));
    
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


qreal milano3::zins_3_function(const qreal& r)
{
    return (1-h)*((1-tax)*pgt+zt)*exp(((1-a)/(1-a-b))*log(r))-n1*exp((a/(1-a-b))*\
    log(a))*exp(((1-a)/(1-a-b))*log(b))*exp(((-a)/(1-a-b))*log(alfa))+\
    (1-h)*n1*exp(((1-b)/(1-a-b))*log(a))*exp((b/(1-a-b))*log(b))*\
    exp(((-a)/(1-a-b))*log(alfa))*r;      
}


qreal milano3::Zins_3(const qreal& r1, const qreal& r2)
{
  //  qreal r3;
    
  //  clog << setprecision(16);

 qreal pol(( b/a/(1-h)) );

 
  qreal oldzero;
  qreal zero=0;


  qreal left(0.001);
     int i; 
  for (int i=1; i<= (int) pow(10,(double)iter_prec); ++i)
    {
      if ( zins_3_function(i*pol/pow(10.0,(double) iter_prec) )>0 ) break;
    }
  left=pol*((i-1)/pow(10.0,(double)iter_prec) );
  
  
  qreal right( pol*0.99999 );
  for (int i=1; i<=pow(10,(double)iter_prec); ++i)
    {
      if ( zins_3_function(pol*(pow(10,(double)iter_prec)-i)/pow(10.0,(double)iter_prec) )<0 ) break;
    }
  right=pol*( (pow(10,(double)iter_prec)-(i-1))/pow(10.0,(double)iter_prec) );
  right*=1.1;
  int iter=0;

  qreal m;
  do
    {
      iter++;
      oldzero=zero;
      qreal value_left( zins_3_function(left) );
      qreal value_right( zins_3_function(right) );
      m=( value_left-value_right )/(left-right);    
      zero=left-value_left/m;

     
      assert(zero>=left && zero<=right);

      qreal value_zero( zins_3_function(zero) );

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
    while (   ( left<r1 && left<r2 ) && iter < max_iter  && qAbs(zero-oldzero) > 1E-8);

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

void milano3::diseq_regime(const qreal& r1, const qreal& r2,
const qreal& r3)
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


    qreal rr1( Zins_1() );
    qreal rr2( Zins_2() );
    qreal rr3( Zins_3(rr1,rr2) );

    if ( rr1<=rr2 )
      {
	if (rr1<=rr3) 
	  rqreal=rr1;
	else
	  rqreal=rr3;
      }
    else
      {
	if (rr2<=rr3)
	  rqreal=rr2;
	else
	  rqreal=rr3;
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

void milano3::system_3(const qreal& r2)
                      
{
    qreal lambdas;
    qreal gammas;
    qreal oldpgt=pgt;
    qreal oldzt=zt;
//    qreal teta;

 //   Log::log() << "Hi! System 3" << endl;
    
   rqreal=r2;
    empl=1/(alfa*(1/a-h))*(h*((1-tax)*pgt+zt)+mqreal+G);
    output=alfa*empl/a;
    capital=alfa*b*empl/(rqreal*a);
    lambdas=empl/Ls;
    gammas=exp((1-a-b)*log(output/n1))*exp((-a)*log(a/alfa))*exp((-b)*
    log(b/rqreal));
    mtr=mqreal;
    wtr=alfa;
    rtr=rqreal;
    teta=exp(psi1*log(gammas));
    pgt=(output-alfa*empl-rqreal*capital)/teta;
    zt=capital*rqreal/teta;
    mqreal=(mqreal+G+(1-tax)*oldpgt+oldzt)/teta-pgt-zt;
    alfa=(exp(nu1*log(lambdas))*alfa)/teta;
   

    
/*    Log::log() << ":3: empl   =" << empl<<endl;
    Log::log() << ":3: output =" << output<<endl;
    Log::log() << ":3: capital=" << capital<<endl;
    Log::log() << ":3: lambdas=" << lambdas<<endl;
    Log::log() << ":3: gammas =" << gammas<<endl;
    Log::log() << ":3: teta   =" << teta<<endl;
   
*/
    if (pgt<0) Log::log() << "  Gewinn ist negativ." << endl;
    if ( ((1-tax)*pgt + zt)<0 ) Log::log() << "  Anfangsausstattung ist negativ." << endl;

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

void milano3::system_G6(const qreal& r3)
{
    qreal lambdas;
    qreal gammad;
    qreal delta;
    qreal beta;
    qreal oldpgt=pgt;
    qreal oldzt=zt;
//    qreal teta;

  //  Log::log() << "Hi! System G6" << endl;

    rqreal=r3;
    empl=n1*exp((1-b)/(1-a-b)*log(a/alfa))*exp(b/(1-a-b)*log(b/rqreal));
    output=alfa*empl/a;
    capital=alfa*b*empl/(rqreal*a);
    lambdas=empl/Ls;
    if (output<G){
	beta=output/G;
	delta=0;
	gammad=0;
    }
    else{
	
	beta=1;
	if (output<(G+mqreal)){
	    delta=(output-G)/mqreal;
	    gammad=0;
	}
	else{
	    delta=1;
	    gammad=(output-mqreal-G)/(h*(alfa*empl+(1-tax)*pgt+zt));
	}
    }
    mtr=mqreal;
    wtr=alfa;
    rtr=rqreal;
    teta=exp(-psi2*log((gammad+delta+beta)/3.0));
   pgt=(output-alfa*empl-rqreal*capital)/teta;
    zt=capital*rqreal/teta;
    mqreal=(delta*mqreal+beta*G+(1-tax)*oldpgt+oldzt)/teta-pgt-zt;
    alfa=(exp(nu1*log(lambdas))*alfa)/teta;
   
    
/*    Log::log() << ":G6: empl   =" << empl<<endl; 
    Log::log() << ":G6: output =" << output<<endl;
    Log::log() << ":G6: capital=" << capital<<endl;
    Log::log() << ":G6: lambdas=" << lambdas<<endl;
    Log::log() << ":G6: beta   =" << beta<<endl;
    Log::log() << ":G6: delta  =" << delta<<endl;
    Log::log() << ":G6: gammad =" << gammad<<endl;
    Log::log() << ":G6: teta   =" << teta<<endl;
 */   

    if (pgt<0) Log::log() << "  Gewinn ist negativ." << endl;
    if ( ((1-tax)*pgt + zt)<0 ) Log::log() << "  Anfangsausstattung ist negativ." << endl;
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

void milano3::system_8(const qreal& r1)
{
    qreal lambdad;
    qreal gammad;
    qreal delta;
    qreal beta;
    qreal oldpgt=pgt;
    qreal oldzt=zt;
//    qreal teta;

//    Log::log() << "Hi! System 8" << endl;
    
    rqreal=r1;
    empl=Ls;
    output=alfa*empl/a;
    capital=alfa*b*empl/(rqreal*a);
    lambdad=empl/(n1*exp((1-b)/(1-a-b)*log(a/alfa))*
            exp(b/(1-a-b)*log(b/rqreal)));

    if (output<G){
	beta=output/G;
	delta=0;
	gammad=0;
    }
    else{
	beta=1;
	if (output<(G+mqreal)){
	    delta=(output-G)/mqreal;
	    gammad=0;
	}
	else{
	    delta=1;
	    gammad=(output-mqreal-G)/(h*(alfa*empl+(1-tax)*pgt+zt));
	}
    }

    mtr=mqreal;
    wtr=alfa;
    rtr=rqreal;
    teta=exp(-psi2*log((gammad+delta+beta)/3.0));
    pgt=(output-alfa*empl-rqreal*capital)/teta;
    zt=capital*rqreal/teta;
    mqreal=(delta*mqreal+beta*G+(1-tax)*oldpgt+oldzt)/teta-pgt-zt;
    alfa=(exp(-nu2*log(lambdad))*alfa)/teta;
  

/*  Log::log() << ":8: empl   =" << empl<<endl;
    Log::log() << ":8: output =" << output<<endl;
    Log::log() << ":8: capital=" << capital<<endl;
    Log::log() << ":8: lambdad=" << lambdad<<endl;
    Log::log() << ":8: gammad =" << gammad<<endl;
    Log::log() << ":8: delta  =" << delta<<endl;
    Log::log() << ":8: beta   =" << beta<<endl;
    Log::log() << ":8: teta   =" << teta<<endl;
  */

    if (pgt<0) Log::log() << "  Gewinn ist negativ." << endl;
    if ( ((1-tax)*pgt + zt)<0 ) Log::log() << "  Anfangsausstattung ist negativ." << endl;

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
    qreal r1;
    qreal r2;
    qreal r3;
	
    r1=Zins_1();
    r2=Zins_2();
    r3=Zins_3(r1,r2);

    diseq_regime(r1,r2,r3);
*/
Log::log() << "model not active - contact A. Foerster" << endl;
}




/* loadParamset Last modified: 31.05.1996 (Jorg Nikutta) */

void milano3::loadParamset(ifstream& inFile)
{
  inFile >> a >> b;
  inFile >> psi1 >> psi2 >> nu1 >> nu2;
  inFile >> n1 >> simd;
  inFile >> h >> pg0 >> alfa0 >> mqreal0 >> z0;
  inFile >> Ls >> G >> tax;
// printf("%f %f \n %f %f\n  %f %f \n %d \n %d \n %f \n %f %f %f \n %f %f\n  %f %f \n %f" ,a,b,psi1,psi2,nu1,nu2,n1,simd,h,pg0,alfa0,mqreal0,p0,z0,Ls,G,tax);

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
    Log::log() << "a      :  " << a <<"\talpha0 : " << alfa0 << endl;
    Log::log() << "b      :  " << b <<"\tmqreal0 : " << mqreal0 << endl;
    Log::log() << "z0 : " << z0 << endl;
    Log::log() << "n1     :  " << n1 << endl;
    Log::log() << "psi1   :  " << psi1 << "\tpsi2 : " << psi2 << endl;
    Log::log() << "nu1    :  " << nu1 << "\tG   : " << G << endl;
    Log::log() << "nu2    :  " << nu2 << "\ttax : " << tax << endl;
    Log::log() << "h      :  " << h << "\tsimd: " << simd << endl;
    Log::log() << "pg0    :  " << pg0 << "\tLs  : " << Ls << endl;
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
    outFile << pg0 << " " << alfa0 << " " << mqreal0 << " " << z0 << endl;
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

qreal* milano3::setLabels(char* name)
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
   if( !strcmp(name,"mqreal") )
	return( &mqreal );
    if( !strcmp(name,"pgt") )
	return( &pgt );
    if( !strcmp(name,"rqreal") )
        return( &rqreal );
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
    if( !strcmp(name,"mqreal0") )
	return( &mqreal0 );
    if( !strcmp(name,"pg0") )
	return( &pg0 );
    if( !strcmp(name,"z0") )
        return( &z0 );
    if( !strcmp(name,"n1") )
	return( (qreal*) &n1 );
    return( NULL );
}

void milano3::initialize()
{
    alfa=alfa0;
    pgt=pg0;
    mqreal=mqreal0;
    zt=z0;

    qreal r1( Zins_1() );
    qreal r2( Zins_2() );
    qreal r3( Zins_3(r1,r2) );

    if ( r1<=r2 )
      {
	if (r1<=r3) 
	  rqreal=r1;
	else
	  rqreal=r3;
      }
    else
      {
	if (r2<=r3)
	  rqreal=r2;
	else
	  rqreal=r3;
      }
}


qreal* milano3::sendModelVar()
{
    return &alfa; // return the main model variable
}

void milano3::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("milano3::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;


	(*stateSpace)[0]=&mqreal;
        (*stateSpace)[1]=&alfa;
        (*stateSpace)[2]=&pgt;
        (*stateSpace)[3]=&rqreal;
        (*stateSpace)[4]=&zt;      

}
// return pointers to all model
					    // variables and the dimension
					    // of the model
void milano3::sendParameters(int&,qreal**)
{
} // write all parameters
                   
                             // into an array and return the numbers
				// of parameters
void milano3::receiveParameters(const qreal*)
{
}
// receive parameter values 






















