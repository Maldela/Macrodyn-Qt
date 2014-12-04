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
	
	
    qreal R;
    qreal F, N, alphaf, alphan, aF, aN;
    qreal aFzuaN;				//Variable fuer aN/aF
    qreal z1uaF;				//Variable fuer 1/aF
    qreal AbZins;				//Variable fuer 1/R
    qreal ef, en, eF, eN;			//einzelne bzw. aggregierte Grundaustattungen
    qreal xAll1, xAll2, xF1, xF2, xN1, xN2;	//aggregierte Aktien-/Portfolioanzahl
    qreal xf1, xf2, xn1, xn2;		//einzelne Portfolioanzahl
    qreal p1, p2, pF1, pF2, pN1, pN2;	//Preise, Preisvorhersagen
    qreal p01, p02, pF01, pF02, pN01, pN02;	//Startwerte fuer Preise und Vorhersagen
    qreal zetamin1, zetamax1, gamma1;	//Parameter fuer Dividendenprozess 1
    qreal zetamin2, zetamax2,gamma2;		//Parameter fuer Dividendenprozess 2
    qreal d1, d2;				//Dividendenzufallsvariablen
    qreal Ed1, Ed2, var1, var2, cov12;	//Momente der Dividendenprozesse
    qreal zetamin3, zetamax3;		//Parameter fuer Fehleinschaetzung
    qreal zetamin4, zetamax4;		//Parameter fuer Fehleinschaetzung
    qreal rho1, rho2;			//Fehleinschaetzung der Noise--Trader
    qreal p1_old, p2_old;			//Auswertungsvariable
    qreal xF1_old, xF2_old, xN1_old, xN2_old;
    qreal quote_F1, quote_F2, quote_N1, quote_N2; //Quoten von Anteilsscheinen (aggregiert)
    qreal quote_f1, quote_f2, quote_n1, quote_n2; //Quoten von Anteilsscheinen
    qreal return_F, return_N, return_f, return_n; //qrealisierter Return (aggregiert und einzeln)
    qreal cF, cN, cf, cn;			//qrealisierter Konsum (aggregiert und einzeln)
    qreal rend_F, rend_N, rend_diff;		//Renditen der Fundamentalisten/ Noise--Trader
    qreal quote_rF, quote_rN;		//Rendite--Quoten
	
	
	matrixFn *point;
	matrixDef *A;		//2x2 Martix fuer Initialisierung
	matrixDef *B;		//2x2 Martix fuer Initialisierung
	matrixDef *C;		//2x1 Vektor fuer Initialisierung
    matrixDef *D;		//2x1 Vektor der qrealisierten Dividenden
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
    qreal* setLabels(char*);
	void initialize();
	void loadParamset(ifstream&);	
/*	void saveParamsetWithNames(ofstream&);*/ 
    void sendStateSpace(int &,const qreal***);
	void iteration(const long&);
/*	void saveParamset();	
	void printParamset();
	void recieve();	
*/
};

#endif



