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
qreal milano2::mu_function(const qreal& mu, const qreal& epsilon, const qreal& b,
         const qreal& c)
{
  return ( epsilon*pow(mu,1-b) 
	   + pow(1-epsilon*mu,1-b)/pow(1-epsilon,-b)
	   -c );
}

qreal milano2::regfal(int qMax_iter, qreal start_left, qreal start_right, qreal c)
{
  qreal oldzero;
  qreal zero(start_left);
  qreal left(start_left);
  qreal right(start_right);

  int iter=0;

  do
    {
      iter++;
      oldzero=zero;
      qreal value_left( mu_function(left,epsilon,b,c) );
      qreal value_right( mu_function(right,epsilon,b,c) );
      qreal m=( value_left-value_right )/(left-right);
      zero=left-value_left/m;

      // assert(zero>=left && zero<=right);

      qreal value_zero(mu_function(zero,epsilon,b,c));

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
  while ( iter < qMax_iter && qAbs(zero-oldzero) > 1E-4);

  // clog << "regfal: needed " << iter << " iterations." 

  return zero;
}



/******************************************************************************/
/*                                                                            */
/*   Function: virtual_s1employment                                           */
/*   Purpose:  compute the employment in the system1 regime  (main loop)      */
/*   Last modified: 31.05.1996 (Jorg Nikutta)                                */
/*                                                                            */
/******************************************************************************/

qreal milano2::virtual_s1employment()
{
    qreal s1_emp;

    s1_emp=1/(alfa*(1/a-h))*(h*((1-tax)*pgt+zt)+mqreal+G);
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

qreal milano2::virtual_s4employment()
{
     qreal s4_emp;

     if(b/(a*(1-h))>rqreal)
        s4_emp=((1-h)*((1-tax)*pgt+zt))/((alfa*b)/(rqreal*a)-(1-h)*alfa);
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

qreal milano2::labour_demand()
{
    qreal l_dem;
    
    l_dem=n1*exp((1-b)/(1-a-b)*log(a/alfa))*exp(b/(1-a-b)*log(b/rqreal));
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

void milano2::diseq_regime(const qreal& s1_emp, const qreal& s4_emp,
const qreal& l_dem)
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

void milano2::system_1(const qreal& s1_emp)
{
    qreal lambdas;
    qreal mus;
    qreal gammas;
    qreal oldpgt=pgt;
    qreal oldzt=zt;
 //   qreal teta;

    empl=s1_emp;
    output=alfa*empl/a;
    capital=alfa*b*empl/(rqreal*a);
    lambdas=empl/Ls;
    mus=capital/((1-h)*(alfa*empl+(1-tax)*pgt+zt));
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
    rqreal=(exp(omikron1*log(mus))*rqreal)/teta;
    if (pgt<0) log() << "  Gewinn ist negativ.";
    if ( ((1-tax)*pgt + zt)<0 ) log() << "  Anfangsausstattung ist negativ.";

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

void milano2::system_2(const qreal& l_dem)
{
    qreal lambdas;
    qreal mus;
    qreal gammad;
    qreal delta;
    qreal beta;
    qreal oldpgt=pgt;
    qreal oldzt=zt;
 //   qreal teta;

    empl=l_dem;
    output=alfa*empl/a;
    capital=alfa*b*empl/(rqreal*a);
    lambdas=empl/Ls;
    mus=capital/((1-h)*(alfa*empl+(1-tax)*pgt+zt));
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
    rqreal=(exp(omikron1*log(mus))*rqreal)/teta;
    if (pgt<0) log() << "  Gewinn ist negativ.";
    if ( ((1-tax)*pgt + zt)<0 ) log() << "  Anfangsausstattung ist negativ.";

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

void milano2::system_4(const qreal& s4_emp)
{
    qreal lambdas;
    qreal mud;
    qreal gammad;
    qreal delta;
    qreal beta;
    qreal oldpgt=pgt;
    qreal oldzt=zt;
//    qreal teta;
    qreal c;

    empl=s4_emp;
    output=alfa*empl/a;
    capital=alfa*b*empl/(rqreal*a);
    lambdas=empl/Ls;

    c=exp((1-a-b)*log(capital/n1))*exp((-1+a)*log(b/rqreal))*
    exp(-a*log(a/alfa));

    if(c>=exp(b*log(1-epsilon)) && c<=1)
      {
    /* qreal mudxxx=(1-epsilon-exp(2*log(c)))/epsilon+2*exp(2*log(c))-2*
	  exp(0.5*log((exp(2*log(c)))/epsilon-(exp(4*log(c)))/epsilon-
		      exp(2*log(c))+exp(4*log(c))));
		      */

	mud=regfal(100,0,1,c);
	// clog << "Hi! did a regfal" 
      }
    else
      {
    if ( n1*exp(b/(1-a-b)*log(b/rqreal))
	     *exp((1-b)/(1-a-b)*log(a/alfa))
	     *exp(b/(1-a-b)*log(1-epsilon)) > s4_emp
	     )
	  {
        rqreal=f*rqreal;
	    return;
	  }
	else
	  {
	    // clog << "milano2::system_4: unzulaessiger Parameterwert fuer c" 
        error("milano2::system_4: unzulaessiger Parameterwert fuer c: %lf", QString::number(c));
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
    rqreal=(exp(-omikron2*log(mud))*rqreal)/teta;
    if (pgt<0) log() << "  Gewinn ist negativ.";
    if ( ((1-tax)*pgt + zt)<0 ) log() << "  Anfangsausstattung ist negativ.";

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

void milano2::system_6(const qreal& l_dem)
{
    qreal lambdad;
    qreal mus;
    qreal gammad;
    qreal delta;
    qreal beta;
    qreal oldpgt=pgt;
    qreal oldzt=zt;
//    qreal teta;
    
    empl=Ls;
    output=alfa*empl/a;
    capital=alfa*b*empl/(rqreal*a);
    lambdad=empl/l_dem;
    mus=capital/((1-h)*(alfa*empl+(1-tax)*pgt+zt));
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
    rqreal=(exp(omikron1*log(mus))*rqreal)/teta;
    if (pgt<0) log() << "  Gewinn ist negativ.";
    if ( ((1-tax)*pgt + zt)<0 ) log() << "  Anfangsausstattung ist negativ.";

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



void milano2::iteration(const qint64& t)
{
/*
    qreal s1_emp;
    qreal s4_emp;
    qreal l_dem;

    s1_emp=virtual_s1employment();
    s4_emp=virtual_s4employment();
    l_dem=labour_demand();
    diseq_regime(s1_emp,s4_emp,l_dem);
*/
log() << "model not active - contact A. Foerster";

}



/* loadParamset Last modified: 31.05.1996 (Jorg Nikutta) */

void milano2::loadParamset(QTextStream& inFile)
{
    inFile >> a >> b;
    inFile >> psi1 >> psi2 >> nu1 >> nu2 >> omikron1 >> omikron2;
    inFile >> n1 >> simd;
    inFile >> h >> pg0 >> alfa0 >> mqreal0 >> rqreal0 >> z0;
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
    log() << "a      :  " << a <<"\talpha0 : " << alfa0;
    log() << "b      :  " << b <<"\tmqreal0 : " << mqreal0;
    log() << "rqreal0 :  " << rqreal0 <<"\tz0 : " << z0;
    log() << "epsilon : " << epsilon;
    log() << "f      :  " << f;
    log() << "n1     :  " << n1;
    log() << "psi1   :  " << psi1 << "\tpsi2 : " << psi2;
    log() << "omikron1 : " << omikron1 << "\tomikron2 : " << omikron2;
    log() << "nu1    :  " << nu1 << "\tG   : " << G;
    log() << "nu2    :  " << nu2 << "\ttax : " << tax;
    log() << "h      :  " << h << "\tsimd: " << simd;
    log() << "pg0    :  " << pg0 << "\tLs  : " << Ls;
}

/******************************************************************************/
/*                                                                           */
/*   Function:       saveParamset                                            */
/*   Purpose:        write current parameterset to the disk                  */
/*   Last modified:  31.05.1996 (Jorg Nikutta)                             */
/*                                                                           */
/*****************************************************************************/
void milano2::saveParamset(QTextStream& outFile)
{
    outFile << a << " " << b;
    outFile << psi1 << " " << psi2;
    outFile << nu1 << " " << nu2 << " " << omikron1 << " " << omikron2;
    outFile << n1 << " " << simd;
    outFile << h << " " << epsilon << " " << f;
    outFile << rqreal0 << " " << pg0 << " " << alfa0 << " " << mqreal0 << " " << z0;
    outFile << Ls << " " << G << " " << tax;
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

qreal* milano2::setLabels(const QString& name)
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
    if (name == "omikron1")
        return( &omikron1 );
    if (name == "omikron2")
        return( &omikron2 );
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
    if (name == "rqreal0")
        return( &rqreal0 );
    if (name == "z0")
        return( &z0 );
    if (name == "epsilon")
        return( &epsilon ); 
    if (name == "f")
        return( &f );
    if (name == "n1")
    return( (qreal*) &n1 );
    return( NULL );
}

void milano2::initialize()
{
    alfa=alfa0;
    pgt=pg0;
    mqreal=mqreal0;
    rqreal=rqreal0;
    zt=z0;
}


qreal* milano2::sendModelVar()
{
    return &alfa; // return the main model variable
}

void milano2::sendStateSpace(int &quantity,QList<qreal *> *stateSpace)
{
    stateSpace->clear();
    quantity = dimension;


    *stateSpace << &mqreal;
    *stateSpace << &alfa;
/*    *stateSpace << &pt;*/
    *stateSpace << &pgt;
    *stateSpace << &rqreal;
    *stateSpace << &zt; 
}


// return pointers to all model
					    // variables and the dimension
					    // of the model
void milano2::sendParameters(int&,qreal**)
{
} // write all parameters
                   
                             // into an array and return the numbers
				// of parameters
void milano2::receiveParameters(const QList<qreal> &parameters)
{
    if (parameters.size() != 21) log() << "Wrong number of parameters!";
    else
    {
        a = parameters.at(0);
        b = parameters.at(1);
        psi1 = parameters.at(2);
        psi2 = parameters.at(3);
        nu1 = parameters.at(4);
        nu2 = parameters.at(5);
        omikron1 = parameters.at(6);
        omikron2 = parameters.at(7);
        n1 = parameters.at(8);
        simd = parameters.at(9);
        h = parameters.at(10);
        pg0 = parameters.at(11);
        alfa0 = parameters.at(12);
        mqreal0 = parameters.at(13);
        rqreal0 = parameters.at(14);
        z0 = parameters.at(15);
        Ls = parameters.at(16);
        G = parameters.at(17);
        tax = parameters.at(18);
        epsilon = parameters.at(19);
        f = parameters.at(20);

        initialize();
    }
}
// receive parameter values 
