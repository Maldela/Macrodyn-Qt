#include <assert.h>
#include "milano3.h"
#include "../error.h"

static const uint max_iter=10000;
static const uint iter_prec=1;

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
  for (i=1; i<= (int) pow(10,(double)iter_prec); ++i)
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
  uint iter=0;

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
    while ( ( left<r1 && left<r2 ) && iter < max_iter && qAbs(zero-oldzero) > 1E-8);

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

 //   log() << "Hi! System 3" << "\n";
    
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
   

    
/*    log() << ":3: empl   =" << empl<<"\n";
    log() << ":3: output =" << output<<"\n";
    log() << ":3: capital=" << capital<<"\n";
    log() << ":3: lambdas=" << lambdas<<"\n";
    log() << ":3: gammas =" << gammas<<"\n";
    log() << ":3: teta   =" << teta<<"\n";
   
*/
    if (pgt<0) log() << "  Gewinn ist negativ." << "\n";
    if ( ((1-tax)*pgt + zt)<0 ) log() << "  Anfangsausstattung ist negativ." << "\n";

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

  //  log() << "Hi! System G6" << "\n";

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
   
    
/*    log() << ":G6: empl   =" << empl<<"\n"; 
    log() << ":G6: output =" << output<<"\n";
    log() << ":G6: capital=" << capital<<"\n";
    log() << ":G6: lambdas=" << lambdas<<"\n";
    log() << ":G6: beta   =" << beta<<"\n";
    log() << ":G6: delta  =" << delta<<"\n";
    log() << ":G6: gammad =" << gammad<<"\n";
    log() << ":G6: teta   =" << teta<<"\n";
 */   

    if (pgt<0) log() << "  Gewinn ist negativ." << "\n";
    if ( ((1-tax)*pgt + zt)<0 ) log() << "  Anfangsausstattung ist negativ." << "\n";
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

//    log() << "Hi! System 8" << "\n";
    
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
  

/*  log() << ":8: empl   =" << empl<<"\n";
    log() << ":8: output =" << output<<"\n";
    log() << ":8: capital=" << capital<<"\n";
    log() << ":8: lambdad=" << lambdad<<"\n";
    log() << ":8: gammad =" << gammad<<"\n";
    log() << ":8: delta  =" << delta<<"\n";
    log() << ":8: beta   =" << beta<<"\n";
    log() << ":8: teta   =" << teta<<"\n";
  */

    if (pgt<0) log() << "  Gewinn ist negativ." << "\n";
    if ( ((1-tax)*pgt + zt)<0 ) log() << "  Anfangsausstattung ist negativ." << "\n";

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

void milano3::iteration(const qint64& t)
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
log() << "model not active - contact A. Foerster" << "\n";
}




/* loadParamset Last modified: 31.05.1996 (Jorg Nikutta) */

void milano3::loadParamset(QDataStream& inFile)
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
    log() << "a      :  " << a <<"\talpha0 : " << alfa0 << "\n";
    log() << "b      :  " << b <<"\tmqreal0 : " << mqreal0 << "\n";
    log() << "z0 : " << z0 << "\n";
    log() << "n1     :  " << n1 << "\n";
    log() << "psi1   :  " << psi1 << "\tpsi2 : " << psi2 << "\n";
    log() << "nu1    :  " << nu1 << "\tG   : " << G << "\n";
    log() << "nu2    :  " << nu2 << "\ttax : " << tax << "\n";
    log() << "h      :  " << h << "\tsimd: " << simd << "\n";
    log() << "pg0    :  " << pg0 << "\tLs  : " << Ls << "\n";
}

/******************************************************************************/
/*                                                                           */
/*   Function:       saveParamset                                            */
/*   Purpose:        write current parameterset to the disk                  */
/*   Last modified:                                                          */
/*                                                                           */
/*****************************************************************************/
void milano3::saveParamset(QDataStream& outFile)
{
    outFile << a << " " << b << "\n";
    outFile << psi1 << " " << psi2 << "\n"; 
    outFile << nu1 << " " << nu2 << "\n";
    outFile << n1 << " " << simd << "\n";
    outFile << h << "\n";
    outFile << pg0 << " " << alfa0 << " " << mqreal0 << " " << z0 << "\n";
    outFile << Ls << " " << G << " " << tax << "\n";
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

qreal* milano3::setLabels(const QString& name)
{
    if (name == "G")
	return( &G );
    if (name == "empl")
	return( &empl );
    if (name == "capital")
        return( &capital );
    if (name == "output")
	return( &output );
    if (name == "a")
	return( &a );
    if (name == "b")
	return( &b );
    if (name == "alfa")
	return( &alfa );
   if (name == "mqreal")
    return( &mqreal );
    if (name == "pgt")
	return( &pgt );
    if (name == "rqreal")
        return( &rqreal );
    if (name == "teta")
         return( &teta );
    if (name == "zt")
        return( &zt ); 
    if (name == "mtr")
	return( &mtr );
    if (name == "wtr")
	return( &wtr );
    if (name == "rtr")
        return( &rtr );
    if (name == "psi1")
	return( &psi1 );
    if (name == "psi2")
	return( &psi2 );
    if (name == "nu1")
	return( &nu1 );
    if (name == "nu2")
	return( &nu2 );
    if (name == "h")
	return( &h );
    if (name == "tax")
	return( &tax );
    if (name == "Ls")
	return( &Ls );
    if (name == "alfa0")
	return( &alfa0 );
    if (name == "mqreal0")
    return( &mqreal0 );
    if (name == "pg0")
	return( &pg0 );
    if (name == "z0")
        return( &z0 );
    if (name == "n1")
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






















