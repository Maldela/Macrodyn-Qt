///////////////////////////////////////////////////////////////////////////////
//
// Klassendefinition
//
// Name: linFima_wage.h
//       lineares Finanzmarktmodell mit Erwartungsr"uckkoppelung
//
// Code: Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

#ifndef linFima_wage_H
#define linFima_wage_H

#include "baseModel.h"
#include "../strnchr.h"
#include "rand_var.h"
#include<stdlib.h>

class linFima_wage : public baseModel {

protected:

    real ( * pf ) ( real k, real a, real b, real c, real d );			// a pointer to the production function
    real ( * pf_prime) ( real k, real a, real b, real c, real d ); 		// and its derivative
    real pf_type;														// the current type of the production function
    real a, b, c, d;		// original parameters
    real A, B, C, D;		// parameters as ][0,1][ normalized input
    void pf_init ( void );	// maps (A,B,C) -> (a,b,c) and sets pf, pf_prime
	
	real nF,tauF,delta,k_0,Fi;
	real wagerate_t;
	real Wagebill_t;
	real Savings_t;

	enum info { bekannt, unbekannt };
	info DividendenErwartung;
	int  perfectPredictor;  // Wahrheitswertig
	real xAll;		// Anzahl der Aktien einer Unternehmung
	real R;			// Zinsfaktor der sicheren Anlage

	real p0;
	real p;		// Preis
	real pOld;
	real pPerf;	// Preis nach perfekter Erw.
	real pPerfOld;
	real pDiff;	// =pPerf-p

// Fundamentalisten
	real rF;	// Risikotoleranz
	real eF;	// Anfangsausstattung
	real VF;	// erw. Varianz der Dividende

	real pF0;
	real pF;		// erw. Preis
	real pFperf;	// erw.Preis unter perfekter Erw.
	real pFShift;
	real pFOld;

	real xF;	// Portfolio
	real xFrel;	// Portfolio/Gesamtaktien
	real xFold;
	real wF;	// Vermoegen
	real RF;	// Rendite
	real RpF;	// Risikopraemie 

// Chartisten
	real rC;
	real eC;
	real VC;
	real vc;	// Skalierung

	real pC0;
	real pC;
	real pCperf;
	real pCShift;
	real pCOld;

	real xC;
	real xCrel;
	real xCold;
	real wC;
	real RC;
	real RpC;

// Dividendenprozess

	real div;
	real varDiv;
	real Ed11,delta_d1,Ed1;
	real zetamin, zetamax, gamma;

	real dOld;
//	real div;	// Dividende
	real Ed;	// Erwartungswert der Dividende
	real Vd;	// Varianz der Dividende
	real de;	// Erwartung auf Dividende (Informationsstand  Ed oder d )

// Hilfsvariablen
	real r;		// gebildet aus (1+rF/rC)^-1 == rC/(rC+rF)
	real b_preis;		// benoetigt im Preisprozess
	real a1VFx;	// benoetigt bei der perfekten Prognose der Fundamentalisten
	real ferrF;	// Prognosefehler der Fundamentalisten
	real ferrC;
	real deltap;	// Preisveraenderung von t-1 auf t
	real deltapF;	// Prognoseveraenderung von t-1 auf t
	real ertrag;	// Ertrag der Aktienanlage

	real errvar;	// berechnete Schaetzer Varianz in der Simulation
	real divvar;	// berechnete Schaetzer der Varianz der Dividende

	rand_var *zvar;

public:
	linFima_wage();
	virtual ~linFima_wage();
	real* setLabels(char*);
	void initialize();
	void loadParamset(ifstream&);	
	void saveParamsetWithNames(ofstream&); 
	void sendStateSpace(int &,const real***);
	void iteration(const long&);
	void wagerate();
	void Wagebill();
	void Savings();
	void EndogenR();

};

#endif
