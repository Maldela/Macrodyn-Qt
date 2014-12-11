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
#include "rand_var.h"

class linFima_wage : public baseModel {

protected:

    qreal ( * pf ) ( qreal k, qreal a, qreal b, qreal c, qreal d );			// a pointer to the production function
    qreal ( * pf_prime) ( qreal k, qreal a, qreal b, qreal c, qreal d ); 		// and its derivative
    qreal pf_type;														// the current type of the production function
    qreal a, b, c, d;		// original parameters
    qreal A, B, C, D;		// parameters as ][0,1][ normalized input
    void pf_init ( void );	// maps (A,B,C) -> (a,b,c) and sets pf, pf_prime
	
	qreal nF,tauF,delta,k_0,Fi;
	qreal wagerate_t;
	qreal Wagebill_t;
	qreal Savings_t;

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

	qreal div;
	qreal varDiv;
	qreal Ed11,delta_d1,Ed1;
    qreal zetamin, zetamax, gamma;

	qreal dOld;
//	qreal div;	// Dividende
	qreal Ed;	// Erwartungswert der Dividende
	qreal Vd;	// Varianz der Dividende
	qreal de;	// Erwartung auf Dividende (Informationsstand  Ed oder d )

// Hilfsvariablen
	qreal r;		// gebildet aus (1+rF/rC)^-1 == rC/(rC+rF)
	qreal b_preis;		// benoetigt im Preisprozess
	qreal a1VFx;	// benoetigt bei der perfekten Prognose der Fundamentalisten
	qreal ferrF;	// Prognosefehler der Fundamentalisten
	qreal ferrC;
	qreal deltap;	// Preisveraenderung von t-1 auf t
	qreal deltapF;	// Prognoseveraenderung von t-1 auf t
	qreal ertrag;	// Ertrag der Aktienanlage

	qreal errvar;	// berechnete Schaetzer Varianz in der Simulation
	qreal divvar;	// berechnete Schaetzer der Varianz der Dividende

	rand_var *zvar;

public:
	linFima_wage();
	virtual ~linFima_wage();
	qreal* setLabels(const QString&);
	void initialize();
	void loadParamset(QTextStream&);	
	void saveParamsetWithNames(QTextStream&); 
    void receiveParameters(const QList<qreal>&);
	void sendStateSpace(int &,const qreal***);
	void iteration(const qint64&);
	void wagerate();
	void Wagebill();
	void Savings();
	void EndogenR();

};

#endif
