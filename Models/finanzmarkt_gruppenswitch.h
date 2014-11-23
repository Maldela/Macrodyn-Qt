///////////////////////////////////////////////////////////////////////////////
//
// Klassendefinition
//
// Name: finanzmarkt_gruppenswitch.h
//       lineares Finanzmarktmodell mit Erwartungsr"uckkoppelung
//	 2 Typen von Anteilsscheinen sowie heterogenen Haendlergruppen	
// Code: Andreas Tonn
//
///////////////////////////////////////////////////////////////////////////////

#ifndef FINANZMARKT_GRUPPENSWITCH_INCLUDED
#define FINANZMARKT_GRUPPENSWITCH_INCLUDED

#include "baseModel.h"
// #include "../strnchr.h"
#include "../matrix.h"
#include <stdlib.h>
#include "rand_var.h"

class finanzmarkt_gruppenswitch : public baseModel {



protected:
	 
	 
    const qreal epsilon;              //Gruppenswitchkonstante
	 			
    qreal R;
    qreal alphaf, alphan, ef, en;
    qreal mu, mu0;			//Gruppenanteilsvariable
    qreal Xi_F, Xi_N;			//Variable fuer aggrgierte Risikoaversion
    qreal Xi_NzuXi_F;			//Variable Xi_F/Xi_N
    qreal z1,z2,beta;			//Hilfsvariable zur Berechnung von mu
    qreal meansum_f,meansum_n;		//Variablen zu Berechnung der Nutzenwerte
    qreal mean_f,mean_n, variance_n, variance_f;    //Variablen zur Berechnung der Nutzenwerte
    qreal U_f,U_n;				//Nutzenwerte
    qreal AbZins;				//Variable fuer 1/R
    qreal xAll1, xAll2, xF1, xF2, xN1, xN2;	//aggregierte Aktien-/Portfolioanzahl
    qreal xf1, xf2, xn1, xn2;		//einzelne Portfolioanzahl
    qreal p1, p2, pF1, pF2, pN1, pN2;	//Preise, Preisvorhersagen
    qreal p01, p02, pF01, pF02, pN01, pN02;	//Startwerte fuer Preise und Vorhersagen
    qreal zetamin1, zetamax1, gamma1;	//Parameter fuer Dividendenprozeﬂ 1
    qreal zetamin2, zetamax2, gamma2;	//Parameter fuer Dividendenprozeﬂ 2
    qreal d1, d2;				//Dividendenzufallsvariablen
    qreal Ed1, Ed2, var1, var2, cov12;	//Momente der Dividendenprozesse
    qreal var1err, var2err, coerr;
    qreal zetamin3, zetamax3;		//Parameter fuer Fehleinschaetzung
    qreal zetamin4, zetamax4;		//Parameter fuer Fehleinschaetzung
    qreal rho1, rho2;			//Fehleinschaetzung der Noise--Trader
    qreal p1_old, p2_old;			//Auswertungsvariable
    qreal xF1_old, xF2_old, xN1_old, xN2_old;//Merkwerte fuer Auswertung
    qreal return_F, return_N; 		//qrealisierter Return (aggregiert)
    qreal return_f, return_n; 		//qrealisierter Return (einzeln)
	
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
	finanzmarkt_gruppenswitch();
	virtual ~finanzmarkt_gruppenswitch();
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



