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
    qreal e,e_0,e_alpha,e_beta,e_old;							// Eigenkapital					// e > 0
    qreal I;							// Fremdkapital				// I > 0, e < I
    qreal I_ini;						// Fremdkapital Initialisierung
    qreal I_min;
    qreal S,s;						// Sparaufkommen			// S > 0, S < nu*I
    qreal S_vol;						// Gesamtsparvolumen
    qreal nu;						// Anteil Unternehmer 		// 0 < nu < 1
    qreal alpha,beta,gamma,delta;	// Regulierungsparameter	// 0 < alpha,beta,gamma,delta < 1
    qreal q;							// Störung					// 0 < q_min < q_max
	rand_var *zvar;
    qreal q_max;						// maximale Störung
    qreal q_min;						// minimale Störung
    qreal a,b,c,d;					// Produktionsparameter		// 0 < a,b,c,d
    qreal df_0;						// Startwert Defizit
    qreal df;						// Defizit
    qreal df_max;					// maximales Defizit
    qreal df_min;					// minimales Defizit
    qreal df_reg;					// Defizit Regulierung
    qreal y;							// Einkommen
    qreal Y;							// Output
    qreal I_G;						// kritische Unternehmer
    qreal R;							// Gleichgewichtszinsfaktor
    qreal R_c;						// Kreditzinsfaktor
    qreal R_reg;						// Einlagenzinsfaktor
    qreal R_min;						// minimaler Einlagenzinsfaktor
    qreal I_B;						// Firmenbankrott
    qreal P;							// Aggr. Output der Firmen
    qreal G;							// Dynamik
    qreal RF;						// Rückzahlungsfähigkeit
    qreal K;							// Kreditvolumen
    qreal B;							// Bankrotte

    qreal df_crit_min;
    qreal df_crit_max;
		
    qreal temp1;
    qreal temp2;

    qreal xBundle;

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

#endif //  _BANKINTERMED_INCLUDED
