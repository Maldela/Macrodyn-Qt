///////////////////////////////////////////////////////////////////////////////
//
// $Header: armax_eindim.h Dec 20 METDST 2000
//
// Module name:		finanzmarkt_JanE.h
// Contents:		Class definition of the class armax_eindim
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _finanzmarkt_JanE_INCLUDED
#define _finanzmarkt_JanE_INCLUDED

#include "baseModel.h"
#include "distribution.h"
#include "rand_var.h"

#include "../matrix_neu.h"
#include <stdio.h>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		finanzmarkt_JanE
// Derived from:	baseModel
// Purpose:		defines the finanzmarkt_JanE
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

class finanzmarkt_JanE : public baseModel {

protected:
	static const int L=2;

	rand_var *zvar1;
	rand_var *zvar2;
	rand_var *zvar3;
	rand_var *zvar4;

    qreal xBundle;
    qreal rand_rec;          			//generates a rectangular unit variate
    qreal b1,b2;							//rechter Rand der Dreiecksverteilungen
    qreal c1,c2;							//peak der Dreiecksverteilungen
    qreal a1,a2;							//linker Rand der Dreiecksverteilungen
	
    qreal z1,z2;
    qreal F_c1, F_c2;
	
    qreal zetamin1, zetamax1, gamma1;	//Parameter für AR1-Prozess der Dividende 1
    qreal zetamin2, zetamax2, gamma2;	//Parameter für AR1-Prozess der Dividende 2
    qreal Ezeta1, Ezeta2;				//Unbedingter Erwartungswert d. Innovationen
										//im Dividendenprozess
 
	matrix_neu* d;						// Dividendenvektor
	matrix_neu* Ed;						// Bedingter Erwartungswert d.Dividendentor
    qreal d1,d2;							//Kompenenten des Dividenden-Vektors
    qreal Ed1,Ed2;						//Kompenenten des Vektors der bedingten Erwartungen
	
    qreal var_d1,var_d2;					//Varianzen der Dividenden-Prozesse

    qreal eta1_F, eta2_F, eta3_F;		//Markanteile d. Fundamentalisten bei den 3 HH
    qreal eta1_C, eta2_C, eta3_C;		//Markanteile d. Chartisten bei den 3 HH
    qreal eta1_N, eta2_N, eta3_N;		//Markanteile d. Noisetraders bei den 3 HH
	

    qreal eta1_F_0, eta2_F_0, eta3_F_0;	//Start-Markanteile d. Fundamentalisten bei den 3 HH
    qreal eta1_C_0, eta2_C_0, eta3_C_0;	//Start-Markanteile d. Chartisten bei den 3 HH
    qreal eta1_N_0, eta2_N_0, eta3_N_0;	//Start-Markanteile d. Noisetraders bei den 3 HH
    qreal w_F, w_C, w_N;					//Investitionsbetrag d. Investoren

    qreal alpha_1, alpha_2, alpha_3;		// Risikoparameter der Haushalte
    qreal beta_1, beta_2, beta_3;		// intensity of choice d. HH
    qreal e_1, e_2, e_3;					// Anfangsausstattung der Haushalte
	
    matrix_neu* Rho[L];					// Array von Diagonalmatrizen für MA
	matrix_neu* q_MA[L];				// Array der gelagten cumdividend-Preise
	
    qreal r, R;							// fixed rate of interest, R=1+r
	
	matrix_neu* p;						// price
    qreal p1, p2;						// Prices of assets
	matrix_neu* p_old;					// price of the previous period
	matrix_neu* q;						// cumdividend price
    matrix_neu* q0;						// Startwerte für q, q_C und q_F
	matrix_neu* q_C;					// cumdividend forecast (Chartists)
	matrix_neu* q_F;					// cumdividend forecast (Fundamentalists)
	matrix_neu* q_Fold;					// cumdividend forecast der Vorperiode (Fundamentalists)
    qreal q_Fold1,q_Fold2;				// Komponenten d. cumdividend forecast der Vorperiode (Fundamentalists)
	matrix_neu* q_N;					// cumdividend forecast (Noisetraders)
	
	matrix_neu* xi;						// noise
    qreal xi1, xi2;						// Komponenten des noise-Vektors
	matrix_neu* E_xi;					// Erwartungswert noise
	matrix_neu* T;						// Transformationsmatrix f.Noise-Prozess
	matrix_neu* T_inv;					// Inverse Transformationsmatrix f.Noise-Prozess
    qreal delta;							// Korrelationsparameter
	matrix_neu* b;						// Shiftvektor b	
    qreal b_1,b_2;						// Komponenten Shiftvektor

	matrix_neu* epsilon;				// transformed noise
    qreal epsilon1, epsilon2;			// Komponenten des epsilon-Vektors
	matrix_neu* E_epsilon;				// Erwartungswert transformed noise
	matrix_neu* V_epsilon;				// Varianz transformed noise
	matrix_neu* V_epsilon_inv;			// Inverse Varianz transformed noise
	
	

	matrix_neu* x_all;					// alle papiere
    qreal x_all1, x_all2;				// Komponenten von alle papiere

    qreal zeta1_F, zeta2_F;				//Anteile f. Startportfolio d. Fundamentalisten
    qreal zeta1_C, zeta2_C;				//Anteile f. Startportfolio d. Chartisten
    qreal zeta1_N, zeta2_N;				//Anteile f. Startportfolio d. Noisetraders
 	
	matrix_neu* x_F;					// Portefeuille d. Fundamentalisten
    qreal x_F1, x_F2;					// Komponenten d. Fundamentalisten-Portef.
	matrix_neu* x_Fold;					// Portefeuille d. Fundamentalisten aus der Vorperiode

	matrix_neu* x_C;					// Portefeuille der Chartisten
    qreal x_C1, x_C2;					// Komponenten des Chartisten-Portef.
	matrix_neu* x_Cold;					// Portefeuille der Chartisten aus der Vorperiode

	matrix_neu* x_N;					// Portefeuille d. Noise-Traders
    qreal x_N1, x_N2;					// Komponenten d. Noise-Trader-Portef.
	matrix_neu* x_Nold;					// Portefeuille d. Noise-Traders aus der Vorperiode

	matrix_neu* A;						// riskadjusted sum of covariance-matrices
	matrix_neu* A2_F;					// riskadjusted sum of covariance-matrices
	matrix_neu* A_C;					// riskadjusted covariance-matrix(Chartist)
	matrix_neu* A_F;					// riskadjusted covariance-matrix(Fundamentalist)
	matrix_neu* A_N;					// riskadjusted covariance-matrix(Noisetraders)

	matrix_neu* V_C;					// Kovarianz-Matrix (Chartists)
	matrix_neu* V_F;					// Kovarianz-Matrix (Fundamentalist)
	matrix_neu* V_N;					// Kovarianz-Matrix (Noisetraders)
	
    qreal wealth_N, wealth_F, wealth_C;	// Endvermögen der Händlergruppen
    qreal rend_N, rend_F, rend_C;		// Rendite der Händlergruppen
	
    qreal mu_F;							// mean of returns der Fundamentalisten
    qreal sigma_F;						// standard deviation of returns der Fundamentalisten
    qreal sr_F;							// sharpe-ratio Fundamentalisten

    qreal mu_N;							// mean of returns der Noise-Trader
    qreal sigma_N;						// standard deviation of returns der Noise-Trader
    qreal sr_N;							// sharpe-ratio Noise-Trader
	
    qreal mu_C;							// mean of returns der Chartisten
    qreal sigma_C;						// standard deviation of returns der Chartisten
    qreal sr_C;							// sharpe-ratio Chartisten
    qreal switch_F, switch_C, switch_N;	//0-1-Schalter f. Investoren

// Hilfsmatrizen:
	matrix_neu* V_C_inv;
	matrix_neu* V_F_inv;
	matrix_neu* V_N_inv;
	matrix_neu* temp1_2_2;
	matrix_neu* temp2_2_2;
	matrix_neu* temp3_2_2; 
	matrix_neu* temp4_2_2; 
	matrix_neu* temp1_2_1;
	matrix_neu* temp2_2_1;
	matrix_neu* temp3_2_1;
	matrix_neu* temp4_2_1;
	matrix_neu* temp1_1_1;
	matrix_neu* V_xi;			// Kovarianz-Matrix (Noise)
	matrix_neu* V_xi_inv;

// Berechnungsfunktionen:
    qreal	risk_tolerance(qreal&, qreal&, qreal&);
    qreal	sharpe_ratio(qreal&, qreal&, qreal&);
	
public:
    finanzmarkt_JanE();				// constructor
    virtual ~finanzmarkt_JanE();		// destructor
    void loadParamset(QDataStream&);	// load parameterset from a file
    void saveParamset(QDataStream&);
    void printParamset();
    void sendParameters(int &,qreal**);
    qreal* sendModelVar(void);
    void receiveParameters(const qreal*);
    qreal* setLabels(const QString&);		// get a variable by a name
    void initialize(void);		// initialize the model
    void iteration(const qint64&);	// perform one iteration
    void sendStateSpace(int &,const qreal***);

};

#endif //  _finanzmarkt_JanE_INCLUDED
