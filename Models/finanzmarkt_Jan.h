///////////////////////////////////////////////////////////////////////////////
//
// $Header: armax_eindim.h Dec 20 METDST 2000
//
// Module name:		armax_eindim.h
// Contents:		Class definition of the class armax_eindim
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _finanzmarkt_Jan_INCLUDED
#define _finanzmarkt_Jan_INCLUDED

#include "baseModel.h"
#include "distribution.h"
#include "../matrix_neu.h"
#include <stdio.h>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		finanzmarkt_Jan
// Derived from:	baseModel
// Purpose:		defines the finanzmarkt_Jan
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

class finanzmarkt_Jan : public baseModel {

protected:
	static const int L=2;

	distribution *rand_dis;			//needed for pseudo-random number
    qreal rand_rec;          		//generates a rectangular unit variate
    qreal b1,b2;						//rechter Rand der Dreiecksverteilungen
    qreal c1,c2;						//peak der Dreiecksverteilungen
    qreal a1,a2;						//linker Rand der Dreiecksverteilungen
    qreal z1,z2;
    qreal F_c1, F_c2;
	
    qreal zetamin1, zetamax1, gamma1;	//Parameter für AR1-Prozess der Dividende 1
    qreal zetamin2, zetamax2, gamma2;	//Parameter für AR1-Prozess der Dividende 2
	matrix_neu* d;					// Dividende
    qreal d1,d2;						//Kompenenten des Dividenden-Vektors
    qreal var_d1,var_d2;				//Varianzen der Dividenden-Prozesse

    qreal eta_F, eta_C;				//Gruppenanteile
    qreal alpha_F, alpha_C;			//Risikoparameter

    matrix_neu* Rho[L];				// Array von Diagonalmatrizen für MA
	matrix_neu* q_MA[L];			// Array der gelagten cumdividend-Preise
    qreal r, R;						// fixed rate of interest, R=1+r
	matrix_neu* p;					// price
	matrix_neu* p_old;				// price of the previous period
	matrix_neu* q;					// cumdividend price
    matrix_neu* q0;					// Startwerte für q, q_C und q_F
	matrix_neu* q_C;				// cumdividend forecast (Chartists)
	matrix_neu* q_F;				// cumdividend forecast (Fundamentalists)
	matrix_neu* xi;					// noise
    qreal xi1, xi2;					// Komponenten des noise-Vektors
	matrix_neu* E_xi;				// Erwartungswert noise
	matrix_neu* x_all;				// alle papiere
    qreal x_all1, x_all2;			// Komponenten von alle papiere
    matrix_neu* x_N;				// Portefeuille der imaginären Noise-Traders
    qreal x_N1, x_N2;				// Komponenten des Noise-Trader-Portef.
	matrix_neu* x_Nold;				// Portefeuille der Noise-Traders aus der Vorperiode
	matrix_neu* x_F;				// Portefeuille der Fundamentalisten
    qreal x_F1, x_F2;				// Komponenten des Fundamentalisten-Portef.
	matrix_neu* x_Fold;				// Portefeuille der Fundamentalisten aus der Vorperiode
	matrix_neu* A;					// riskadjusted sum of covariance-matrices
	matrix_neu* A_C;				// riskadjusted covariance-matrix(Chartist)
	matrix_neu* A_F;				// riskadjusted covariance-matrix(Fundamentalist)
	matrix_neu* V_C;				// Kovarianz-Matrix (Chartists)
	matrix_neu* V_F;				// Kovarianz-Matrix (Fundamentalist)
    qreal e_N, e_F;					// Anfangsausstattung der Händlergruppen
    qreal wealth_N, wealth_F;		// Endvermögen der Händlergruppen
    qreal rend_N, rend_F;			// Rendite der Händlergruppen
    qreal mu_F;						// mean of returns der Fundamentalisten
    qreal sigma_F;					// standard deviation of returns der Fundamentalisten
    qreal mu_N;						// mean of returns der Noise-Trader
    qreal sigma_N;					// standard deviation of returns der Noise-Trader
    qreal sr_N;						// sharpe-ratio Noise-Trader
    qreal sr_F;					// sharpe-ratio Fundamentalisten




// Hilfsmatrizen:
	matrix_neu* V_C_inv;
	matrix_neu* V_F_inv;
	matrix_neu* temp1_2_2;
	matrix_neu* temp2_2_2;
	matrix_neu* temp3_2_2; 
	matrix_neu* temp1_2_1;
	matrix_neu* temp2_2_1;
	matrix_neu* temp3_2_1;
	matrix_neu* temp1_1_1;
	matrix_neu* V_xi;			// Kovarianz-Matrix (Noise)
	matrix_neu* V_xi_inv;

public:
    finanzmarkt_Jan();				// constructor
    virtual ~finanzmarkt_Jan();		// destructor
    void loadParamset(ifstream&);	// load parameterset from a file
    void saveParamset(ofstream&);
    void printParamset();
    void sendParameters(int &,qreal**);
    qreal* sendModelVar(void);
    void receiveParameters(const qreal*);
    qreal* setLabels(char*);		// get a variable by a name
    void initialize(void);		// initialize the model
    void iteration(const qint64&);	// perform one iteration
    void sendStateSpace(int &,const qreal***);

};

#endif //  _finanzmarkt_Jan_INCLUDED
