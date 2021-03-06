/******************************************************************************/
/*                                                                           */
/* Module name:  milano_bcw.C                                                       */
/* Contents:     simlinph.c                                                  */
/*                                                                           */
/* Modified:     98/08/17 Marc Mueller                                       */
/*               99/07/16 Gerd,Luca                                          */
/******************************************************************************/

#include "milano_bcw.h"
#include "../logger.h"

/******************************************************************************/
milano_bcw::milano_bcw() : baseModel(3) {		    // default constructor
	}

//  milano_bcw::~milano_bcw();		            // destructor

/******************************************************************************/
qreal milano_bcw::virtual_Kemployment() {
	 double K_emp;

	 if (h*b<1)
		  K_emp=b/(alfa*(1-h*b))*(h*(1-tax)*pgt+mqreal+G);
	 else
		  K_emp=Ls+1;
	 return K_emp;
}

/******************************************************************************/
qreal milano_bcw::labour_demand() {
	 double l_dem;
	 l_dem=n1*exp(1/(b-1)*log(alfa/(a*b)));
	 return l_dem;
}

/******************************************************************************/
void milano_bcw::diseq_regime(qreal K_emp, qreal l_dem) {
	 if (K_emp<=l_dem)

		 if (K_emp<=Ls)
			 keynesian_unemployment(K_emp);

		 else
			 repressed_inflation(l_dem);
	 else
		 if (l_dem<=Ls)
			 classical_unemployment(l_dem);

		 else
			 repressed_inflation(l_dem);

}

/******************************************************************************/
void milano_bcw::keynesian_unemployment(qreal K_emp) {
	  double lambdas;
	  double gammas;
	  double oldpgt=pgt;
	  double teta;
	  double oldalfa=alfa;

	  empl=K_emp;
	  output=alfa*empl/b;
	  lambdas=empl/Ls;
	  gammas=exp((1-b)*log(output/(n1*a)))*exp(b*log(alfa/(a*b)));
//	  mtr=mqreal;
//	  wtr=alfa;
	  teta=1-mu1*(1-gammas);
	  pgt=(output-alfa*empl)/teta;
	  mqreal=(mqreal+G+(1-tax)*oldpgt)/teta-pgt;
	  alfa=(1-nu1*(1-lambdas))*alfa/teta;
          unemp=((Ls-empl)/Ls)*100;
//	  teta1=(teta-1)*100;
	  winf=(alfa*teta-oldalfa)/oldalfa;
}

/******************************************************************************/
void milano_bcw::repressed_inflation (qreal l_dem) {
	  double lambdad;
	  double gammad;
	  double delta;
	  double epsilon;
	  double oldpgt=pgt;
	  double teta;
	  double oldalfa=alfa;

	  empl=Ls;
	  output=alfa*empl/b;
	  lambdad=Ls/l_dem;
	  if (output<G){
		  epsilon=output/G;
		  delta=0;
		  gammad=0;
	  }
	  else{
			epsilon=1;
			if (output<G+(mqreal)){
				delta=(output-G)/mqreal;
				gammad=0;
			}
			else{
				delta=1;
				gammad=(output-mqreal-G)/(h*(1-tax)*pgt+h*alfa*empl);
			}
	  }
	  teta=1+mu2*(1-(gammad+epsilon+delta)/3.0);
	  pgt=(output-alfa*empl)/teta;
//	  mtr=mqreal;
//	  wtr=alfa;
	  mqreal=(delta*mqreal+epsilon*G+(1-tax)*oldpgt)/teta-pgt;
	  alfa=(1+nu2*(1-lambdad))*alfa/teta;
          unemp=((Ls-empl)/Ls)*100;
//	  teta1=(teta-1)*100;
	  winf=(alfa*teta-oldalfa)/oldalfa;
}

/******************************************************************************/
void milano_bcw::classical_unemployment(qreal l_dem) {
	  double lambdas;
	  double gammad;
	  double delta;
	  double epsilon;
	  double oldpgt=pgt;
	  double teta; 
	  double oldalfa=alfa;

	  empl=l_dem;
	  output=alfa*empl/b;
	  lambdas=empl/Ls;
	  if (output<G){
		  epsilon=output/G;
		  delta=0;
		  gammad=0;
	  }
	  else{
		  epsilon=1;
		  if (output<G+(mqreal)){
				delta=(output-G)/mqreal;
				gammad=0;
		  }
		  else{
				delta=1;
				gammad=(output-mqreal-G)/(h*(1-tax)*pgt+h*alfa*empl);
			}
	  }
	  teta=1+mu2*(1-(gammad+epsilon+delta)/3.0);
	  pgt=(output-alfa*empl)/teta;
//	  mtr=mqreal;
//	  wtr=alfa;
	  mqreal=(delta*mqreal+epsilon*G+(1-tax)*oldpgt)/teta-pgt;
	  alfa=(1-nu1*(1-lambdas))*alfa/teta;
          unemp=((Ls-empl)/Ls)*100;
//	  teta1=(teta-1)*100;
	  winf=(alfa*teta-oldalfa)/oldalfa;
}

/******************************************************************************/
qreal* milano_bcw::setLabels(const QString& name) {       // return a pointer to a variable or
					// a parameter specified by its name
	 if (name == "G")
		  return( &G );
	 if (name == "empl")
		  return( &empl );
	 if (name == "unemp")
		  return( &unemp );
	 if (name == "teta1")
		  return( &teta1 );
	 if (name == "output")
		  return( &output );
	 if (name == "winf")
		  return( &winf );
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
	 if (name == "mtr") 
		  return( &mtr );
	 if (name == "wtr")
		  return( &wtr );
	 if (name == "mu1")
		  return( &mu1 );
	 if (name == "mu2")
		  return( &mu2 );
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
	 if (name == "n1")
		  return( (qreal*) &n1 );
	 return( NULL );
}

/******************************************************************************/
void milano_bcw::initialize() {	    // initialize the model
    alfa=alfa0;
    pgt=pg0;
    mqreal=mqreal0;
}

/******************************************************************************/
void milano_bcw::loadParamset(QTextStream& inputFile) { // load parameterset from a file
    inputFile >> a >> b;
    inputFile >> mu1 >> mu2 >> nu1 >> nu2;
    inputFile >> n1 >> length;
    inputFile >> h >> pg0 >> alfa0 >> mqreal0;
    inputFile >> Ls >> G >> tax;
    initialize();
}

/******************************************************************************/
void milano_bcw::saveParamsetWithNames(QTextStream& outputFile) {  // add parametset to XPM printerfile
    outputFile << "defaultModel:\n\t";
    outputFile << "a = " << a << "\tb = " << b << "\n\t";
    outputFile << "mu1 = " << mu1 << "\tmu2 = " << mu2 << "\t";
    outputFile << "nu1 = " << nu1 << "\tnu2 = " << nu2 << "\n\t";
    outputFile << "n1 = " << n1 << "\tlength = " << length << "\n\t";
    outputFile << "h = " << h << "\tpg0 = " << pg0 << "\t";
    outputFile << "alfa0 = " << alfa0 << "\tmqreal0 = " << mqreal0 << "\n\t";
    outputFile << "Ls = " << Ls << "\tG = " << G << "\ttax = " << tax;
}

/******************************************************************************/
void milano_bcw::sendStateSpace(int &quantity,QList<qreal *> *stateSpace) {
//return pointers to all model variables and the dimension of the model
    stateSpace->clear();
    quantity = dimension;
    *stateSpace << &mqreal;
    *stateSpace << &alfa;
    *stateSpace << &pgt;
}

/******************************************************************************/
void milano_bcw::iteration(const qint64&) {  // perform one iteration 
	double K_emp;
	double l_dem;

	K_emp=virtual_Kemployment();
	l_dem=labour_demand();
	diseq_regime(K_emp,l_dem);
//log() << "teta1=" << teta1 << "\tunemp=" << unemp 
}
