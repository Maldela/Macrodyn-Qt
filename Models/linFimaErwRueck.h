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
	qreal xAll;		// Anzahl der Aktien einer Unternehmung
	qreal R;			// Zinsfaktor der sicheren Anlage

	qreal p0;
	qreal p;		// Preis
	qreal pOld;
	qreal pPerf;	// Preis nach perfekter Erw.
	qreal pPerfOld;
	qreal pDiff;	// =pPerf-p

// Fundamentalisten
	qreal rF;	// Risikotoleranz
	qreal eF;	// Anfangsausstattung
	qreal VF;	// erw. Varianz der Dividende

	qreal pF0;
	qreal pF;		// erw. Preis
	qreal pFperf;	// erw.Preis unter perfekter Erw.
	qreal pFShift;
	qreal pFOld;

	qreal xF;	// Portfolio
	qreal xFrel;	// Portfolio/Gesamtaktien
	qreal xFold;
	qreal wF;	// Vermoegen
	qreal RF;	// Rendite
	qreal RpF;	// Risikopraemie 

// Chartisten
	qreal rC;
	qreal eC;
	qreal VC;
	qreal vc;	// Skalierung

	qreal pC0;
	qreal pC;
	qreal pCperf;
	qreal pCShift;
	qreal pCOld;

	qreal xC;
	qreal xCrel;
	qreal xCold;
	qreal wC;
	qreal RC;
	qreal RpC;

// Dividendenprozess
	qreal dOld;
	qreal d;		// Dividende
	qreal Ed;	// Erwartungswert der Dividende
	qreal Vd;	// Varianz der Dividende
	qreal de;	// Erwartung auf Dividende (Informationsstand  Ed oder d )

// Hilfsvariablen
	qreal r;		// gebildet aus (1+rF/rC)^-1 == rC/(rC+rF)
	qreal b;		// benoetigt im Preisprozess
	qreal a1VFx;	// benoetigt bei der perfekten Prognose der Fundamentalisten
	qreal ferrF;	// Prognosefehler der Fundamentalisten
	qreal ferrC;
	qreal deltap;	// Preisveraenderung von t-1 auf t
	qreal deltapF;	// Prognoseveraenderung von t-1 auf t
	qreal ertrag;	// Ertrag der Aktienanlage

	qreal errvar;	// berechnete Schaetzer Varianz in der Simulation
	qreal divvar;	// berechnete Schaetzer der Varianz der Dividende

// Funktionen
	void getString(ifstream&,char*); // l"asst Kommentare im .sim File zu	
	qreal getqreal(ifstream&); 		 // l"asst Kommentare im .sim File zu
	int getInt(ifstream&); 			 // l"asst Kommentare im .sim File zu

public:
	linFimaErwRueck();
	virtual ~linFimaErwRueck();
	qreal* setLabels(char*);
	void initialize();
	void loadParamset(ifstream&);	
	void saveParamsetWithNames(ofstream&); 
	void sendStateSpace(int &,const qreal***);
	void iteration(const long&);
};

#endif
