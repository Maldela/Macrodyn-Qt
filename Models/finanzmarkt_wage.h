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
	
	
    static const qreal  epsilon=1e-16;

/**** production function ****/

    qreal ( * pf ) ( qreal k, qreal a, qreal b, qreal c, qreal d );			// a pointer to the production function
    qreal ( * pf_prime) ( qreal k, qreal a, qreal b, qreal c, qreal d ); 		// and its derivative
    qreal pf_type;														// the current type of the production function
    qreal a, b, c, d;		// original parameters
    qreal A_pf, B_pf, C_pf, D_pf;		// parameters as ][0,1][ normalized input
    void pf_init ( void );	// maps (A,B,C) -> (a,b,c) and sets pf, pf_prime

/**** new Parameter ****/
	
    qreal n_F,tau_F,delta,k_0,Fi;
    qreal wagerate_t;
    qreal Wagebill_t;
    qreal Savings_t;
    qreal R;

/**** old Parameter ****/

    qreal alpha, ef, eF;
    qreal mu, mu0;			//Gruppenanteilsvariable
    qreal Xi_F;				//Variable fuer aggrgierte Risikoaversion
    qreal Xi_NzuXi_F;		//Variable Xi_F/Xi_N
    qreal AbZins;			//Variable fuer 1/R
    qreal p01, pF01;			//Startwerte fuer Preise und Vorhersagen
    qreal p_0, p1, pF1, pR1;		//Preise, Preisvorhersagen
    qreal pN_old, pN_old2, Ed_old, Ed_old2;    //Fuer die Vorhersagefehlerberechung
    qreal err_div,err_div2, err_pN, err_pN2, err_qN, err_qN2;	//Vorhersagefehler der N--Traders (dividenden, ex--dividend, cum--dividend)
    qreal delta_p, delta_p2, delta_div, delta_div2, delta_q, delta_q2;
    qreal xAll, xAll2, xF1, xF2, xN1, xN2;						//aggregierte Aktien-/Portfolioanzahl
    qreal xF11, xF22, xN11, xN22;	        					//andere Skalierung fuer graphische Ausgabe
    qreal xf1, xf2, xn1, xn2;									//einzelne Portfolioanzahl
    qreal pxF, pxN, pxf, pxn;									//Variablen zur Schuldenberechnung
    qreal Haben_F, Haben_f;
    qreal zetamin, zetamax, gamma;							//Parameter fuer Dividendenprozeﬂ 1
    qreal div;														//Dividendenzufallsvariablen
    qreal Ed, Ed2, Ed11, Ed22, var, var2, cov12;				//Momente der Dividendenprozesse
    qreal delta_d1, delta_d2;									//d_{t+1}-\E_t(d_{t+1}) fuer alternative Renditenberechung
    qreal var1err, var2err, coerr;								//Fehler der Noise--Trader auf die Kovarianzmatrix
    qreal a1, b1, c1, F_c1;										//Parameterwerte der 1. Dreiecksverteilung,
    qreal a2, b2, c2, F_c2;										//Parameterwerte der 2. Dreiecksverteilung,
    qreal rho1, rho2, Erho1, Erho2;	     					    //fuer die Fehleinschaetzung der Noise--Trader
    qreal p1_old, p2_old, d1_old, d2_old;						//Auswertungsvariable
    qreal xF_old, xF_old2, xN_old1, xN_old2;
    qreal return_F, return_N, return_f, return_n; 				//qrealisierter Return (aggregiert und einzeln)
    qreal rend_F, rend_N, rend_f, rend_n;						//Renditen der Fundamentalisten/ Noise--Trader
    qreal rendf_neu, rendn_neu;									//alternativ berechnete Renditen
    qreal ErendF_p, ErendF_ges; 									//Erwartete Renditen
    qreal ErendN_p, ErendN_ges; 									//Erwartete Renditen
    qreal RPF, RPN, sdRF, sdRN; 									//Risikopr‰mien und deren Standardaweichungen
    qreal Eff_F, Eff_N, delta_Eff;								//Steigungen der Effizienzlinien
    qreal z1, z2, z3, z4, z5, z7, z8, muN;  						//Zischenrechnungsvariablen
    qreal xBundle;
    qreal mRF, mRN, sRF, sRN, vRF, vRN;      //Variablen fuer Renditenmittelwerte,
    qreal SIZF, SIZN, SIF, SIN, delta_SI;    //Standardabweichungen und Varianzen fuer SharpeIndex
    qreal wechselanteil;						//Anteil der Fundamentalisten der je Periode switcht
    qreal etafix, etaF;						//Anteile der ,,Unbelehrbaren'' Strategen
    qreal beta;                              //Gewichtungskonstante fuer Gruppenswitch
    qreal zwischen1, zwischen2, zwischen3;   //Zischenrechnungsvariablen fuer Gruppenswitch
	
qreal point;
qreal A;		//2x2 Martix fuer Initialisierung
qreal B; 	  //2x2 Martix fuer Initialisierung
qreal C; 	  //2x1 Vektor fuer Initialisierung
qreal D; 	  //2x1 Vektor der qrealisierten Dividenden
qreal deltaD;  //2x1 Vektor fuer alternative Renditenberechnung
qreal ED;	   //2x1 Vektor der Dividendenerwartungswerte
qreal ED2;	  //2x1 Vektor der Dividendenerwartungswerte
qreal Id;	  //2x2 Identitaet
qreal P; 	  //2x1 Vektor der Wertpapierpreise
qreal P_old;   //2x1 Vektor der Wertpapierpreise der Vorperiode
qreal PF;	  //2x1 Vektor der Preisvorhersage der Fundamentalisten
qreal PN;	  //2x1 Vektor der Preisvorhersage der Noise--Trader
qreal RHO;	  //2x1 Vektor der Preisfehleinschaetzung der Noise--Trader
qreal RHO_old; //2x1 Vektor fuer alternative Renditenberechnung
qreal ERHO;    //2x1 Vektor der Preisfehleinschaetzung der Noise--Trader
qreal VF;	  //2x2 Kovarianzmatrix der Fundamentalisten
qreal VN;	  //2x2 Kovarianzmatrix der Noise--Trader
qreal X; 	  //2x1 Vektor aller Anteilsscheine (X=XF+XN)
qreal XF;	  //2x1 Vektor aller Anteilsscheine im Besitz der Fundamentalisten
qreal XN;	  //2x1 Vektor aller Anteilsscheine im Besitz der Noise--Trader
qreal XF_old;  //2x1 Vektor aller Anteilsscheine im Besitz der alten Fundamentalisten
qreal XN_old;  //2x1 Vektor aller Anteilsscheine im Besitz der alten Noise--Trader
qreal PXF;	  //1x1 Vektor zur Schuldenberechnung
qreal PXN;	  //1x1 Vektor zur Schuldenberechnung
qreal Return_F;//1x1 Martix als output fuer Skalarprodukt zweier Vektoren (Konsum)
qreal Return_N;//1x1 Martix als output fuer Skalarprodukt zweier Vektoren (Konsum)
qreal Z1;	  //2x2 Zwischenrechnungsmatrix
qreal Z2;	  //2x1 Zwischenrechnungsvektor
qreal Z3;	  //2x1 Zwischenrechnungsvektor
qreal Z4;	  //2x2 Zwischenrechnungsmatrix
qreal Z5;	  //2x2 Zwischenrechnungsmatrix
qreal Z6;	  //2x1 Zwischenrechnungsvektor
qreal Z7;	  //1x1 Zwischenrechnungsmatrix
qreal Z8;	  //1x1 Zwischenrechnungsvektor
qreal Z9;	  //2x1 Zwischenrechnungsvektor
	
qreal wichtig1;  	//2x2 Matrix die nach Initialisierung heilig ist
qreal wichtig2;    //2x2 Matrix die nach Initialisierung heilig ist
		
	rand_var *zvar;

public:
	finanzmarkt_wage();
	virtual ~finanzmarkt_wage();
    qreal* setLabels(char*);
	void initialize();
	void loadParamset(ifstream&);	
/*	void saveParamsetWithNames(ofstream&);*/ 
    void sendStateSpace(int &,const qreal***);
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

