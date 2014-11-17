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
	
	real beta;				//Einheitsparameter für Jobs ohne Bundleunterstützung
	real dummy;
	real etaC_us, etaC_os;			//Unter- und Oberschranke für eta_C

	rand_var *zvar1;
	rand_var *zvar2;
	rand_var *zvar3;
	rand_var *zvar4;

	real rand_rec;          			//generates a rectangular unit variate	
	real b1,b2;							//rechter Rand der Dreiecksverteilungen
	real c1,c2;							//peak der Dreiecksverteilungen
	real a1,a2;							//linker Rand der Dreiecksverteilungen
	
	real z1,z2;
	real F_c1, F_c2;
	
	real zetamin1, zetamax1, gamma1;	//Parameter für AR1-Prozess der Dividende 1
	real zetamin2, zetamax2, gamma2;	//Parameter für AR1-Prozess der Dividende 2
	real Ezeta1, Ezeta2;				//Unbedingter Erwartungswert d. Innovationen 
										//im Dividendenprozess
 
	matrix_neu* d;						// Dividendenvektor
	matrix_neu* Ed;						// Bedingter Erwartungswert d.Dividendentor
	real d1,d2;							//Kompenenten des Dividenden-Vektors
	real Ed1,Ed2;						//Kompenenten des Vektors der bedingten Erwartungen
	real q1_0,q2_0;						//Startcdpreise
	
	real var_d1,var_d2;					//Varianzen der Dividenden-Prozesse

	real eta1_F, eta2_F, eta3_F;		//Markanteile d. Fundamentalisten bei den 3 HH
	real eta1_C, eta2_C, eta3_C;		//Markanteile d. Chartisten bei den 3 HH
	real eta1_N, eta2_N, eta3_N;		//Markanteile d. Noisetraders bei den 3 HH
	

	real eta1_F_0, eta2_F_0, eta3_F_0;	//Start-Markanteile d. Fundamentalisten bei den 3 HH
	real eta1_C_0, eta2_C_0, eta3_C_0;	//Start-Markanteile d. Chartisten bei den 3 HH
	real eta1_N_0, eta2_N_0, eta3_N_0;	//Start-Markanteile d. Noisetraders bei den 3 HH
	real w_F, w_C, w_N;			//Investitionsbetrag d. Investoren	

	real alpha_1, alpha_2, alpha_3;		// Risikoparameter der Haushalte
	real beta_1, beta_2, beta_3;		// intensity of choice d. HH
	real e_1, e_2, e_3;			// Anfangsausstattung der Haushalte
	
	matrix_neu* Rho[L];			// Array von Diagonalmatrizen für MA
	real rho[L];				// Werte für MA Matrizen
	matrix_neu* q_MA[L];			// Array der gelagten cumdividend-Preise
	
	real r, R;							// fixed rate of interest, R=1+r
	
	matrix_neu* p;						// price
	real p1, p2;						// Prices of assets
	matrix_neu* p_old;					// price of the previous period
	matrix_neu* q;						// cumdividend price
	matrix_neu* q0;						// Startwerte für q, q_C und q_F
	matrix_neu* q_C;					// cumdividend forecast (Chartists)
	matrix_neu* q_F;					// cumdividend forecast (Fundamentalists)
	matrix_neu* q_Fold;					// cumdividend forecast der Vorperiode (Fundamentalists)
	real q_Fold1,q_Fold2;				// Komponenten d. cumdividend forecast der Vorperiode (Fundamentalists)
	matrix_neu* q_N;					// cumdividend forecast (Noisetraders)
	
	matrix_neu* xi;						// noise
	real xi1, xi2;						// Komponenten des noise-Vektors
	matrix_neu* E_xi;					// Erwartungswert noise
	matrix_neu* T;						// Transformationsmatrix f.Noise-Prozess
	matrix_neu* T_inv;					// Inverse Transformationsmatrix f.Noise-Prozess
	real delta;							// Korrelationsparameter
	matrix_neu* b;						// Shiftvektor b	
	real b_1,b_2;						// Komponenten Shiftvektor

	matrix_neu* epsilon;				// transformed noise
	real epsilon1, epsilon2;			// Komponenten des epsilon-Vektors
	matrix_neu* E_epsilon;				// Erwartungswert transformed noise
	matrix_neu* V_epsilon;				// Varianz transformed noise
	matrix_neu* V_epsilon_inv;			// Inverse Varianz transformed noise
	
	

	matrix_neu* x_all;					// alle papiere
	real x_all1, x_all2;				// Komponenten von alle papiere

	real zeta1_F, zeta2_F;				//Anteile f. Startportfolio d. Fundamentalisten
 	real zeta1_C, zeta2_C;				//Anteile f. Startportfolio d. Chartisten
 	real zeta1_N, zeta2_N;				//Anteile f. Startportfolio d. Noisetraders
 	
	matrix_neu* x_F;					// Portefeuille d. Fundamentalisten
	real x_F1, x_F2;					// Komponenten d. Fundamentalisten-Portef.
	matrix_neu* x_Fold;					// Portefeuille d. Fundamentalisten aus der Vorperiode

	matrix_neu* x_C;					// Portefeuille der Chartisten
	real x_C1, x_C2;					// Komponenten des Chartisten-Portef.
	matrix_neu* x_Cold;					// Portefeuille der Chartisten aus der Vorperiode

	matrix_neu* x_N;					// Portefeuille d. Noise-Traders
	real x_N1, x_N2;					// Komponenten d. Noise-Trader-Portef.
	matrix_neu* x_Nold;					// Portefeuille d. Noise-Traders aus der Vorperiode

	matrix_neu* A;						// riskadjusted sum of covariance-matrices
	matrix_neu* A2_F;					// riskadjusted sum of covariance-matrices
	matrix_neu* A_C;					// riskadjusted covariance-matrix(Chartist)
	matrix_neu* A_F;					// riskadjusted covariance-matrix(Fundamentalist)
	matrix_neu* A_N;					// riskadjusted covariance-matrix(Noisetraders)

	matrix_neu* V_C;					// Kovarianz-Matrix (Chartists)
	matrix_neu* V_F;					// Kovarianz-Matrix (Fundamentalist)
	matrix_neu* V_N;					// Kovarianz-Matrix (Noisetraders)
	
	real wealth_N, wealth_F, wealth_C;	// Endvermögen der Händlergruppen
	real rend_N, rend_F, rend_C;		// Rendite der Händlergruppen
	
	real delta_sr;
	real delta_mu;
	real delta_sigma;
	
	real mu_F;							// mean of returns der Fundamentalisten
	real sigma_F;						// standard deviation of returns der Fundamentalisten
	real var_F;						// uncentered second moment of returns fundamentalists
	real sr_F;							// sharpe-ratio Fundamentalisten

	real mu_N;							// mean of returns der Noise-Trader
	real sigma_N;						// standard deviation of returns der Noise-Trader
	real var_N;						// uncentered second moment of returns noise-traders
	real sr_N;							// sharpe-ratio Noise-Trader
	
	real mu_C;							// mean of returns der Chartisten
	real sigma_C;						// standard deviation of returns der Chartisten
	real var_C;						// uncentered second moment of returns chartists
	real sr_C;							// sharpe-ratio Chartisten
	real switch_F, switch_C, switch_N;	//0-1-Schalter f. Investoren

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
	real	risk_tolerance(real&, real&, real&);
	real	sharpe_ratio(real&, real&, real&);
	real	sharpe_ratio2(real&, real&, real&);
	
public:
    finanzmarkt_JanE();				// constructor
    virtual ~finanzmarkt_JanE();		// destructor
    void loadParamset(ifstream&);	// load parameterset from a file
    void saveParamset(ofstream&);
    void saveParamsetWithNames(ofstream&);
    void printParamset();
    void sendParameters(int &,real**);
    real* sendModelVar(void);
    void receiveParameters(const real*);
    real* setLabels(char*);		// get a variable by a name
    void initialize(void);		// initialize the model
    void iteration(const long&);	// perform one iteration 	
    void sendStateSpace(int &,const real***);

};

#endif //  _finanzmarkt_JanE_INCLUDED
