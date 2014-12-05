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
        qreal x0;		// Initialisierung
        qreal x;
            qreal x1;		// Lag 1
        qreal x2;		// Lag 2

	// state variable - Erwartungen qe	
            qreal y0;                // Initialisierung
            qreal y;
        qreal y1;		// Lag 1
        qreal y2;		// Lag 2
	
	// noise
        qreal xi0;		// Initialisierung
		rand_var *zvar;		// Zufallsvariable
        qreal xi_min, xi_max;    // Grenzen der Störgröße
				

	// Dividendenprozess d = psi * d + xi
        qreal d0;      		// Initialisierung
        qreal d;			// Zufallsvariable Dividende
        qreal d1;		// Lag1
        qreal d2;		// Lag2
        qreal psi;		// Parameter des Dividendenprozesses

	// Parameter	
        qreal alpha;		// Zinsrate (R)
        qreal beta;		// Heterogenitäten der Ökonomie(invphi)
	
        qreal gamma;		// Fehlerkorrekturfaktor der Naiven Erwartungen
        qreal L;			// Lag der Durchschnittlichen Erwartungen
	
	// Iterationen
       // geschützte Variable aus baseModel

	// Modellauswahl
		int MODELL;      	// 1= Rationale Erwartungen (RE) 
				 	// 2= naive Erwartungen (NE) 			
					// 3= durchschnittliche Erwartungen(AE)
	
	// Berechnung
        qreal summex;
        qreal summe1x;
        qreal summey;
        qreal summe1y;
        qreal mx;		// Rekursiver Mittelwert Preis
        qreal my;		// Rekursiver Mittelwert cum Dividenden Preis
		
	//Funktionen
        qreal rekursiverMittelwertx(const qint64);
        qreal rekursiverMittelwerty(const qint64);


public:
	linFima_basic();
	//virtual ~linFima_basic();	
    void loadParamset(QDataStream&);
    void saveParamset(QDataStream&);
	void printParamset();
	void initialize();
    qreal* sendModelVar();
    qreal* setLabels(const QString&);
    void sendStateSpace(int &,const qreal***);
    void sendParameters(int&, qreal**);
    void receiveParameters(const qreal*);
    void iteration(const qint64&t);

};

#endif
