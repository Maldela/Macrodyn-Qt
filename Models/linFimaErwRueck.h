///////////////////////////////////////////////////////////////////////////////
//
// Klassendefinition
//
// Name: linFimaErwRueck.h
//       lineares Finanzmarktmodell mit Erwartungsr"uckkoppelung
//
// Code: Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

#ifndef linFimaErwRueck_H
#define linFimaErwRueck_H

#include "baseModel.h"
#include "../strnchr.h"
#include<stdlib.h>

class linFimaErwRueck : public baseModel {

protected:

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
	real dOld;
	real d;		// Dividende
	real Ed;	// Erwartungswert der Dividende
	real Vd;	// Varianz der Dividende
	real de;	// Erwartung auf Dividende (Informationsstand  Ed oder d )

// Hilfsvariablen
	real r;		// gebildet aus (1+rF/rC)^-1 == rC/(rC+rF)
	real b;		// benoetigt im Preisprozess
	real a1VFx;	// benoetigt bei der perfekten Prognose der Fundamentalisten
	real ferrF;	// Prognosefehler der Fundamentalisten
	real ferrC;
	real deltap;	// Preisveraenderung von t-1 auf t
	real deltapF;	// Prognoseveraenderung von t-1 auf t
	real ertrag;	// Ertrag der Aktienanlage

	real errvar;	// berechnete Schaetzer Varianz in der Simulation
	real divvar;	// berechnete Schaetzer der Varianz der Dividende

// Funktionen
	void getString(ifstream&,char*); // l"asst Kommentare im .sim File zu	
	real getReal(ifstream&); 		 // l"asst Kommentare im .sim File zu
	int getInt(ifstream&); 			 // l"asst Kommentare im .sim File zu

public:
	linFimaErwRueck();
	virtual ~linFimaErwRueck();
	real* setLabels(char*);
	void initialize();
	void loadParamset(ifstream&);	
	void saveParamsetWithNames(ofstream&); 
	void sendStateSpace(int &,const real***);
	void iteration(const long&);
};

#endif
