///////////////////////////////////////////////////////////////////////////////
//
// Klassendefinition
//
// Name: finanzmarkt_2Dim.h
//       lineares Finanzmarktmodell mit Erwartungsr"uckkoppelung
//	 2 Typen von Anteilsscheinen sowie heterogenen Haendlergruppen	
// Code: Andreas Tonn
//
///////////////////////////////////////////////////////////////////////////////

#ifndef FINANZMARKT_2DIM_INCLUDED
#define FINANZMARKT_2DIM_INCLUDED

#include "baseModel.h"
// #include "../strnchr.h"
#include "../matrix.h"
#include <stdlib.h>
#include "rand_var.h"

class finanzmarkt_2Dim : public baseModel {

protected:
	
	
	real R;
	real F, N, alphaf, alphan, aF, aN;
	real aFzuaN;				//Variable fuer aN/aF
	real z1uaF;				//Variable fuer 1/aF
	real AbZins;				//Variable fuer 1/R
	real ef, en, eF, eN;			//einzelne bzw. aggregierte Grundaustattungen
	real xAll1, xAll2, xF1, xF2, xN1, xN2;	//aggregierte Aktien-/Portfolioanzahl
	real xf1, xf2, xn1, xn2;		//einzelne Portfolioanzahl
	real p1, p2, pF1, pF2, pN1, pN2;	//Preise, Preisvorhersagen
	real p01, p02, pF01, pF02, pN01, pN02;	//Startwerte fuer Preise und Vorhersagen
	real zetamin1, zetamax1, gamma1;	//Parameter fuer Dividendenprozeﬂ 1
	real zetamin2, zetamax2,gamma2;		//Parameter fuer Dividendenprozeﬂ 2	
	real d1, d2;				//Dividendenzufallsvariablen
	real Ed1, Ed2, var1, var2, cov12;	//Momente der Dividendenprozesse
	real zetamin3, zetamax3;		//Parameter fuer Fehleinschaetzung
	real zetamin4, zetamax4;		//Parameter fuer Fehleinschaetzung
	real rho1, rho2;			//Fehleinschaetzung der Noise--Trader
	real p1_old, p2_old;			//Auswertungsvariable
	real xF1_old, xF2_old, xN1_old, xN2_old;
	real quote_F1, quote_F2, quote_N1, quote_N2; //Quoten von Anteilsscheinen (aggregiert)
	real quote_f1, quote_f2, quote_n1, quote_n2; //Quoten von Anteilsscheinen
	real return_F, return_N, return_f, return_n; //Realisierter Return (aggregiert und einzeln)
	real cF, cN, cf, cn;			//Realisierter Konsum (aggregiert und einzeln)
	real rend_F, rend_N, rend_diff;		//Renditen der Fundamentalisten/ Noise--Trader
	real quote_rF, quote_rN;		//Rendite--Quoten
	
	
	matrixFn *point;
	matrixDef *A;		//2x2 Martix fuer Initialisierung
	matrixDef *B;		//2x2 Martix fuer Initialisierung
	matrixDef *C;		//2x1 Vektor fuer Initialisierung
	matrixDef *D;		//2x1 Vektor der realisierten Dividenden	
	matrixDef *ED;		//2x1 Vektor der Dividendenerwartungswerte
	matrixDef *Id; 		//2x2 Identitaet
	matrixDef *P;		//2x1 Vektor der Wertpapierpreise
	matrixDef *P_old;	//2x1 Vektor der Wertpapierpreise der Vorperiode
	matrixDef *PF;		//2x1 Vektor der Preisvorhersage der Fundamentalisten
	matrixDef *PN;		//2x1 Vektor der Preisvorhersage der Noise--Trader
	matrixDef *RHO; 	//2x1 Vektor der Preisfehleinschaetzung der Noise--Trader
	matrixDef *VF;  	//2x2 Kovarianzmatrix der Fundamentalisten
	matrixDef *VN;  	//2x2 Kovarianzmatrix der Noise--Trader
	matrixDef *X;   	//2x1 Vektor aller Anteilsscheine (X=XF+XN)
	matrixDef *XF;  	//2x1 Vektor aller Anteilsscheine im Besitz der Fundamentalisten
	matrixDef *XN;  	//2x1 Vektor aller Anteilsscheine im Besitz der Noise--Trader
	matrixDef *XF_old;  	//2x1 Vektor aller Anteilsscheine im Besitz der alten Fundamentalisten
	matrixDef *XN_old;  	//2x1 Vektor aller Anteilsscheine im Besitz der alten Noise--Trader
	matrixDef *Return_F;	//1x1 Martix als output fuer Skalarprodukt zweier Vektoren (Konsum)
	matrixDef *Return_N;	//1x1 Martix als output fuer Skalarprodukt zweier Vektoren (Konsum)	
	matrixDef *Z1;  	//2x2 Zwischenrechnungsmatrix 
	matrixDef *Z2;  	//2x1 Zwischenrechnungsvektor 
	matrixDef *Z3;  	//2x1 Zwischenrechnungsvektor
	matrixDef *Z4;  	//2x2 Zwischenrechnungsmatrix 
	matrixDef *Z5;  	//2x2 Zwischenrechnungsmatrix
	matrixDef *Z6;  	//2x1 Zwischenrechnungsvektor

	rand_var *zvar1;
	rand_var *zvar2;
	rand_var *zvar3;
	rand_var *zvar4;	


public:
	finanzmarkt_2Dim();
	virtual ~finanzmarkt_2Dim();
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



