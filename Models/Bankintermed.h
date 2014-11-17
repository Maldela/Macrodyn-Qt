///////////////////////////////////////////////////////////////////////////////
//
// $Header: Bankintermed.h Fri Feb  2 11:35:08 MET 2001 mmeyer 
//
// Module name:		Bankintermed.h
// Contents:		Class definition of the class 
//
// Author:			Michael Meyer
// Last modified:	Fri Feb  2 11:35:19 MET 2001
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _BANKINTERMED_INCLUDED
#define _BANKINTERMED_INCLUDED

#include "baseModel.h"
#include "rand_var.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Bankintermed
// Derived from:	baseModel
// Purpose:			defines the Bankintermed
//
// Author:			Michael Meyer
// Last modified:	Wed Feb  7 15:53:11 MET 2001
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

class Bankintermed : public baseModel {

protected:	
	real e,e_0,e_alpha,e_beta,e_old;							// Eigenkapital					// e > 0
	real I;							// Fremdkapital				// I > 0, e < I
	real I_ini;						// Fremdkapital Initialisierung
	real I_min;
	real S,s;						// Sparaufkommen			// S > 0, S < nu*I
	real S_vol;						// Gesamtsparvolumen		
	real nu;						// Anteil Unternehmer 		// 0 < nu < 1
	real alpha,beta,gamma,delta;	// Regulierungsparameter	// 0 < alpha,beta,gamma,delta < 1  
	real q;							// Störung					// 0 < q_min < q_max	
	rand_var *zvar;
	real q_max;						// maximale Störung	
	real q_min;						// minimale Störung	
	real a,b,c,d;					// Produktionsparameter		// 0 < a,b,c,d
	real df_0;						// Startwert Defizit
	real df;						// Defizit
	real df_max;					// maximales Defizit
	real df_min;					// minimales Defizit
	real df_reg;					// Defizit Regulierung
	real y;							// Einkommen
	real Y;							// Output	
	real I_G;						// kritische Unternehmer
	real R;							// Gleichgewichtszinsfaktor
	real R_c;						// Kreditzinsfaktor
	real R_reg;						// Einlagenzinsfaktor
	real R_min;						// minimaler Einlagenzinsfaktor
	real I_B;						// Firmenbankrott
	real P;							// Aggr. Output der Firmen
	real G;							// Dynamik
	real RF;						// Rückzahlungsfähigkeit 
	real K;							// Kreditvolumen
	real B;							// Bankrotte

	real df_crit_min;
	real df_crit_max;
		
	real temp1;
	real temp2;

	real xBundle;

	void I_G_Fktn();
	void R_Fktn();
	void R_c_Fktn();
	void R_reg_Fktn();
	void R_min_Fktn();
	void I_B_Fktn();
	void P_Fktn();
	void I_Fktn();
		
public:
    Bankintermed();				// constructor
    virtual ~Bankintermed();		// destructor
    void loadParamset(ifstream&);	// load parameterset from a file
    void saveParamset(ofstream&);
    void printParamset();
    void sendParameters(int &,real**);
    real* sendModelVar(void);
    void receiveParameters(const real*);
    real* setLabels(char*);		// get a variable by a name
    void initialize(void);		// initialize the model
    void iteration(const long&);	// perform one iteration 	
    void sendStateSpace(int &,const real***);

};

#endif //  _BANKINTERMED_INCLUDED
