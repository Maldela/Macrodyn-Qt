///////////////////////////////////////////////////////////////////////////////
//
// $Header: finanzmarkt_JanE.C, Thu Jul  6 17:09:23 METDST 2000 mmeyer
//
// Module name:		finanzmarkt_JanE.C
// Contents:		Member functions of the class finanzmarkt_JanE
//
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "finanzmarkt_JanE.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	finanzmarkt_JanE
// Member function:	constructor
// Purpose:		constructor
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

finanzmarkt_JanE::finanzmarkt_JanE() : baseModel(2)
{
	zvar1	= NULL;
	zvar2	= NULL;
	zvar3	= NULL;
	zvar4	= NULL;
	cout << "Reached constructor in finanzmarkt_JanE" << endl;

	d = new matrix_neu(2,1);
	if( !(d) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	Ed = new matrix_neu(2,1);
	if( !(Ed) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	p = new matrix_neu(2,1);
	if( !(p) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	p_old = new matrix_neu(2,1);
	if( !(p_old) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	
	q0 = new matrix_neu(2,1);
	if( !(q0) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	q = new matrix_neu(2,1);
	if( !(q) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	q_C = new matrix_neu(2,1);
	if( !(q_C) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	q_F = new matrix_neu(2,1);
	if( !(q_F) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	q_Fold = new matrix_neu(2,1);
	if( !(q_Fold) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	q_N = new matrix_neu(2,1);
	if( !(q_N) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	
	x_all = new matrix_neu(2,1);
	if( !(x_all) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	x_N = new matrix_neu(2,1);
	if( !(x_N) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	x_F = new matrix_neu(2,1);
	if( !(x_F) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	x_C = new matrix_neu(2,1);
	if( !(x_C) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	
	x_Nold = new matrix_neu(2,1);
	if( !(x_Nold) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	x_Fold = new matrix_neu(2,1);
	if( !(x_Fold) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	x_Cold = new matrix_neu(2,1);
	if( !(x_Cold) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	
	xi = new matrix_neu(2,1);
	if( !(xi) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	E_xi = new matrix_neu(2,1);
	if( !(E_xi) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	T = new matrix_neu(2,2);
	if( !(T) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	T_inv = new matrix_neu(2,2);
	if( !(T_inv) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	epsilon = new matrix_neu(2,1);
	if( !(epsilon) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	E_epsilon = new matrix_neu(2,1);
	if( !(E_epsilon) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	V_epsilon = new matrix_neu(2,2);
	if( !(V_epsilon) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	V_epsilon_inv = new matrix_neu(2,2);
	if( !(V_epsilon_inv) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	b = new matrix_neu(2,1);
	if( !(b) )
		fatalError("finanzmarkt_JanE:","can't create matrix b");
	A = new matrix_neu(2,2);
	if( !(A) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	A2_F = new matrix_neu(2,2);
	if( !(A2_F) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	A_C = new matrix_neu(2,2);
	if( !(A_C) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	A_F = new matrix_neu(2,2);
	if( !(A_F) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	A_N = new matrix_neu(2,2);
	if( !(A_N) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	

	V_C = new matrix_neu(2,2);
	if( !(V_C) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	V_F = new matrix_neu(2,2);
	if( !(V_F) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	V_N = new matrix_neu(2,2);
	if( !(V_N) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	

	for(int i=0;i<L;i++){
		Rho[i] = new matrix_neu(2,2);
		if( !(Rho[i]) )
			fatalError("finanzmarkt_JanE:","can't create matrix");	
		q_MA[i] = new matrix_neu(2,1);
		if( !(q_MA[i]) )
			fatalError("finanzmarkt_JanE:","can't create matrix");	
	}

	V_C_inv = new matrix_neu(2,2);
	if( !(V_C_inv) )
		fatalError("finanzmarkt_JanE:","can't create matrix");
	V_F_inv = new matrix_neu(2,2);
	if( !(V_F_inv) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	V_N_inv = new matrix_neu(2,2);
	if( !(V_N_inv) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	temp1_2_2 = new matrix_neu(2,2);
	if( !(temp1_2_2) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	temp2_2_2 = new matrix_neu(2,2);
	if( !(temp2_2_2) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	temp3_2_2 = new matrix_neu(2,2);
	if( !(temp3_2_2) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	temp4_2_2 = new matrix_neu(2,2);
	if( !(temp4_2_2) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	temp1_2_1 = new matrix_neu(2,1);
	if( !(temp1_2_1) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	temp2_2_1 = new matrix_neu(2,1);
	if( !(temp2_2_1) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	temp3_2_1 = new matrix_neu(2,1);
	if( !(temp3_2_1) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	temp4_2_1 = new matrix_neu(2,1);
	if( !(temp4_2_1) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	temp1_1_1 = new matrix_neu(1,1);
	if( !(temp1_1_1) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	V_xi = new matrix_neu(2,2);
	if( !(V_xi) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	
	V_xi_inv = new matrix_neu(2,2);
	if( !(V_xi_inv) )
		fatalError("finanzmarkt_JanE:","can't create matrix");	


//if(zvar3) delete zvar3;
//	zvar3	=	new rand_var("ranf",1, 0, 1);
//if(zvar4) delete zvar4;
//	zvar4	=	new rand_var("ranf",1, 0, 1);
	
	beta=-100;	//Wert bei dem Beta-Unterstützung ausfällt
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	finanzmarkt_JanE	
// Member function:	 ~finanzmarkt_JanE
// Purpose:		destructor
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

finanzmarkt_JanE::~finanzmarkt_JanE()
{
	cout << "Reached destructor in finanzmarkt_JanE" << endl;
	if(zvar1) 		delete zvar1;
	if(zvar2) 		delete zvar2;
	if(zvar3) 		delete zvar3;
	if(zvar4) 		delete zvar4;
	if(d) 				delete d;
	if(Ed) 				delete Ed;
	if(p) 				delete p;
	if(p_old)			delete p_old;
	
	if(q0) 		 		delete q0;
	if(q) 		 		delete q;
	if(q_C) 			delete q_C;
	if(q_F) 			delete q_F;
	if(q_Fold) 			delete q_Fold;
	if(q_N) 			delete q_N;
	
	if(xi) 				delete xi;
	if(E_xi) 			delete E_xi;
	if(T) 				delete T;
	if(T_inv) 			delete T_inv;
	if(epsilon) 		delete epsilon;
	if(E_epsilon) 		delete E_epsilon;
	if(V_epsilon) 		delete V_epsilon;
	if(b) 				delete b;
	
	if(x_all) 			delete x_all;
	
	if(x_N) 			delete x_N;
	if(x_F) 			delete x_F;
	if(x_C) 			delete x_C;
	
	if(x_Nold) 			delete x_Nold;
	if(x_Fold) 			delete x_Fold;
	if(x_Cold) 			delete x_Cold;

	if(A) 				delete A;
	if(A2_F) 			delete A2_F;
	if(A_F) 			delete A_F;
	if(A_C) 			delete A_C;
	if(A_N) 			delete A_N;
	
	if(V_F) 			delete V_F;
	if(V_C) 			delete V_C;
	if(V_N) 			delete V_N;
	
	if(Rho) 			delete Rho;
	for(int i=0;i<L;i++){
		if(Rho[i]) delete Rho[i];
		if(q_MA[i]) delete q_MA[i];
	}

	if(V_C_inv) delete(V_C_inv);
	if(V_F_inv) delete(V_F_inv);
	if(V_N_inv) delete(V_N_inv);
	
	if(temp1_2_2) delete(temp1_2_2);
	if(temp2_2_2) delete(temp2_2_2);
	if(temp3_2_2) delete(temp3_2_2);
	if(temp4_2_2) delete(temp4_2_2);
	if(temp1_2_1) delete(temp1_2_1);
	if(temp2_2_1) delete(temp2_2_1);
	if(temp3_2_1) delete(temp3_2_1);
	if(temp4_2_1) delete(temp4_2_1);
	if(temp1_1_1) delete(temp1_1_1);

	if(V_xi_inv) delete(V_xi_inv);
	if(V_xi) delete(V_xi);
	if(V_epsilon_inv) delete(V_epsilon_inv);
	
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	finanzmarkt_JanE
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void finanzmarkt_JanE::loadParamset(ifstream& inFile)
{ 	

	inFile >> eta1_F_0 >> eta2_F_0 >> eta3_F_0;	//Gruppenanteile d. Fundamentalisten (Startwert)			
	inFile >> eta1_C_0 >> eta2_C_0 >> eta3_C_0;	//Gruppenanteile d. Chartisten (Startwert)		
	inFile >>  alpha_1 >> alpha_2 >> alpha_3;	//Risikoparameter d.HH
	inFile >>  beta_1 >> beta_2 >> beta_3;		//Intensity of choice for households
	inFile >>  e_1 >> e_2 >> e_3;			//Anfangsausstattung d. HH
	inFile >>  r;					//Zinssatz
	inFile >>  x_all1 >> x_all2;			//Anzahl Aktien
	inFile >>  zeta1_F >> zeta2_F;			//Startportfolio Fundamentalist (Anteilswerte)
	inFile >>  zeta1_C >> zeta2_C;			//Startportfolio Chartisten (Anteilswerte)
	inFile >>  zeta1_N >> zeta2_N;			//Startportfolio Noisetraders (Anteilswerte)
	inFile >> switch_F >> switch_C >> switch_N;	//Schalter Investoren

	for(int i=0;i<L;i++)
	{						//Diagonalmatrix für Moving Average
	inFile >> rho[i];			
	}

	inFile >> q1_0;
	inFile >> q2_0;
						//Verzoegerte cd-Preise auf Startwert gesetzt		
    
	inFile >> a1 >> b1 >> c1;		//Parameter fuer die Dreieicksverteilung,
	inFile >> a2 >> b2 >> c2;		//des noise xi
	inFile >> delta;			//Korrelationsparameter
	inFile >> b_1 >> b_2;			//Shift-Parameter f. transformierte ZV epsilon
    
	if((b1<a1)||(b2<a2))
		fatalError("finanzmarkt_JanE::loadParameterset","invalid parameter");	
   	if((b1<c1)||(c1<a1))
		fatalError("finanzmarkt_JanE::loadParameterset","invalid parameter");	
   	if((b2<c2)||(c2<a2))
		fatalError("finanzmarkt_JanE::loadParameterset","invalid parameter");	
   	if(delta>=1||delta<=-1)
		fatalError("finanzmarkt_JanE::loadParameterset","invalid correlation-parameter");	
		   	
	inFile >> zetamin1 >> zetamax1 >> gamma1; //Parameterwerte fuer die 
	inFile >> zetamin2 >> zetamax2 >> gamma2;  //Dividenenprozesse
	inFile >> length;
/*auskommentiert!!!
if(zvar1) delete zvar1;
	zvar1	=	new rand_var("ranf",1, zetamin1, zetamax1);
if(zvar2) delete zvar2;
	zvar2	=	new rand_var("ranf",1, zetamin2, zetamax2);
*/

//	cout << "c-switch in ::loadParamset" << endl;
//	c1=-c1;
//	c2=-c2;
//	cout << "c-switch in ::initialize for bifurcation over c only" << endl;
	cout << "c-switch deactivated, new model with q_N=q_F+eps ..."<<endl;
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	finanzmarkt_JanE	
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void finanzmarkt_JanE::initialize()
{	
//	c1=-c1;
//	c2=-c2;
	
	real var1, var2;
	int i;
	R = 1+r;
	
	if ( beta != -100 ) { 
		beta_1=beta;
		beta_2=beta;
		beta_3=beta;
	}	
	
	for(i=0;i<L;i++)
		for(int j=0;j<2;j++)		// MA-Koeffizienten auf die Diagonalen setzen
			(*Rho[i])(j,j) = rho[i];	

/*	// previous initialized with r(?) changing to 0
	mu_F = 000;
	mu_C = 1000;	
	mu_N = 0;
*/
	mu_F = 0;
	mu_C = 0;	
	mu_N = 0;
	//again some dummy shit...
/*	if ( dummy >=0 ){
		mu_F = dummy;
		mu_C = mu_N = 0;
	} else {
		mu_C = (-1.0)*dummy;
		mu_F = mu_N = 0;
	};
*/	
	sigma_F = 0;
	sigma_C = 0;
	sigma_N = 0;
	
	var_F = 0;
	var_C = 0;
	var_N = 0;
	
	sr_F = 0;
	sr_C = 0;
	sr_N = 0;

	rend_F = 0;
	rend_C = 0;
	rend_N = 0;

//Berechnung der Marktanteile der Noisetraders
//Achtung: Die Marktanteile werden fuer Periode 0 (Investitionsbetrag der Investore)
//UND Periode 1 (Berechnung von a_F, a_C, a_N) verwendet!
	eta1_F = eta1_F_0;
	eta2_F = eta2_F_0;
	eta3_F = eta3_F_0;

	eta1_C = eta1_C_0;
	eta2_C = eta2_C_0;
	eta3_C = eta3_C_0;
	
	eta1_N = 1-eta1_F - eta1_C;
	eta2_N = 1-eta2_F - eta2_C;
	eta3_N = 1-eta3_F - eta3_C;

//Kleine Veränderung unbedingt wieder zurücksetzen, sobald Plots fertig sind!!!
/*	eta1_F = eta2_F = eta3_F = 1.0-dummy;
	eta1_C = eta2_C = eta3_C = dummy;
	eta1_N = eta2_N = eta3_N = 0;
	zeta1_F = 1.0-dummy;
	zeta2_F = 1.0-dummy;
	zeta1_N = dummy;
	zeta2_N = dummy;
*/	
	
//Startportfolios auf Null setzen
	
	(*x_all)(0,0) = x_all1;
	(*x_all)(1,0) = x_all2;


	x_F1 = zeta1_F*x_all1;
	x_F2 = zeta2_F*x_all2;
	x_C1 = zeta1_C*x_all1;
	x_C2 = zeta2_C*x_all2;
	x_N1 = zeta1_N*x_all1;
	x_N2 = zeta2_N*x_all2;


	(*x_F)(0,0) = x_F1;
	(*x_F)(1,0) = x_F2;
	(*x_C)(0,0) = x_C1;
	(*x_C)(1,0) = x_C2;
	(*x_N)(0,0) = x_N1;
	(*x_N)(1,0) = x_N2;


	(*q0)(0,0)= q1_0;
	(*q0)(1,0)= q2_0;
	

//Startwert der Dividenden = unbedingter Mittelwert
	d1 = ((zetamax1-zetamin1)/2)/(1-gamma1);	
	d2 = ((zetamax2-zetamin2)/2)/(1-gamma2);
	(*d)(0,0) = d1;
	(*d)(1,0) = d2;

//Bedingter Dividendenerwartungswert

Ed1 = gamma1*d1+(zetamax1-zetamin1)/2;
Ed2 = gamma2*d2+(zetamax2-zetamin2)/2;

(*Ed)(0,0) = Ed1;
(*Ed)(1,0) = Ed2;




//Errechnen des Wertes der Peaks, (c1,c2), der Verteilungsfunktion für Noise
	z1=b1-a1;				
	if (z1==0){				
		F_c1=0;				
		}
	else {
		F_c1=(c1-a1)/(b1-a1);		
	}			
	z2=b2-a2;
	if (z2==0){
		F_c2=0;	
		}
	else {
		F_c2=(c2-a2)/(b2-a2);	
	}

	(*E_xi)(0,0) = (b1+a1+c1)/3;
	(*E_xi)(1,0) = (b2+a2+c2)/3;
	
	(*V_xi)(0,0) = (a1*a1 + b1*b1 + c1*c1 - a1*b1 + a1*c1 + b1*c1)/18;
	(*V_xi)(0,1) = 0;
	(*V_xi)(1,0) = 0;
	(*V_xi)(1,1) = (a2*a2 + b2*b2 + c2*c2 - a2*b2 + a2*c2 + b2*c2)/18;

	//Inverse Transformationsmatrix
	
	(*T_inv)(0,0) 	 = 	1;
	(*T_inv)(0,1) = delta;
	(*T_inv)(1,0) = delta;
	(*T_inv)(1,1) = 1;

	(*T) = T_inv->inverse();

	// Transformierter Stoerprozess 

	(*E_epsilon) = (*T)*(*E_xi);
	

	//Berechnung der VCV-Matrix der transformierten Stoerung
 
	(*V_epsilon) = (*T) * (*V_xi) * (*T); 
	//Achtung: Geht da T_inv symmetrisch!!!	

	(*V_epsilon_inv) = V_epsilon->inverse();

	//Berechnung der subjektiven VCV Matrizen: gleich unbedingter Varianz der Dividenden

	var1 = ((zetamax1-zetamin1)*(zetamax1-zetamin1))/12;
	var2 = ((zetamax2-zetamin2)*(zetamax2-zetamin2))/12;
	(*V_F)(0,0)=var1;
	(*V_F)(1,1)=var2;
	(*V_F)(1,0)=0;
	(*V_F)(0,1)=0;

	(*V_C) = (*V_F);	//Spezialfall: Alle Investoren haben identische VCV-Matrizen
	(*V_N) = (*V_F);	//Spezialfall: Alle Investoren haben identische VCV-Matrize
	(*V_C_inv) = V_C->inverse();
	(*V_F_inv) = V_F->inverse();
	(*V_N_inv) = V_N->inverse();


	//Startwerte für Vorhersagen und cumdividend-Preise, Achtung: Vorhersagen jetzt auf ex-dividend Preise!
	(*q) = (*q0);
	(*p) = (*q0)-(*d);
	(*q_F) = (*p);
	(*q_C) = (*p);
	(*q_N) = (*q_F) + (*E_xi);
	

	//Initialisierung für Moving Average
	for(i=0;i<L;i++)
		(*q_MA[i]) = (*p);	

/*	
cout << "Parameterset"	<< endl;
cout <<	"eta1_F = " << eta1_F << endl;
cout <<	"eta2_F = " << eta2_F << endl;
cout << "eta3_F = " << eta3_F << endl;

cout <<	"eta1_C = " << eta1_C << endl;
cout <<	"eta2_C = " << eta2_C << endl;
cout << "eta3_C = " << eta3_C << endl;

cout <<	"eta1_N = " << eta1_N << endl;
cout <<	"eta2_N = " << eta2_N << endl;
cout << "eta3_N = " << eta3_N << endl;

cout <<	"alpha_1 = " << alpha_1 << endl;
cout <<	"alpha_2 = " << alpha_2 << endl;
cout << "alpha_3 = " << alpha_3 << endl;

cout <<	"beta_1 = " << beta_1 << endl;
cout <<	"beta_2 = " << beta_2 << endl;
cout << "beta_3 = " << beta_3 << endl;

cout <<	"e_1 = " << e_1 << endl;
cout <<	"e_2 = " << e_2 << endl;
cout <<	"e_3 = " << e_3 << endl;

cout <<	"sr_F = " << sr_F << endl;
cout <<	"sr_C = " << sr_C << endl;
cout <<	"sr_N = " << sr_N << endl;

cout <<	"a1 = " << a1 << endl;
cout <<	"b1 = " << b1 << endl;
cout <<	"c1 = " << c1 << endl;
cout <<	"a2 = " << a2 << endl;
cout <<	"b2 = " << b2 << endl;
cout <<	"c2 = " << c2 << endl;
cout <<	"r = " << r << endl;

*/

//cout << "c1 = " << c1 <<endl;
//cout << "c2 = " << c2 <<endl;
//cout << "b1 = " << beta_1 <<endl;
//cout << "b2 = " << beta_2 << endl;
//cout << "b3 = " << beta_3 << endl;

	
	delta_sr=sr_F-sr_C;
	delta_mu=mu_F-mu_C;
	delta_sigma=sigma_F-sigma_C;
	
	//initialising boundaries for etaC using rho[0] for delta!
	//adding or subtracting epsilon since boundaries result from inequalities!!!
	etaC_us = (R - 1) / (2*rho[0] - 1) + 0.01;
	real tmp_bound1 = 1.0 / ( 1.0 + rho[0] - rho[0]*rho[0] );
	real tmp_bound2 = (2 + rho[0]*(1+R)) / 2.0 * tmp_bound1;
	etaC_os = tmp_bound2 - sqrt(tmp_bound2*tmp_bound2-tmp_bound1) - 0.002;
	
	//cout << "\netaC_us : " << etaC_us;
	//cout << "\netaC_os : " << etaC_os << endl;
	//checking values for lower and upper bound
	if ( etaC_us>=etaC_os ) fatalError("finanzmart_JanE::initialize","etaC_us >= etaC_os" );

//Achtung: Jetzt in initialize(), vorher in loadParamset()
if(zvar1) delete zvar1;
	zvar1	=	new rand_var("ranf",1, zetamin1, zetamax1);
if(zvar2) delete zvar2;
	zvar2	=	new rand_var("ranf",1, zetamin2, zetamax2);
if(zvar3) delete zvar3;
	zvar3	=	new rand_var("ranf",1, 0, 1);
if(zvar4) delete zvar4;
	zvar4	=	new rand_var("ranf",1, 0, 1);
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		finanzmarkt_JanE
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void finanzmarkt_JanE::iteration(const long& t)
{ 	

	real a_F,a_C, a_N;					//gruppenspezifische Risikotoleranz
	real temp1,temp2;
	int i;	

	//cout.precision(16);
	//Abspeichern von Werten aus der Vorperiode 
	//(für die Berechnung der Portefeuillerenditen)
	
	(*p_old) = (*p);
	(*x_Fold) = (*x_F);
	(*x_Cold) = (*x_C);
	(*x_Nold) = (*x_N);

	//Zufallsdividende fuer die laufende Periode 
	//ACHTUNG: Position der Berechnung ist abhängig von Datierung 
	//der bedingten Erwartungswerte
	
	//Modifikation!!!!

///////////////////////////////////////////////////////
	(*Ed)(0,0) = gamma1*d1+(zetamax1-zetamin1)/2;
	(*Ed)(1,0) = gamma2*d2+(zetamax2-zetamin2)/2;

	Ed1=(*Ed)(0,0);
	Ed2=(*Ed)(1,0);

///////////////////////////////////////////////////////


	//Dividenden folgen AR(1)-Prozessen
	d1=gamma1 * d1 + zvar1->dice();	
	d2=gamma2 * d2 + zvar2->dice();	
	(*d)(0,0) = d1;
	(*d)(1,0) = d2;

	//Berechnung der gruppenspezifische Risikotoleranzen
	// paper equation (2)

	a_F = risk_tolerance(eta1_F,eta2_F, eta3_F) ;
	a_C = risk_tolerance(eta1_C,eta2_C, eta3_C) ;
	a_N = risk_tolerance(eta1_N,eta2_N, eta3_N) ;

	//Berechnung der subjektiven Kovarianz-Matrizen
	//Spezialfall: V_C = V_F = const

	//A=(a_F*(V_F^-1) + a_C*(V_C^-1) + a_N*(V_N^-1))^-1
	// paper equation (4)
	(*temp1_2_2) = V_C_inv->multiplyScalar(a_C); 
	(*temp2_2_2) = V_F_inv->multiplyScalar(a_F); 
	(*temp3_2_2) = V_N_inv->multiplyScalar(a_N); 
	(*temp4_2_2) = (*temp1_2_2) + (*temp2_2_2) + (*temp3_2_2);
	(*A) = temp4_2_2->inverse();

	//function: adjusted_VCV();
	// paper equation (6)
	//A_F=a_F*(V_F*A^-1)^-1
	(*temp1_2_2) = (*V_F) * (*temp4_2_2);  //temp4_2_2 = A^-1
	(*temp2_2_2) = temp1_2_2->inverse();
	(*A_F) = temp2_2_2->multiplyScalar(a_F);

	//A_C=a_C*(V_C*A^-1)^-1
	(*temp1_2_2) = (*V_C) * (*temp4_2_2);
	(*temp2_2_2) = temp1_2_2->inverse();
	(*A_C) = temp2_2_2->multiplyScalar(a_C);

	//New: Noisetraders werden wie Investorgruppe behandelt

 	//A_N=a_N*(V_N*A^-1)^-1
	(*temp1_2_2) = (*V_N) * (*temp4_2_2);
	(*temp2_2_2) = temp1_2_2->inverse();
	(*A_N) = temp2_2_2->multiplyScalar(a_N);

	//New:
/*
	cout << "A=" <<(*A);
	cout << "A_F=" <<(*A_F);
	cout << "A_C=" <<(*A_C);
	cout << "A_N=" <<(*A_N);
*/	
 	//A2_F = A_F + A_N
	(*A2_F) =  (*A_F) + (*A_N);


	// Generierung der Dreiecksverteilung für den Noise-Prozess
	if(F_c1==0){
		xi1=0;		
		}
	else  {
		xi1=zvar3->dice();
		if(xi1<=F_c1){
		 	xi1=xi1*(b1-a1)*(c1-a1);
			xi1=a1+sqrt(xi1);
			}
		else{
			xi1=(1-xi1)*(b1-a1)*(b1-c1);
			xi1=b1-sqrt(xi1);	
			}	
	}

	if(F_c2==0){
		xi2=0;		
		}	
	else  {
		xi2=zvar4->dice();
		if(xi2<=F_c2){
			xi2=xi2*(b2-a2)*(c2-a2);
			xi2=a2+sqrt(xi2);
			}
		else{
			xi2=(1-xi2)*(b2-a2)*(b2-c2);
			xi2=b2-sqrt(xi2);	
		}	
	}
	(*xi)(0,0) = xi1;
	(*xi)(1,0) = xi2;

//Transformierte Stoerung
//	(*temp1_2_1) = (*xi) - (*E_xi);
//	(*temp2_2_1) = (*T) * (*temp1_2_1);
//	(*epsilon) = (*temp2_2_1) + (*E_epsilon);

	(*epsilon) = (*T) * (*xi);
	epsilon1 = (*epsilon)(0,0);
	epsilon2 = (*epsilon)(1,0);
 


	//Vorhersage der Chartisten, q_C (durch Moving Average)

	temp1_2_1->set_to_zero();	
	for(i=0;i<L;i++)
		(*temp1_2_1) = (*temp1_2_1) + (*Rho[i]) * (*q_MA[i]);	
	(*q_C) = (*temp1_2_1);
//	cout << "q_C=" << (*q_C);

	//Vorhersage der Fundamentalisten, q_F (perfect mean-predictor)
	// geändert auf Version des Papers vom 14.08.2003

	(*q_Fold) = (*q_F);

	// q_F = A2_F^-1*[R*(q_Fold-Ed)- A_N*epsilon - A_C*q_C + A*x_all )] 
	// paper equation (15)
	(*temp1_2_1) = (*A)*(*x_all) - (*A_N)*(*epsilon) - (*A_C)*(*q_C);
	(*temp2_2_1) = (*q_Fold) - (*Ed);
	(*temp3_2_1) = temp2_2_1->multiplyScalar(R);
	(*temp2_2_1) = (*temp3_2_1) + (*temp1_2_1);
	(*temp1_2_2) = A2_F->inverse();
	(*q_F) = (*temp1_2_2) * (*temp2_2_1);
//	cout << "q_F=" << (*q_F);

	//Vorhersage der Noisetraders, q_N (Fehler auf q_F)
	// q_N = q_F + epsilon
	// paper equation (30)
 	(*q_N) = (*q_F) + (*epsilon);
//	cout << "q_N=" << (*q_N);

	//Preisprozess:
	// p=1/R * [A_C*q_C + A_F*q_F + A_N*q_N - A*x_all] 
	// paper equation (5)
	(*temp1_2_1) = (*A_F) * (*q_F) + (*A_C)*(*q_C) + (*A_N)*(*q_N) - (*A)*(*x_all);
	(*p) = temp1_2_1->multiplyScalar(1/R);
//	cout << "p=" << (*p);

	//cumdividend-Preis: q=p+d
	(*q) = (*p) + (*d);	
//	cout << "q=" << (*q);

	//Berechnung der nach dem Handel realisierten Aktienportefeuilles:
	
	//Aktienportefeuille der Fundamentalisten: x_F=a_F*(V_F)^-1*(q_F-R*p)
	// paper equation (1)
	// R*p will be calculated only ones and used for all
	(*temp1_2_1) = p->multiplyScalar(R);
	
	(*temp2_2_1) = (*V_F_inv) * ((*q_F) - (*temp1_2_1));
	(*x_F) = temp2_2_1->multiplyScalar(a_F);
//	cout << "x_F=" << (*x_F);
	
	//Aktienportefeuille der Chartisten: x_C=a_C*(V_C)^-1(q_C-R*p)
	(*temp2_2_1) = (*V_C_inv) * ((*q_C) - (*temp1_2_1));
	(*x_C) = temp2_2_1->multiplyScalar(a_C);
//	cout << "x_C=" << (*x_C);
	
	//Aktienportefeuille der Noise-Traders: 
	(*temp2_2_1) = (*V_N_inv) * ((*q_N) - (*temp1_2_1));
	(*x_N) = temp2_2_1->multiplyScalar(a_N);
//	cout << "x_N=" << (*x_N);

	//Berechnung der in Periode t realisierten Renditen der Investoren
	//Realisiert Renditen der Fundamentalisten	

	w_F = eta1_F*e_1 + eta2_F*e_2 + eta3_F*e_3;
	// rend_i=r+1/w_i*[q-R*p_old]T*x_i
	// paper equation (8)
	// temp1_2_1 and temp2_2_1 will be calculated only once
	(*temp1_2_1) = p_old->multiplyScalar(R);
	(*temp2_2_1) = (*q) - (*temp1_2_1);
	
//	cout << "Preise " << (*temp2_2_1) << endl;
	
	(*temp1_1_1) = temp2_2_1->multiplyATB(*x_Fold);
	wealth_F = (*temp1_1_1)(0,0);
	if(w_F == 0)
	rend_F=0;
	else 
	rend_F=r+(wealth_F/w_F);

	//Realisierte Renditen der Chartisten 
	w_C = eta1_C*e_1 + eta2_C*e_2 + eta3_C*e_3;
	
	(*temp1_1_1) = temp2_2_1->multiplyATB(*x_Cold);
	wealth_C = (*temp1_1_1)(0,0);
//	cout << "Stücke " << (*x_Cold) << endl << "Wert " << wealth_C << endl;
	if(w_C == 0)
	rend_C=0;
	else 
	rend_C=r+(wealth_C/w_C);

	//Realisierte Renditen der Noise-Traders 
	w_N = eta1_N*e_1 + eta2_N*e_2 + eta3_N*e_3;

	(*temp1_1_1) = temp2_2_1->multiplyATB(*x_Nold);
	wealth_N = (*temp1_1_1)(0,0);
	if(w_N == 0)
	rend_N=0;
	else 
	rend_N=r+(wealth_N/w_N);

//	cout << "w_C " << w_C << endl;
	//Rekursive Momente der bisher realisierten Renditen der Investoren  	
	//mean and standard deviation of return of fundamentalists
	//paper equation (9)
/*
	mu_F = switch_F*(1/real(t+1)) * (rend_F + real(t)*mu_F);
	temp1 = ((rend_F - mu_F)*(rend_F - mu_F))/real(t+1);
	temp2 = (real(t)/real(t+1)) * sigma_F*sigma_F;
	sigma_F = switch_F*sqrt(temp1 + temp2);

	//mean and standard deviation of return of chartists
	mu_C = switch_C*(1/real(t+1)) * (rend_C + real(t)*mu_C);
	temp1 = ((rend_C - mu_C)*(rend_C - mu_C))/real(t+1);
	temp2 = (real(t)/real(t+1)) * sigma_C*sigma_C;
	sigma_C = switch_C*sqrt(temp1 + temp2);


	//mean and standard deviation of return of noise traders
	mu_N = switch_N*(1/real(t+1)) * (rend_N + real(t)*mu_N);
	temp1 = ((rend_N - mu_N)*(rend_N - mu_N))/real(t+1);
	temp2 = (real(t)/real(t+1)) * sigma_N*sigma_N;
	sigma_N = switch_N*sqrt(temp1 + temp2);
*/
	//alternative computation for mean and standard deviation
	
	mu_F = switch_F*(1/real(t+1)) * (rend_F - r + real(t)*mu_F);
	temp1 = (rend_F-r) * (rend_F-r) / real(t+1);
	var_F = (real(t)/real(t+1)) * var_F + temp1;
//	sigma_F = switch_F * sqrt(var_F - mu_F*mu_F);

	mu_N = switch_N*(1/real(t+1)) * (rend_N - r + real(t)*mu_N);
	temp1 = (rend_N-r) * (rend_N-r) / real(t+1);
	var_N = (real(t)/real(t+1)) * var_N + temp1;
//	sigma_N = switch_N * sqrt(var_N - mu_N*mu_N);
	
	mu_C = switch_C*(1/real(t+1)) * (rend_C - r + real(t)*mu_C);
	temp1 = (rend_C-r) * (rend_C-r) / real(t+1);
	var_C = (real(t)/real(t+1)) * var_C + temp1;
//	sigma_C = switch_C * sqrt(var_C - mu_C*mu_C);

/*
	//Sharpe ratio Fundamentalisten, Chartisten, Noisetraders
	sr_F = sharpe_ratio(mu_F,sigma_F, switch_F);
	sr_C = sharpe_ratio(mu_C,sigma_C, switch_C);
	sr_N = sharpe_ratio(mu_N,sigma_N, switch_N);
*/

	//alternative sharpe ratios
	sr_F = sharpe_ratio2(mu_F,sigma_F, switch_F);
	sr_C = sharpe_ratio2(mu_C,sigma_C, switch_C);
	sr_N = sharpe_ratio2(mu_N,sigma_N, switch_N);

	
	delta_sr = sr_F-sr_C;
	delta_mu = mu_F-mu_C;
	delta_sigma = sigma_F - sigma_C;

	//shifting for the next period
	for (i=1;i<L;i++)
		(*q_MA[L-i]) = (*q_MA[L-1-i]);
	(*q_MA[0]) = (*p);

	//Berechnung der neuen Gruppenanteile mithilfe des Discrete Choice Models 
	//paper equation (10)	

	//Aufteilung Haushalt 1	
/*	eta1_F	=	switch_F/(switch_F + switch_N*exp(beta_1*(sr_N-sr_F)) + switch_C*exp(beta_1*(sr_C-sr_F)));
	eta1_C	=	switch_C/(switch_C + switch_N*exp(beta_1*(sr_N-sr_C)) + switch_F*exp(beta_1*(sr_F-sr_C)));
	eta1_N	=	switch_N/(switch_N + switch_F*exp(beta_1*(sr_F-sr_N)) + switch_C*exp(beta_1*(sr_C-sr_N)));


	//Aufteilung Haushalt 2	
	eta2_F	=	switch_F/(switch_F + switch_N*exp(beta_2*(sr_N-sr_F)) + switch_C*exp(beta_2*(sr_C-sr_F)));
	eta2_C	=	switch_C/(switch_C + switch_N*exp(beta_2*(sr_N-sr_C)) + switch_F*exp(beta_2*(sr_F-sr_C)));
	eta2_N	=	switch_N/(switch_N + switch_F*exp(beta_2*(sr_F-sr_N)) + switch_C*exp(beta_2*(sr_C-sr_N)));


	//Aufteilung Haushalt 3	
	eta3_F	=	switch_F/(switch_F + switch_N*exp(beta_3*(sr_N-sr_F)) + switch_C*exp(beta_3*(sr_C-sr_F)));
	eta3_C	=	switch_C/(switch_C + switch_N*exp(beta_3*(sr_N-sr_C)) + switch_F*exp(beta_3*(sr_F-sr_C)));
	eta3_N	=	switch_N/(switch_N + switch_F*exp(beta_3*(sr_F-sr_N)) + switch_C*exp(beta_3*(sr_C-sr_N)));
*/
//alternativer Haushaltsansatz, nur für zwei Gruppen Chartisten und Fundamentalisten!!!
/*	eta1_C	=	(etaC_os - etaC_us) / (exp(beta_1*delta_sr)+1) + etaC_us;
	eta2_C	=	(etaC_os - etaC_us) / (exp(beta_2*delta_sr)+1) + etaC_us;
	eta3_C	=	(etaC_os - etaC_us) / (exp(beta_3*delta_sr)+1) + etaC_us;
	
	eta1_F	= 1.0 - eta1_C;
	eta2_F	= 1.0 - eta2_C;
	eta3_F	= 1.0 - eta3_C;
	// !!!
*/
//alternativer Haushaltsansatz, nur für zwei Gruppen Chartisten und Fundamentalisten 
//und Performancemaß delta_mu
/*	eta1_C	=	(etaC_os - etaC_us) / (exp(beta_1*delta_mu)+1) + etaC_us;
	eta2_C	=	(etaC_os - etaC_us) / (exp(beta_2*delta_mu)+1) + etaC_us;
	eta3_C	=	(etaC_os - etaC_us) / (exp(beta_3*delta_mu)+1) + etaC_us;
	
	eta1_F	= 1.0 - eta1_C;
	eta2_F	= 1.0 - eta2_C;
	eta3_F	= 1.0 - eta3_C;
*/
//	F(beta,eta_C)
//	dummy = (etaC_os-etaC_us)/(exp(beta_1*delta_mu)+1)+etaC_us;
//	F^(-1)(beta,eta_C)
//	dummy	=	1.0/beta_1*log((etaC_os-eta1_C)/(eta1_C-etaC_us));
//	Projektion F'(beta,eta_C)*(f_T(eta_C)-F^(-1)(beta,eta_C))

//	dummy = -(etaC_os-etaC_us)/pow((exp(beta_1*delta_mu)+1),2.0)*beta_1*exp(beta_1*delta_mu)
//		*(delta_mu-1.0/beta_1*log((etaC_os-eta1_C)/(eta1_C-etaC_us)));

//	Projektion F'(beta,F^(-1)(beta,eta_C))*(f_T(eta_C)-F^(-1)(beta,eta_C))

	dummy = -(etaC_os-etaC_us)/pow((exp(beta_1*1.0/beta_1*log((etaC_os-eta1_C)/(eta1_C-etaC_us)))+1),2.0)
		*beta_1*exp(beta_1*1.0/beta_1*log((etaC_os-eta1_C)/(eta1_C-etaC_us)))
		*(delta_mu-1.0/beta_1*log((etaC_os-eta1_C)/(eta1_C-etaC_us)));

	//if ( t==10000 ) cout << "t=10000: " << sr_C << "\t" << sr_F << endl;
	//Ausgabe zur Kontrolle:
/*
	cout << "r =" << r << endl;
	cout << "rend_F =" << rend_F<< endl;
	cout << "rend_C =" << rend_C<< endl;
	cout << "rend_N =" << rend_N<< endl;	
	cout << "sr_F =" << sr_F << endl;
	cout << "sr_C =" << sr_C << endl;
	cout << "sr_N =" << sr_N << endl;
	cout << "mu_F =" << mu_F << endl;
	cout << "mu_C =" << mu_C << endl;
	cout << "mu_N =" << mu_N << endl;
	cout << "sigma_F =" << sigma_F << endl;
	cout << "sigma_C =" << sigma_C << endl;
	cout << "sigma_N =" << sigma_N << endl;
	cout << "eta1_F =" << eta1_F << endl;
	cout << "eta2_F =" << eta2_F << endl;
	cout << "eta3_F =" << eta3_F << endl;

	cout << "eta1_C =" << eta1_C << endl;
	cout << "eta2_C =" << eta2_C << endl;
	cout << "eta3_C =" << eta3_C << endl;

	cout << "eta1_N =" << eta1_N << endl;
	cout << "eta2_N =" << eta2_N << endl;
	cout << "eta3_N =" << eta3_N << endl;

	cout << "epsilon1 =" << epsilon1 << endl;
	cout << "epsilon2 =" << epsilon2 << endl;
*/

}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		finanzmarkt_JanE
// Member function:	sharpe_ratio
// Purpose:			calculate sharpe ratios 
//
// Author:		    mhillebrand
// Last modified:	Die Mai 13 11:26:57 CEST 2003
// By:				mhillebrand
//
///////////////////////////////////////////////////////////////////////////////

real finanzmarkt_JanE::sharpe_ratio(real &mu, real &sigma, real &sw)
{
	real sr;
//	sigma = 1;
	if(sigma==0)
	sr =0;
	else
	sr = sw*(mu - r)/sigma;
	return(sr); 
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		finanzmarkt_JanE
// Member function:	sharpe_ratio2
// Purpose:			calculate sharpe ratios 
//
// Author:		    astarke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

real finanzmarkt_JanE::sharpe_ratio2(real &mu, real &sigma, real &sw)
{
	real sr;
//	sigma = 1;
	if(sigma==0)
	sr =0;
	else
	sr = sw * mu / sigma;
	return(sr); 
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		finanzmarkt_JanE
// Member function:	risk_tolerance
// Purpose:			calculate risk-tolerances 
//
// Author:		    mhillebrand
// Last modified:	Die Mai 13 11:26:57 CEST 2003
// By:				mhillebrand
//
///////////////////////////////////////////////////////////////////////////////

real finanzmarkt_JanE::risk_tolerance(real &eta_1, real &eta_2, real &eta_3)
{
	real a;
	a =  (eta_1/alpha_1) +(eta_2/alpha_2) + (eta_3/alpha_3) ; 
	return(a); 
}



///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		finanzmarkt_JanE
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

real* finanzmarkt_JanE::setLabels(char* label)
{	
	if( !strcmp(label,"xBundle") ) return(&xBundle);
	if( !strcmp(label,"yBundle") ) return(&yBundle);
	
	if( !strcmp(label,"beta") ) return(&beta);
		
	if( !strcmp(label,"eta1_F") ) return(&eta1_F);
	if( !strcmp(label,"eta2_F") ) return(&eta2_F);
	if( !strcmp(label,"eta3_F") ) return(&eta3_F);
	if( !strcmp(label,"eta1_C") ) return(&eta1_C);
	if( !strcmp(label,"eta2_C") ) return(&eta2_C);
	if( !strcmp(label,"eta3_C") ) return(&eta3_C);
	if( !strcmp(label,"eta1_F_0") ) return(&eta1_F_0);
	if( !strcmp(label,"eta2_F_0") ) return(&eta2_F_0);
	if( !strcmp(label,"eta3_F_0") ) return(&eta3_F_0);
	if( !strcmp(label,"eta1_C_0") ) return(&eta1_C_0);
	if( !strcmp(label,"eta2_C_0") ) return(&eta2_C_0);
	if( !strcmp(label,"eta3_C_0") ) return(&eta3_C_0);
	if( !strcmp(label,"eta1_N") ) return(&eta1_N);
	if( !strcmp(label,"eta2_N") ) return(&eta2_N);
	if( !strcmp(label,"eta3_N") ) return(&eta3_N);

	if( !strcmp(label,"etaC_os") ) return(&etaC_os);
	if( !strcmp(label,"etaC_us") ) return(&etaC_us);		
	
	if( !strcmp(label,"alpha_1") ) return(&alpha_1);
	if( !strcmp(label,"alpha_2") ) return(&alpha_2);
	if( !strcmp(label,"alpha_3") ) return(&alpha_3);
	
	if( !strcmp(label,"beta_1") ) return(&beta_1);
	if( !strcmp(label,"beta_2") ) return(&beta_2);
	if( !strcmp(label,"beta_3") ) return(&beta_3);

	if( !strcmp(label,"gamma_1") ) return(&gamma1);
	if( !strcmp(label,"gamma_2") ) return(&gamma2);
		
	if( !strcmp(label,"e_1") ) return(&e_1);
	if( !strcmp(label,"e_2") ) return(&e_2);
	if( !strcmp(label,"e_3") ) return(&e_3);
	
	if( !strcmp(label,"rho1") ) return(&rho[0]);
	if( !strcmp(label,"rho2") ) return(&rho[1]);

	if( !strcmp(label,"r") ) return(&r);
	if( !strcmp(label,"xi1") ) return(&xi1);
	if( !strcmp(label,"xi2") ) return(&xi2);
	if( !strcmp(label,"epsilon1") ) return(&epsilon1);
	if( !strcmp(label,"epsilon2") ) return(&epsilon2);
	if( !strcmp(label,"delta") ) return(&delta);
	if( !strcmp(label,"d1") ) return(&d1);
	if( !strcmp(label,"d2") ) return(&d2);
	if( !strcmp(label,"Ed1") ) return(&Ed1);
	if( !strcmp(label,"Ed2") ) return(&Ed2);
	if( !strcmp(label,"p1") ) return(&(*p)(0,0));
	if( !strcmp(label,"p2") ) return(&(*p)(1,0));
	if( !strcmp(label,"q1") ) return(&(*q)(0,0));
	if( !strcmp(label,"q2") ) return(&(*q)(1,0));
	if( !strcmp(label,"q1_0") ) return(&q1_0);
	if( !strcmp(label,"q2_0") ) return(&q2_0);
	if( !strcmp(label,"q_Fold1") ) return(&(*q_Fold)(0,0));
	if( !strcmp(label,"q_Fold2") ) return(&(*q_Fold)(1,0));
	
	
	if( !strcmp(label,"x_N1") ) return(&(*x_N)(0,0));
	if( !strcmp(label,"x_N2") ) return(&(*x_N)(1,0));
	if( !strcmp(label,"x_F1") ) return(&(*x_F)(0,0));
	if( !strcmp(label,"x_F2") ) return(&(*x_F)(1,0));
	if( !strcmp(label,"x_C1") ) return(&(*x_C)(0,0));
	if( !strcmp(label,"x_C2") ) return(&(*x_C)(1,0));

	if( !strcmp(label,"rend_N") ) return(&rend_N);
	if( !strcmp(label,"rend_F") ) return(&rend_F);
	if( !strcmp(label,"rend_C") ) return(&rend_C);
	
	if( !strcmp(label,"sr_N") ) return(&sr_N);
	if( !strcmp(label,"sr_F") ) return(&sr_F);
	if( !strcmp(label,"sr_C") ) return(&sr_C);
	
	if( !strcmp(label,"delta_sr") ) return(&delta_sr);
	if( !strcmp(label,"delta_mu") ) return(&delta_mu);	
	if( !strcmp(label,"delta_sigma") ) return(&delta_sigma);
	
	if( !strcmp(label,"mu_N") ) return(&mu_N);
	if( !strcmp(label,"mu_F") ) return(&mu_F);
	if( !strcmp(label,"mu_C") ) return(&mu_C);

	if( !strcmp(label,"var_N") ) return(&var_N);
	if( !strcmp(label,"var_F") ) return(&var_F);
	if( !strcmp(label,"var_C") ) return(&var_C);
	
	if( !strcmp(label,"sigma_N") ) return(&sigma_N);
	if( !strcmp(label,"sigma_F") ) return(&sigma_F);
	if( !strcmp(label,"sigma_C") ) return(&sigma_C);
	if( !strcmp(label,"a1") ) return(&a1);
	if( !strcmp(label,"b1") ) return(&b1);
	if( !strcmp(label,"c1") ) return(&c1);
	if( !strcmp(label,"a2") ) return(&a2);
	if( !strcmp(label,"b2") ) return(&b2);
	if( !strcmp(label,"c2") ) return(&c2);
	if( !strcmp(label,"b_2") ) return(&b_2);

	if( !strcmp(label,"dummy") ) return(&dummy);
	
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	finanzmarkt_JanE	
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void finanzmarkt_JanE::saveParamset(ofstream& outFile)
{	

	outFile << eta1_F << eta2_F << eta3_F;
	outFile << eta1_C << eta2_C << eta3_C;
	
    outFile <<  alpha_1 << alpha_2 << alpha_3;
	outFile <<  beta_1 << beta_2 << beta_3;
	outFile << 	e_1 << e_2 << e_3;
	outFile << 	r;
	outFile << 	x_all1 << x_all2;
	outFile << 	zeta1_F << zeta2_F;
	outFile << 	zeta1_C << zeta2_C;
	outFile << 	zeta1_N << zeta2_N;
	outFile <<  switch_F << switch_C << switch_N;
	
	for(int i=0;i<L;i++)
		outFile << (*Rho[i])(0,0);	
	for(i=0;i<2;i++)
		outFile << (*q0)(i,0);

	outFile << 	a1 << b1 << c1;
	outFile << 	a2 << b2 << c2;
	outFile << delta;
   	outFile << 	b_1 << b_2;
	outFile << 	zetamin1 << zetamax1 << gamma1;
   	outFile << 	zetamin2 << zetamax2 << gamma2;
	outFile << 	length;

}

///////////////////////////////////////////////////////////////////////////////
//
// Member function:	saveParamsetWithNames
// Purpose:		write parameterset into a file
//
// Author:		Andreas Starke
// Last modified:			
//
///////////////////////////////////////////////////////////////////////////////

void finanzmarkt_JanE::saveParamsetWithNames(ofstream& outputFile)
{
	outputFile << "Finanzmarkt_JanE:\n\t";
	outputFile << "eta1_F="<<eta1_F <<"\teta2_F="<< eta2_F 
	<<"\teta3_F="<< eta3_F;
	outputFile << "\n\teta1_C="<<eta1_C<<"\teta2_C="<< eta2_C 
	<< "\teta3_C="<< eta3_C;
	
	outputFile <<"\n\talpha_1="<<alpha_1<<"\talpha_2="<<alpha_2 
	<<"\talpha_3="<< alpha_3;
	outputFile <<"\n\tbeta_1="<<beta_1<<"\tbeta_2="<<beta_2 
	<<"\tbeta_3="<< beta_3;
	outputFile <<"\n\te_1="<<e_1<<"\te_2="<<e_2 
	<<"\te_3="<< e_3;
	outputFile <<"\n\tr="<<r;
	outputFile <<"\n\tx_all1="<<x_all1<<"\tx_all2="<< x_all2;
	outputFile <<"\n\tzeta1_F="<<zeta1_F<<"\tzeta2_F=" << zeta2_F;
	outputFile <<"\n\tzeta1_C="<<zeta1_C<<"\tzeta2_C=" << zeta2_C;
	outputFile <<"\n\tzeta1_N="<<zeta1_N<<"\tzeta2_N=" << zeta2_N;	
	outputFile <<"\n\tswitch_F="<<switch_F<<"\tswitch_C="<<switch_C 
	<<"\tswitch_N="<< switch_N;
	
	for(int i=0;i<L;i++)
		outputFile << "\n\tRho["<<i+1<<"](0,0)="<<(*Rho[i])(0,0);	
	for(i=0;i<2;i++)
		outputFile << "\n\tq"<<i+1<<"_0="<<(*q0)(i,0);

	outputFile <<"\n\ta1="<<a1<<"\tb1="<<b1<<"\tc1="<<c1;
	outputFile <<"\n\ta2="<<a2<<"\tb2="<<b2<<"\tc2="<<c2;
	outputFile <<"\n\tdelta="<<delta;
   	outputFile <<"\n\tb_1="<<b_1<<"\tb_2="<< b_2;
	outputFile <<"\n\tzetamin1="<<zetamin1<<"\tzetamax1="<<zetamax1
	<<"\tgamma1="<< gamma1;
	outputFile <<"\n\tzetamin2="<<zetamin2<<"\tzetamax2="<<zetamax2
	<<"\tgamma2="<< gamma2;
	outputFile <<"\n\tlength="<<length<<"\n";
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	finanzmarkt_JanE	
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void finanzmarkt_JanE::printParamset()
{ 	
	
	cout << eta1_F << eta2_F << eta3_F << endl;
	cout << eta1_C << eta2_C << eta3_C << endl;
	cout <<  alpha_1 << alpha_2 << alpha_3 << endl;
	cout <<  beta_1 << beta_2 << beta_3 << endl;
	cout << e_1 << e_2 << e_3 << endl;
	cout << r << endl;
	cout << x_all1 << x_all2 << endl;
	cout << zeta1_F << zeta2_F;
	cout << zeta1_C << zeta2_C;
	cout << zeta1_N << zeta2_N;
	cout << switch_F << switch_C << switch_N;
	
	for(int i=0;i<L;i++)
		cout << (*Rho[i])(0,0) << endl;	
	for(i=0;i<2;i++)
		cout << (*q0)(i,0) << endl;
	
	cout << a1 << b1 << c1 << endl;
	cout << a2 << b2 << c2 << endl;
   	cout << delta << endl;
   	cout << b_1 << b_2 << endl;
	cout << zetamin1 << zetamax1 << gamma1 << endl;
   	cout << zetamin2 << zetamax2 << gamma2 << endl;
	cout << length << endl;

}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	finanzmarkt_JanE	
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void finanzmarkt_JanE::sendStateSpace(int &quantity,const real*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const real* [dimension];
    if( !(*stateSpace) )
	fatalError("finanzmarkt_JanE::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&p1;
	(*stateSpace)[1]=&p2;


};
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		
// Member functions:	sendModelVar, sendParameters, receiveParameters
// Purpose:		for compatibity reasons only
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////
real* finanzmarkt_JanE::sendModelVar(void)
{ 
  return &p1;
}

void finanzmarkt_JanE::sendParameters(int& amount ,real** parameters)
{ 
   cout << "Warning: function sendParameters() is in use" << endl;
   if( *parameters )
	delete *parameters;
    amount=47;
    *parameters= new real[amount];
    if( !parameters )
	fatalError("finanzmarkt_JanE::sendParameters","Can't create array for parameters");
    (*parameters[0])=eta1_F_0;
	(*parameters[1])=eta2_F_0;
	(*parameters[2])=eta3_F_0;
	(*parameters[3])=eta1_C_0;
	(*parameters[4])=eta2_C_0;
	(*parameters[5])=eta3_C_0;
	(*parameters[6])=alpha_1;
	(*parameters[7])=alpha_2;
	(*parameters[8])=alpha_3;
	(*parameters[9])=beta_1;
	(*parameters[10])=beta_2;
	(*parameters[11])=beta_3;
	(*parameters[12])=e_1;
	(*parameters[13])=e_2;
	(*parameters[14])=e_3;
	(*parameters[15])=r;
	(*parameters[16])=x_all1;
	(*parameters[17])=x_all2;
	(*parameters[18])=zeta1_F;
	(*parameters[19])=zeta2_F;
	(*parameters[20])=zeta1_C;
	(*parameters[21])=zeta2_C;
	(*parameters[22])=zeta1_N;
	(*parameters[23])=zeta2_N;
	(*parameters[24])=switch_F;
	(*parameters[25])=switch_C;
	(*parameters[26])=switch_N;
	(*parameters[27])=(*Rho[0])(0,0);	
	(*parameters[28])=(*Rho[1])(0,0);	
	(*parameters[29])=(*q0)(0,0);
	(*parameters[30])=(*q0)(1,0);
	(*parameters[31])=a1;
	(*parameters[32])=b1;
	(*parameters[33])=c1;
	(*parameters[34])=a2;
	(*parameters[35])=b2;
	(*parameters[36])=c2;
	(*parameters[37])=delta;
	(*parameters[38])=b_1;
	(*parameters[39])=b_2;
	(*parameters[40])=zetamin1;
	(*parameters[41])=zetamax1;
	(*parameters[42])=gamma1;
	(*parameters[43])=zetamin2;
	(*parameters[44])=zetamax2;
	(*parameters[45])=gamma2;
    (*parameters[46])=length;
}

void finanzmarkt_JanE::receiveParameters(const real* parameters)
{
  cout << "Warning: function receiveParameters() is in use" << endl;
   eta1_F_0 = parameters[0];
   eta2_F_0 = parameters[1];
   eta3_F_0 = parameters[2];
   eta1_C_0 = parameters[3];
   eta2_C_0 = parameters[4];
   eta3_C_0 = parameters[5];
	alpha_1 = parameters[6];
	alpha_2 = parameters[7];
	alpha_3 = parameters[8];
	beta_1	= parameters[9];
	beta_2	= parameters[10];
	beta_3	= parameters[11];
	e_1		= parameters[12];
	e_2		= parameters[13];
	e_3		= parameters[14];
	r		= parameters[15];	
	x_all1	= parameters[16];
	x_all2	= parameters[17];
	zeta1_F = parameters[18];
	zeta2_F = parameters[19];
	zeta1_C = parameters[20];
	zeta2_C = parameters[21];
	zeta1_N = parameters[22];
	zeta2_N = parameters[23];
	switch_F = parameters[24];
	switch_C = parameters[25];
	switch_N = parameters[26];
	(*Rho[0])(0,0) = parameters[27];	
	(*Rho[1])(0,0) = parameters[28];	
	(*q0)(0,0) = parameters[29];
	(*q0)(1,0) = parameters[30];
	a1	=	parameters[31];
	b1	=	parameters[32];
	c1	=	parameters[33];
	a2	=	parameters[34];
	b2	=	parameters[35];
	c2	=	parameters[36];
	delta	= parameters[37];
	b_1 = parameters[38];
	b_2 = parameters[39];
	zetamin1 = parameters[40];
	zetamax1 = parameters[41];
	gamma1 	 = parameters[42];
	zetamin2 = parameters[43];
	zetamax2 = parameters[44];
	gamma2 	 = parameters[45];
 	length=(long)(parameters[46]);
} 

