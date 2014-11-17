///////////////////////////////////////////////////////////////////////////////
//
// Klassendefinition
//
// Name: linFima_basic.h
//       lineares Finanzmarktmodell mit Erwartungsrückkoppelung 
//       (Basis Version)
//
// Code: Markus Kopyciok
//
///////////////////////////////////////////////////////////////////////////////

#ifndef linFima_basic_H
#define	linFima_basic_H

#include "baseModel.h"
#include "rand_var.h"

#include "../strnchr.h"
#include<stdlib.h>

class linFima_basic : public baseModel
{

protected:

	// state variable - Preis p 
   		real x0;		// Initialisierung
		real x;
    		real x1;		// Lag 1
		real x2;		// Lag 2

	// state variable - Erwartungen qe	
       		real y0;                // Initialisierung
        	real y;
		real y1;		// Lag 1
		real y2;		// Lag 2
	
	// noise
		real xi0;		// Initialisierung
		rand_var *zvar;		// Zufallsvariable
		real xi_min, xi_max;    // Grenzen der Störgröße
				

	// Dividendenprozess d = psi * d + xi
		real d0;      		// Initialisierung 
		real d;			// Zufallsvariable Dividende
		real d1;		// Lag1
		real d2;		// Lag2
		real psi;		// Parameter des Dividendenprozesses

	// Parameter	
		real alpha;		// Zinsrate (R)
		real beta;		// Heterogenitäten der Ökonomie(invphi)
	
		real gamma;		// Fehlerkorrekturfaktor der Naiven Erwartungen
		real L;			// Lag der Durchschnittlichen Erwartungen
	
	// Iterationen
	   // geschützte Variable aus baseModel

	// Modellauswahl
		int MODELL;      	// 1= Rationale Erwartungen (RE) 
				 	// 2= naive Erwartungen (NE) 			
					// 3= durchschnittliche Erwartungen(AE)
	
	// Berechnung
		real summex;
		real summe1x;
		real summey;
		real summe1y;
		real mx;		// Rekursiver Mittelwert Preis
		real my;		// Rekursiver Mittelwert cum Dividenden Preis
		
	//Funktionen
		real rekursiverMittelwertx(const long);
		real rekursiverMittelwerty(const long);


public:
	linFima_basic();
	//virtual ~linFima_basic();	
	void loadParamset(ifstream&);
	void saveParamset(ofstream&);
	void printParamset();
	void initialize();
	real* sendModelVar();
	real* setLabels(char*);
	void sendStateSpace(int &,const real***);
	void sendParameters(int&, real**);
	void receiveParameters(const real*);
	void iteration(const long&t);

};

#endif
