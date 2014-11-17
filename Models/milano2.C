#include "milano2.h"
#include "../error.h"

//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/milano2.C,v 1.1 2000/08/31 16:06:32 mmeyer Exp $

/******************************************************************************/
/*                                                                            */
/* Class name:      milano2                                                    */
/* Member function: milano2                                                    */
/* Purpose:         constructor                                               */
/* Last modified:   14.06.1996 (Uli Middelberg)                               */
/*                                                                            */
/******************************************************************************/


milano2::milano2() : baseModel(5)
{
}

milano2::~milano2()
{
}




/******************************************************************************/
/*                                                                            */
/*   Function: regfal                                           */
/*   Purpose:                                                                */
/*   Last modified:                                                          */
/*                                                                            */
/******************************************************************************/
real milano2::mu_function(const real& mu, const real& epsilon, const real& b,
		 const real& c)
{
  return ( epsilon*pow(mu,1-b) 
	   + pow(1-epsilon*mu,1-b)/pow(1-epsilon,-b)
	   -c );
}

real milano2::regfal(int max_iter, real start_left, real start_right, real c)
{
  real oldzero;
  real zero(start_left);
  real left(start_left);
  real right(start_right);

  int iter=0;

  do
    {
      iter++;
      oldzero=zero;
      real value_left( mu_function(left,epsilon,b,c) );
      real value_right( mu_function(right,epsilon,b,c) );
      real m=( value_left-value_right )/(left-right);    
      zero=left-value_left/m;

      // assert(zero>=left && zero<=right);

      real value_zero(mu_function(zero,epsilon,b,c));

      if ( value_left*value_zero < 0 )
	{
	  right=zero;
	}
      else
	{
	  //  assert( value_right*value_zero < 0 );
	  left=zero;
	}
    }
  while ( iter < max_iter && abs(zero-oldzero) > 1E-4);

  // clog << "regfal: needed " << iter << " iterations." << endl;

  return zero;
}



/******************************************************************************/
/*                                                                            */
/*   Function: virtual_s1employment                                           */
/*   Purpose:  compute the employment in the system1 regime  (main loop)      */
/*   Last modified: 31.05.1996 (Jorg Nikutta)                                */
/*                                                                            */
/******************************************************************************/

real milano2::virtual_s1employment()
{
    real s1_emp;

    s1_emp=1/(alfa*(1/a-h))*(h*((1-tax)*pgt+zt)+mreal+G);
    return s1_emp;
}

/****************************************************************************/
/*                                                                           */
/*                                                                       */
/*   Function: virtual_s4employment                                          */
/*                                                                        */
/*   Purpose:  compute the employment in the system4 regime (main loop)      */
/*                                                                        */   
/*****************************************************************************/

real milano2::virtual_s4employment()
{
     real s4_emp;

     if(b/(a*(1-h))>rreal)
        s4_emp=((1-h)*((1-tax)*pgt+zt))/((alfa*b)/(rreal*a)-(1-h)*alfa);
     else
        s4_emp=Ls+1;
     return s4_emp;
}    
/******************************************************************************/
/*                                                                            */
/*   Function: labour_demand                                                  */
/*   Input:                                                                   */
/*   Output:                                                                  */
/*   Purpose: compute the labour demand (main loop)                           */
/*   Last modified: 31.05.1996 (Jorg Nikutta)                               */
/*                                                                            */
/******************************************************************************/

real milano2::labour_demand()
{
    real l_dem;
    
    l_dem=n1*exp((1-b)/(1-a-b)*log(a/alfa))*exp(b/(1-a-b)*log(b/rreal));
    return l_dem;
}

/******************************************************************************/
/*                                                                            */
/*   Function: diseq_regime                                                   */
/*   Input:                                                                   */
/*   Output:                                                                  */
/*   Purpose: define the disequilibrium regimes (main loop)                   */
/*   Last modified: 07.02.1995                                                */
/*                                                                            */
/******************************************************************************/

void milano2::diseq_regime(const real& s1_emp, const real& s4_emp,
const real& l_dem)
{
    if(s1_emp<=Ls)
        if(s1_emp<=l_dem)
            if(s1_emp<s4_emp)
              system_1(s1_emp);    
            else
              system_4(s4_emp);
        else 
	    if(l_dem<s4_emp)
              system_2(l_dem);
            else
              system_4(s4_emp);
    else
	if(Ls<=l_dem)  
	    if(Ls<s4_emp)
              system_6(l_dem);
            else
              system_4(s4_emp);
	else
            if(l_dem<s4_emp)
              system_2(l_dem);
            else
              system_4(s4_emp); 
	   
}

/******************************************************************************/
/*                                                                            */
/*   Function: system_1                                                      */
/*   Input:                                                                   */
/*   Output: employment,output,capital stock,profit,wage,interest rate,   */
/*           prices,money stock,interest                                  */
/*   Purpose: analysis of system_1                                        */
/*   Last modified: 31.05.1996 (Jorg Nikutta)                                */
/*                                                                            */
/******************************************************************************/

void milano2::system_1(const real& s1_emp)
{
    real lambdas;
    real mus;
    real gammas;
    real oldpgt=pgt;
    real oldzt=zt;
 //   real teta;

    empl=s1_emp;
    output=alfa*empl/a;
    capital=alfa*b*empl/(rreal*a);
    lambdas=empl/Ls;
    mus=capital/((1-h)*(alfa*empl+(1-tax)*pgt+zt));
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
    rreal=(exp(omikron1*log(mus))*rreal)/teta;
    if (pgt<0) cout << "  Gewinn ist negativ." << endl;
    if ( ((1-tax)*pgt + zt)<0 ) cout << "  Anfangsausstattung ist negativ." << endl;

}

/******************************************************************************/
/*                                                                            */
/*   Function: system_2                                                       */
/*   Input:                                                                   */
/*   Output: employment,output,capital stock,profit,wage,interest rate,      */
/*           prices,money stock,interest                                     */
/*                                                                           */
/*   Purpose: analysis of system_2                                           */
/*   Last modified: 31.05.1996 (Jorg Nikutta)                                */
/*                                                                            */
/******************************************************************************/

void milano2::system_2(const real& l_dem)
{
    real lambdas;
    real mus;
    real gammad;
    real delta;
    real beta;
    real oldpgt=pgt;
    real oldzt=zt;
 //   real teta;

    empl=l_dem;
    output=alfa*empl/a;
    capital=alfa*b*empl/(rreal*a);
    lambdas=empl/Ls;
    mus=capital/((1-h)*(alfa*empl+(1-tax)*pgt+zt));
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
    rreal=(exp(omikron1*log(mus))*rreal)/teta;
    if (pgt<0) cout << "  Gewinn ist negativ." << endl;
    if ( ((1-tax)*pgt + zt)<0 ) cout << "  Anfangsausstattung ist negativ." << endl;

}

/******************************************************************************/
/*                                                                            */
/*   Function: system_4                                                      */
/*   Input:                                                                   */
/*   Output: employment,output,capital stock,profit,wage,interest rate,      */
/*           prices,money stock,interest                                     */
/*   Purpose: analysis of system_4                                           */
/*   Last modified:31.05.1996 Jorg Nikutta*/
/*                                                                            */
/******************************************************************************/

void milano2::system_4(const real& s4_emp)
{
    real lambdas;
    real mud;
    real gammad;
    real delta;
    real beta;
    real oldpgt=pgt;
    real oldzt=zt;
//    real teta;
    real c;

    empl=s4_emp;
    output=alfa*empl/a;
    capital=alfa*b*empl/(rreal*a);
    lambdas=empl/Ls;

    c=exp((1-a-b)*log(capital/n1))*exp((-1+a)*log(b/rreal))*
    exp(-a*log(a/alfa));

    if(c>=exp(b*log(1-epsilon)) && c<=1)
      {
	/* real mudxxx=(1-epsilon-exp(2*log(c)))/epsilon+2*exp(2*log(c))-2*
	  exp(0.5*log((exp(2*log(c)))/epsilon-(exp(4*log(c)))/epsilon-
		      exp(2*log(c))+exp(4*log(c))));
		      */

	mud=regfal(100,0,1,c);
	// clog << "Hi! did a regfal" << endl;
      }
    else
      {
	if ( n1*exp(b/(1-a-b)*log(b/rreal))
	     *exp((1-b)/(1-a-b)*log(a/alfa))
	     *exp(b/(1-a-b)*log(1-epsilon)) > s4_emp
	     )
	  {
	    rreal=f*rreal;
	    return;
	  }
	else
	  {
	    // clog << "milano2::system_4: unzulaessiger Parameterwert fuer c" << endl;
	    error("milano2::system_4: unzulaessiger Parameterwert fuer c: %lf", c);
	    // exit(1);
	  }
      }      

     
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
    rreal=(exp(-omikron2*log(mud))*rreal)/teta;
    if (pgt<0) cout << "  Gewinn ist negativ." << endl;
    if ( ((1-tax)*pgt + zt)<0 ) cout << "  Anfangsausstattung ist negativ." << endl;

}

/******************************************************************************/
/*                                                                            */
/*   Function: system_6                                                       */
/*   Input:                                                                   */
/*   Output: employment,output,capital stock,profit,wage,interest rate,      */
/*           prices,money stock,interest                                     */
/*   Purpose: analysis of system_6                                           */
/*   Last modified:31.05.1996 Jorg Nikutta*/
/*                                                                            */
/******************************************************************************/

void milano2::system_6(const real& l_dem)
{
    real lambdad;
    real mus;
    real gammad;
    real delta;
    real beta;
    real oldpgt=pgt;
    real oldzt=zt;
//    real teta;
    
    empl=Ls;
    output=alfa*empl/a;
    capital=alfa*b*empl/(rreal*a);
    lambdad=empl/l_dem;
    mus=capital/((1-h)*(alfa*empl+(1-tax)*pgt+zt));
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
    rreal=(exp(omikron1*log(mus))*rreal)/teta;
    if (pgt<0) cout << "  Gewinn ist negativ." << endl;
    if ( ((1-tax)*pgt + zt)<0 ) cout << "  Anfangsausstattung ist negativ." << endl;

}

/******************************************************************************/
/*                                                                            */
/*   Function: iteration                                                      */
/*   Input:                                                                   */
/*   Output:                                                                  */
/*   Purpose: return to the main loop and continue the simulation             */
/*   Last modified:07.02.1995                                                 */
/*                                                                            */
/******************************************************************************/



void milano2::iteration(const long& t)
{
/*
    real s1_emp;
    real s4_emp;
    real l_dem;

    s1_emp=virtual_s1employment();
    s4_emp=virtual_s4employment();
    l_dem=labour_demand();
    diseq_regime(s1_emp,s4_emp,l_dem);
*/
cout << "model not active - contact A. Foerster" << endl;

}



/* loadParamset Last modified: 31.05.1996 (Jorg Nikutta) */

void milano2::loadParamset(ifstream& inFile)
{
    inFile >> a >> b;
    inFile >> psi1 >> psi2 >> nu1 >> nu2 >> omikron1 >> omikron2;
    inFile >> n1 >> simd;
    inFile >> h >> pg0 >> alfa0 >> mreal0 >> rreal0 >> z0;
    inFile >> Ls >> G >> tax >> epsilon >> f;

    initialize();
}

/******************************************************************************/
/*                                                                            */
/*   Function:       printParamset                                            */
/*   Purpose:        print current parameterset                               */
/*   Last modified:  31.05.1996 (Jorg Nikutta)                              */
/*                                                                            */
/******************************************************************************/

 

void milano2::printParamset()
{
    cout << "a      :  " << a <<"\talpha0 : " << alfa0 << endl;
    cout << "b      :  " << b <<"\tmreal0 : " << mreal0 << endl;
    cout << "rreal0 :  " << rreal0 <<"\tz0 : " << z0 << endl;
    cout << "epsilon : " << epsilon << endl;
    cout << "f      :  " << f << endl;
    cout << "n1     :  " << n1 << endl;
    cout << "psi1   :  " << psi1 << "\tpsi2 : " << psi2 << endl;
    cout << "omikron1 : " << omikron1 << "\tomikron2 : " << omikron2 << endl;
    cout << "nu1    :  " << nu1 << "\tG   : " << G << endl;
    cout << "nu2    :  " << nu2 << "\ttax : " << tax << endl;
    cout << "h      :  " << h << "\tsimd: " << simd << endl;
    cout << "pg0    :  " << pg0 << "\tLs  : " << Ls << endl;
}

/******************************************************************************/
/*                                                                           */
/*   Function:       saveParamset                                            */
/*   Purpose:        write current parameterset to the disk                  */
/*   Last modified:  31.05.1996 (Jorg Nikutta)                             */
/*                                                                           */
/*****************************************************************************/
void milano2::saveParamset(ofstream& outFile)
{
    outFile << a << " " << b << endl;
    outFile << psi1 << " " << psi2 << endl; 
    outFile << nu1 << " " << nu2 << " " << omikron1 << " " << omikron2 << endl;
    outFile << n1 << " " << simd << endl;
    outFile << h << " " << epsilon << " " << f << endl;
    outFile << rreal0 << " " << pg0 << " " << alfa0 << " " << mreal0 << " " << z0 << endl;
    outFile << Ls << " " << G << " " << tax << endl;
}

/******************************************************************************/
/*                                                                            */
/*   Function:       set_labels                                               */
/*   Input:          pointer to the name of the label                         */
/*   Return:         pointer to the corresponding variable                    */
/*   Purpose:        set pointer to the variable that shall be plotted on the */
/*                   label                                                    */
/*   Last modified:  31.05.1996 (Jorg Nikutta)                                */
/*                                                                            */
/******************************************************************************/

real* milano2::setLabels(char* name)
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
    if( !strcmp(name,"omikron1") )
        return( &omikron1 );
    if( !strcmp(name,"omikron2") )
        return( &omikron2 );
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
    if( !strcmp(name,"rreal0") )
        return( &rreal0 );
    if( !strcmp(name,"z0") )
        return( &z0 );
    if( !strcmp(name,"epsilon") )
        return( &epsilon ); 
    if( !strcmp(name,"f") )
        return( &f );
    if( !strcmp(name,"n1") )
	return( (real*) &n1 );
    return( NULL );
}

void milano2::initialize()
{
    alfa=alfa0;
    pgt=pg0;
    mreal=mreal0;
    rreal=rreal0;
    zt=z0;
}


real* milano2::sendModelVar()
{
    return &alfa; // return the main model variable
}

void milano2::sendStateSpace(int &quantity,const real*** stateSpace)
{
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const real* [dimension];
    if( !(*stateSpace) )
	fatalError("milano2::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;


    (*stateSpace)[0]=&mreal;
    (*stateSpace)[1]=&alfa;
/*    (*stateSpace)[2]=&pt;*/
    (*stateSpace)[2]=&pgt;
    (*stateSpace)[3]=&rreal;
    (*stateSpace)[4]=&zt; 
}


// return pointers to all model
					    // variables and the dimension
					    // of the model
void milano2::sendParameters(int&,real**)
{
} // write all parameters
                   
                             // into an array and return the numbers
				// of parameters
void milano2::receiveParameters(const real*)
{
}
// receive parameter values 
