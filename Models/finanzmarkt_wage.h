///////////////////////////////////////////////////////////////////////////////
//
// Klassendefinition
//
// Name: finanzmarkt_2Dim_1.h
//       lineares Finanzmarktmodell mit Erwartungsr"uckkoppelung
//	 2 Typen von Anteilsscheinen sowie heterogenen Haendlergruppen	
// Code: Andreas Tonn
//
///////////////////////////////////////////////////////////////////////////////

#ifndef FINANZMARKT_WAGE_INCLUDED
#define FINANZMARKT_WAGE_INCLUDED

#include "baseModel.h"
// #include "../strnchr.h"
#include "../matrix.h"
#include <stdlib.h>
#include "rand_var.h"

class finanzmarkt_wage : public baseModel {

protected:
	
	
	static const real  epsilon=1e-16;

/**** production function ****/

    real ( * pf ) ( real k, real a, real b, real c, real d );			// a pointer to the production function
    real ( * pf_prime) ( real k, real a, real b, real c, real d ); 		// and its derivative
    real pf_type;														// the current type of the production function
    real a, b, c, d;		// original parameters
    real A_pf, B_pf, C_pf, D_pf;		// parameters as ][0,1][ normalized input
    void pf_init ( void );	// maps (A,B,C) -> (a,b,c) and sets pf, pf_prime

/**** new Parameter ****/
	
	real n_F,tau_F,delta,k_0,Fi;
	real wagerate_t;
	real Wagebill_t;
	real Savings_t;
	real R;

/**** old Parameter ****/

	real alpha, ef, eF;
	real mu, mu0;			//Gruppenanteilsvariable
	real Xi_F;				//Variable fuer aggrgierte Risikoaversion
	real Xi_NzuXi_F;		//Variable Xi_F/Xi_N
	real AbZins;			//Variable fuer 1/R
	real p01, pF01;			//Startwerte fuer Preise und Vorhersagen
	real p_0, p1, pF1, pR1;		//Preise, Preisvorhersagen
	real pN_old, pN_old2, Ed_old, Ed_old2;    //Fuer die Vorhersagefehlerberechung
	real err_div,err_div2, err_pN, err_pN2, err_qN, err_qN2;	//Vorhersagefehler der N--Traders (dividenden, ex--dividend, cum--dividend)
	real delta_p, delta_p2, delta_div, delta_div2, delta_q, delta_q2;
	real xAll, xAll2, xF1, xF2, xN1, xN2;						//aggregierte Aktien-/Portfolioanzahl
	real xF11, xF22, xN11, xN22;	        					//andere Skalierung fuer graphische Ausgabe
	real xf1, xf2, xn1, xn2;									//einzelne Portfolioanzahl
	real pxF, pxN, pxf, pxn;									//Variablen zur Schuldenberechnung
	real Haben_F, Haben_f;
	real zetamin, zetamax, gamma;							//Parameter fuer Dividendenprozeﬂ 1
	real div;														//Dividendenzufallsvariablen
	real Ed, Ed2, Ed11, Ed22, var, var2, cov12;				//Momente der Dividendenprozesse
	real delta_d1, delta_d2;									//d_{t+1}-\E_t(d_{t+1}) fuer alternative Renditenberechung
	real var1err, var2err, coerr;								//Fehler der Noise--Trader auf die Kovarianzmatrix
	real a1, b1, c1, F_c1;										//Parameterwerte der 1. Dreiecksverteilung,
	real a2, b2, c2, F_c2;										//Parameterwerte der 2. Dreiecksverteilung,	
	real rho1, rho2, Erho1, Erho2;	     					    //fuer die Fehleinschaetzung der Noise--Trader
	real p1_old, p2_old, d1_old, d2_old;						//Auswertungsvariable
	real xF_old, xF_old2, xN_old1, xN_old2;
	real return_F, return_N, return_f, return_n; 				//Realisierter Return (aggregiert und einzeln)
	real rend_F, rend_N, rend_f, rend_n;						//Renditen der Fundamentalisten/ Noise--Trader
	real rendf_neu, rendn_neu;									//alternativ berechnete Renditen
	real ErendF_p, ErendF_ges; 									//Erwartete Renditen
	real ErendN_p, ErendN_ges; 									//Erwartete Renditen
	real RPF, RPN, sdRF, sdRN; 									//Risikopr‰mien und deren Standardaweichungen
	real Eff_F, Eff_N, delta_Eff;								//Steigungen der Effizienzlinien
	real z1, z2, z3, z4, z5, z7, z8, muN;  						//Zischenrechnungsvariablen
	real xBundle;
	real mRF, mRN, sRF, sRN, vRF, vRN;      //Variablen fuer Renditenmittelwerte,
    real SIZF, SIZN, SIF, SIN, delta_SI;    //Standardabweichungen und Varianzen fuer SharpeIndex 	
	real wechselanteil;						//Anteil der Fundamentalisten der je Periode switcht
	real etafix, etaF;						//Anteile der ,,Unbelehrbaren'' Strategen
	real beta;                              //Gewichtungskonstante fuer Gruppenswitch
	real zwischen1, zwischen2, zwischen3;   //Zischenrechnungsvariablen fuer Gruppenswitch 
	
real point;
real A;		//2x2 Martix fuer Initialisierung
real B; 	  //2x2 Martix fuer Initialisierung
real C; 	  //2x1 Vektor fuer Initialisierung
real D; 	  //2x1 Vektor der realisierten Dividenden
real deltaD;  //2x1 Vektor fuer alternative Renditenberechnung
real ED;	   //2x1 Vektor der Dividendenerwartungswerte
real ED2;	  //2x1 Vektor der Dividendenerwartungswerte
real Id;	  //2x2 Identitaet
real P; 	  //2x1 Vektor der Wertpapierpreise
real P_old;   //2x1 Vektor der Wertpapierpreise der Vorperiode
real PF;	  //2x1 Vektor der Preisvorhersage der Fundamentalisten
real PN;	  //2x1 Vektor der Preisvorhersage der Noise--Trader
real RHO;	  //2x1 Vektor der Preisfehleinschaetzung der Noise--Trader
real RHO_old; //2x1 Vektor fuer alternative Renditenberechnung
real ERHO;    //2x1 Vektor der Preisfehleinschaetzung der Noise--Trader
real VF;	  //2x2 Kovarianzmatrix der Fundamentalisten
real VN;	  //2x2 Kovarianzmatrix der Noise--Trader
real X; 	  //2x1 Vektor aller Anteilsscheine (X=XF+XN)
real XF;	  //2x1 Vektor aller Anteilsscheine im Besitz der Fundamentalisten
real XN;	  //2x1 Vektor aller Anteilsscheine im Besitz der Noise--Trader
real XF_old;  //2x1 Vektor aller Anteilsscheine im Besitz der alten Fundamentalisten
real XN_old;  //2x1 Vektor aller Anteilsscheine im Besitz der alten Noise--Trader
real PXF;	  //1x1 Vektor zur Schuldenberechnung
real PXN;	  //1x1 Vektor zur Schuldenberechnung
real Return_F;//1x1 Martix als output fuer Skalarprodukt zweier Vektoren (Konsum)
real Return_N;//1x1 Martix als output fuer Skalarprodukt zweier Vektoren (Konsum)
real Z1;	  //2x2 Zwischenrechnungsmatrix
real Z2;	  //2x1 Zwischenrechnungsvektor
real Z3;	  //2x1 Zwischenrechnungsvektor
real Z4;	  //2x2 Zwischenrechnungsmatrix
real Z5;	  //2x2 Zwischenrechnungsmatrix
real Z6;	  //2x1 Zwischenrechnungsvektor
real Z7;	  //1x1 Zwischenrechnungsmatrix
real Z8;	  //1x1 Zwischenrechnungsvektor
real Z9;	  //2x1 Zwischenrechnungsvektor
	
real wichtig1;  	//2x2 Matrix die nach Initialisierung heilig ist
real wichtig2;    //2x2 Matrix die nach Initialisierung heilig ist
		
	rand_var *zvar;

public:
	finanzmarkt_wage();
	virtual ~finanzmarkt_wage();
	real* setLabels(char*);
	void initialize();
	void loadParamset(ifstream&);	
/*	void saveParamsetWithNames(ofstream&);*/ 
	void sendStateSpace(int &,const real***);
	void iteration(const long&);
	void wagerate();
	void Wagebill();
	void Savings();
	void EndogenR();
	void SaveValue();
	void Dividende();
/*	void saveParamset();	
	void printParamset();
	void recieve();	
*/
};

#endif

