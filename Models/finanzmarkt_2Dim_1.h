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

#ifndef FINANZMARKT_2DIM_1_INCLUDED
#define FINANZMARKT_2DIM_1_INCLUDED

#include "baseModel.h"
// #include "../strnchr.h"
#include "../matrix.h"
#include <stdlib.h>
#include "rand_var.h"

class finanzmarkt_2Dim_1 : public baseModel {

protected:
	
	
	static const real  epsilon=1e-16;
	
	int noise;
	real R;
	real alphaf, alphan, ef, en, eN, eF;
	real mu, mu0;			//Gruppenanteilsvariable
	real Xi_F, Xi_N;			//Variable fuer aggrgierte Risikoaversion
	real Xi_NzuXi_F;			//Variable Xi_F/Xi_N
	real AbZins;				//Variable fuer 1/R
	real p01, p02, pF01, pF02, pN01, pN02;	//Startwerte fuer Preise und Vorhersagen
	real p1, p2, pF1, pF2, pN1, pN2, pR1;	//Preise, Preisvorhersagen
	real pN_old1, pN_old2, Ed_old1, Ed_old2;//Fuer die Vorhersagefehlerberechung
	real err_div1,err_div2, err_pN1, err_pN2, err_qN1, err_qN2;//Vorhersagefehler der N--Traders (dividenden, ex--dividend, cum--dividend)
	real delta_p1, delta_p2, delta_div1, delta_div2, delta_q1, delta_q2;
	real xAll1, xAll2, xF1, xF2, xN1, xN2;	//aggregierte Aktien-/Portfolioanzahl
	real xF11, xF22, xN11, xN22;	        //andere Skalierung fuer graphische Ausgabe
	real xf1, xf2, xn1, xn2;		//einzelne Portfolioanzahl
	real pxF, pxN, pxf, pxn;		//Variablen zur Schuldenberechnung
	real Haben_F, Haben_N, Haben_f, Haben_n;
	real zetamin1, zetamax1, gamma1;	//Parameter fuer Dividendenprozeﬂ 1
	real zetamin2, zetamax2, gamma2;	//Parameter fuer Dividendenprozeﬂ 2	
	real d1, d2;				//Dividendenzufallsvariablen
	real Ed1, Ed2, Ed11, Ed22, var1, var2, cov12;	//Momente der Dividendenprozesse
	real delta_d1, delta_d2;		//d_{t+1}-\E_t(d_{t+1}) fuer alternative Renditenberechung
	real var1err, var2err, coerr;		//Fehler der Noise--Trader auf die Kovanrianmatrix
	real a1, b1, c1, F_c1;			//Parameterwerte der 1. Dreiecksverteilung,
	real a2, b2, c2, F_c2;			//Parameterwerte der 2. Dreiecksverteilung,	
	real rho1, rho2, Erho1, Erho2;	        //fuer die Fehleinschaetzung der Noise--Trader
	real p1_old, p2_old, d1_old, d2_old;			//Auswertungsvariable
	real xF_old1, xF_old2, xN_old1, xN_old2;
	real return_F, return_N, return_f, return_n; //Realisierter Return (aggregiert und einzeln)
	real rend_F, rend_N, rend_f, rend_n;	//Renditen der Fundamentalisten/ Noise--Trader
	real rendf_neu, rendn_neu;		//alternativ berechnete Renditen
	real ErendF_p, ErendF_ges; 		//Erwartete Renditen
	real ErendN_p, ErendN_ges; 		//Erwartete Renditen
	real RPF, RPN, sdRF, sdRN; 		//Risikopr‰mien und deren Standardaweichungen
	real Eff_F, Eff_N, delta_Eff;		//Steigungen der Effizienzlinien
	real z1, z2, z3, z4, z5, z7, z8, muN;  	//Zischenrechnungsvariablen
	real xBundle;
	real mRF, mRN, sRF, sRN, vRF, vRN;      //Variablen fuer Renditenmittelwerte,
    real SIZF, SIZN, SIF, SIN, delta_SI;    //Standardabweichungen und Varianzen fuer SharpeIndex 	
	real wechselanteil;			//Anteil der Fundamentalisten der je Periode switcht
	real etafix, etaF;			//Anteile der ,,Unbelehrbaren'' Strategen
	real beta;                              //Gewichtungskonstante fuer Gruppenswitch
	real zwischen1, zwischen2, zwischen3;   //Zischenrechnungsvariablen fuer Gruppenswitch 
	
	matrixFn *point;
	matrixDef *A;		//2x2 Martix fuer Initialisierung
	matrixDef *B;		//2x2 Martix fuer Initialisierung
	matrixDef *C;		//2x1 Vektor fuer Initialisierung
	matrixDef *D;		//2x1 Vektor der realisierten Dividenden	
	matrixDef *deltaD;	//2x1 Vektor fuer alternative Renditenberechnung
	matrixDef *ED;		//2x1 Vektor der Dividendenerwartungswerte
	matrixDef *ED2;		//2x1 Vektor der Dividendenerwartungswerte
	matrixDef *Id; 		//2x2 Identitaet
	matrixDef *P;		//2x1 Vektor der Wertpapierpreise
	matrixDef *P_old;	//2x1 Vektor der Wertpapierpreise der Vorperiode
	matrixDef *PF;		//2x1 Vektor der Preisvorhersage der Fundamentalisten
	matrixDef *PN;		//2x1 Vektor der Preisvorhersage der Noise--Trader
	matrixDef *RHO; 	//2x1 Vektor der Preisfehleinschaetzung der Noise--Trader
	matrixDef *RHO_old; 	//2x1 Vektor fuer alternative Renditenberechnung
	matrixDef *ERHO; 	//2x1 Vektor der Preisfehleinschaetzung der Noise--Trader
	matrixDef *VF;  	//2x2 Kovarianzmatrix der Fundamentalisten
	matrixDef *VN;  	//2x2 Kovarianzmatrix der Noise--Trader
	matrixDef *X;   	//2x1 Vektor aller Anteilsscheine (X=XF+XN)
	matrixDef *XF;  	//2x1 Vektor aller Anteilsscheine im Besitz der Fundamentalisten
	matrixDef *XN;  	//2x1 Vektor aller Anteilsscheine im Besitz der Noise--Trader
	matrixDef *XF_old;  	//2x1 Vektor aller Anteilsscheine im Besitz der alten Fundamentalisten
	matrixDef *XN_old;  	//2x1 Vektor aller Anteilsscheine im Besitz der alten Noise--Trader
	matrixDef *PXF;		//1x1 Vektor zur Schuldenberechnung
	matrixDef *PXN;		//1x1 Vektor zur Schuldenberechnung 
	matrixDef *Return_F;	//1x1 Martix als output fuer Skalarprodukt zweier Vektoren (Konsum)
	matrixDef *Return_N;	//1x1 Martix als output fuer Skalarprodukt zweier Vektoren (Konsum)	
	matrixDef *Z1;  	//2x2 Zwischenrechnungsmatrix 
	matrixDef *Z2;  	//2x1 Zwischenrechnungsvektor 
	matrixDef *Z3;  	//2x1 Zwischenrechnungsvektor
	matrixDef *Z4;  	//2x2 Zwischenrechnungsmatrix 
	matrixDef *Z5;  	//2x2 Zwischenrechnungsmatrix
	matrixDef *Z6;  	//2x1 Zwischenrechnungsvektor
	matrixDef *Z7;  	//1x1 Zwischenrechnungsmatrix
	matrixDef *Z8;  	//1x1 Zwischenrechnungsvektor
	matrixDef *Z9;  	//2x1 Zwischenrechnungsvektor
	
	matrixDef *wichtig1;  	//2x2 Matrix die nach Initialisierung heilig ist
	matrixDef *wichtig2;  	//2x2 Matrix die nach Initialisierung heilig ist
	
	
	
	rand_var *zvar1;
	rand_var *zvar2;
	rand_var *zvar3;
	rand_var *zvar4;	


public:
	finanzmarkt_2Dim_1();
	virtual ~finanzmarkt_2Dim_1();
	real* setLabels(char*);
	void initialize();
	void loadParamset(ifstream&);	
/*	void saveParamsetWithNames(ofstream&);*/ 
	void sendStateSpace(int &,const real***);
	void iteration(const long&);
/*	void saveParamset();	
	void printParamset();
	void recieve();	
*/
};

#endif

